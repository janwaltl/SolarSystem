
void ExplicitEuler::operator()(double step)
{

	//Dočasná proměnná pro uložení rychlostí a poloh objektů
	std::vector<VelPos> temps < -data->vel, data->pos;

	//Projde všechny dvojice
	for (size_t i = 0; i < data->size(); ++i)
	{
		auto& left = (*data)[i];

		for (size_t j = i + 1; j < data->size(); ++j)
		{
			auto& right = (*data)[j];
			auto distLR = dist(temps[i].pos, temps[j].pos);
			distLR = distLR*distLR*distLR;//R^3

			Vec2 dir = temps[i].pos - temps[j].pos;//Vzdálenost
			Vec2 acc = -physicsUnits::G / distLR * dir;//Zrychlení bez hmotnosti
			// v(t+dt) = v(t) + dt*a(t); - explicitní Euler
			left.vel += step*acc*right.mass;// správné	hmotnosti
			right.vel -= step*acc*left.mass;// správné	hmotnosti, opačný směr
		}
		// x(t+dt) = x(t) + dt*v(t); - explicitní Euler
		left.pos += step*temps[i].vel;
	}
}
