#pragma once

#include"Service.h"

namespace Storm
{
	class CameraComponent;

	class CameraService final :public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Camera);

		//void SetCamera(const char* gameObjectName);
		const Graphics::Camera& GetCamera() const;

	private:
		friend class CameraComponent;

		void Register(const CameraComponent* cameraComponent);
		void Unregister(const CameraComponent* cameraComponent);

		std::vector<const CameraComponent*> mCameraEntries;
		const CameraComponent* mCurrentCamera;
	};
}