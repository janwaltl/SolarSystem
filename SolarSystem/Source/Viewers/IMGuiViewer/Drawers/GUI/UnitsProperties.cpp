#include "UnitsProperties.h"

#include "Source/Units/PhysicsUnits.h"
#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imgui.h"

namespace solar
{
	namespace gui
	{
		const Unit UnitsProperties::center(Vec3d(), Vec3d(), 0.0, "Center");
		namespace
		{
			void InfoBox(const char* text)
			{
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), "(?)");
				ImGui::TextTooltipOnHover(text);
			}
		}
		UnitsProperties::UnitsProperties(Vec2d winPos, Vec2d winSize) :
			winPos(winPos), winSize(winSize), refUnit(&center)
		{
			this->winSize = Vec2d(800, 300);
			this->winPos = Vec2d(200, 200);
			distPosCombo = speedVelCombo = timeCombo = massCombo = lenCombo = 0;
			massRatio = timeRatio = lenRatio = 1.0;
		}

		void UnitsProperties::operator()(SimData & data)
		{
			ImGui::SetNextWindowPos(winPos, ImGuiSetCond_Once);
			ImGui::SetNextWindowSize(winSize, ImGuiSetCond_Once);
			if (ImGui::Begin("Units' properties", NULL))
			{
				ListHeader(data);

				if (ImGui::BeginChild("Units", ImVec2 {}, false, ImGuiWindowFlags_AlwaysUseWindowPadding))
				{
					ImGui::Columns(4);
					ImGui::SetColumnOffset(1, 90);
					ImGui::SetColumnOffset(2, 370);
					ImGui::SetColumnOffset(3, 650);

					ImGui::Text("Name"); InfoBox("Right click on names of units for more actions");
					ImGui::NextColumn();
					ImGui::Combo("##DistPos", &distPosCombo, "Distance\0Position\0"); ImGui::NextColumn();
					ImGui::Combo("##SpeedVel", &speedVelCombo, "Speed\0Velocity\0"); ImGui::NextColumn();
					ImGui::Text("Mass"); ImGui::NextColumn();
					ImGui::Separator();
					ImGui::Separator();

					for (size_t i = 0; i < data->size(); ++i)
					{
						ImGui::PushID(i);
						ImGui::Text(data[i].name.c_str());
						if (ImGui::BeginPopupContextItem("UnitDetails"))
						{
							if (ImGui::Selectable("Set as frame of reference"))
								frameOfRef = i + 1;
							ImGui::Text("Edit values");
							ImGui::Text("Focus");
							ImGui::Text("Set as grid's frame of reference");
							ImGui::Text("Other cool buttons");
							ImGui::EndPopup();
						}
						ImGui::NextColumn();

						Vec3d relPos = data[i].pos - refUnit->pos;
						Vec3d relVel = data[i].vel - refUnit->vel;

						if (distPosCombo == 1)
							ImGui::Text("(%9.3g, %9.3g, %9.3g) %s", relPos.x*lenRatio, relPos.y*lenRatio, relPos.z*lenRatio, lenSuff);
						else
							ImGui::Text("%9.3g %s ", relPos.Length()*lenRatio, lenSuff);
						ImGui::NextColumn();

						if (speedVelCombo == 1)
							ImGui::Text("(%+9.3g, %9.3g, %9.3g) %s/%s", relVel.x*lenRatio / timeRatio, relVel.y*lenRatio / timeRatio,
										relVel.z*lenRatio / timeRatio, lenSuff, timeSuff);
						else
							ImGui::Text("%9.3g %s/%s", relVel.Length()*lenRatio / timeRatio, lenSuff, timeSuff);

						ImGui::NextColumn();

						ImGui::Text("%9.3g %s", data[i].mass*massRatio, massSuff); ImGui::NextColumn();
						ImGui::Separator();
						ImGui::PopID();
					}
					ImGui::EndChild();
				}
				ImGui::End();
			}
		}
		void UnitsProperties::ListHeader(solar::SimData & data)
		{
			SetLenUnits(data);
			SetTimeUnits(data);
			SetMassUnits(data);
			SetFrameOfRef(data);

			ImGui::PushItemWidth(100);

			ImGui::Text("Frame of reference:"); InfoBox("Point against which are speed(velocity) and position(distance) measured.");
			ImGui::SameLine();
			if (ImGui::Combo("##FrameOfRef", &frameOfRef, UnitNameGetter, &data, data->size() + 1, 10))
				SetFrameOfRef(data);

			ImGui::SameLine(0.0f, 50.0f);

			ImGui::Text("Time:"); ImGui::SameLine();
			if (ImGui::Combo("##Time", &timeCombo, "Seconds\0Minutes\0Hours\0Days\0Years\0"))
				SetTimeUnits(data);
			ImGui::SameLine();
			ImGui::PopItemWidth();
			ImGui::PushItemWidth(100);
			ImGui::Text("Length:"); ImGui::SameLine();
			if (ImGui::Combo("##Length", &lenCombo, "Meters\0Kilometers\0AU\0"))
				SetLenUnits(data);
			ImGui::SameLine();
			ImGui::PopItemWidth();
			ImGui::PushItemWidth(120);
			ImGui::Text("Mass:"); ImGui::SameLine();
			if (ImGui::Combo("##Mass", &massCombo, "Kilograms\0Earth masses\0Solar masses\0Relative\0"))
				SetMassUnits(data);
			ImGui::TextTooltipOnHover("Relative means relative to frame of reference object");
			ImGui::PopItemWidth();
		}
		void UnitsProperties::SetFrameOfRef(solar::SimData & data)
		{
			if (frameOfRef != 0)
				refUnit = &data[frameOfRef - 1];
			else
				refUnit = &center;
		}
		void UnitsProperties::SetMassUnits(solar::SimData & data)
		{
			switch (massCombo)
			{
			case 3:
				if (frameOfRef != 0)
				{
					massRatio = 100.0 / refUnit->mass;						massSuff = "%";			break;
				}
				else
					massCombo = 0;
			case 0: massRatio = data.RatioOfMassTo(PhysUnits::kilogram);	massSuff = "kg";		break;
			case 1: massRatio = data.RatioOfMassTo(PhysUnits::earth);		massSuff = "Earths";	break;
			case 2: massRatio = data.RatioOfMassTo(PhysUnits::sun);			massSuff = "Suns";		break;
			}
		}
		void UnitsProperties::SetLenUnits(solar::SimData & data)
		{
			switch (lenCombo)
			{
			case 0: lenRatio = data.RatioOfDistTo(PhysUnits::meter);		lenSuff = "m";		break;
			case 1: lenRatio = data.RatioOfDistTo(PhysUnits::kilometer);	lenSuff = "km";		break;
			case 2: lenRatio = data.RatioOfDistTo(PhysUnits::AU); 			lenSuff = "AU";		break;
			}
		}
		void UnitsProperties::SetTimeUnits(solar::SimData & data)
		{
			switch (timeCombo)
			{
			case 0: timeRatio = data.RatioOfTimeTo(PhysUnits::second);		timeSuff = "s";		break;
			case 1: timeRatio = data.RatioOfTimeTo(PhysUnits::minute);		timeSuff = "min";	break;
			case 2: timeRatio = data.RatioOfTimeTo(PhysUnits::hour); 		timeSuff = "h";		break;
			case 3: timeRatio = data.RatioOfTimeTo(PhysUnits::day); 		timeSuff = "d";		break;
			case 4: timeRatio = data.RatioOfTimeTo(PhysUnits::year); 		timeSuff = "y";		break;
			}
		}
		bool UnitsProperties::UnitNameGetter(void * data, int index, const char ** result)
		{

			if (index == 0)
				*result = center.name.c_str();
			else
			{
				auto& simData = *reinterpret_cast<SimData*>(data);
				*result = simData[index - 1].name.c_str();// correct index is ensured by ImGui::Combo call
			}
			return true;
		}

	}
}