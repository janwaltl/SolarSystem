#include "CameraControls.h"
#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imgui.h"
#include "Source/Viewers/IMGuiViewer/Camera.h"
#include "Source/Math/Math.h"
#include "Source/Units/Unit.h"
#include <algorithm>
#include <GLFW/glfw3.h>
namespace solar
{
	namespace gui
	{
		CameraControls::CameraControls() :
			camTypeCombo(0), camPosCombo(0), camTargetCombo(0), zoomSpeed(0.1f), forwSpeed(0.1f)
		{}

		CameraControls & CameraControls::operator()(Camera & cam, SimData& data)
		{
			MouseControls(cam, data);
			ImGui::SetNextWindowPos(ImVec2(300, 300), ImGuiSetCond_Once);
			ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiSetCond_Once);
			if (ImGui::Begin("CameraControls"))
			{
				ImGui::Text("Camera's type: "); ImGui::SameLine();
				if (ImGui::Combo("##CamsType", &camTypeCombo, "Perspective\0Orthographic\0"))
					SetCamType(cam);
				ImGui::Text("Camera's target:"); ImGui::SameLine();
				ImGui::Combo("##CamTarget", &camTargetCombo, UnitNameGetter, &data, data->size() + 1);
				ImGui::TextTooltipOnHover("Locks camera's orientation to always point to targeted object.\n"
										  "Only zooming and trackball works while set.");
				ImGui::Text("Camera's position:"); ImGui::SameLine();
				ImGui::Combo("##CamPosition", &camPosCombo, UnitNameGetter, &data, data->size() + 1);
				ImGui::TextTooltipOnHover("Sets and locks camera's position to be same as chosen object's.\n"
										  "Only 1st-person camera works while set.");

				ImGui::Text("Controlled by Mouse: ");
				ImGui::Text("Left button:  Trackball rotation around camera's target.\n"
							"Right button: Move to sides.\n"
							"ALT+LB:       1st-person camera.\n"
							"Wheel:        Zoom to target.\n"
							"ALT+Wheel:    Move forwards/backwards.\n");
			}
			ImGui::End();
			return *this;
		}
		void CameraControls::SetCamType(solar::Camera & cam)
		{
			switch (camTypeCombo)
			{
			case 0: cam.MakePerspective(90, 1.7f, 0.001f, 10000.0f); break;
			case 1: SetOrthoCamera(cam); break;
			}
		}
		void CameraControls::SetOrthoCamera(solar::Camera & cam)
		{
			auto dist = cam.GetDistToTarget();
			cam.MakeOrtho(dist*1.7, dist, 0.1f, 10000.0f);
		}
		bool CameraControls::UnitNameGetter(void * data, int index, const char ** result)
		{
			static const char* none = "<Free>";
			if (index == 0)
				*result = none;
			else
			{
				auto& simData = *reinterpret_cast<SimData*>(data);
				*result = simData[index - 1].name.c_str();// correct index is ensured by ImGui::Combo call
			}
			return true;
		}
		void CameraControls::MouseControls(solar::Camera & cam, SimData& data)
		{
			auto io = ImGui::GetIO();


			//If any mouse button has just been pressed
			if (ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1) || ImGui::IsMouseClicked(2))
				CacheCamera(cam);

			//Only count dragging in the center of the screen(not in GUI windows) and don't bother with small angles
			if (!ImGui::IsMouseHoveringAnyWindow())
			{

				if (ImGui::IsMouseDragging(0, 5.0f))//Left button 
				{
					if (camTargetCombo == 0 && io.KeyAlt)//ALT+Left button
						FirstPersonCam(cam);
					else if (camPosCombo == 0)
						Trackball(cam);
				}
				else if (abs(io.MouseWheel) > epsilon<float>)// Wheel 
				{
					if (camPosCombo == 0 && camTargetCombo == 0 && io.KeyAlt)// ALT+ Wheel 
						ForwBackMovement(cam);
					else if (camPosCombo == 0)
						Zooming(cam);
				}
				else if (camTargetCombo == 0 && ImGui::IsMouseDragging(1, 5.0f))//Right button 
					SideMovement(cam);
			}
			if (camTargetCombo > 0)
			{
				cam.LookAt(cam.CamPos(), data[camTargetCombo - 1].pos, cam.UpDir());
				cache.TargetPos = data[camTargetCombo - 1].pos; //Also update cache in case we are also dragging.
				//cache.CamUp = cam.UpDir();
			}
			if (camPosCombo > 0)
			{
				cam.LookAt(data[camPosCombo - 1].pos, cam.TargetPos(), cam.UpDir());
				cache.CamPos = cam.CamPos();
				//cache.CamUp = cam.UpDir();
			}
		}

		void CameraControls::Zooming(solar::Camera & cam)
		{
			constexpr float maxSpeed = 2.0f;

			auto io = ImGui::GetIO();
			//clamp and map to 0.0,max
			double scroll = (std::max(std::min(maxSpeed, io.MouseWheel*zoomSpeed), -1.0f*maxSpeed) + maxSpeed)*0.5f;//Clamp
			auto vec = -1.0*(cam.TargetPos() - cam.CamPos())*scroll;
			if (vec.Length() > 0.05f)
			{
				if (camTypeCombo == 1)
					SetOrthoCamera(cam);
				cam.LookAt(cam.TargetPos() + vec, cam.TargetPos(), cam.UpDir());
			}
		}

		void CameraControls::ForwBackMovement(solar::Camera & cam)
		{
			constexpr float maxSpeed = 1.0f;

			auto io = ImGui::GetIO();
			//clamp and map to -max,max
			double scroll = (std::max(std::min(maxSpeed, io.MouseWheel*forwSpeed), -1.0f*maxSpeed));//Clamp
			auto vec = (cam.TargetPos() - cam.CamPos())*scroll;
			cam.LookAt(cam.CamPos() + vec, cam.TargetPos() + vec, cam.UpDir());
		}

		void CameraControls::FirstPersonCam(solar::Camera & cam)
		{
			Vec2d endPoint = 2.0*Vec2d(ImGui::GetMouseDragDelta(0)) / cache.displaySize;
			endPoint.y *= -1.0;
			endPoint += cache.StartPoint;

			Mat4d rotMatrix = TrackBallRotation(cache.StartPoint, endPoint, cache.invView, false);//Reversed order
																								  //Rotate point around camera's position
			auto delta4 = rotMatrix*(Vec4d(cache.TargetPos - cache.CamPos, 1.0));
			//Compute new up vector to enable camera roll
			auto up4 = rotMatrix*Vec4d(cache.CamUp, 0.0);

			cam.LookAt(cache.CamPos, cache.CamPos + Vec3d(delta4.x, delta4.y, delta4.z), Vec3d(up4.x, up4.y, up4.z));
		}
		void CameraControls::SideMovement(solar::Camera & cam)
		{
			Vec2d drag = 2.0*Vec2d(ImGui::GetMouseDragDelta(1)) / cache.displaySize;
			drag.y *= -cache.displaySize.y / cache.displaySize.x;//ImGui has reversed Y and correct for aspect ratio

																 //Map to camera's coords
			Vec3d delta = cam.RightDir()*drag.x + cam.UpDir()*drag.y;
			delta *= (cache.TargetPos - cache.CamPos).Length();
			cam.LookAt(cache.CamPos - delta, cache.TargetPos - delta, cam.UpDir());
		}

		void CameraControls::Trackball(solar::Camera & cam)
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

		Mat4d CameraControls::TrackBallRotation(Vec2d startPoint, Vec2d endPoint, const Mat4f & viewToWorld, bool onFront)
		{
			double sign = onFront ? 1.0f : -1.0f;
			//Screen->Sphere
			double startLength = startPoint.LengthSq();
			Vec3d startVector(startPoint.x, startPoint.y, startLength >= 1.0 ? 0.0 : sign * sqrt(1.0 - startLength));
			double endLength = endPoint.LengthSq();
			Vec3d endVector(endPoint.x, endPoint.y, endLength >= 1.0 ? 0.0 : sign* sqrt(1.0 - endLength));
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

		void CameraControls::CacheCamera(const Camera & cam)
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
