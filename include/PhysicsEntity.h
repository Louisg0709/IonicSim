#ifndef PHYSICS_ENTITY_H
#define PHYSICS_ENTITY_H

//My includes
#include "EcsConfigDefault.h"
#include "MathUtilities.h"

//Standard library include
#include <vector>

//external libs
#include <raylib.h>

class PhysicsContainer; 

class PhysicsEntity {

protected:
	//Properties all physics entities will have
	Vector2 Position;
	Vector2 Velocity;

	double Mass;   //Kilograms
	double Radius; //Metres, everything is represented by a point charge (radius is only for colisions and graphics)
	double Charge; //Coloumbs

	EntityCollisionData CollisionData;
	
public:

	PhysicsEntity(Vector2 position, Vector2 velocity, float mass, float radius, float charge, ENTITY_ID_TYPE id, PhysicsContainer* owner);

	//Functions for all physics entities
	void Step(float TimeStep, std::vector<ContainerStepData>*, std::vector<PhysicsEntity*>* Entities); //Function for custom logic on step

	double KineticEnergy(); //Joules
	double ElectricPotential(std::vector<PhysicsEntity*>* Entities); //Joules
	Vector2 ElectricAcceleration(std::vector<PhysicsEntity*>* Entities);
	
	Vector2 GetPosition();
	Vector2 GetVelocity();
	double GetRadius();
	double GetCharge();
	double GetMass();

	void AddVelocity(Vector2 VelocityIncrease);

	ENTITY_ID_TYPE GetID();
	ENTITY_ID_TYPE ID;
	PhysicsContainer* Owner;

private:
	Vector2 Acceleration;
};

#endif // !PHYSICS_ENTITY_H
