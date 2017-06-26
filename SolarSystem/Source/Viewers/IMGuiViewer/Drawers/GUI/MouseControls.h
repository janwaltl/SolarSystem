#ifndef IMGUIVIEWER_GUI_MOUSE_CONTROLS_HEADER
#define IMGUIVIEWER_GUI_MOUSE_CONTROLS_HEADER

#include "Source/Math/Math.h"

namespace solar
{
	class OMSAR;
	class Camera;

	namespace gui
	{
		class MouseControls
		{
		public:
			MouseControls& operator()(Camera& cam);
		private:
			void Zooming(solar::Camera & cam);
			void FirstPersonCam(solar::Camera & cam);
			void SideMovement(solar::Camera & cam);
			void Trackball(solar::Camera & cam);
			//Returns rotation matrix to rotate from startPoint around origin to endPoint (in world coords)
			//Both points are in [-1,1] box range
			//Whether should points be projection onto front side of sphere or backside
			Mat4d TrackBallRotation(Vec2d startPoint, Vec2d endPoint, const Mat4f& viewToWorld, bool onFront);
			void CacheCamera(const Camera& cam);
			struct Cache
			{
				Vec3d CamPos, CamUp, TargetPos;
				Vec2d StartPoint;
				Mat4f invView;
				Vec2d displaySize;
			}cache;
			float zoomSpeed = 0.1f;
		};
	}
}

#endif