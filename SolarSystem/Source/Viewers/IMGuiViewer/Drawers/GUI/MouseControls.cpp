#include "MouseControls.h"
#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imgui.h"
#include "Source/Viewers/IMGuiViewer/OMSARPolicy.h"
#include "Source/Viewers/IMGuiViewer/Camera.h"
#include "Source/Math/Math.h"

#include <algorithm>
namespace solar
{
	namespace gui
	{
		namespace
		{
			//Returns rotation matrix to rotate from startPoint around origin to endPoint (in world coords)
			//Both points are in [-1,1] box range
			Mat4d TrackBall(Vec2d startPoint, Vec2d endPoint, const Mat4f& viewToWorld)
			{
				//Screen->Sphere
				float startLength = startPoint.LengthSq();
				Vec3d startVector(startPoint.x, startPoint.y, startLength >= 1.0f ? 0.0f : sqrt(1.0f - startLength));
				float endLength = endPoint.LengthSq();
				Vec3d endVector(endPoint.x, endPoint.y, endLength >= 1.0f ? 0.0f : sqrt(1.0f - endLength));
				//If length > 1.0
				startVector.Normalize();
				endVector.Normalize();
				//angle is immune to coord transformations
				double angle = acos(DotProduct(startVector, endVector));
				if (angle > 0.01)//So the axis is well defined
				{
					auto axis = CrossProduct(endVector, startVector).Normalize();
					auto axis4 = viewToWorld*Vec4d(axis, 0.0);//So returned matrix is in world coordinates
					return MakeRotation(Vec3d(axis4.x, axis4.y, axis4.z), angle);
				}
				else//Do not rotate
					return MakeIdentity<double>();
			}
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

		void GrabControl(Camera& cam)
		{
			//Statics are fine, because there is only one mouse to be dragged at the moment
			static Vec3d cachedCamPos, cachedCamUp;
			static Vec2d cachedStartPoint;
			static Mat4f cachedinvView;
			auto io = ImGui::GetIO();

			//If mouse has just been pressed
			if (ImGui::IsMouseClicked(0))
			{
				cachedCamPos = cam.CamPos();
				cachedCamUp = cam.UpDir();
				cachedinvView = Inverse(cam.ViewMatrix());
				//Map to -1,1
				cachedStartPoint = 2.0*(Vec2d(ImGui::GetMousePos()) - 0.5*Vec2d(io.DisplaySize)) / Vec2d(io.DisplaySize);
				cachedStartPoint.y *= -1.0;//So plus is up instead of down
			}
			//Only count dragging in the center of the screen(not in side windows)
			if (!ImGui::IsMouseHoveringAnyWindow())
			{
				if (ImGui::IsMouseDragging(0, 5.0))//Left button = Trackball
				{
					Vec2d endPoint = 2.0*Vec2d(ImGui::GetMouseDragDelta()) / Vec2d(io.DisplaySize);
					endPoint.y *= -1.0;
					endPoint += cachedStartPoint;

					Mat4d rotMatrix = TrackBall(cachedStartPoint, endPoint, cachedinvView);
					//Rotate point around target
					auto delta4 = rotMatrix*(Vec4d(cachedCamPos - cam.TargetPos(), 1.0));
					//Compute new up vector to enable camera roll
					auto up4 = rotMatrix*Vec4d(cachedCamUp, 0.0);

					cam.LookAt(cam.TargetPos() + Vec3d(delta4.x, delta4.y, delta4.z), cam.TargetPos(), Vec3d(up4.x, up4.y, up4.z));
				}
				else if (ImGui::IsMouseDragging(1, 5.0))//Right button = panning
				{

				}
			}
		}
	}
}
