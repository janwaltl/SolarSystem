
class SimMethod : public SystemUnit
{
public:
	//Provede jeden krok simulace
	virtual void operator()(double deltaT) = 0;
	//Voláno před spuštěním simulace
	virtual void Prepare() {};
	virtual ~SimMethod() = default;
protected:
	//Ukazatel na simulovaná data NENÍ validní v konstruktoru dědících tříd,
	//ale až ve volání Prepare() a operator() (=po spojeni s třídou Simulation)
	simData_t* data;
};