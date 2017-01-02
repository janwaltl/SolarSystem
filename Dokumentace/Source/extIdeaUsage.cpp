int main()
{
	//Nejdříve simulaci zaznamenáme

	auto parser = std::make_unique<FormattedFileParser>("vstup.txt");
	auto method = std::make_unique<RK4>();
	//auto viewer = std::make_unique<IMGuiViewer>(1200, 700, "Title");
	auto viewer = std::make_unique<ViewAndRecord<IMGUIViewer>>("zaznam.txt", 1200, 700, "Title");

	Simulation record(std::move(parser), std::move(method), std::move(viewer));
	//Simulaci klasicky pustíme a nikdo ani nepozná, že je zaznamenávána
	record.Start(/*Parametry simulace...*/);

	//Pak ji můžeme přehrát
	parser = std::make_unique<ReplayerParser>("zaznam.txt");
	method = std::make_unique<ReplayerMethod>("zaznam.txt");
	viewer = std::make_unique<ReplayerViewer>(/*...*/);

	Simulation replay(std::move(parser), std::move(method), std::move(viewer));

	replay.Start(10ms);
	return 0;
}