#ifndef IMGUIVIEWER_GUI_GRAPHS_HEADER
#define IMGUIVIEWER_GUI_GRAPHS_HEADER


#include "Source/Units/Unit.h"
#include "Source/Math/Math.h"
#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imguiExtended.h"
#include "Source/Common/Timing.h"
#include <vector>
#include <memory>
#include <functional>
#include "Source/Units/PhysicsUnits.h"

namespace solar
{
	namespace gui
	{
		//Window that shows list of graphs
		//Allows creating of new LineGraphs that show various statistics about ongoing simulation
		class Graphs
		{
		public:
			Graphs();
			//Draws Graph's main window as well as any opened graph windows
			void operator()(SimData& data, const stepTime_t& realTime, const simulatedTime& simTime);
			void SelectedGraphControls();
		private:
			//Resets tempGraph variable to its basic state
			void ResetTempGraph();
			//Updates active graphs with simulation's data
			void SampleGraphs(const SimData & data, const stepTime_t& realTime, const simulatedTime& simTime);
			//Draws small preview of selected graph in graph list
			void Preview();
			void NewGraphPopUp(SimData & data, const stepTime_t& realTime, const simulatedTime& simTime);
			void ChooseXAxisUnits();
			void ChooseSpeedUnits(solar::SimData & data);
			void ChooseDistUnits(solar::SimData & data);
			void ChooseEnergyUnits(solar::SimData & data);
			void ChooseReferenceFrame(solar::SimData & data);
			void ChooseTarget(solar::SimData & data);
			void CreateNewGraph(solar::SimData & data, const stepTime_t& realTime, const simulatedTime& simTime);
			void ListOfGraphs();
			struct graphData
			{
				bool active;//Whether graph is being sampled
				std::function<bool(const stepTime_t& realTime, const simulatedTime& simTime)> sampleCond;
				std::function<double(const stepTime_t& realTime, const simulatedTime& simTime)> xSampler;
				std::function<double(const SimData&)> ySampler;
				std::unique_ptr<ImGuiE::LineGraph> graph;
				std::string name;
			};
			std::vector<graphData> graphs;
			size_t selected;//Index to vector of graphs
			bool showPreview;
			struct//Temporary data for new graph
			{
				struct//Combos for ImGui
				{
					int plottedQuantity;
					int target;
					int refFrame;
					int xAxis;
					int xAxisUnits;

					int energyUnits;
					int distUnits;
					int timeUnits;

				}combo;
				struct
				{
					PhysUnits::ratio energy;
					PhysUnits::ratio dist;
					PhysUnits::ratio time;
					PhysUnits::ratio x;
				}units;
				struct//Units of X,Y axes
				{
					std::string x, y;
				}unitLabels;
				char name[200];
			} tempGraph;
		};

	}
}

#endif