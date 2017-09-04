#ifndef DRAWERS_GUIDRAWER_HEADER
#define DRAWERS_GUIDRAWER_HEADER


#include "Source/Units/Unit.h"
#include "Source/Viewers/Viewer.h"
#include <imgui/imguiExtended.h>
#include "GUI/UnitsProperties.h"
#include "GUI/VisualPreferences.h"
#include "GUI/Graphs.h"
#include "GUI/ObjectContextMenu.h"

namespace solar
{
	namespace drawers
	{
		class SceneDrawer;

		//Renders User interface for simulation via ImGUI
		class GUIDrawer
		{
		public:
			GUIDrawer(const SimData& data);
			void Draw(SimData& data, Viewer& viewer, SceneDrawer& scene, size_t w, size_t h);
		private:
			void TopMenuBar(SimData& data, Viewer& viewer, SceneDrawer& scene, size_t w, size_t h);
			void TopRightSwitches(SceneDrawer& scene,solar::Viewer & viewer, ImDrawList * draw);
			void TopButtons(solar::Viewer & viewer, ImDrawList * draw);
			void DrawFasterTriangles(ImDrawList * draw, ImVec2 &cursorPos, ImGuiStyle &style, ImVec2 &buttonSize, const ImU32 &textCol);
			void DrawSlowerTriangles(ImDrawList * draw, ImVec2 &cursorPos, ImVec2 &buttonSize, ImGuiStyle &style, const ImU32 &textCol);
			void BotttomMenuBar(SimData& data, Viewer& viewer, SceneDrawer& scene, size_t w, size_t h);
			void GridSize(const size_t &w, ImGuiContext * context, float menuBarHeight, solar::drawers::SceneDrawer & scene);
			gui::UnitsProperties unitsProps;
			gui::VisualPreferences visualPrefs;
			gui::Graphs graphs;
			gui::ObjectContextMenu objectContextMenu;
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

			struct
			{
				float grid;
				float pause;
				float prefs;
				float speedAndFollow;
			} offsets;//Horizontal offsets for positioning of GUI elements
			bool drawGUI;
		};
	}
}


#endif