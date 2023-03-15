#pragma once
#include <Core/Inc/Core.h>
#include <Math/Inc/EngineMath.h>
#include <Input/Inc/Input.h>
#include <Graphics/Inc/Graphics.h>
#include <Physics/Inc/Physics.h>
#include <Bullet/btBulletCollisionCommon.h>
#include <Bullet/btBulletDynamicsCommon.h>

#include "Component.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "GameWorld.h"

template<class T>
inline void SafeReleasePhysics(T*& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

inline btVector3 ConverTobtVector3(const Storm::Math::Vector3& vec)
{
	return {vec.x,vec.y,vec.z};
}

inline Storm::Math::Vector3 ConverToVector3(const btVector3& vec)
{
	return { vec.x(),vec.y(),vec.z()};
}

inline btTransform ConvertTobtTransform(const Storm::Graphics::Transform& trans)
{
	return btTransform(btQuaternion(trans.rotation.x,trans.rotation.y, trans.rotation.z, trans.rotation.w),
		btVector3(trans.position.x, trans.position.y, trans.position.z));
}

inline Storm::Graphics::Transform ConverToTransform(const btTransform& trans)
{
	const auto& orign = trans.getOrigin();
	const auto& rotation = trans.getRotation();
	Storm::Graphics::Transform retTrans;
	retTrans.position = Storm::Math::Vector3(orign.x(), orign.y(), orign.z());
	retTrans.rotation = Storm::Math::Quaternion(rotation.x(), rotation.y(), rotation.z(), rotation.w());
	return retTrans;
}

inline void ConverToTransform(const btTransform& trans, Storm::Graphics::Transform& transform)
{
	const auto& orign = trans.getOrigin();
	const auto& rotation = trans.getRotation();
	Storm::Graphics::Transform retTrans;
	transform.position = Storm::Math::Vector3(orign.x(), orign.y(), orign.z());
	transform.rotation = Storm::Math::Quaternion(rotation.x(), rotation.y(), rotation.z(), rotation.w());
}
