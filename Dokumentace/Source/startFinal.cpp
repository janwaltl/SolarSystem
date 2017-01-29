void Simulation::Start(time deltaT, size_t rawMult, size_t DTMult, time maxSimTime)
{
	// Připadné uložení parametrů

	//Parser načte data
	auto data = parser->Load();
	//Možná si potřebují simMethod a viewer něco připravit ještě před simulací,
	//ale potřebují k tomu už znát data - např. jejich velikost
	simMethod->_Prepare();
	viewer->_Prepare();

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
			simMethod->Step(deltaT*DTMult); // Uděláme jeden krok simulace
			acc -= deltaT*DTMult;
			}
		}
		viewer->ViewData();// Podíváme se na simulovaná data
	}
	//Po doběhnutí simulace případně uložíme výsledná data
	parser->Save(data);
}
