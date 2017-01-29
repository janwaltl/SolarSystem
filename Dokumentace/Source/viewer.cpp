
class Viewer :public SystemUnit
{
public:
	//Volána pří každém průchodu smyčkou
	virtual void operator()() = 0;
	//Příprava svých dat, pokud je potřeba
	virtual void Prepare() {}
	virtual ~Viewer() = default;
protected:
	//Ukazatel na simulovaná data NENÍ validní v konstruktoru dědících tříd,
	//ale až ve volání Prepare() a operator()
	simData_t* data;
};