#include "SemiImplicitEuler.h"
#include <iostream>

namespace solar
{
	void SemiImplicitEuler::operator()( double step)
	{
		//Go through all pairs
		for (auto left = data->begin(); left != data->end(); ++left)
		{
			for (auto right = left + 1; right != data->end(); ++right)
			{
				auto distLR = dist(left->pos, right->pos);
				distLR = distLR*distLR*distLR;

				// acceleration = - G* R/R^3
				//Acceleration of left unit gained from attraction to right unit, WITHOUT mass of correct unit
				//Minus for the force to be attractive, not repulsive
				Vec2 dir = left->pos - right->pos;
				Vec2 acc = -physicsUnits::G / distLR * dir;
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
