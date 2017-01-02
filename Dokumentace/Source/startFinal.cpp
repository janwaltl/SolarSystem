void Simulation::Start(time deltaT,time rawMult, time DTMult, time maxSimTime)
{
	// Připadné uložení parametrů

	//Parser načte data
	auto data = parser->Load();
	//Možná si potřebují simMethod a viewer něco připravit ještě před simulací,
	//ale potřebují k tomu už znát data - např. jejich velikost
	//Moduly tímto také dostanou přístup k datům.
	simMethod->_Prepare(&data);
	viewer->_Prepare(&data);

	auto acc = 0;

	//Simulace může být ukončena uplynutím zadaného času
	while (!konec && ElapsedTime()<maxSimTime)
	{
		//Akumulátor času
		acc += LastFrameTime();
		while (acc > deltaT)
		{
			for (size_t i = 0; i < rawMult; i++)
			{
			simMethod->Step(deltaT*); // Uděláme jeden krok simulace
			acc -= deltaT;
			}
		}
		viewer->ViewData();// Podíváme se na simulovaná data
	}

	//Po doběhnutí simulace případně uložíme výsledná data
	parser->Save(data);
}
