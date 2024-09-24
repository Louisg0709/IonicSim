#ifndef MATH_UTILITIES_H
#define MATH_UTILITIES_H

#include<cstdint> //Contains unsigned integer types. Unsure why this header required here but knowhere else.
#include<vector>

#include<raylib.h>
#include<raymath.h>

#include "EcsConfigDefault.h"

class PhysicsEntity;

struct ContainerStepData{
	float Wait;
	std::ENTITY_ID_TYPE ID; 
};

Vector2 PositionFromTimeAndProperties(Vector2 InitialPos, Vector2 InitialVelocity, Vector2 Acceleration, float DeltaTime) {
	return Vector2Add(InitialPos, Vector2Add(Vector2Scale(InitialVelocity, DeltaTime), Vector2Scale(Acceleration, DeltaTime * DeltaTime * 0.5)));
}

//Doubles as general data on where character is going
struct EntityCollisionData {
	float Wait;
	std::vector<PhysicsEntity*> InterceptingEntities; //Vec of pointers to entities colliding next(at the same time)
	Vector2 CollisionLocation;
};

void RadiusRadiusCollision(Vector2 E1Start, Vector2 E1InitialVelocity, Vector2 E1Acceleration, double E1Radius,
	Vector2 E2Start, Vector2 E2InitialVelocity, Vector2 E2Acceleration, double E2Radius,
	float TimeStep, EntityCollisionData* Data, PhysicsEntity* E2)
{

	//First check both can even reach each other first
	if ((Vector2Length(Vector2Add(E1InitialVelocity, Vector2Scale(E1Acceleration, TimeStep * 0.5))) + E1Radius + Vector2Length
	(Vector2Add(E2InitialVelocity, Vector2Scale(E2Acceleration, TimeStep * 0.5))) + E2Radius) >= Vector2Distance(E1Start, E2Start))
	{
		//saving useful values such as differences
		double RadiusCombined = E1Radius + E2Radius;
		Vector2 Distance = Vector2Subtract(E1Start, E2Start);
		Vector2 DifferenceInV = Vector2Subtract(E1InitialVelocity, E2InitialVelocity);
		Vector2 DifferenceInA = Vector2Subtract(E1Acceleration, E2Acceleration);

		//quadratic formula
		double a = 0.25f * (DifferenceInA.x * DifferenceInA.x + DifferenceInA.y * DifferenceInA.y);
		double b = DifferenceInV.x * DifferenceInA.x + DifferenceInV.y * DifferenceInA.y;
		double c = Distance.x * Distance.x + Distance.y * Distance.y - RadiusCombined * RadiusCombined;

		double discriminant = b * b - 4 * a * c;
		if (discriminant < 0) { return; } //If discriminant is negative there are no solutions.
		double SqrtDiscriminant = sqrtf(discriminant);
		//Find solutions
		double t1 = (-b - SqrtDiscriminant) / (2 * a);
		double t2 = (-b + SqrtDiscriminant) / (2 * a);
		//Check they are in range
		if (0 < t1 <= TimeStep) //0 is out of bounds because that would count as a collision on the previous step. 
		{
			//Check there isn't any sooner collision and set data
			if (t1 < (*Data).Wait)
			{
				(*Data).Wait = t1;
				(*Data).InterceptingEntities.clear(); //Get rid of any outdated data
				(*Data).InterceptingEntities.push_back(E2);
				(*Data).CollisionLocation = PositionFromTimeAndProperties(E1Start, E1InitialVelocity, E1Acceleration, TimeStep);
			}
			else if (t1 == (*Data).Wait) { (*Data).InterceptingEntities.push_back(E2); } //If multiple entities colide at once all are kept track of. Probably a bad idea
			//Same thing again but skip if t1 and t2 are the same
			if ((0 < t2 <= TimeStep) && t2 != t1)
			{
				if (t2 < (*Data).Wait)
				{
					(*Data).Wait = t2;
					(*Data).InterceptingEntities.clear();
					(*Data).InterceptingEntities.push_back(E2);
				}
				else if (t2 == (*Data).Wait) { (*Data).InterceptingEntities.push_back(E2); }
			}

		}
	}
}


#endif // !MATH_UTILITIES_H
