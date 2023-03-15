#pragma once

namespace Storm
{
	class GameObject;

	class GameObjectFactory
	{
	public:
		static void Make(std::filesystem::path templateFile, GameObject& gameObject);
	};
}