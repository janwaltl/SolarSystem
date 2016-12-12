#include "RK4.h"
#include <cmath>
#include <vector>
#include <array>

namespace solar
{
	const double cbrt2 = cbrt(2);
	const double c1 = 1 / (2 * (2 - cbrt2));
	const double c4 = c1;
	const double c2 = (1 - cbrt2) / (2 * (2 - cbrt2));
	const double c3 = c2;
	const double d1 = 1 / (2 - cbrt2);
	const double d3 = d1;
	const double d2 = -cbrt2 / (2 - cbrt2);
	const double d4 = 0;
	void RK4::operator()(simData_t & data, double step)
	{
		//Acc|Vel
		struct AccVel
		{
			Vec2 acc, vel;
		};
		struct VelPos
		{
			Vec2 vel, pos;
		};

		std::vector<VelPos> temps;//Temps pro vypocet koeficientu
		temps.reserve(data.size());
		//inicialozvat primo data
		for (auto& unit : data)
			temps.push_back({unit.vel,unit.pos});

		std::array<std::vector<AccVel>, 4> kXs;//Koeficienty k1..4 pro kazdou unitu
		kXs[0] = {data.size(), {{0.0,0.0}, {0.0,0.0}}};
		kXs[1] = {data.size(), {{0.0,0.0}, {0.0,0.0}}};
		kXs[2] = {data.size(), {{0.0,0.0}, {0.0,0.0}}};
		kXs[3] = {data.size(), {{0.0,0.0},{0.0,0.0}}};


		//Go through all pairs
		auto rovnice2 = [&](double mult, size_t x)
		{
			x = x - 1;//To allow adressin by k1,k2,k3,k4
			for (size_t i = 0; i < data.size(); ++i)
			{
				auto& left = temps[i];
				for (size_t j = i + 1; j < data.size(); ++j)
				{
					auto& right = temps[j];
					auto distLR = dist(left.pos, right.pos);
					distLR = distLR*distLR*distLR;
					Vec2 acc = Vec2((left.pos.X() - right.pos.X()), (left.pos.Y() - right.pos.Y()))*G * (-1 / distLR);
					//Update left's and right's accelerations, store them in k1's acc
					kXs[x][i].acc += acc*data[j].mass;
					kXs[x][j].acc -= acc*data[i].mass;
				}
				//Store left's velocity in k1's vel
				kXs[x][i].vel = left.vel;
				left.vel = data[i].vel + mult*step*kXs[x][i].acc;
				left.pos = data[i].pos + mult*step*kXs[x][i].vel;
			}
		};

		rovnice2(0.5, 1);
		rovnice2(0.5, 2);
		rovnice2(1.0, 3);
		rovnice2(0.0, 4);//Temp uz me nezajima ted

		for (size_t i = 0; i < data.size(); ++i)
		{
			data[i].vel += step / 6.0 *(kXs[0][i].acc + 2 * kXs[1][i].acc + 2 * kXs[2][i].acc + kXs[3][i].acc);
			data[i].pos += step / 6.0 *(kXs[0][i].vel + 2 * kXs[1][i].vel + 2 * kXs[2][i].vel + kXs[3][i].vel);
		}
	}
}
