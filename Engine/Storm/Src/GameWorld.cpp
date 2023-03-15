#include "Precompiled.h"
#include "GameWorld.h"
#include"GameObjectFactory.h"
using namespace Storm;

void GameWorld::Initialize(uint32_t capacity)
{
	ASSERT(!mInitialized, "GameWorld - Already initialized.");

	for (auto& service : mServices)
		service->Initialize();

	mGameObjectSlots.resize(capacity);
	mFreeSlots.resize(capacity);
	std::iota(mFreeSlots.rbegin(), mFreeSlots.rend(), 0);

	mInitialized = true;
}

void GameWorld::Terminate()
{

	ASSERT(!mUpdating, "GameWoeld - Cannot terminate world during update.");
	
	if (!mInitialized)
		return;
	
	//Destory all remaining game objects
	for (auto gameObject : mUpdateList)
		DestoryGameObject(gameObject->GetHandle());
	ProcessDestoryList();
	ASSERT(mUpdateList.empty(), "GameWorld - Failed to clean up game objects.");
	
	for (auto& service : mServices)
		service->Terminate();
	
	mInitialized = false;
}

void GameWorld::Update(float deltaTime)
{
	ASSERT(!mUpdating, "GameWorld - Already updating the woeld.");

	
	//This defers game object destruction
	mUpdating = true;

	for (auto& service : mServices)
		service->Update(deltaTime);

	for (size_t i = 0; i < mUpdateList.size(); ++i)
		//for(auto iter = mUpdateList.begin(); iter !=mUpdateList.end();++iter)
	{
		GameObject* gameObject = mUpdateList[i];
		if (IsValid(gameObject->GetHandle()))
			//	auto gameObject = *iter;
			gameObject->Update(deltaTime);
	}

	//Allow game obhect destructions
	mUpdating = false;

	//Now we can safely destory objects
	ProcessDestoryList();
}

void GameWorld::Render()
{
	for (auto& service : mServices)
		service->Render();
}

void GameWorld::DebugUI()
{
	ImGui::Begin("Services", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	for (auto& service : mServices)
		service->DebugUI();
	ImGui::End();
}

void GameWorld::DebugRender()
{
	for (size_t i = 0; i < mUpdateList.size(); ++i)
	{
		GameObject* gameObject = mUpdateList[i];
		gameObject->DebugUI();
	}
}

GameObject* GameWorld::CreateGameObject(std::filesystem::path templateFile)
{
	ASSERT(mInitialized, "GameWorld - World must be initialized first before creating game objects.");

	if (mFreeSlots.empty())
		return nullptr;


	const uint32_t freeSlot = mFreeSlots.back();
	mFreeSlots.pop_back();

	auto& slot = mGameObjectSlots[freeSlot];
	auto& newObject = slot.gameObject;
	newObject = std::make_unique<GameObject>();
	
	//TODO-add all the components here...
	GameObjectFactory::Make(templateFile, *newObject);
	
	//Initialize handle
	GameObjectHandle handle;
	handle.mIndex = freeSlot;
	handle.mGeneration = slot.generation;

	//Initialize game object
	newObject->mWorld = this;
	newObject->mHandle = handle;
	newObject->Initialize();

	//Add game objdet to update list
	mUpdateList.push_back(newObject.get());
	return newObject.get();
}

GameObject* GameWorld::FindGameObject(const std::string& name)
{
	auto iter = std::find_if(
		mUpdateList.begin(),
		mUpdateList.end(),
		[&](auto gameObject)
		{
			return gameObject->GetName() == name;

		});

	//C++20
	//auto iter = std::ranges::find_if(mUpdateList, name, &GameObject::mName);
	return iter == mUpdateList.end() ? nullptr : *iter;
}

GameObject* GameWorld::GetGameObject(const GameObjectHandle& handle)
{
	if (IsValid(handle))
		return mGameObjectSlots[handle.mIndex].gameObject.get();

	return nullptr;
}

void GameWorld::DestoryGameObject(const GameObjectHandle& handle)
{
	if (!IsValid(handle))
	return;

	auto& slot = mGameObjectSlots[handle.mIndex];
	slot.generation++; //This invalidates all existing handles to the solt
	mToBeDestoryed.push_back(handle.mIndex);
}


bool GameWorld::IsValid(const GameObjectHandle& handle) const
{
	if (handle.mIndex < 0 || handle.mIndex >= mGameObjectSlots.size())
	{
		return false;
	}

	if (mGameObjectSlots[handle.mIndex].generation != handle.mGeneration)
	{
		return false;
	}

	return true;

	//return
	//	handle.mIndex >= 0 &&
	//	handle.mIndex < mGameObjectSlots.size() &&
	//	mGameObjectSlots[handle.mIndex].generation == handle.mGeneration;
}

void GameWorld::ProcessDestoryList()
{
	ASSERT(!mUpdating, "GameWoeld - Cannot destory game objects during update.");
	for (auto index : mToBeDestoryed)
	{
		auto& slot = mGameObjectSlots[index];
		GameObject* gameObject = slot.gameObject.get();
		ASSERT(!IsValid(gameObject->GetHandle()), "GameWorld - Game object is still valid!");

		//Remoce from update list
		mUpdateList.erase(std::remove(mUpdateList.begin(), mUpdateList.end(), gameObject)); //Erase-Remove Idiom

		//Terminate game Object
		gameObject->Terminate();

		//Free and destryctor GameObject
		slot.gameObject.reset();
		mFreeSlots.push_back(index);
		//Get a free slot
	}
	mToBeDestoryed.clear();
}
