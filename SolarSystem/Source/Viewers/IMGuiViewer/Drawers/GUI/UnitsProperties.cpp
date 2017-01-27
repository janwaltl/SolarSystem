#include "UnitsProperties.h"

#include "Source/Units/PhysicsUnits.h"
#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imgui.h"
#include "Source/Viewers/IMGuiViewer/OMSARPolicy.h"

namespace solar
{
	namespace gui
	{
		const Unit UnitsProperties::centerRefSystem {};

		UnitsProperties::UnitsProperties(Vec2 winPos, Vec2 winSize) :
			winPos(winPos), winSize(winSize)
		{
		}

		void UnitsProperties::operator()(simData_t & data, OMSAR * follow)
		{
			ImGui::SetNextWindowPos(winPos, ImGuiSetCond_Once);
			ImGui::SetNextWindowSize(winSize, ImGuiSetCond_Once);
			if (ImGui::Begin("Units' properties", NULL, ImGuiWindowFlags_NoCollapse |
							 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
			{
				if (data.size() > 0)//If there are units
				{
					assert(0 <= selectedUnit && selectedUnit < (int)data.size());
					assert(0 <= selectedRefUnit && selectedRefUnit < (int)data.size() + 1);

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
					ImGui::NewLine();
					if (ImGui::Button(!following ? "Follow selected unit" : "Cancel following"))
						following = !following;

				}
				else
					ImGui::TextColored({1.0,0.0,0.0,1.0}, "There are no units.");
				ImGui::End();
			}

			if (following && follow)
				Follow(data, *follow);

			SelectedUnitTextLabel(data, follow);
		}

		void UnitsProperties::SelectedUnitTextLabel(solar::simData_t & data, solar::OMSAR * follow)
		{
			//Normalized position on the screen
			auto tmp = data[selectedUnit].pos*follow->ScaleFactor() + follow->GetOffset();
			tmp.x *= 600;
			tmp.y *= -350 * follow->AspectRatio();
			tmp += Vec2 {600,350 - 6};//Half window's size and 6px higher
			//tmp is now position in pixels

			ImGui::SetNextWindowPos(tmp, ImGuiSetCond_Always);
			auto flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar
				| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs;

			auto style = ImGui::GetStyle();
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {10,0});
			ImGui::PushStyleColor(ImGuiCol_WindowBg, {0.0,0.0,0.0,0.0});

			if (ImGui::Begin("Planet", nullptr, flags))
			{
				ImGui::TextColored({1.0,1.0,1.0,1.0}, data[selectedUnit].name.c_str());
				ImGui::End();
			}
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
		}

		bool UnitsProperties::UnitNameGetter(void * data, int index, const char ** result)
		{
			auto& simData = *reinterpret_cast<simData_t*>(data);
			*result = simData[index].name.c_str();// correct index is ensured by ImGui::Combo call
			return true;
		}

		bool UnitsProperties::RefUnitNameGetter(void * data, int index, const char ** result)
		{
			auto& simData = *reinterpret_cast<simData_t*>(data);
			if (index == 0)
				*result = centerSystem;
			else
				*result = simData[index - 1].name.c_str();// correct index is ensured by ImGui::Combo call
			return true;
		}

		void UnitsProperties::Follow(const simData_t& data, OMSAR & follow)
		{
			assert((int)data.size() > selectedUnit && selectedUnit >= 0);

			// Move to this position to move followed Unit to screen's center
			follow.Move(-1.0*follow.ScaleFactor()*data[selectedUnit].pos);
		}
	}
}
