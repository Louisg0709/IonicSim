#ifndef PHYSICS_CONTAINER_H
#define PHYSICS_CONTAINER_H

//My includes
#include "EcsConfigDefault.h"
#include "MathUtilities.h"

//Standard library includes
#include <vector>
#include <type_traits>

//external libs
#include <raylib.h>

class PhysicsEntity;
class PhysicsSituation;

class PhysicsContainer {
protected:
	PhysicsContainer();
	~PhysicsContainer();

private:
	std::vector<PhysicsEntity*> Entities;

protected:
	ENTITY_ID_TYPE GetMinAvailableID(); //Find smallest possible id
	ENTITY_ID_TYPE GetIndexFromID(ENTITY_ID_TYPE ID);

public:
	void Step(float TimeStep);

	std::vector<PhysicsEntity*> FindEntitiesInRange(Vector2 Center, float Radius); //Find entities in a circle
	std::vector<PhysicsEntity*> FindEntitiesByRaycast(Vector2 Start, Vector2 End); //Find entites on a straight line ordered closest to furthest.
	PhysicsEntity* FindPhysicsEntityById(ENTITY_ID_TYPE ID);
	PhysicsEntity* FindPhysicsEntityFromIndex(ENTITY_ID_TYPE Index);

	void CreateBasePhysicsEntity(Vector2 position, Vector2 velocity, float mass, float radius, float charge); //Spawn a Physics Entity from the base class

	template<typename PhysicsEntityType>
	typename std::enable_if<std::is_base_of<PhysicsEntity, PhysicsEntityType>::value>::type*
	CreatePhysicsEntity(); //Spawn a Physics Entity from any of the child classes (not the base)

	void DestroyPhysicsEntity(ENTITY_ID_TYPE ID); //Destroy Physics Entity
};

#endif // !PHYSICS_CONTAINER_H