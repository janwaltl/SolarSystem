#include "ConsoleControl.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include "Common/Common.h"
#include "Exception.h"
#include "Simulation.h"
#include "RecordedSimulation.h"
#include "ReplayedSimulation.h"
#include "Parsers/FormattedFileParser.h"
#include "Parsers/SolarParser.h"
#include "SimMethods/SemiImplicitEuler.h"
#include "SimMethods/RK4.h"
#include "Viewers/EmptyViewer.h"
#include "Viewers/IMGuiViewer.h"
namespace solar
{
	namespace console
	{
		namespace
		{
			//////////////////////////////////////////////////////
			/////////////////////DECLARATIONS/////////////////////
			//////////////////////////////////////////////////////
			using argument = std::string;
			//List of all passed console arguments
			using arguments = std::vector<argument>;
			//Parametres need for Simulation::Start
			struct simParams
			{
				Simulation::stepTime_t dt;
				size_t rawMult, DTMult;
				std::chrono::seconds maxSimTime;
			};
			void PrintHelp(const arguments& cmds);

			//Standard simulation
			void Simulate(const arguments& cmds);
			parser_p GetParser(const arguments& cmds);
			parser_p MakeFormattedParser(const arguments& cmds);
			simMethod_p GetSimMethod(const arguments& cmds);
			viewer_p GetViewer(const arguments& cmds);
			viewer_p GetWinViewer(const arguments& cmds);
			simParams GetSimParams(const arguments& cmds);

			//Records simulation
			void Record(const arguments& cmds);

			//Replays recorded simulation
			void Replay(const arguments& cmds);

			//Automatically determine which mode to run
			void AutoMode(const arguments& cmds);


			//Returns pointer(in cmds array) to argument that is directly after(=value) passed argument(=key).
			//Nullptr if key or value were not found.
			const argument* GetValue(const arguments& cmds, const argument& key);

			//Returns whether passed argument is in cmds array
			bool IsThere(const arguments& cmds, const argument& arg);
			////////////////////////////////////////////////////////
			/////////////////////IMPLEMENTATION/////////////////////
			////////////////////////////////////////////////////////

			void PrintHelp(const arguments& cmds)
			{
				if (IsThere(cmds, "cz"))
					std::cout << R"(
For english version, use "SolarSystem -help"
Pouziti:
    SolarSystem.exe [-rezim] [-prikazy hodnoty]

Dostupne rezimy:
    sim = Standartní simulace
    record = Zaznamenavana simulace
    replay = Prehravani zaznamenane simulace
    help = Zobrazi tuto napovedu.
    [cokoliv jineho] = automaticky rezim

Nasleduje vycet veskerych prikazu(P=povinne,N=nepovinne):
(Technicke pojmy jsou podrobne vysvetleny v dokumentaci)
    1. sim:
        -m [metoda] - N, ktera metoda ma byt pouzita k simulaci
                       - dostupne metody:
                            semiEuler = semi-implicitni euler
                            RK4 - Runge Kutta ctvrteho radu (vychozi)
        -p [parser] - N, ktery parser ma byt pouzit pro nacteni dat
                    - dostupne moznosti:
                        solar - zabudovana Slunecni soustava(vychozi)
                        formatted - FormattedFileParser,
                                  - potrebuje dodatecne prikazy, viz. nize
        -v [viewer] - N, ktery viewer ma byt pouzit pro pozorovani simulace
                    - dostupne moznosti:
                        none - EmptyViewer = zadny
                        win - ImGUIViewer(vychozi) = graficke prostredi
                            - obsahuje dodatecne prikazy, viz. nize
        -u [] - N, simulace bez casovani - viz. dokumentace

        -dt ['nezaporne desetinne cislo' v sekundach] - N, deltaTime pro simulaci
                                                      - 10ms vychozi
        -rm ['nezaporne cislo'] - N, raw multiplier
                                - 1 vychozi
        -dm ['nezaporne cislo'] - N, delta time multiplier
                                - 1 vychozi
        -x ['nezaporne cislo' v sekundach] - N, maximalni doba simulace
                                           - 0 pro neomezenou (vychozi)

    1.1 dodatecne prikazy pro 'formatted' parser:
        -i [jmeno souboru] - P, ktery soubor ma byt pouzit jako vstupni
                           - vcetne cesty k souboru a jeho pripony
        -o [jmeno souboru] - N, kam ma byt ulozen vystup
                           - vcetne cesty k souboru a jeho pripony

    1.2 dodatecne prikazy pro'win' viewer:
        -w ['nezaporne cislo'] - N, sirka okna v pixelech
                                - 1200 vychozi, doporuceno minimalne 1000+
        -h ['nezaporne cislo'] - N, vyska okna v pixeles
                                - 700 vyhozi, doporuceno minimalne 700+

    2. record: 
        vsechny argumenty pro rezim 'sim'
        -r [jmeno souboru] - P, soubor, kam ma byt simulace zaznamenana
                           - vcetne cesty k souboru a jeho pripony
                           - SMAZE aktualni obsah souboru pokud existuje

    3. replay:
        -r [jmeno souboru] - P, soubor obsahujici zaznamenanou simulaci
                           - SMAZE aktualni obsah souboru pokud existuje
                           - vcetne cesty k souboru a jeho pripony

    4. help [jazyk]: 
                cz cestina
                en anglictina(vychozi)

    5. [cokoliv jineho]: Jakykoliv jiny text je povazovan za jmeno souboru
                         Pokud jsou prvni dva bajty toho souboru rovny 'R' resp. 'E',
                         tak je povazovan za validni zaznam simulace.
                         V ostatnich pripadech je povazovan jako formatovany text
                         pro rezim simulace s 'formatted' parser.

Nasleduje par prikladu volani techto prikazu pri startu aplikace:
    1.Example: SolarSystem
        = Spusti simulaci s 'solar' jako parser,'RK4' jako simMethod a 'win' jako viewer.
          Simulace probiha v realnem case a je zobrazena v 1200x700 oknì s uzivatelskym rozhranim.
    2.Example: SolarSystem vstup.txt
        = Spusti simulaci s 'formatted' jako parser,'RK4' jako simMethod a 'win' jako viewer.
          Simulace probiha v realnem case a je zobrazena v 1200x700 oknì s uzivatelskym r
          (Pouzil se automaticky rezim a vstup.txt byl rozpoznan jako formatovany text, pokud
           tedy neobsahoval 'R' a 'E' jako prvni dva bajty)
    3.Example: SolarSystem record -r out.replay -p formatted -i vstup.txt
        = Zaznamenavana simulace, vstupni data jsou nacteny z 'vstup.txt',
          zaznam ulozen do  'out.replay'.
          Zbytek stejny jako v druhem prikladu.
    SolarSystem record -r out.replay -v none -u -m semiEuler -i vstup.txt -o out.txt
        = Zaznamenavana necasovana simulace, vstupni data jsou nacteny z 'vstup.txt',
          pomoci 'formatted' parseru. SimMethod je semi-implicitni Euler, viewer neni.
          Zaznam je ulozen do 'out.replay'. Konecna simulovana data jsou ulozeny do 'out.txt'.

)";
				else
					std::cout << R"(
Pro ceskou verzi pouzijte "SolarSystem -help cz"
Usage:
    SolarSystem.exe [-mode] [-argKey argValue]

Available modes:
    sim = Standrad simulation
    record = Recorded simulation
    replay = Replays saved simulation
    help = Shows this text.
    [anything else] = Automatic mode

Following is description of all arguments(M=mandatory,O=optional):
(See documentation for explanation of used terms)
    1. sim:
        -m [simMethod] - O, which method should be used for simulation
                       - available simMethods:
                            semiEuler = semi implicit euler
                            RK4 - fourth order RungeKutta (default)
        -p [parser] - O, which parser use to obtain simulated data
                    - available parsers:
                        solar - hardcoded solar system(default)
                        formatted - FormattedFileParser,
                                  - has additional args - see below
        -v [viewer] - O, which viewer is used for viewing of simulation
                    - available names:
                        none - EmptyViewer, does nothing
                        win - ImGUIViewer(default)
                            - additional args - see below
        -u [] - O, untimed simulation - see documentation

        -dt ['double' in seconds] - O, deltaTime for Simulation
                                 - 10ms default
        -rm ['integer'] - O, raw multiplier
                        - 1 default
        -dm ['integer'] - O, delta time multiplier
                        - 1 default
        -x ['integer' in seconds] - O, max simTime in seconds,
                                  - 0 for unlimited (default)

    1.1 additional arguments for 'formatted' parser:
        -i [input filename] - M, specifies which file is used as input
                            - including path and extension.
        -o [output filename] - O, where should output be saved at
                             - including path and extension.
    
    1.2 additional arguments for 'win' viewer:
        -w ['unsigned integer'] - O, width of window in pixels
                                - 1200default, 1000+ recommended
        -h ['unsigned integer'] - O, height of window in pixels
                                - 700 default, 700+ recommended
    
    2. record: 
        all valid arguments for sim
        -r [record fileName] - M, name of the file, where replay should be saved at
                             - including path and extension.
                             - OVERWRITES any existing file or creates a new one.
    
    3. args for replay:
        -r [record fileName] - M, name of the .replay file
                             - including path and extension.

    4. help [language]: 
                cz Czech version
                en English version(default)

    5. [other]: Any other string instead of mode is treated as filename.
                If first two bytes of that file are 'R' and 'E' it is assumed to be
                a replay file and replay mode is called with that string as record's filename.
                Otherwise it is treated as formatted text file and passed to sim mode
                with 'formatted' parser and this string as its input file.

Following are examples of correct calls to this application:
    1.Example: SolarSystem
        = Starts timed simulation with 'solar' as parser,'RK4' as simMethod and 'win' as viewer.
          Simulation is played at real-time in 1200x700 window with GUI.
    
    2.Example: SolarSystem vstup.txt
        = Timed simulation with 'formatted' as parser,'RK4' as simMethod and 'win' as viewer.
          Simulation is played at real-time in 1200x700 window with GUI.
          (Used [other] mode to determine that, assuming 'vstup.txt' does not start with 'RE'.)
    3.Example: SolarSystem record -r out.replay -i vstup.txt
        = Recorded,timed simulation, data loaded from 'vstup.txt',
          record saved at 'out.replay'.
          Rest is same as in 2.Example.
    SolarSystem record -r out.replay -v none -u -m semiEuler -i vstup.txt -o out.txt
        = Recorded untimed simulation, data loaded from 'vstup.txt', parsed via 'formatted' parser.
          No viewer, simulated using semiImplicitEuler. Recording saved into 'out.replay'.
          Final simulated data are saved to 'out.txt'

)";
			}

			void Simulate(const arguments& cmds)
			{
				std::cout << "Simulation mode:\n";
				Simulation sim(GetParser(cmds), GetSimMethod(cmds), GetViewer(cmds));

				auto params = GetSimParams(cmds);

				if (IsThere(cmds, "-u"))
				{
					std::cout << "Starting untimed simulation\n";
					sim.StartNotTimed(params.dt, params.rawMult, params.maxSimTime);
				}
				else
				{
					std::cout << "Starting timed simulation\n";
					sim.Start(params.dt, params.rawMult, params.DTMult, params.maxSimTime);
				}
			}

			parser_p GetParser(const arguments & cmds)
			{
				std::cout << "\tParser: ";
				auto val = GetValue(cmds, "-p");

				if (val && *val == "formatted")
					return MakeFormattedParser(cmds);
				else//Default
				{
					std::cout << ((val && *val == "solar") ? (*val) : ("default(solar)")) << '\n';
					return std::make_unique<SolarParser>();
				}
			}

			parser_p MakeFormattedParser(const arguments & cmds)
			{
				std::cout << "formatted - ";
				auto inputFile = GetValue(cmds, "-i");
				if (!inputFile)
					throw Exception("Formatted parser needs -i argument.");
				std::cout << "input file: " << *inputFile;
				auto outputFile = GetValue(cmds, "-o");

				if (outputFile)
				{
					std::cout << "; output file:" << *outputFile << "\n";
					return std::make_unique<FormattedFileParser>(*inputFile, *outputFile);
				}
				else
				{
					std::cout << "\n";
					return std::make_unique<FormattedFileParser>(*inputFile);
				}
			}

			simMethod_p GetSimMethod(const arguments & cmds)
			{
				std::cout << "\tSimMethod: ";
				auto val = GetValue(cmds, "-m");

				if (val && *val == "semiEuler")
				{
					std::cout << "semiEuler\n";
					return std::make_unique<SemiImplicitEuler>();
				}
				else//Default
				{
					std::cout << ((val&&*val == "RK4") ? (*val) : ("default(RK4)")) << '\n';
					return std::make_unique<RK4>();
				}
			}

			viewer_p GetViewer(const arguments & cmds)
			{
				std::cout << "\tViewer: ";
				auto val = GetValue(cmds, "-v");

				if (val && *val == "none")
				{
					std::cout << "none\n";
					return std::make_unique<EmptyViewer>();
				}
				else
					return GetWinViewer(cmds);
			}

			viewer_p GetWinViewer(const arguments & cmds)
			{
				std::cout << "win - ";
				auto val = GetValue(cmds, "-w");
				size_t width = val ? std::stoi(*val) : 1200;
				val = GetValue(cmds, "-h");
				size_t height = val ? std::stoi(*val) : 700;
				std::cout << "width=" << width << " height=" << height << '\n';
				return std::make_unique<IMGuiViewer>(width, height, "SolarSystem - simulation");
			}

			simParams GetSimParams(const arguments& cmds)
			{
				std::cout << "\tParametres: ";
				using namespace std::chrono_literals;
				simParams params;
				auto val = GetValue(cmds, "-dt");
				using time_type = decltype(params.dt);
				if (val)
					params.dt = time_type(static_cast<time_type::rep>(std::atof(val->c_str())*
																	  time_type::period::den / time_type::period::num));
				else
					params.dt = 10ms;
				val = GetValue(cmds, "-rm");
				params.rawMult = val ? std::stoi(val->c_str()) : 1;

				val = GetValue(cmds, "-dm");
				params.DTMult = val ? std::stoi(val->c_str()) : 1;

				val = GetValue(cmds, "-x");
				params.maxSimTime = std::chrono::seconds(val ? std::stoll(val->c_str()) : 0);

				std::cout << "dt=" << params.dt.count() / time_type::period::den << "s rm=" << params.rawMult;
				std::cout << " dm=" << params.DTMult;
				if (params.maxSimTime == params.maxSimTime.zero())
					std::cout << " x=unlimited\n";
				else
					std::cout << "x=" << params.maxSimTime.count() << "s\n";

				return params;
			}

			void Record(const arguments & cmds)
			{
				std::cout << "Recorded simulation mode:\n";
				auto replayFile = GetValue(cmds, "-r");
				if (!replayFile)
					throw Exception("Recorded simulation needs -r argument followed by output replay filename.");
				std::cout << "\tReplay file: " << *replayFile << '\n';
				auto params = GetSimParams(cmds);

				RecordedSimulation sim(GetParser(cmds), GetSimMethod(cmds), GetViewer(cmds), *replayFile);
				if (IsThere(cmds, "-u"))
				{
					std::cout << "Starting untimed recorded simulation\n";
					sim.StartNotTimed(params.dt, params.rawMult, params.maxSimTime);
				}
				else
				{
					std::cout << "Starting timed recorded simulation\n";
					sim.Start(params.dt, params.rawMult, params.DTMult, params.maxSimTime);
				}
			}

			void Replay(const arguments & cmds)
			{
				using namespace std::chrono_literals;
				std::cout << "Replay mode:\n";
				auto replayFile = GetValue(cmds, "-r");
				if (!replayFile)
					throw Exception("Replayed simulation needs -r argument followed by input replay filename.");
				std::cout << "\t replayFile - " << *replayFile << '\n';
				auto val = GetValue(cmds, "-w");
				size_t width = val ? std::stoi(*val) : 1200;
				val = GetValue(cmds, "-h");
				size_t height = val ? std::stoi(*val) : 700;
				std::cout << "Starting replay simulation";
				ReplayedSimulation sim(*replayFile, width, height, "SolarSystem - replay: " + *replayFile);
				sim.Start(10ms, 1, 1, 0s);//Does not matter, will be set internally by parser anyway
			}

			void AutoMode(const arguments & cmds)
			{
				std::cout << "Auto mode:\n";
				if (cmds.empty())
				{
					std::cout << "\tNo arguments have been passed. Default solar simulation will be started. \n"
						"\tFor list of all available modes and their arguments, run this program with -help argument.\n";

					Simulate(cmds);
				}
				else
				{
					std::ifstream file(cmds[0], std::ios::in | std::ios::binary);
					char magic[2] = {0,0};
					if (file.is_open())
						file.read(magic, 2);
					if (magic[0] == 'R' && magic[1] == 'E')//Is replay
					{
						std::cout << "\t" << cmds[0] << " recognized as replay file \n";
						arguments newCmds {"-r",cmds[0]};
						Replay(newCmds);
					}
					else//Is formatted text(hopefully)
					{
						std::cout << "\t" << cmds[0] << " treated as formatted text file.\n";
						arguments newCmds {"-i",cmds[0],"-p","formatted"};
						Simulate(newCmds);
					}
				}
			}

			const argument* GetValue(const arguments & cmds, const argument & key)
			{
				auto IT = std::find(cmds.begin(), cmds.end(), key);
				if (IT != cmds.end() && ++IT != cmds.end())
					return &*IT;
				else
					return nullptr;
			}

			bool IsThere(const arguments & cmds, const argument & arg)
			{
				return std::find(cmds.begin(), cmds.end(), arg) != cmds.end();
			}
		}

		void Control(int argc, char * argv[])
		{
			arguments cmds;
			for (int i = 1; i < argc; ++i)//Ignore exe's name
				cmds.push_back(argv[i]);

			if (IsThere(cmds, "-help"))
				PrintHelp(cmds);
			else if (IsThere(cmds, "-sim"))
				Simulate(cmds);
			else if (IsThere(cmds, "-record"))
				Record(cmds);
			else if (IsThere(cmds, "-replay"))
				Replay(cmds);
			else
				AutoMode(cmds);
		}
	}
}
