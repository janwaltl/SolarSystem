class Simulation
{
public:

	Simulation(parser_p parser, simMethod_p simMethod, viewer_p viewer);
	void Start(stepTime_t dt, size_t rawMult = 1, size_t DTMult = 1,
			   seconds maxSimTime = 0);
	void StartNotTimed(stepTime_t dt, size_t rawMult = 1, seconds maxRunTime = 0);

	//Nasledující jsou řídící funkce
	void StopSimulation();
	void PauseSimulation();
	void ResumeSimulation();
	void StepSimulation();
	bool IsPaused();
	bool IsRunnig();
	double GetDtime() const;
	void SetDTime(double newDT);
	double GetRunTime() const;
	const simulatedTime& GetSimTime() const;
	void SetSimTime(simulatedTime newSimTime);
	double GetFrameTime() const;
	size_t GetRawMultiplier() const;
	size_t GetDTMultiplier() const;
	void SetRawMultiplier(size_t newRawMult);
	void SetDTMultiplier(size_t newDTMult);
};