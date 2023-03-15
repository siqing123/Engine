#pragma once

#include "Common.h"

namespace Storm
{
	//Not using class based enusms because we planned to use there as integers
	enum class ComponentId
	{
		Animator,
		Camera,
		Collider,
		FPSCameraController,
		FollowCameraController,
		PlayerController,
		Model,
		Transform,
		Count,
		Renderer,
		RigidBody,
		Input,
		Physics
		
	};

	enum class ServiceId
	{
		Camera,
		Render,
		Count,
		Terrain,
		Physics
	};

}
#define SET_TYPE_ID(id)\
static uint32_t StaticGetTypeId() {return static_cast<uint32_t>(id);}\
uint32_t GetTypeId() const override {return StaticGetTypeId();}