#include "UnitsProperties.h"

#include "Source/Units/PhysicsUnits.h"
#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imgui.h"

namespace solar
{
	namespace gui
	{
		const Unit UnitsProperties::centerRefSystem {};

		UnitsProperties::UnitsProperties(Vec2d winPos, Vec2d winSize) :
			winPos(winPos), winSize(winSize)
		{}

		void UnitsProperties::operator()(SimData & data)
		{
			ImGui::SetNextWindowPos(winPos, ImGuiSetCond_Once);
			ImGui::SetNextWindowSize(winSize, ImGuiSetCond_Once);
			if (ImGui::Begin("Units' properties", NULL))
			{
				if (data->size() > 0)//If there are units
				{
					assert(0 <= selectedUnit && selectedUnit < (int)data->size());
					assert(0 <= selectedRefUnit && selectedRefUnit < (int)data->size() + 1);

					//Shows list of all units and offers some things to do with them
					ImGui::Text("Show info about selected unit:");

					ImGui::Combo("##SelectedUnit", &selectedUnit, UnitNameGetter, &data, data->size(), 5);
					ImGui::Text("Reference system:");
					ImGui::TextTooltipOnHover("Physical units are relative with respect to selected reference system.");

					//Zero index is reserved for centerSystem, so there is one more than data->size()
					ImGui::Combo("##SelectedRefSystem", &selectedRefUnit, RefUnitNameGetter, &data, data->size() + 1, 5);

					auto& refUnit = selectedRefUnit > 0 ? data.Get()[selectedRefUnit - 1] : centerRefSystem;
					auto& unit = data.Get()[selectedUnit];
					auto distRatio = data.RatioOfDistTo(PhysUnits::AU);
					auto speedRatio = data.RatioOfDistTo(PhysUnits::kilometer) / data.RatioOfTimeTo(PhysUnits::second);
					auto massRatio = data.RatioOfMassTo(PhysUnits::kilogram);
					ImGui::Text("Properties:");
					ImGui::Columns(2, "##UnitsCol", false);
					ImGui::SetColumnOffset(1, 80.0f);

					ImGui::Text("Position:"); ImGui::NextColumn();
					ImGui::Text("X %2.6f AU", (unit.pos.x - refUnit.pos.x)*distRatio);
					ImGui::Text("Y %2.6f AU", (unit.pos.y - refUnit.pos.y)*distRatio); ImGui::NextColumn();

					ImGui::Text("Distance:"); ImGui::NextColumn();
					ImGui::TextTooltipOnHover("Distance from center of reference system.");
					ImGui::Text(" %2.6f AU", (unit.pos - refUnit.pos).Length()*distRatio);
					ImGui::NextColumn();

					ImGui::Text("Velocity:"); ImGui::NextColumn();
					ImGui::Text("X %+2.6f km/s", (unit.vel.x - refUnit.vel.x)*speedRatio);
					ImGui::Text("Y %+2.6f km/s", (unit.vel.y - refUnit.vel.y)*speedRatio);
					ImGui::NextColumn();

					ImGui::Text("Speed:"); ImGui::NextColumn();
					ImGui::Text(" %2.6f km/s", (unit.vel - refUnit.vel).Length()*speedRatio);
					ImGui::NextColumn();

					ImGui::Text("Mass:"); ImGui::NextColumn();
					ImGui::Text(" %1.4e kg", unit.mass * massRatio); ImGui::NextColumn();
					ImGui::Columns(1);
					ImGui::NewLine();
					if (ImGui::Button(!following ? "Follow selected unit" : "Cancel following"))
						following = !following;

				}
				else
					ImGui::TextColored({1.0,0.0,0.0,1.0}, "There are no units.");
			}
			ImGui::End();

			//if (following && follow)
			//	Follow(data, *follow);

			//SelectedUnitTextLabel(data, follow);
		}

		void UnitsProperties::SelectedUnitTextLabel(solar::SimData & data/*, solar::OMSAR * follow*/)
		{
			/*//Normalized position on the screen
			auto tmp = data[selectedUnit].pos*follow->ScaleFactor() + follow->GetOffset();
			tmp.x *= 600;
			tmp.y *= -350 * follow->AspectRatio();
			tmp += Vec2d {600,350 - 6};//Half window's size and 6px higher
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
			ImGui::PopStyleVar();*/
		}

		bool UnitsProperties::UnitNameGetter(void * data, int index, const char ** result)
		{
			auto& simData = *reinterpret_cast<SimData*>(data);
			*result = simData.Get()[index].name.c_str();// correct index is ensured by ImGui::Combo call
			return true;
		}

		bool UnitsProperties::RefUnitNameGetter(void * data, int index, const char ** result)
		{
			auto& simData = *reinterpret_cast<SimData*>(data);
			if (index == 0)
				*result = centerSystem;
			else
				*result = simData.Get()[index - 1].name.c_str();// correct index is ensured by ImGui::Combo call
			return true;
		}

		void UnitsProperties::Follow(const SimData& data, OMSAR & follow)
		{
			/*assert((int)data->size() > selectedUnit && selectedUnit >= 0);

			// Move to this position to move followed Unit to screen's center
			follow.Move(-1.0*follow.ScaleFactor()*data[selectedUnit].pos);*/
		}
	}
}
