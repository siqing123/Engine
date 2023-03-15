#pragma once

#include "Component.h"
#include "GameObjectHandle.h"

namespace Storm
{
	class GameWorld;

	class GameObject final
	{
	public:
		MEMORY_POOL_DECLARE

		GameObject() = default;

		void Initialize();
		void Terminate();

		void Update(float deltaTime);
		void DebugUI();

		template <class ComponentType>
		ComponentType* AddComponent()
		{
			ASSERT(!mInitialized, "GameObject - Can only add component before object is initialized.");
			auto& newComponent = mComponents.emplace_back(std::make_unique <ComponentType>());
			newComponent->mOwner = this;
			return static_cast<ComponentType*>(newComponent.get());
		}

		template<class ComponentType>
		ComponentType* GetComponent()
		{
			auto constMe = static_cast<const GameObject*>(this);
			return const_cast<ComponentType*>(constMe->GetComponent<ComponentType>());
		}

		template<class ComponentType>
		const ComponentType* GetComponent() const
		{
			for (auto& component : mComponents)
			{
				if (component->GetTypeId() == ComponentType::StaticGetTypeId())
					return static_cast<const ComponentType*>(component.get());
			}
			return nullptr;
		}

		//void Test()
		//{
		//	TransdformConponent* transform = gameObject.GetComponent<TransdformComponent>();
		//}

	/*	void Test()
		{
			GameObject gameObject;
			TransformComponrent* transform = gameObject.AddComponent<TransformComponent>();
			transform->SetPosition()...
			gameObject.AddComponent<ColliderComponent>();
		}*/

		GameWorld& GetWorld() { return *mWorld; }
		const GameWorld& GetWorld() const { return *mWorld; }

		GameObjectHandle GetHandle() const { return mHandle; }

		void SetName(const char* name) { mName = name; }
		const std::string& GetName() const { return mName; }

	//private:
		friend class GameWorld;

		using Components = std::vector<std::unique_ptr<Component>>;

		Components mComponents;
		GameWorld* mWorld = nullptr;
		GameObjectHandle mHandle;
		std::string mName = "NoName";
		bool mInitialized = false;
	};
}