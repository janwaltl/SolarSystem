class Parser : public SystemUnit
{
public:
	//Načte a vrátí data
	virtual simData_t Load() = 0;
	//Případně uloží výsledná data
	virtual void Save(const simData_t&) {};
	virtual ~Parser() = default;
};