#include "ConsoleControl.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include "Common/Common.h"
#include "Exception.h"
#include "Simulation.h"
#include "RecordedSimulation.h"
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
			//List of all passed console arguments
			using argument = std::string;
			using arguments = std::vector<argument>;
			struct simParams
			{
				Simulation::stepTime_t dt;
				size_t rawMult, DTMult;
				std::chrono::seconds maxSimTime;
			};
			void PrintHelp();

			//Runs simulation, with parametres based on other arguments
			void Simulate(const arguments& cmds);
			//Returns parser for simulation, based on arguments
			parser_p GetParser(const arguments& cmds);
			parser_p MakeFormattedParser(const arguments& cmds);
			simMethod_p GetSimMethod(const arguments& cmds);
			viewer_p GetViewer(const arguments& cmds);
			viewer_p GetWinViewer(const arguments& cmds);
			simParams GetSimParams(const arguments& cmds);

			void Record(const arguments& cmds);
			//Returns pointer(in cmds array) to argument that is directly after(=value) passed argument(=key).
			//Nullptr if key or value were not found.
			const argument* GetValue(const arguments& cmds, const argument& key);
			//Returns whether passed argument is in cmds array
			bool IsThere(const arguments& cmds, const argument& arg);
			////////////////////////////////////////////////////////
			/////////////////////IMPLEMENTATION/////////////////////
			////////////////////////////////////////////////////////
			void PrintHelp()
			{
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
				size_t width = val ? std::atoi(val->c_str()) : 1200;
				val = GetValue(cmds, "-h");
				size_t height = val ? std::atoi(val->c_str()) : 700;

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
				params.rawMult = val ? std::atoi(val->c_str()) : 1;

				val = GetValue(cmds, "-dm");
				params.DTMult = val ? std::atoi(val->c_str()) : 1;

				val = GetValue(cmds, "-x");
				params.maxSimTime = std::chrono::seconds(val ? std::atoll(val->c_str()) : 0);

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
			for (int i = 1; i < argc; ++i)//Don't record exe's name
				cmds.push_back(argv[argc]);

			if (cmds.empty())
				std::cout << "No argument passed, for help, type -help in arguments\n";
			else if (IsThere(cmds, "-help"))
				PrintHelp();
			else if (IsThere(cmds, "-sim"))
				Simulate(cmds);
			//else if (coms[0] == "-record")
			//	Record(coms);
			//else if (coms[0] == "-replay")
			//	Replay(coms);
			//else
			//	AutoFile(coms[0]);
		}
	}
}
