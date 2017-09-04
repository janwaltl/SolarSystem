class ViewAndRecord : public Viewer
{
public:
	//Zaznamenává simulaci do souboru 'outFileName'
	ViewAndRecord(const std::string& outFileName, viewer_p viewer);
	~ViewAndRecord()override final;
	void Prepare() override final;
	void operator()() override final;
private:
	//Implementační detaily...
};

class RecordedSimulation
{
public:
	//Outfile - soubor kam se uloží záznam
	RecordedSimulation(parser_p parser, simMethod_p simMethod, viewer_p viewer,
					   const std::string& outFile) :
		sim(std::move(parser), std::move(simMethod),
			std::make_unique<ViewAndRecord>(outFile, std::move(viewer)))
	{
	}
	//Stejné funkce pro spuštění,které zavolají sim.XXX() verzi.
	void Start(stepTime_t dt, size_t rawMult, size_t DTMult, seconds maxRunTime);
	void StartNotTimed(stepTime_t dt, size_t rawMult, seconds maxRunTime);
private:
	Simulation sim;
};
