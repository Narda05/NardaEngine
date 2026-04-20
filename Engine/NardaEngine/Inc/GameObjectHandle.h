#pragma once

namespace NardaEngine
{
	class GameObjectHandle
	{
	public: 
		GameObjectHandle() = default;
	private: 
		friend class GameWorld;
		int mIndex = -1; // is going to be the index of the slot in game world.
		int mGeneration = -1; // to verify if item is already tagged for deletion/removed
	};
}