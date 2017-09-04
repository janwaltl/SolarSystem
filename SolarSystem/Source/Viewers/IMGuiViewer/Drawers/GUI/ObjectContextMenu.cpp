#include "ObjectContextMenu.h"
#include "Source/Viewers/IMGuiViewer/Drawers/SceneDrawer.h"
#include "Source/Viewers/IMGuiViewer/Drawers/Scene/SimDataDrawer.h"

#include <imgui/imgui.h>
#include <imgui/imguiExtended.h>
#include <imgui/imguiExtended.h>
namespace solar
{
	namespace gui
	{
		namespace
		{
			constexpr Vec2f offset {5.0f,5.0f};
		}
		ObjectContextMenu::ObjectContextMenu(const SimData& data) :
			objectsStates(data.Get().size(), nothing)
		{
			contextWinOpened = false;
		}
		void ObjectContextMenu::Draw(const SimData & data, drawers::SceneDrawer & scene)
		{
			auto anyHovered = SetObjectsStates(data, scene);

			if (contextWinOpened)
			{
				ImGui::SetNextWindowPos(contextWinPos + offset, ImGuiSetCond_Always);
				ImGui::Begin("ObjectContextMenu", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
							 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
				{
					const auto& objects = data.Get();
					assert(data.Get().size() == objects.size());
					for (size_t i = 0; i < objects.size(); ++i)
						if (objectsStates[i] == inContext)
						{
							ImGui::PushID(i);
							ImGui::TextColored(objects[i].color, objects[i].name.c_str());
							ImGui::SameLine(100);
							if (ImGui::SmallButton("Follow"))
							{
								scene.GetActiveCam().FollowObject(i);
								CloseContextMenu();
							}
							ImGui::SameLine();

							auto radius = objects[i].radius;
							//Only allow to go to objects that actually have a radius > 10m
							if (radius > 10.0 * data.RatioOfDistTo(PhysUnits::meter) && ImGui::SmallButton("Go to"))
							{
								auto& cam = scene.GetActiveCam();
								//Zooms to fourth times the radius
								auto viewDist = (objects[i].pos - cam.CamPos()).Normalize() *radius*4.0;
								cam.LookAt(objects[i].pos - viewDist, objects[i].pos, cam.UpDir());
								cam.FollowObject(i);
								CloseContextMenu();
							}
							ImGui::PopID();
						}
					if (ImGui::SmallButton("Close"))
						CloseContextMenu();
				}
				ImGui::End();
			}
			if (anyHovered)
			{
				ImVec2 mousePos = ImGui::GetMousePos();
				ImGui::SetNextWindowPos(mousePos + offset, ImGuiSetCond_Always);
				ImGui::Begin("HoverContextMenu", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
							 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
				{
					const auto& objects = data.Get();
					assert(data.Get().size() == objects.size());
					for (size_t i = 0; i < objects.size(); ++i)
						if (objectsStates[i] == hovered)
							ImGui::TextColored(objects[i].color, objects[i].name.c_str());
				}
				ImGui::End();
			}
		}

		bool ObjectContextMenu::SetObjectsStates(const SimData & data, const drawers::SceneDrawer & scene)
		{
			bool anyHovered = false;
			//Only update if we are not hovering anything - Will prevent moving context menu underneath the mouse and picking objects behind other windows
			if (!ImGui::IsMouseHoveringAnyWindow())
			{
				bool clicked = ImGui::IsMouseClicked(1);//Right click
				if (clicked)
					CloseContextMenu();
				const auto& objects = data.Get();
				auto hovObjectIT = objects.begin();
				ImVec2 mousePos = ImGui::GetMousePos();
				ImVec2 winSize = ImGui::GetIO().DisplaySize;
				ImVec2 screenMousePos = mousePos / winSize*2.0f - ImVec2(1.0f, 1.0f);//Convert mouse to -1,1
				screenMousePos.y *= -1.0f;//Y is up in OpenGL screen

				Vec2f newContextWinPos {0,0};
				for (size_t i = 0; i < objects.size(); ++i)
				{
					auto& object = objects[i];
					auto objectPos = scene.GetSimDataDrawer().GetScreenPos(object, scene.GetActiveCam());
					float radius = scene.GetSimDataDrawer().GetScreenRadius(object, scene.GetActiveCam());
					auto distVec = screenMousePos - objectPos;
					//If mouse is hovering the object
					if (distVec.x*distVec.x + distVec.y* distVec.y <= radius*radius)
					{
						if (clicked)//Rightclicked on the object -> add it to context menu, set its position based on first object in it
							objectsStates[i] = inContext;
						else if (!contextWinOpened || objectsStates[i] != inContext)//Don't replace objects in context menu unless we clicked for a new one or closed the old one
						{
							objectsStates[i] = hovered;
							anyHovered = true;
						}
					}
					else if (!contextWinOpened || clicked || objectsStates[i] != inContext)//Don't replace objects in context menu unless we clicked for a new one or closed the old one
						objectsStates[i] = nothing;
					if (objectsStates[i] == inContext)
					{
						objectPos.y *= -1.0f;//Y is up
						newContextWinPos = (objectPos*0.5f + 0.5f)*Vec2f(winSize);
						OpenContextMenu();
					}
				}
				contextWinPos = newContextWinPos;
			}
			return anyHovered;
		}
	}
}
