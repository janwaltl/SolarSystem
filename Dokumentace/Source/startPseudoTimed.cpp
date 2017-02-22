void Simulation::Start(time deltaT,/*další parametry*/)
{
	//Beze změny

	unsigned int acc = 0;
	while (!konec)//Necháme simulaci běžet
	{
		//Akumulátor času
		acc += LastFrameTime();
		while (acc > deltaT)
		{
			simMethod->Step(deltaT); // Uděláme jeden krok simulace
			acc -= deltaT;
		}
		viewer->ViewData();// Podíváme se na simulovaná data
	}
	//Beze změny
}
