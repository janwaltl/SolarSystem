
struct AccVel//Derivace u(t)
{ Vec2 acc, vel; };
struct VelPos//u(t)
{ Vec2 vel, pos; };

//Pomocná proměnná pro každý objekt,
//která ukládá dočasný stav pro výpočet koeficientů
std::vector<VelPos> temps;
//Koeficienty k1,k2,k3,k4 pro každý objekt
std::array<std::vector<AccVel>, 4> kXs;

//Inicializujeme temps a vynulujeme koeficienty pro první voláni funkce.

void RK4::operator()(double step)
{
	//Vypočte koeficient
	auto computeKx = [&](size_t x, double mult)
	{
		x = x - 1;//Číslování od jedničky -  k1,k2,k3,k4
		//Projdeme všechny dvojice
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

				//Spočítáme zrychlení
				//Které uložíme do koefficientů
				kXs[x][i].acc += acc*(*data)[j].mass;
				kXs[x][j].acc -= acc*(*data)[i].mass;
			}
			//Spočítáme rychlost
			kXs[x][i].vel = left.vel;
			//Posuneme dočasný stav pro výpočet dalšího koeficientu
			left.vel = (*data)[i].vel + mult*step*kXs[x][i].acc;
			left.pos = (*data)[i].pos + mult*step*kXs[x][i].vel;
		}
	};

	computeKx(1, 0.5);
	computeKx(2, 0.5);
	computeKx(3, 1.0);
	computeKx(4, 0.0);//Už nikam neposouváme

	for (size_t i = 0; i < data->size(); ++i)
	{
		//provedeme krok podle RK4

		(*data)[i].vel += step / 6.0 *(kXs[0][i].acc + 2 * kXs[1][i].acc + 2 * kXs[2][i].acc + kXs[3][i].acc);
		(*data)[i].pos += step / 6.0 *(kXs[0][i].vel + 2 * kXs[1][i].vel + 2 * kXs[2][i].vel + kXs[3][i].vel);

		//Načteneme nový stav do dočasných proměnných
		temps[i] = {(*data)[i].vel,(*data)[i].pos};
		//Vynulujeme koeficienty
		kXs[0][i] = {Vec2(),Vec2()};
		kXs[1][i] = {Vec2(),Vec2()};
		kXs[2][i] = {Vec2(),Vec2()};
		kXs[3][i] = {Vec2(),Vec2()};
	}

}