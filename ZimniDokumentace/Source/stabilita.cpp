#include <iostream>
#include <math.h>
#include <fstream>

void AnalyticSolution(const std::string& outFname, double dt, size_t n)
{
	std::ofstream out(outFname, std::ios::out | std::ios::trunc);
	if (out.is_open())
	{
		double x, y, t;
		t = 0.0;
		x = 1.0;
		y = 0.0;

		out << t << '\t' << cos(t) << '\t' << sin(t) << '\n';
		for (size_t i = 0; i < n; ++i)
		{
			t += dt;
			out << t << '\t' << cos(t) << '\t' << sin(t) << '\n';
		}
	}
}

void ExplicitEuler(const std::string& outFname, double dt, size_t n)
{
	std::ofstream out(outFname, std::ios::out | std::ios::trunc);
	if (out.is_open())
	{
		double x, y, vx, vy, t;
		x = 1.0; vx = 0.0;
		y = 0.0; vy = 1.0;
		t = 0.0;

		out << t << '\t' << x << '\t' << y << '\n';
		for (size_t i = 0; i < n; ++i)
		{
			double tmpX, tmpY;
			tmpX = x;
			tmpY = y;
			x += dt*vx;
			y += dt*vy;
			vx -= dt*tmpX;
			vy -= dt*tmpY;
			t += dt;
			out << t << '\t' << x << '\t' << y << '\n';
		}
	}
}

void ImplicitEuler(const std::string& outFname, double dt, size_t n)
{
	std::ofstream out(outFname, std::ios::out | std::ios::trunc);
	if (out.is_open())
	{
		double x, y, vx, vy, t;
		x = 1.0; vx = 0.0;
		y = 0.0; vy = 1.0;
		t = 0.0;

		out << t << '\t' << x << '\t' << y << '\n';
		for (size_t i = 0; i < n; ++i)
		{
			x = (x + dt*vx) / (1 + dt*dt);
			y = (y + dt*vy) / (1 + dt*dt);
			vy += -dt*y;
			vx += -dt*x;
			t += dt;
			out << t << '\t' << x << '\t' << y << '\n';
		}
	}
}

void SemiImplEuler(const std::string& outFname, double dt, size_t n)
{
	std::ofstream out(outFname, std::ios::out | std::ios::trunc);
	if (out.is_open())
	{
		double x, y, vx, vy, t;
		x = 1.0; vx = 0.0;
		y = 0.0; vy = 1.0;
		t = 0.0;

		out << t << '\t' << x << '\t' << y << '\n';
		for (size_t i = 0; i < n; ++i)
		{
			vx += -dt*x;
			vy += -dt*y;
			x += dt*vx;
			y += dt*vy;
			t += dt;
			out << t << '\t' << x << '\t' << y << '\n';
		}
	}
}

struct Utype//Soustava
{
	long double x, y, vx, vy;
	Utype operator*(double f)
	{
		return {x*f,y*f,vx*f,vy*f};
	}
	Utype operator+(Utype r)
	{
		return {x + r.x,y + r.y,vx + r.vx,vy + r.vy};
	}
	friend Utype operator*(double f, const Utype& r);
};
Utype operator*(double f, const Utype& r)
{
	return Utype(r)*f;
}
void RK4(const std::string& outFname, double dt, size_t n)
{
	//Vraci derivaci soustavy
	auto derivation = [](const Utype& u)->Utype {
		return {u.vx,u.vy,-u.x,-u.y};
	};

	std::ofstream out(outFname, std::ios::out | std::ios::trunc);
	if (out.is_open())
	{
		Utype u {1.0,0.0,0.0,1.0};

		double t = 0.0;
		double x = 1.0;
		double vx = 0.0;
		out << t << '\t' << u.x << '\t' << u.y << '\n';
		for (size_t i = 0; i < n; ++i)
		{
			Utype k1, k2, k3, k4, temp;
			/*k1 = derivation(u);
			temp = u + k1*(0.5*dt);
			u = u + dt*derivation(temp);*/

			k1 = derivation(u);
			temp = u + k1*(dt / 2);
			k2 = derivation(temp);
			temp = u + k2*(dt / 2);
			k3 = derivation(temp);
			temp = u + k3*dt;
			k4 = derivation(temp);
			u = u + (dt / 6)*(k1 + (2 * k2) + (2 * k3) + k4);
			/*
			double k1x, k2x, k3x, k4x, k1vx, k2vx, k3vx, k4vx, tempX, tempVx;
			k1x = vx;
			k1vx = -x;
			tempVx = vx + k1vx*dt / 2.0;
			tempX = x + k1x*dt / 2.0;
			k2x = tempVx;
			k2vx = -tempX;
			tempVx = vx + k2vx*dt / 2.0;
			tempX = x + k2x*dt / 2.0;
			k3x = tempVx;
			k3vx = -tempX;
			tempVx = vx + k3vx*dt;
			tempX = x + k3x*dt;
			k4x = tempVx;
			k4vx = -tempX;
			vx += (dt / 6)* (k1vx + 2 * k2vx + 2 * k3vx + k4vx);
			x += (dt / 6)* (k1x + 2 * k2x + 2 * k3x + k4x);
			out << t << '\t' << x << '\n';
			*/
			t += dt;
			out << t << '\t' << u.x << '\t' << u.y << '\n';
		}
	}
}

int main()
{
	size_t time = 10;

	AnalyticSolution("analyticSol.txt", 0.1, time / 0.1);

	ExplicitEuler("explicitEuler1.txt", 0.5, time / 0.5);
	ExplicitEuler("explicitEuler2.txt", 0.1, time / 0.1);
	ExplicitEuler("explicitEuler3.txt", 0.05, time / 0.05);
	ExplicitEuler("explicitEuler4.txt", 0.01, time / 0.01);

	ImplicitEuler("implicitEuler1.txt", 0.5, time / 0.5);
	ImplicitEuler("implicitEuler2.txt", 0.1, time / 0.1);
	ImplicitEuler("implicitEuler3.txt", 0.05, time / 0.05);
	ImplicitEuler("implicitEuler4.txt", 0.01, time / 0.01);

	SemiImplEuler("semiImplEuler1.txt", 0.5, time / 0.5);
	SemiImplEuler("semiImplEuler2.txt", 0.25, time / 0.25);
	SemiImplEuler("semiImplEuler3.txt", 0.1, time / 0.1);
	SemiImplEuler("semiImplEuler4.txt", 0.01, time / 0.01);

	RK4("RK41.txt", 1, time / 1.0);
	RK4("RK42.txt", 0.5, time / 0.5);
	RK4("RK43.txt", 0.1, time / 0.1);
	RK4("RK44.txt", 0.01, time / 0.01);

	time = 100;
	SemiImplEuler("semiImplEulerStab1.txt", 0.5, time / 0.5);
	SemiImplEuler("semiImplEulerStab3.txt", 0.1, time / 0.1);
	SemiImplEuler("semiImplEulerStab4.txt", 0.01, time / 0.01);
	RK4("RK41Stab.txt", 1, time / 1.0);
	RK4("RK42Stab.txt", 0.5, time / 0.5);
	RK4("RK43Stab.txt", 0.1, time / 0.1);


	return 0;
}