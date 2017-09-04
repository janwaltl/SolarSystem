#include "Graphs.h"
#include <imgui/imgui.h>
#include <imgui/imguiExtended.h>

#include <sstream>
#include <cstring>
namespace solar
{
	namespace
	{
		const char* newGraphPopUp = "NewGraph##PopUp";
		const size_t noneSelected = SIZE_MAX;
		const float comboOffset = 200.0f;
		//Enums for individual ImGui's combos
		namespace plottedQuantity
		{
			enum : int
			{
				systemEnergy = 0,
				objectEnergy = 1,
				distance = 2,
				speed = 3,
			};
		}
		const char* plottedQunatityStr = "Energy of the system\0Energy(Kin+Potencial)\0Distance\0Speed";
		namespace energyUnits
		{
			enum : int
			{
				joules = 0,
				terajoules = 1,
			};
		}
		const char* energyUnitsStr = "Joules\0Terajoules";
		namespace distUnits
		{
			enum : int
			{
				metres = 0,
				kilometres = 1,
				AU = 2,
			};
		}
		const char* distUnitsStr = "Metres\0Kilometres\0AU\0";
		namespace timeUnits
		{
			enum : int
			{
				secs = 0,
				mins = 1,
				hours = 2,
				days = 3,
				years = 4,
			};
		}
		const char* timeUnitsStr = "Seconds\0Minutes\0Hours\0Days\0Years\0";
		namespace xAxis
		{
			enum : int
			{
				realTime = 0,
				simTIme = 1,
			};
		}
		const char* xAxisStr = "RealTime\0SimTime\0";

		bool UnitNameGetter(void * data, int index, const char ** result)
		{
			auto& simData = *reinterpret_cast<SimData*>(data);
			*result = simData[index].name.c_str();// correct index is ensured by ImGui::Combo call
			return true;
		}
		bool UnitNameGetterOrigin(void * data, int index, const char ** result)
		{
			if (index == 0)
				*result = "<Origin>";
			else
			{
				auto& simData = *reinterpret_cast<SimData*>(data);
				*result = simData[index - 1].name.c_str();// correct index is ensured by ImGui::Combo call
			}
			return true;
		}
		double ComputeSystemEnergy(const SimData& data)
		{
			double potential = 0.0, kinetic = 0.0;
			for (size_t left = 0; left < data->size(); ++left)
			{
				kinetic += 0.5*data[left].mass *data[left].vel.Length();
				for (size_t right = left + 1; right < data->size(); ++right)
					potential += -G<double>*data[left].mass*data[right].mass / (data[right].pos - data[left].pos).LengthSq();
			}
			return potential + kinetic;
		};
	}
	gui::Graphs::Graphs()
	{
		showPreview = false;
		selected = noneSelected;

		ResetTempGraph();
	}
	void gui::Graphs::ResetTempGraph()
	{
		tempGraph.combo.plottedQuantity = plottedQuantity::systemEnergy;
		tempGraph.combo.energyUnits = energyUnits::joules;
		tempGraph.combo.refFrame = 0;
		tempGraph.combo.target = 0;
		tempGraph.combo.xAxis = xAxis::realTime;
		tempGraph.combo.xAxisUnits = timeUnits::secs;
		tempGraph.xRange[0] = tempGraph.yRange[0] = -1.0f;
		tempGraph.xRange[1] = tempGraph.yRange[1] = +1.0f;
		strcpy(tempGraph.name, "New graph");
	}
	void gui::Graphs::operator()(SimData & data, const stepTime_t& realTime, const simulatedTime& simTime)
	{
		SampleGraphs(data, realTime, simTime);
		auto menuBarHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;

		ImGui::SetNextWindowPos(ImVec2(0.0f, menuBarHeight), ImGuiSetCond_Once);
		ImGui::SetNextWindowSize(ImVec2(showPreview ? 730.0f : 500.0f, 250.0f), ImGuiSetCond_Always);
		if (ImGui::Begin("Graphs", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
		{
			ImGui::BeginGroup();
			ImGui::AlignFirstTextHeightToWidgets();
			ImGui::Text("List of graphs:"); ImGui::SameLine(415);
			if (ImGui::Button("New graph"))
				ImGui::OpenPopup(newGraphPopUp);
			NewGraphPopUp(data, realTime, simTime);

			ListOfGraphs();
			SelectedGraphControls();
			ImGui::EndGroup();
			ImGui::SameLine();
			if (selected != noneSelected)
			{
				ImGui::BeginGroup();
				Preview();
				ImGui::EndGroup();
			}
		}
		ImGui::End();

		DrawSeparateGraphWindows();
	}

	void gui::Graphs::DrawSeparateGraphWindows()
	{
		for (auto& graph : graphs)
		{
			if (graph.separateWindow)
			{
				if (ImGui::Begin(graph.name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{
					graph.graph->SetSize(ImVec2 {300.0f,300.0f});
					graph.graph->Draw();

					ImGui::AlignFirstTextHeightToWidgets(); ImGui::Text("Autoscale:");
					ImGui::SameLine();
					if (ImGui::Checkbox("X", &graph.autoScale.x))
						graph.graph->Autoscale(graph.autoScale.x, graph.autoScale.y);
					ImGui::SameLine();
					if (ImGui::Checkbox("Y", &graph.autoScale.y))
						graph.graph->Autoscale(graph.autoScale.x, graph.autoScale.y);

					if (ImGui::Button("Show all"))
						graph.graph->ShowAll();
					ImGui::SameLine();
					if (ImGui::Button("Clear"))
						graph.graph->Clear();
					ImGui::SameLine();
					if (ImGui::Button("Close"))
						graph.separateWindow = false;
				}
				ImGui::End();
			}
		}
	}

	void gui::Graphs::SelectedGraphControls()
	{
		if (selected != noneSelected)
		{
			if (ImGui::Button(graphs[selected].active ? "Pause" : "Resume"))
				graphs[selected].active = !graphs[selected].active;
			ImGui::SameLine();
			if (ImGui::Button("Remove"))
			{
				graphs.erase(graphs.begin() + selected);
				selected = noneSelected;
				if (graphs.size() == 0)
					showPreview = false;
			}
			ImGui::SameLine();
			if (ImGui::Button("Open in separate window"))
				graphs[selected].separateWindow = true;
			ImGui::SameLine(395);
			if (ImGui::Button(showPreview ? "Hide preview" : "Show preview"))
				showPreview = !showPreview;
		}
	}

	void gui::Graphs::SampleGraphs(const solar::SimData & data, const stepTime_t& realTime, const simulatedTime& simTime)
	{
		for (auto& graph : graphs)
			if (graph.active && graph.sampleCond(realTime, simTime))
			{
				auto point = ImVec2(graph.xSampler(realTime, simTime), graph.ySampler(data));
				graph.graph->AddPoint(point);
			}
	}
	void gui::Graphs::Preview()
	{
		graphs[selected].graph->SetSize(ImVec2 {150.0f,150.0f});
		graphs[selected].graph->Draw();
	}
	void gui::Graphs::NewGraphPopUp(SimData & data, const stepTime_t& realTime, const simulatedTime& simTime)
	{
		if (ImGui::BeginPopupModal(newGraphPopUp, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::AlignFirstTextHeightToWidgets();
			ImGui::PushItemWidth(215);
			ImGui::Text("Plot"); ImGui::SameLine(comboOffset);
			ImGui::Combo("##Quantity", &tempGraph.combo.plottedQuantity, plottedQunatityStr);
			switch (tempGraph.combo.plottedQuantity)
			{
			case plottedQuantity::systemEnergy:
				//ImGui::Text("Tracks change of energy, which should be zero -> Shows error of simulation");
				ChooseEnergyUnits(data);
				break;
			case plottedQuantity::objectEnergy:
				ChooseTarget(data);
				ChooseReferenceFrame(data);
				ChooseEnergyUnits(data);
				break;
			case plottedQuantity::distance:
				ChooseTarget(data);
				ChooseReferenceFrame(data);
				ChooseDistUnits(data);
				break;
			case plottedQuantity::speed:
				ChooseTarget(data);
				ChooseReferenceFrame(data);
				ChooseSpeedUnits(data);
				break;
			}

			ChooseXAxisUnits();

			ImGui::Text("Graph's name");
			ImGui::SameLine(comboOffset);
			ImGui::InputText("##NameOfGraph", tempGraph.name, sizeof(tempGraph.name) / sizeof(tempGraph.name[0]));
			ImGui::Text("XRange"); ImGui::SameLine(comboOffset); ImGui::InputFloat2("##XRange", tempGraph.xRange);
			ImGui::Text("YRange"); ImGui::SameLine(comboOffset); ImGui::InputFloat2("##YRange", tempGraph.yRange);

			if (ImGui::Button("Create"))
			{
				CreateNewGraph(data, realTime, simTime);
				ResetTempGraph();
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("close"))
			{
				ResetTempGraph();
				ImGui::CloseCurrentPopup();
			}

			ImGui::PopItemWidth();
			ImGui::EndPopup();
		}
	}
	void gui::Graphs::ChooseXAxisUnits()
	{
		ImGui::AlignFirstTextHeightToWidgets();
		ImGui::Text("X-axis"); ImGui::SameLine(comboOffset);
		ImGui::Combo("##Xaxis", &tempGraph.combo.xAxis, xAxisStr);
		ImGui::Text("X-axis units"); ImGui::SameLine(comboOffset);
		ImGui::Combo("##XUnits", &tempGraph.combo.xAxisUnits, timeUnitsStr);
		switch (tempGraph.combo.xAxisUnits)
		{
		case timeUnits::secs:
			tempGraph.unitLabels.x = "secs";
			tempGraph.units.x = PhysUnits::second; break;
		case timeUnits::mins:
			tempGraph.unitLabels.x = "mins";
			tempGraph.units.x = PhysUnits::minute; break;
		case timeUnits::hours:
			tempGraph.unitLabels.x = "hours";
			tempGraph.units.x = PhysUnits::hour; break;
		case timeUnits::days:
			tempGraph.unitLabels.x = "days";
			tempGraph.units.x = PhysUnits::day; break;
		case timeUnits::years:
			tempGraph.unitLabels.x = "years";
			tempGraph.units.x = PhysUnits::year; break;
		}
	}
	void gui::Graphs::ChooseSpeedUnits(solar::SimData & data)
	{
		ImGui::AlignFirstTextHeightToWidgets();
		ImGui::PushItemWidth(100);
		ImGui::Text("Units of speed:"); ImGui::SameLine(comboOffset);
		ImGui::Combo("##DistUnits", &tempGraph.combo.distUnits, distUnitsStr);
		switch (tempGraph.combo.distUnits)
		{
		case distUnits::metres:
			tempGraph.units.dist = data.RatioOfDistTo(PhysUnits::meter); break;
		case distUnits::kilometres:
			tempGraph.units.dist = data.RatioOfDistTo(PhysUnits::kilometer); break;
		case distUnits::AU:
			tempGraph.units.dist = data.RatioOfDistTo(PhysUnits::AU); break;
		}
		ImGui::SameLine(); ImGui::Text("/"); ImGui::SameLine();
		ImGui::Combo("##TimeUnits", &tempGraph.combo.timeUnits, timeUnitsStr);
		switch (tempGraph.combo.timeUnits)
		{
		case timeUnits::secs:
			tempGraph.units.time = data.RatioOfDistTo(PhysUnits::second); break;
		case timeUnits::mins:
			tempGraph.units.time = data.RatioOfDistTo(PhysUnits::minute); break;
		case timeUnits::hours:
			tempGraph.units.time = data.RatioOfDistTo(PhysUnits::hour); break;
		case timeUnits::days:
			tempGraph.units.time = data.RatioOfDistTo(PhysUnits::day); break;
		case timeUnits::years:
			tempGraph.units.time = data.RatioOfDistTo(PhysUnits::year); break;
		}
		ImGui::PopItemWidth();
	}
	void gui::Graphs::ChooseDistUnits(solar::SimData & data)
	{
		ImGui::AlignFirstTextHeightToWidgets();
		ImGui::Text("Units of distance"); ImGui::SameLine(comboOffset);
		ImGui::Combo("##DistUnits", &tempGraph.combo.distUnits, distUnitsStr);
		switch (tempGraph.combo.distUnits)
		{
		case distUnits::metres:
			tempGraph.unitLabels.y = "m";
			tempGraph.units.dist = data.RatioOfDistTo(PhysUnits::meter); break;
		case distUnits::kilometres:
			tempGraph.unitLabels.y = "km";
			tempGraph.units.dist = data.RatioOfDistTo(PhysUnits::kilometer); break;
		case distUnits::AU:
			tempGraph.unitLabels.y = "AU";
			tempGraph.units.dist = data.RatioOfDistTo(PhysUnits::AU); break;
		}
	}
	void gui::Graphs::ChooseEnergyUnits(solar::SimData & data)
	{
		ImGui::AlignFirstTextHeightToWidgets();
		ImGui::Text("Units of energy"); ImGui::SameLine(comboOffset);
		ImGui::Combo("##EnergyUnits", &tempGraph.combo.energyUnits, energyUnitsStr);

		tempGraph.units.energy = 1.0;//Joules
		tempGraph.unitLabels.y.clear();
		switch (tempGraph.combo.energyUnits)
		{
		case energyUnits::terajoules:
			tempGraph.units.energy /= 10e9;
			tempGraph.unitLabels.y.push_back('T');
		case energyUnits::joules://Joules
			auto mass = data.RatioOfMassTo(PhysUnits::kilogram);
			auto dist = data.RatioOfDistTo(PhysUnits::meter);
			auto time = data.RatioOfTimeTo(PhysUnits::second);
			tempGraph.units.energy *= mass*dist*dist / time / time;
			tempGraph.unitLabels.y.push_back('J');
			break;
		}
	}
	void gui::Graphs::ChooseReferenceFrame(solar::SimData & data)
	{
		ImGui::AlignFirstTextHeightToWidgets();
		ImGui::Text("Frame of reference");
		ImGui::TextTooltipOnHover("Quantity is measured with respect to this object.");
		ImGui::SameLine(comboOffset);
		ImGui::Combo("##RefFrame", &tempGraph.combo.refFrame, UnitNameGetterOrigin, &data, data->size() + 1);
	}
	void gui::Graphs::ChooseTarget(solar::SimData & data)
	{
		ImGui::AlignFirstTextHeightToWidgets();
		ImGui::Text("Target"); ImGui::SameLine(comboOffset);
		ImGui::Combo("##Target", &tempGraph.combo.target, UnitNameGetter, &data, data->size());
	}
	void gui::Graphs::CreateNewGraph(solar::SimData & data, const stepTime_t& realTime, const simulatedTime& simTime)
	{
		graphData newGraph;
		std::string xLabel, yLabel;

		newGraph.name = tempGraph.name;
		newGraph.autoScale = {false,false};
		switch (tempGraph.combo.plottedQuantity)//Create sampler for chosen quantity
		{
		case plottedQuantity::systemEnergy:
		{
			yLabel = "Energy of system";
			auto currentEnergy = ComputeSystemEnergy(data);
			newGraph.ySampler = [currentEnergy, units = tempGraph.units.energy](const SimData& data) ->float {
				return static_cast<float>((ComputeSystemEnergy(data) - currentEnergy)*units); };
			break;
		}
		case plottedQuantity::objectEnergy:
			yLabel = data[tempGraph.combo.target].name + "'s energy";
			//TODO implement
			newGraph.ySampler = [units = tempGraph.units.energy](const SimData& data)->float { return static_cast<float>(1.0*units); };
			break;
		case plottedQuantity::distance:
			yLabel = data[tempGraph.combo.target].name + "-" + data[tempGraph.combo.refFrame].name + " distance";
			newGraph.ySampler = [target = tempGraph.combo.target, ref = tempGraph.combo.refFrame, units = tempGraph.units.dist](const SimData& data)->float {
				return static_cast<float>((data[target].pos - data[ref].pos).Length()*units); };
			break;
		case plottedQuantity::speed:
			yLabel = "Speed of " + data[tempGraph.combo.target].name + " w.r.t." + data[tempGraph.combo.refFrame].name;

			newGraph.ySampler = [target = tempGraph.combo.target, ref = tempGraph.combo.refFrame, distUnits = tempGraph.units.dist, timeUnits = tempGraph.units.time](const SimData& data)->float {
				return static_cast<float>((data[target].vel - data[ref].vel).Length()*distUnits / timeUnits); };
			break;
		}
		switch (tempGraph.combo.xAxis)
		{
		case xAxis::realTime:
			{
				xLabel = "RealTime";
				auto step = stepTime_t(static_cast<stepTime_t::rep>(tempGraph.units.x*stepTime_t::period::den));
				newGraph.sampleCond = [counter = realTime, step = step](const stepTime_t& realTime, const simulatedTime&) mutable {
					if (realTime - counter > step)
					{
						counter = realTime;
						return true;
					}
					else
						return false; };
				newGraph.xSampler = [units = tempGraph.units.x](const stepTime_t& realTime, const simulatedTime&)->float {
					return static_cast<float>(realTime.count() / float(stepTime_t::period::den) * units); };//Convert to seconds, then to desired units
				break;
			}
		case xAxis::simTIme:
			{
				xLabel = "SimTime";
				simulatedTime stepS(1.0*tempGraph.units.x);
				newGraph.sampleCond = [counter = simTime, step = stepS](const stepTime_t&, const simulatedTime& simTime) mutable {
					if (simTime - counter > step)
					{
						counter = simTime;
						return true;
					}
					else
						return false; };
				newGraph.xSampler = [units = tempGraph.units.x](const stepTime_t&, const simulatedTime& simTime)->float {
					return static_cast<float>(simTime.seconds.count()*units); };//Convert to seconds, then to desired units
				break;
			}
		}

		xLabel = xLabel + "[" + tempGraph.unitLabels.x + "]";
		yLabel = yLabel + "[" + tempGraph.unitLabels.y + "]";
		ImVec2 xRange {tempGraph.xRange[0],tempGraph.xRange[1]};
		ImVec2 yRange {tempGraph.yRange[0],tempGraph.yRange[1]};
		newGraph.graph = std::make_unique<ImGuiE::LineGraph>(ImVec2(150, 150), xRange, yRange, 1000, xLabel, yLabel);
		newGraph.active = true;
		newGraph.separateWindow = false;
		graphs.emplace_back(std::move(newGraph));
		selected = graphs.size() - 1;//Select it
	}
	void gui::Graphs::ListOfGraphs()
	{
		if (ImGui::BeginChild("List of graphs", ImVec2(500, 150), false, ImGuiWindowFlags_AlwaysUseWindowPadding))
		{
			if (graphs.size() == 0)
				ImGui::Text("There are no graphs currently present.");
			else
			{
				ImGui::Columns(3);
				ImGui::SetColumnOffset(1, 20);
				ImGui::SetColumnOffset(2, 400);
				size_t i = 0;
				for (const auto& g : graphs)
				{
					ImGui::PushID(i);
					ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.90f, 0.78f, 0.00f, 1.00f));
					if (ImGui::Selectable(std::to_string(i + 1).c_str(), i == selected, ImGuiSelectableFlags_SpanAllColumns))
						selected = i;
					ImGui::PopStyleColor();
					ImGui::NextColumn();
					ImGui::Text(g.name.c_str());
					ImGui::NextColumn();
					ImGui::Text(g.active ? "Sampling" : "Paused");
					ImGui::NextColumn();
					ImGui::Separator();
					ImGui::PopID();
					i++;
				}
			}
		}
		ImGui::EndChild();
	}
}
