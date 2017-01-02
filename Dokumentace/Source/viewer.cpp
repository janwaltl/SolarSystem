
class Viewer :public SystemUnit
{
public:
	//Volána každý průchod smyčkou
	virtual void operator()() = 0;
	//Příprava svých dat, pokud je potřeba
	virtual void Prepare() {}
	virtual ~Viewer() = default;
	//Voláno jen ze třídy Simulation, slouží k incializaci ukazatele na data.
	void _Prepare(simData_t* simData);
protected:
	//Ukazatel na simulovaná data NENÍ validní v konstruktoru dědících tříd,
	//ale až ve volání Prepare() a operator()
	simData_t* data;
};