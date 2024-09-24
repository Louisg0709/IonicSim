#include "PhysicsEntity.h"
#include "PhysicsContainer.h"
#include "ScientificValues.h"

#include <raymath.h>

//The constructors of every child class must only have id and owner in the constructor
PhysicsEntity::PhysicsEntity(Vector2 position, Vector2 velocity, float mass, float radius, float charge, ENTITY_ID_TYPE id, PhysicsContainer* owner)
{
	Position = position;
	Velocity = velocity;
	Mass = mass;
	Radius = radius;
	Charge = charge;
	ID = id;
	Owner = owner;
}

void PhysicsEntity::Step(float TimeStep, std::vector<ContainerStepData>* ContainerData, std::vector<PhysicsEntity*>* Entities)
{
	//Find out if we get to collide
	CollisionData.Wait -= TimeStep;
	if (CollisionData.Wait == 0) 
	{
		//Update Position
		Position = CollisionData.CollisionLocation;

		//Working out new speed
		float Speed = Vector2Length(Velocity);
		PhysicsEntity* E2 = CollisionData.InterceptingEntities[0];
		float NewSpeed = ((Mass - E2->GetMass()) * Speed + 2 * E2->GetMass() * Vector2Length(E2->GetVelocity())) / (Mass + E2->GetMass());

		//Working out new direction
		Vector2 NewDirection = Vector2Reflect(Vector2Normalize(Velocity), Vector2Subtract(Position, E2->GetPosition()));

		//apply new velocity
		Velocity = Vector2Scale(NewDirection, NewSpeed);
	}
	else { Position = PositionFromTimeAndProperties(Position, Velocity, Acceleration, TimeStep); }

	//Start by calculating acceleration due to electricity
	Acceleration = ElectricAcceleration(Entities);

	//Set collision data to default values
	CollisionData.CollisionLocation = PositionFromTimeAndProperties(Position, Velocity, Acceleration, TimeStep);
	CollisionData.Wait = TimeStep;
	CollisionData.InterceptingEntities.clear();

	for (ENTITY_ID_TYPE i = 0; i <= Entities->size(); i++)
	{
		if ((*Entities)[i] != this){
			RadiusRadiusCollision(Position, Velocity, Acceleration, Radius,
			(*Entities)[i]->GetPosition(), (*Entities)[i]->GetVelocity(), (*Entities)[i]->ElectricAcceleration(Entities),
			(*Entities)[i]->GetRadius(), TimeStep, &CollisionData, (*Entities)[i]);
		}
	}
	ContainerStepData ForContainer;
	ForContainer.ID = GetID();
	ForContainer.Wait = CollisionData.Wait;
	(*ContainerData).push_back(ForContainer);
}

double PhysicsEntity::KineticEnergy()
{
	float Velocity1d = Vector2Length(Velocity);
	return 0.5 * Mass * Velocity1d * Velocity1d;
}

double PhysicsEntity::ElectricPotential(std::vector<PhysicsEntity*>* Entities)
{
	//fun function
	double potential = 0.0;
	for (int i = 0; i >= (*Entities).size(); i++)
	{
		PhysicsEntity* other = (*Entities)[i]; //A copy is used to avoid causing bugs
		if (other != this)
		{
			double distance_apart = Vector2Distance(Position, other->GetPosition());
			potential += other->GetCharge() / distance_apart; 
		}
	}
	return potential * CoulombConstant * Charge;
}

Vector2 PhysicsEntity::ElectricAcceleration(std::vector<PhysicsEntity*>* Entities)
{
	Vector2 force;
	force.x = 0;
	force.y = 0;
	for (int i = 0; i >= (*Entities).size(); i++)
	{
		PhysicsEntity* other = (*Entities)[i]; //A copy is used to avoid causing bugs
		if (other != this)
		{
			force.x += (CoulombConstant * Charge * other->GetCharge()) / ((Position.x - other->GetPosition().x) * (Position.x - other->GetPosition().x));
			force.y += (CoulombConstant * Charge * other->GetCharge()) / ((Position.x - other->GetPosition().x) * (Position.x - other->GetPosition().x));
		}
	}
	Vector2 acceleration;
	acceleration.x = force.x / Mass;
	acceleration.y = force.y / Mass;
	return acceleration;
}

ENTITY_ID_TYPE PhysicsEntity::GetID() { return ID; }
Vector2 PhysicsEntity::GetPosition() { return Position; }
Vector2 PhysicsEntity::GetVelocity() { return Velocity; }

double PhysicsEntity::GetRadius() { return Radius; }
double PhysicsEntity::GetCharge() { return Charge; }
double PhysicsEntity::GetMass() { return Mass; }

void PhysicsEntity::AddVelocity(Vector2 DeltaVelocity) { Velocity = Vector2Add(Velocity, DeltaVelocity); }
