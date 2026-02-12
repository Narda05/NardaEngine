#include "Precompiled.h"
#include "CollisionShape.h"

using namespace NardaEngine;
using namespace NardaEngine::Physics;


CollisionShape::~CollisionShape()
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape:terminate must be terminated");
}

void CollisionShape::InitializeEmpty() 
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape: is ready initialized");
	mCollisionShape = new btEmptyShape();
}
void CollisionShape::InitializeSphere(float radius)
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape: is ready initialized");
	mCollisionShape = new btSphereShape(radius);
}
void CollisionShape::InitilizeCapsule(float radius, float height) 
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape: is ready initialized");
	mCollisionShape = new btCapsuleShape(radius, height);

}
void CollisionShape::InitializeBox(const Math::Vector3& halfExtents) 
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape: is ready initialized");
	mCollisionShape = new btBoxShape(TobtVector3(halfExtents));

}
void CollisionShape::InitializeHull(const Math::Vector3& halfExtents, const Math::Vector3& origin) 
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape: is ready initialized");
	btConvexHullShape* hullShape = new btConvexHullShape();
	std::vector<Math::Vector3> points =
	{
		{ -halfExtents.x, -halfExtents.y, -halfExtents.z },
		{ -halfExtents.x,  halfExtents.y, -halfExtents.z},
		{  halfExtents.x,  halfExtents.y, -halfExtents.z},
		{  halfExtents.x, -halfExtents.y, -halfExtents.z},
		{ -halfExtents.x, -halfExtents.y,  halfExtents.z },
		{ -halfExtents.x,  halfExtents.y,  halfExtents.z },
		{  halfExtents.x,  halfExtents.y,  halfExtents.z },
		{  halfExtents.x, -halfExtents.y,  halfExtents.z }
	};
	for (Math::Vector3& point : points)
	{
		hullShape->addPoint(TobtVector3(point + origin), false);
	}
	hullShape->recalcLocalAabb();
	mCollisionShape = hullShape;
}
void CollisionShape::Terminate() 
{
	SafeDelete(mCollisionShape);
}