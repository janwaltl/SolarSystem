#include "MouseControls.h"
#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imgui.h"
#include "Source/Viewers/IMGuiViewer/OMSARPolicy.h"
#include "Source/Viewers/IMGuiViewer/Camera.h"
#include "Source/Math/Math.h"

#include <iostream>
#include <algorithm>
namespace solar
{
	namespace gui
	{
		MouseControls & MouseControls::operator()(Camera & cam)
		{
			auto io = ImGui::GetIO();

			//If any mouse button has just been pressed
			if (ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1) || ImGui::IsMouseClicked(2))
				CacheCamera(cam);

			//Only count dragging in the center of the screen(not in GUI windows) and don't bother with small angles
			if (!ImGui::IsMouseHoveringAnyWindow())
			{
				if (ImGui::IsMouseDragging(0, 5.0f))//Left button 
					Trackball(cam);
				else if (ImGui::IsMouseDragging(1, 5.0f))//Right button 
					SideMovement(cam);
				else if (ImGui::IsMouseDragging(2, 5.0f))//Middle button 
					FirstPersonCam(cam);
				else if (abs(io.MouseWheel) > epsilon<float>)// Wheel 
					Zooming(cam);
			}
			return *this;
		}

		void MouseControls::Zooming(solar::Camera & cam)
		{
			constexpr float maxSpeed = 2.0f;

			auto io = ImGui::GetIO();
			//clamp and map to 0.0,max
			double scroll = (std::max(std::min(maxSpeed, io.MouseWheel*zoomSpeed), -1.0f*maxSpeed) + maxSpeed)*0.5f;//Clamp
			auto vec = (cam.CamPos() - cam.TargetPos())*scroll;
			cam.LookAt(cam.TargetPos() + vec, cam.TargetPos(), cam.UpDir());
		}

		void MouseControls::FirstPersonCam(solar::Camera & cam)
		{
			Vec2d endPoint = 2.0*Vec2d(ImGui::GetMouseDragDelta(2)) / cache.displaySize;
			endPoint.y *= -1.0;
			endPoint += cache.StartPoint;

			Mat4d rotMatrix = TrackBallRotation(cache.StartPoint, endPoint, cache.invView, false);//Reversed order
																								  //Rotate point around camera's position
			auto delta4 = rotMatrix*(Vec4d(cache.TargetPos - cache.CamPos, 1.0));
			//Compute new up vector to enable camera roll
			auto up4 = rotMatrix*Vec4d(cache.CamUp, 0.0);

			cam.LookAt(cache.CamPos, cache.CamPos + Vec3d(delta4.x, delta4.y, delta4.z), Vec3d(up4.x, up4.y, up4.z));
		}
		void MouseControls::SideMovement(solar::Camera & cam)
		{
			Vec2d drag = 2.0*Vec2d(ImGui::GetMouseDragDelta(1)) / cache.displaySize;
			drag.y *= -cache.displaySize.y / cache.displaySize.x;//ImGui has reversed Y and correct for aspect ratio

																 //Map to camera's coords
			Vec3d delta = cam.RightDir()*drag.x + cam.UpDir()*drag.y;
			cam.LookAt(cache.CamPos - delta, cache.TargetPos - delta, cam.UpDir());
		}

		void MouseControls::Trackball(solar::Camera & cam)
		{

			Vec2d endPoint = 2.0*Vec2d(ImGui::GetMouseDragDelta(0)) / cache.displaySize;
			endPoint.y *= -1.0;
			endPoint += cache.StartPoint;

			Mat4d rotMatrix = TrackBallRotation(cache.StartPoint, endPoint, cache.invView, true);
			//Rotate point around camera's target
			auto delta4 = rotMatrix*(Vec4d(cache.CamPos - cache.TargetPos, 1.0));
			//Compute new up vector to enable camera roll
			auto up4 = rotMatrix*Vec4d(cache.CamUp, 0.0);

			cam.LookAt(cache.TargetPos + Vec3d(delta4.x, delta4.y, delta4.z), cache.TargetPos, Vec3d(up4.x, up4.y, up4.z));
		}

		Mat4d MouseControls::TrackBallRotation(Vec2d startPoint, Vec2d endPoint, const Mat4f & viewToWorld, bool onFront)
		{
			float sign = onFront ? 1.0f : -1.0f;
			//Screen->Sphere
			float startLength = startPoint.LengthSq();
			Vec3d startVector(startPoint.x, startPoint.y, startLength >= 1.0f ? 0.0f : sign * sqrt(1.0f - startLength));
			float endLength = endPoint.LengthSq();
			Vec3d endVector(endPoint.x, endPoint.y, endLength >= 1.0f ? 0.0f : sign* sqrt(1.0f - endLength));
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

		void MouseControls::CacheCamera(const Camera & cam)
		{
			auto io = ImGui::GetIO();
			cache.displaySize = io.DisplaySize;
			cache.CamPos = cam.CamPos();
			cache.CamUp = cam.UpDir();
			cache.TargetPos = cam.TargetPos();
			cache.invView = Inverse(cam.ViewMatrix());
			//Map to -1,1
			cache.StartPoint = 2.0*(Vec2d(ImGui::GetMousePos()) - 0.5*Vec2d(io.DisplaySize)) / cache.displaySize;
			cache.StartPoint.y *= -1.0;//So plus is up instead of down
		}
	}
}
