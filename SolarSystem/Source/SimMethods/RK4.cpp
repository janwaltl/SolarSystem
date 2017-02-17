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
		temps.reserve(data->size());
		for (auto& unit : *data)//First step of RK4 uses unit's values
			temps.push_back({unit.vel,unit.pos});

		//Zero all coefficients
		kXs[0].resize(data->size());
		kXs[1].resize(data->size());
		kXs[2].resize(data->size());
		kXs[3].resize(data->size());
		timing.Reset();
		numTimeSamples = 0;
	}
	void RK4::operator()(double step)
	{
		timing.Start();
		step /= physicsUnits::YtoS;

		//Go through all pairs
		auto computeKx = [&](size_t x, double mult)
		{
			x = x - 1;//To allow adressing by k1,k2,k3,k4
			for (size_t i = 0; i < data->size(); ++i)
			{
				auto& left = temps[i];
				for (size_t j = i + 1; j < data->size(); ++j)
				{
					auto& right = temps[j];

					//Calculate mutual acceleration via Newton

					auto distLR = dist(left.pos, right.pos);
					distLR = distLR*distLR*distLR;
					Vec2 dir = left.pos - right.pos;
					Vec2 acc = -physicsUnits::G / distLR * dir;

					//Update left's and right's accelerations, store them in k1's acc
					kXs[x][i].acc += acc*(*data)[j].mass;
					kXs[x][j].acc -= acc*(*data)[i].mass;
				}
				kXs[x][i].vel = left.vel; //Store left's velocity in k1's vel

				//Update left temp unit to next time step
				//Which can be done now, because it won't be accessed in this loop anymore
				left.vel = (*data)[i].vel + mult*step*kXs[x][i].acc;
				left.pos = (*data)[i].pos + mult*step*kXs[x][i].vel;
			}
		};

		computeKx(1, 0.5);
		computeKx(2, 0.5);
		computeKx(3, 1.0);
		computeKx(4, 0.0);//Temp is not needed anymore, so mult=0 is fine

		for (size_t i = 0; i < data->size(); ++i)
		{
			//Updates units based on RK4 method

			(*data)[i].vel += step / 6.0 *(kXs[0][i].acc + 2 * kXs[1][i].acc + 2 * kXs[2][i].acc + kXs[3][i].acc);
			(*data)[i].pos += step / 6.0 *(kXs[0][i].vel + 2 * kXs[1][i].vel + 2 * kXs[2][i].vel + kXs[3][i].vel);

			//Reinitialize temps for next integration step
			temps[i] = {(*data)[i].vel,(*data)[i].pos};

			//Clear coefficients for next step
			//(Atleast acc needs to be cleared, because it is additive)
			kXs[0][i] = {Vec2(),Vec2()};
			kXs[1][i] = {Vec2(),Vec2()};
			kXs[2][i] = {Vec2(),Vec2()};
			kXs[3][i] = {Vec2(),Vec2()};
		}
		timing.End();
		if (++numTimeSamples == 100'000)
		{
			std::cout << "\n RK4:" << (timing.GetMeasurement().count() / 100'000.0) / 10e6 << "ms\n";
			this->StopSimulation();
		}
	}
}
