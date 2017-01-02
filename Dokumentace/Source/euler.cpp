void SemiImplicitEuler::operator()(double step)
{
	//Projdeme všechny dvojice
	for (auto left = data->begin(); left != data->end(); ++left)
	{
		for (auto right = left + 1; right != data->end(); ++right)
		{
			auto distLR = dist(left->pos, right->pos);
			distLR = distLR*distLR*distLR;

			Vec2 dir = left->pos - right->pos;
			Vec2 acc = - kappa / distLR * dir;//Bez hmotnosti
			// v(t+dt) = v(t) + dt*acc(t) - explicitní
			left->vel += step*acc*right->mass;// Přidáme správnou hmotnost
			right->vel -= step*acc*left->mass;// a opačný směr

		}
		//x(t+dt) = x(t) + dt * v(t + dt); - implicitní
		//XXX-> je nyní v čase t+dt
		left->pos += step*left->vel;
	}
}