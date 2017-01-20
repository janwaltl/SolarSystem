#include "UnitsProperties.h"

#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imgui.h"


namespace solar
{
	namespace gui
	{
		const Unit UnitsViewer::centerRefSystem {};

		void UnitsViewer::operator()(simData_t & data, OMSAR * follow)
		{
			if (data.size() > 0)//If there are units
			{
				assert(0 <= selectedUnit && selectedUnit < data.size());
				assert(0 <= selectedRefUnit && selectedRefUnit < data.size() + 1);

				//Shows list of all units and offers some things to do with them
				ImGui::Text("Show info about selected unit:");

				ImGui::Combo("##SelectedUnit", &selectedUnit, UnitNameGetter, &data, data.size(), 5);
				ImGui::Text("Reference system:");
				ImGui::TextTooltipOnHover("Physical units are relative with respect to selected reference system.");

				//Zero index is reserved for centerSystem, so there is one more than data->size()
				ImGui::Combo("##SelectedRefSystem", &selectedRefUnit, RefUnitNameGetter, &data, data.size() + 1, 5);

				auto& refUnit = selectedRefUnit > 0 ? data[selectedRefUnit - 1] : centerRefSystem;
				auto& unit = data[selectedUnit];

				ImGui::Text("Properties:");
				if (ImGui::SmallButton(!following ? "Follow" : "Cancel")) following = !following;
				ImGui::TextTooltipOnHover(!following ? "Follow selected unit around" : "Stop following selected unit");

				ImGui::Columns(2, "##UnitsCol", false);
				ImGui::SetColumnOffset(1, 80.0f);

				ImGui::Text("Position:"); ImGui::NextColumn();
				ImGui::Text("X %2.6f AU", unit.pos.x - refUnit.pos.x);
				ImGui::Text("Y %2.6f AU", unit.pos.y - refUnit.pos.y); ImGui::NextColumn();

				ImGui::Text("Distance:"); ImGui::NextColumn();
				ImGui::TextTooltipOnHover("Distance from center of reference system.");
				ImGui::Text(" %2.6f AU", length(unit.pos - refUnit.pos));
				ImGui::NextColumn();

				ImGui::Text("Velocity:"); ImGui::NextColumn();
				ImGui::Text("X %+2.6f km/s", (unit.vel.x - refUnit.vel.x)*physicsUnits::AUpYtoMpS / 1000.0);
				ImGui::Text("Y %+2.6f km/s", (unit.vel.y - refUnit.vel.y)*physicsUnits::AUpYtoMpS / 1000.0);
				ImGui::NextColumn();

				ImGui::Text("Speed:"); ImGui::NextColumn();
				ImGui::Text(" %2.6f km/s", length(unit.vel - refUnit.vel)*physicsUnits::AUpYtoMpS / 1000.0);
				ImGui::NextColumn();

				ImGui::Text("Mass:"); ImGui::NextColumn();
				ImGui::Text(" %1.4e kg", unit.mass * physicsUnits::SMtoKG); ImGui::NextColumn();
				ImGui::Columns(1);
			}
			else
				ImGui::TextColored({1.0,0.0,0.0,1.0}, "There are no units.");

			if (following && follow)
				Follow(data, *follow);
		}

		bool UnitsViewer::UnitNameGetter(void * data, int index, const char ** result)
		{
			auto& simData = *reinterpret_cast<simData_t*>(data);
			*result = simData[index].name.c_str();// correct index is ensured by ImGui::Combo call
			return true;
		}

		bool UnitsViewer::RefUnitNameGetter(void * data, int index, const char ** result)
		{
			auto& simData = *reinterpret_cast<simData_t*>(data);
			if (index == 0)
				*result = centerSystem;
			else
				*result = simData[index - 1].name.c_str();// correct index is ensured by ImGui::Combo call
			return true;
		}
		void UnitsViewer::Follow(const simData_t& data, OMSAR & follow)
		{
			assert(data.size() > selectedUnit && selectedUnit >= 0);

			// Move to this position to move followed Unit to screen's center
			follow.Move(-1.0*follow.ScaleFactor()*data[selectedUnit].pos);
		}
	}
}
