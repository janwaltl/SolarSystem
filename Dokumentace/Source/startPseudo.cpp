void Simulation::Start(/*Parametry simulace*/)
{
	//Uložíme si parametry simulace

	//Parser načte data
	auto data = parser->Load();
	//Možná si potřebují simMethod a viewer něco připravit ještě před simulací,
	//ale potřebují k tomu už znát data - např. jejich velikost
	simMethod->Prepare();
	viewer->Prepare();
	
	while(!konec)//Necháme simulaci běžet
	{
		simMethod->Step(); // Uděláme jeden krok simulace
		viewer->ViewData();// Podíváme se na simulovaná data
	}
	//Po doběhnutí simulace případně uložíme výsledná data
	parser->Save(data);
}
