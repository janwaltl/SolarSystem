#ifndef DRAWERS_GUIDRAWER_HEADER
#define DRAWERS_GUIDRAWER_HEADER


#include "GUI/UnitsProperties.h"
#include "GUI/CameraControls.h"
#include "GUI/Graphs.h"
#include "Source/Units/Unit.h"
#include "Source/Viewers/Viewer.h"
#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imguiExtended.h"
namespace solar
{
	namespace drawers
	{
		class SceneDrawer;
			//Renders User interface for simulation via ImGUI
		class GUIDrawer
		{
		public:
			void Draw(SimData& data, Viewer& viewer, SceneDrawer& scene, size_t w, size_t h);
		private:
			void TopMenuBar(SimData& data, Viewer& viewer, SceneDrawer& scene, size_t w, size_t h);
			void TopRightSwitches(solar::Viewer & viewer, ImDrawList * draw);
			void TopButtons(solar::Viewer & viewer, ImDrawList * draw);
			void DrawFasterTriangles(ImDrawList * draw, ImVec2 &cursorPos, ImGuiStyle &style, ImVec2 &buttonSize, const ImU32 &textCol);
			void DrawSlowerTriangles(ImDrawList * draw, ImVec2 &cursorPos, ImVec2 &buttonSize, ImGuiStyle &style, const ImU32 &textCol);
			gui::UnitsProperties unitsProps;
			gui::CameraControls camControls;
			gui::Graphs graphs;

			struct
			{
				ImGuiE::StateButton graphs;
				ImGuiE::StateButton unitsProps;
				ImGuiE::StateButton simControls;
				ImGuiE::StateButton visuals;
			} buttons;
			struct
			{
				ImGuiE::StateButton grid;
				ImGuiE::StateButton lineTrails;
				ImGuiE::StateButton planetScale;
				ImGuiE::StateButton play;
				ImGuiE::StateButton pause;
			}switches;
		};
	}
}


#endif