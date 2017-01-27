#include "RK4.h"
#include <cmath>
#include <vector>
#include <array>
#include "Source/Units/PhysicsUnits.h"


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
	}
	void RK4::operator()(double step)
	{
		step /= physicsUnits::YtoS;
		//Go through all pairs
		auto computeKx = [&](size_t x, double mult)
		{
			x = x - 1;//To allow adressin by k1,k2,k3,k4
			for (size_t i = 0; i < data->size(); ++i)
			{
				auto& left = temps[i];
				for (size_t j = i + 1; j < data->size(); ++j)
				{
					auto& right = temps[j];
					auto distLR = dist(left.pos, right.pos);
					distLR = distLR*distLR*distLR;
					Vec2 dir = left.pos - right.pos;
					Vec2 acc = -physicsUnits::G / distLR * dir;
					//Update left's and right's accelerations, store them in k1's acc
					kXs[x][i].acc += acc*(*data)[j].mass;
					kXs[x][j].acc -= acc*(*data)[i].mass;
				}
				//Store left's velocity in k1's vel
				kXs[x][i].vel = left.vel;
				left.vel = (*data)[i].vel + mult*step*kXs[x][i].acc;
				left.pos = (*data)[i].pos + mult*step*kXs[x][i].vel;
			}
		};

		computeKx(1, 0.5);
		computeKx(2, 0.5);
		computeKx(3, 1.0);
		computeKx(4, 0.0);//Temp is not needed now, so mult=0 is fine

		for (size_t i = 0; i < data->size(); ++i)
		{
			(*data)[i].vel += step / 6.0 *(kXs[0][i].acc + 2 * kXs[1][i].acc + 2 * kXs[2][i].acc + kXs[3][i].acc);
			(*data)[i].pos += step / 6.0 *(kXs[0][i].vel + 2 * kXs[1][i].vel + 2 * kXs[2][i].vel + kXs[3][i].vel);
			temps[i] = {(*data)[i].vel,(*data)[i].pos}; //Reinitialize temps for next integration step
			//Clear coefficients for next step
			kXs[0][i] = {Vec2(),Vec2()};
			kXs[1][i] = {Vec2(),Vec2()};
			kXs[2][i] = {Vec2(),Vec2()};
			kXs[3][i] = {Vec2(),Vec2()};
		}

	}
}
