#include "ExplicitEuler.h"
#include <iostream>
#include "Source/Units/PhysicsUnits.h"
#include <algorithm>
void solar::ExplicitEuler::operator()(double step)
{
	step /= data->RatioOfTimeTo(PhysUnits::second);
	//Gravitational constant converted from SI to current units
	const auto grav = G<double> / pow(data->RatioOfDistTo(PhysUnits::meter), 3) * data->RatioOfMassTo(PhysUnits::kilogram) * pow(data->RatioOfTimeTo(PhysUnits::second), 2);

	struct VelPos
	{
		Vec3d vel, pos;
	};
	std::vector<VelPos> temps;
	temps.reserve(data->Get().size());

	for (const auto& unit : data->Get())
		temps.push_back({unit.vel,unit.pos});

	//Go through all pairs
	for (size_t i = 0; i < data->Get().size(); ++i)
	{
		auto& left = data->Get()[i];

		for (size_t j = i + 1; j < data->Get().size(); ++j)
		{
			auto& right = data->Get()[j];
			auto distLR = (temps[i].pos- temps[j].pos).Length();
			distLR = distLR*distLR*distLR;

			// acceleration = - G* R/R^3
			//Acceleration of left unit gained from attraction to right unit, WITHOUT mass of correct unit
			//Minus for the force to be attractive, not repulsive
			Vec3d dir = temps[i].pos - temps[j].pos;
			Vec3d acc = -grav / distLR * dir;
			// velocity(t+dt) = velocity(t) + dt*acc(t); - explicit Euler
			left.vel += step*acc*right.mass;// with correct mass
			right.vel -= step*acc*left.mass;// with correct mass, opposite direction
		}

		left.pos += step*temps[i].vel;
	}
}
