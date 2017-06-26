#include "SemiImplicitEuler.h"
#include <iostream>
#include "Source/Units/PhysicsUnits.h"
namespace solar
{
	void SemiImplicitEuler::operator()(double step)
	{

		step /= data->RatioOfTimeTo(PhysUnits::second);
		//Gravitational constant converted from SI to current units
		const auto grav = G<double> / pow(data->RatioOfDistTo(PhysUnits::meter), 3) * data->RatioOfMassTo(PhysUnits::kilogram) * pow(data->RatioOfTimeTo(PhysUnits::second), 2);

		//Go through all pairs
		for (auto left = data->Get().begin(); left != data->Get().end(); ++left)
		{
			for (auto right = left + 1; right != data->Get().end(); ++right)
			{
				auto distLR = (left->pos - right->pos).Length();
				distLR = distLR*distLR*distLR;
				//minute -> hour = 1m=1/60h
				// acceleration = - G* R/R^3
				//Acceleration of left unit gained from attraction to right unit, WITHOUT mass of correct unit
				//Minus for the force to be attractive, not repulsive
				Vec3d dir = left->pos - right->pos;
				Vec3d acc = -grav / distLR * dir;
				// velocity(t+dt) = velocity(t) + dt*acc(t); - explicit Euler
				left->vel += step*acc*right->mass;// with correct mass
				right->vel -= step*acc*left->mass;// with correct mass, opposite direction

			}
			//position(t+dt) = position(t) + dt * velocity(t + dt); - implicit Euler
			//XX->vel is now at time (t+dt)
			left->pos += step*left->vel;
		}
	}
}
