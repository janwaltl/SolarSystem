#include "RK4.h"
#include <cmath>
#include <vector>
#include <array>
#include "Source/Units/PhysicsUnits.h"
#include <iostream>
namespace solar
{

	void RK4::Prepare()
	{
		temps.reserve(data->Get().size());
		for (auto& unit : data->Get())//First step of RK4 uses unit's values
			temps.push_back({unit.vel,unit.pos});

		//Zero all coefficients
		size_t dataSize = data->Get().size();
		kXs[0].resize(dataSize);
		kXs[1].resize(dataSize);
		kXs[2].resize(dataSize);
		kXs[3].resize(dataSize);
	}
	void RK4::operator()(double step)
	{
		step /= data->RatioOfTimeTo(PhysUnits::second);
		//Gravitational constant converted from SI to current units
		const auto grav = G<double> / pow(data->RatioOfDistTo(PhysUnits::meter), 3) * data->RatioOfMassTo(PhysUnits::kilogram) * pow(data->RatioOfTimeTo(PhysUnits::second), 2);

		//Go through all pairs
		auto computeKx = [&](size_t x, double mult)
		{
			x = x - 1;//To allow adressing by k1,k2,k3,k4
			for (size_t i = 0; i < data->Get().size(); ++i)
			{
				auto& left = temps[i];
				for (size_t j = i + 1; j < data->Get().size(); ++j)
				{
					auto& right = temps[j];

					//Calculate mutual acceleration via Newton

					auto distLR = (left.pos - right.pos).Length();
					distLR = distLR*distLR*distLR;
					Vec3d dir = left.pos - right.pos;
					Vec3d acc = -grav / distLR * dir;

					//Update left's and right's accelerations, store them in k1's acc
					kXs[x][i].acc += acc*data->Get()[j].mass;
					kXs[x][j].acc -= acc*data->Get()[i].mass;
				}
				kXs[x][i].vel = left.vel; //Store left's velocity in k1's vel

				//Update left temp unit to next time step
				//Which can be done now, because it won't be accessed in this loop anymore
				left.vel = data->Get()[i].vel + mult*step*kXs[x][i].acc;
				left.pos = data->Get()[i].pos + mult*step*kXs[x][i].vel;
			}
		};

		computeKx(1, 0.5);
		computeKx(2, 0.5);
		computeKx(3, 1.0);
		computeKx(4, 0.0);//Temp is not needed anymore, so mult=0 is fine

		for (size_t i = 0; i < data->Get().size(); ++i)
		{
			//Updates units based on RK4 method

			data->Get()[i].vel += step / 6.0 *(kXs[0][i].acc + 2.0 * kXs[1][i].acc + 2.0 * kXs[2][i].acc + kXs[3][i].acc);
			data->Get()[i].pos += step / 6.0 *(kXs[0][i].vel + 2.0 * kXs[1][i].vel + 2.0 * kXs[2][i].vel + kXs[3][i].vel);

			//Reinitialize temps for next integration step
			temps[i] = {data->Get()[i].vel,data->Get()[i].pos};

			//Clear coefficients for next step
			//(Atleast acc needs to be cleared, because it is additive)
			kXs[0][i] = {Vec3d(),Vec3d()};
			kXs[1][i] = {Vec3d(),Vec3d()};
			kXs[2][i] = {Vec3d(),Vec3d()};
			kXs[3][i] = {Vec3d(),Vec3d()};
		}
	}
}
