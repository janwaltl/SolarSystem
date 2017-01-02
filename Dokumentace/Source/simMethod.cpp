
class SimMethod : public SystemUnit
{
public:
	//Provede jeden krok simulace
	virtual void operator()(double deltaT) = 0;
	virtual void Prepare() {};
	virtual ~SimMethod() = default;
	//Voláno jen ze třídy Simulation, slouží k incializaci ukazatele na data.
	void _Prepare(simData_t* simData);
protected:
	//Ukazatel na simulovaná data NENÍ validní v konstruktoru dědících tříd,
	//ale až ve volání Prepare() a operator()
	simData_t* data;
};