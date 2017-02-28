#include "ExplicitEuler.h"
#include <iostream>
#include "Source/Units/PhysicsUnits.h"
#include <algorithm>
void solar::ExplicitEuler::operator()(double step)
{
	step /= physicsUnits::YtoS;

	struct VelPos
	{
		Vec2 vel, pos;
	};
	std::vector<VelPos> temps;
	temps.reserve(data->size());

	for (const auto& unit : *data)
		temps.push_back({unit.vel,unit.pos});

	//Go through all pairs
	for (size_t i = 0; i < data->size(); ++i)
	{
		auto& left = (*data)[i];

		for (size_t j = i + 1; j < data->size(); ++j)
		{
			auto& right = (*data)[j];
			auto distLR = dist(temps[i].pos, temps[j].pos);
			distLR = distLR*distLR*distLR;

			// acceleration = - G* R/R^3
			//Acceleration of left unit gained from attraction to right unit, WITHOUT mass of correct unit
			//Minus for the force to be attractive, not repulsive
			Vec2 dir = temps[i].pos - temps[j].pos;
			Vec2 acc = -physicsUnits::G / distLR * dir;
			// velocity(t+dt) = velocity(t) + dt*acc(t); - explicit Euler
			left.vel += step*acc*right.mass;// with correct mass
			right.vel -= step*acc*left.mass;// with correct mass, opposite direction
		}

		left.pos += step*temps[i].vel;
	}
}
