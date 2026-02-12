#pragma once

// is an object that connects to the physics worls 
// must be inherited to register with the world

namespace NardaEngine::Physics
{
	class PhysicsObject
	{
	public: 
		PhysicsObject() = default;
		virtual ~PhysicsObject() = default;

	protected: 
		friend class PhysicsWorld;

		// abstract fuction 
		virtual void SyncWithGraphics() = 0;
		virtual btRigidBody* GetRigidBody() { return nullptr; }
	};
}
