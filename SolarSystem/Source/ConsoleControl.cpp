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
				if (IsThere(cmds, "-cz"))
					std::cout << "Napoveda\n";
				else
					std::cout << R"(Usage:
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

        -t ['double' in seconds] - O, deltaTime for Simulation
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

    4. help: 
        -cz [] - O, Czech version
        -en [] - O, English version(default)

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
        = Recorded untimed simulation, data loaded from 'vstup.txt', parsed view 'formatted'.
          No viewer, simulated using semiImplicitEuler. Recording saved into 'out.replay'.
          Final simulated data are saved to 'out.txt'

)";
			}

			void Simulate(const arguments& cmds)
			{
				Simulation sim(GetParser(cmds), GetSimMethod(cmds), GetViewer(cmds));

				auto params = GetSimParams(cmds);

				if (IsThere(cmds, "-u"))
					sim.StartNotTimed(params.dt, params.rawMult, params.maxSimTime);
				else
					sim.Start(params.dt, params.rawMult, params.DTMult, params.maxSimTime);
			}

			parser_p GetParser(const arguments & cmds)
			{
				auto val = GetValue(cmds, "-p");

				if (val && *val == "formatted")
					return MakeFormattedParser(cmds);
				else//Default
					assert(0);//Make solar parser
			}

			parser_p MakeFormattedParser(const arguments & cmds)
			{
				auto inputFile = GetValue(cmds, "-i");
				if (!inputFile)
					throw Exception("Formatted parser needs -i argument.");
				auto outputFile = GetValue(cmds, "-o");

				if (outputFile)
					return std::make_unique<FormattedFileParser>(*inputFile, *outputFile);
				else
					return std::make_unique<FormattedFileParser>(*inputFile);
			}

			simMethod_p GetSimMethod(const arguments & cmds)
			{
				auto val = GetValue(cmds, "-m");

				if (val && *val == "semiEuler")
					return std::make_unique<SemiImplicitEuler>();
				else//Default
					return std::make_unique<RK4>();
			}

			viewer_p GetViewer(const arguments & cmds)
			{
				auto val = GetValue(cmds, "-s");

				if (val && *val == "win")
					return GetWinViewer(cmds);
				else
					return std::make_unique<EmptyViewer>();
			}

			viewer_p GetWinViewer(const arguments & cmds)
			{
				auto val = GetValue(cmds, "-w");
				size_t width = val ? std::stoi(val->c_str()) : 1200;
				val = GetValue(cmds, "-h");
				size_t height = val ? std::stoi(val->c_str()) : 700;

				return std::make_unique<IMGuiViewer>(width, height, "SolarSystem - simulation");
			}

			simParams GetSimParams(const arguments& cmds)
			{
				using namespace std::chrono_literals;
				simParams params;
				auto val = GetValue(cmds, "-t");
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

				return params;
			}

			void Record(const arguments & cmds)
			{
				auto replayFile = GetValue(cmds, "-r");
				if (!replayFile)
					throw Exception("Recorded simulation needs -r argument followed by output replay filename.");

				auto params = GetSimParams(cmds);

				RecordedSimulation sim(GetParser(cmds), GetSimMethod(cmds), GetViewer(cmds), *replayFile);
				if (IsThere(cmds, "-u"))
					sim.StartNotTimed(params.dt, params.rawMult, params.maxSimTime);
				else
					sim.Start(params.dt, params.rawMult, params.DTMult, params.maxSimTime);
			}

			void Replay(const arguments & cmds)
			{
				using namespace std::chrono_literals;

				auto replayFile = GetValue(cmds, "-r");
				if (!replayFile)
					throw Exception("Replayed simulation needs -r argument followed by input replay filename.");
				auto val = GetValue(cmds, "-w");
				size_t width = val ? std::stoi(*val) : 1200;
				val = GetValue(cmds, "-h");
				size_t height = val ? std::stoi(*val) : 700;

				ReplayedSimulation sim(*replayFile, width, height, "SolarSystem - replay: " + *replayFile);
				sim.Start(10ms, 1, 1, 0s);//Does not matter, will be set internally by parser anyway
			}

			void AutoMode(const arguments & cmds)
			{
				if (cmds.empty())
				{
					std::cout << "No arguments have been passed. Default simulation will be started. "
						"For list of all available modes, run this program with -help argument.\n";
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
						arguments newCmds {"-r",cmds[0]};
						Replay(newCmds);
					}
					else//Is formatted text(hopefully)
					{
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
				cmds.push_back(argv[argc]);

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
