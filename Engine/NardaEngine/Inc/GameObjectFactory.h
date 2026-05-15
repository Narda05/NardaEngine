#pragma once


namespace NardaEngine
{
	class GameObject;
	class GameWorld;
	class Component;

	//Define a custum component 
	using CustomComponent = std::function<Component* (const std::string&, GameObject&)>;

	namespace GameObjectFactory
	{
		void SetCustomMake(CustomComponent callback);
		void SetCustomGet(CustomComponent callback);

		void Make(const std::filesystem::path& templatePath, GameObject& gameObject, GameWorld& gameWorld);
		void OverrideDeserialize(const rapidjson::Value& value, GameObject& gameObject);
	}
}