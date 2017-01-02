class Simulation
{
public:
	using clock_t = std::chrono::high_resolution_clock;
	using stepTime_t = clock_t::duration;
	
	using parser_p = std::unique_ptr<Parser>;
	using simMethod_p = std::unique_ptr<SimMethod>;
	using viewer_p = std::unique_ptr<Viewer>;

	Simulation(parser_p parser, simMethod_p simMethod, viewer_p viewer);
	void Start(stepTime_t dt, size_t rawMult = 1, size_t DTMult = 1,
			   std::chrono::seconds maxSimTime = std::chrono::seconds::zero());
	
	//Nasledující jsou řídící funkce

	void StopSimulation();
	void PauseSimulation();
	void ResumeSimulation();
	void StepSimulation();
	bool IsPaused();
	bool IsRunnig();
	double GetDtime();
	double GetRunTime();
	double GetSimTime();
	double GetFrameTime();
	size_t GetRawMultiplier();
	size_t GetDTMultiplier();
	void SetRawMultiplier(size_t newRawMult);
	void SetDTMultiplier(size_t newDTMult);
};