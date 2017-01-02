
template<typename someViewer>
class ViewAndRecord :public Viewer{
public:
	template<typename... someViewerArgs>
	ViewAndRecord(std::string outFile, someViewerArgs&&.. args):
		outFile(outFile),viewer(std::forward<soViewerArgs...>(args...))
	void operator()(){
		//Necháme simulaci přehrávat libovolným způsobem
		viewer();
		//Ale poznamenáme si aktuální data, včetně aktuálního času do souboru
		AddToFile(simTime, data);
	}
	//Prepare metoda
private:
	void AddToFile();
	someViewer viewer;
	std::string outFile;//Soubor kam budeme provádět záznam
};

class ReplayerParser:public Parser
{
public:
	ReplayerParser(std::string inFile)
	simData_t Load();
};

class ReplayerMethod :public SimMethod
public:
	ReplayerMethod(std::string inFile);
	void operator()(time deltaT){
		simTime += deltaT;
		//Načteme data ve správný čas
		data = GetDataFromFile(simTime);
	}
	void 
private:
	void GetDataFromFile();
	time simTime;
};

class ReplayerViewer :public solar::Viewer{
public:
	template<typename... someViewerArgs>
	ReplayerViewer(ReplayerMethod*);
	void operator()(){
		//Necháme vykreslit uživatelské rozhraní
		viewer();
		//Dokreslíme si potřebné ovládání pro přehrávání
		DrawReplayControls();
	}
private:
	void DrawReplayControls();
	solar::IMGuiViewer viewer;
	ReplayerMethod* method;
};