#include "SceneDrawer.h"
#include "Source/Units/Unit.h"
#include <gl/glew.h>
namespace solar
{
	namespace drawers
	{
		namespace
		{
			constexpr size_t gridRes = 100;
			constexpr size_t smallToBig = 10;
			constexpr float aspectRatio = 1200 / 700.0;
		}
		SceneDrawer::SceneDrawer(const SimData & data) :
			//Near=1 meter, Far=10 light years
			camera {PerspectiveCamera(90.0f, aspectRatio, 1.0 / data.RatioOfDistTo(PhysUnits::meter), 10.0 / data.RatioOfDistTo(PhysUnits::lightYear)),
					ScaledOrthoCamera(aspectRatio, 1.0f,  1.0 / data.RatioOfDistTo(PhysUnits::kilometer), 100.0 / data.RatioOfDistTo(PhysUnits::AU))},
			activeCamera(&camera.perspective), grid(data, GetActiveCam(), gridRes, smallToBig), lineTrails(data->size(), GetActiveCam()), simData(GetActiveCam())
#ifdef TEST_DRAWER
			, test(GetActiveCam())
#endif
		{
			//Sets both cameras to look into same direction
			camera.perspective.LookAt(Vec3d(0, 0, 1.0), Vec3d(0, 0, 0));
			camera.scaledOrtho.LookAt(Vec3d(0, 0, 1.0), Vec3d(0, 0, 0));
			gridPlane = plane::XY;
			gridEnabled = true;
			gridOffset = -data.RatioOfDistTo(PhysUnits::AU)*0.1f;
		}
		void SceneDrawer::Draw(const SimData & data)
		{
			GetActiveCam().Bind();
			camControls.Update(GetActiveCam());
			GetActiveCam().Update(data);
#ifdef TEST_DRAWER
			test.Draw(data);
#else 
			lineTrails.Draw(data);
			simData.Draw(data);
#endif
			//Disables writing to depth buffer
			//Prevents unnecessary z-fighting between both grids, and because they are drawn last(well except GUI) there isn't any harm in it
			glDepthMask(GL_FALSE);
			//Make small grid to be 1AU big
			auto ratio = data.RatioOfDistTo(PhysUnits::AU);
			Vec2f scale {float(ratio), float(ratio)};
			//Draw it, convert small grid scale back to meters
			if (gridEnabled)
				gridScale = grid.Draw(data, GetActiveCam(), gridPlane, scale, gridOffset)*ratio*PhysUnits::AU;
			glDepthMask(GL_TRUE);
		}
		Camera & SceneDrawer::GetActiveCam()
		{
			assert(activeCamera); return *activeCamera;
		}
		const Camera & SceneDrawer::GetActiveCam() const
		{
			assert(activeCamera); return *activeCamera;
		}
		void SceneDrawer::SetActiveCam(camType cam, bool sync)
		{
			const auto& oldCam = *activeCamera;
			if (cam == perspective)
				activeCamera = &camera.perspective;
			else
				activeCamera = &camera.scaledOrtho;
			if (sync)
			{
				activeCamera->LookAt(oldCam.CamPos(), oldCam.TargetPos(), oldCam.UpDir());
				activeCamera->FollowObject(oldCam.GetFollowedObjectIndex());
			}
		}
		PhysUnits::ratio SceneDrawer::GetGridScale()
		{
			return gridScale;
		}
	}
}
