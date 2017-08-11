#include "ObjectContextMenu.h"
#include "Source/Viewers/IMGuiViewer/Drawers/SceneDrawer.h"
#include "Source/Viewers/IMGuiViewer/Drawers/Scene/SimDataDrawer.h"

#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imgui.h"
#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imguiExtended.h"
namespace solar
{
	namespace gui
	{
		ObjectContextMenu::ObjectContextMenu()
		{
			open = false;
		}
		void ObjectContextMenu::Draw(const SimData & data, const drawers::SceneDrawer & scene)
		{
			if (!open)
			{
				if (!ImGui::IsMouseHoveringAnyWindow())
				{
					const auto& objects = data.Get();
					auto hovObjectIT = objects.begin();
					ImVec2 mousePos = ImGui::GetMousePos();
					ImVec2 winSize = ImGui::GetIO().DisplaySize;
					ImVec2 screenMousePos = mousePos / winSize*2.0f - ImVec2(1.0f, 1.0f);//Convert mouse to -1,1
					screenMousePos.y *= -1.0f;//Y is up in OpenGL screen

					//Goes through each object and checks if we are hovering any
					//
					for (auto IT = objects.begin(); IT != objects.end(); ++IT)
					{
						auto objectPos = scene.GetSimDataDrawer().GetScreenPos(*IT, scene.GetCam());
						float radius = scene.GetSimDataDrawer().GetScreenRadius(*IT, scene.GetCam());
						auto distVec = screenMousePos - objectPos;

						if (distVec.x*distVec.x + distVec.y* distVec.y <= radius*radius)
						{
							ImVec2 offset {5.0f,5.0f};
							ImGui::SetNextWindowPos(mousePos + offset, ImGuiSetCond_Always);
							if (ImGui::Begin("ObjectHoverContextMenu", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
											 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
							{
								ImGui::TextColored(IT->color,IT->name.c_str());
							}
							ImGui::End();
						}
					}
				}
			}
		}
	}
}
