#include "MouseControls.h"
#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imgui.h"
#include "Source/Viewers/IMGuiViewer/OMSARPolicy.h"

#include <algorithm>
namespace solar
{
	namespace gui
	{
		void MouseControl(OMSAR & sys)
		{
			ZoomControl(sys);
			GrabControl(sys);
		}
		void ZoomControl(OMSAR& sys)
		{
			float currentZoom = static_cast<float>(sys.ScaleFactor());

			ImGuiIO& io = ImGui::GetIO();

			//Zoom based on mouse scrolling this frame
			//Allow scrolling only in center region, not in the left window
			//Also zooms faster for already zoomed screen
			
			if (!ImGui::IsMouseHoveringAnyWindow())
				currentZoom += static_cast<float>(io.MouseWheel *currentZoom / 30.0f);

			//Clamp between 0.01 and 1000.0f, this works for AU units
			currentZoom = std::min(1000.0f, std::max(0.01f, currentZoom));

			// We want to zoom to current screen center, not 0,0 coordinate.
			// This progresivelly corrects offset to maintain same screen center regardless of zoom
			sys.Move(sys.GetOffset()*(currentZoom / sys.ScaleFactor()));
			sys.ScaleFactor(currentZoom);
		}
		void GrabControl(OMSAR & sys)
		{
			//Static are fine, because there is only one mouse to be dragged at the moment
			static Vec2 offset, drag;

			//If mouse will be dragged, store current offset
			if (ImGui::IsMouseClicked(0))
				offset = sys.GetOffset();
			//Only count dragging in the center of the screen(not in side windows)
			//Also only count dragging longer than 5 pixels 
			if (!ImGui::IsMouseHoveringAnyWindow() && ImGui::IsMouseDragging(0, 5.0f))
			{
				ImGuiIO& io = ImGui::GetIO();
				auto val = ImGui::GetMouseDragDelta();
				//Normalize values
				val.x /= io.DisplaySize.x;
				val.y /= io.DisplaySize.x;// .x is correct here
				drag = Vec2(val.x * 2, -val.y * 2);
				sys.Move(drag + offset);
			}
		}
	}
}
