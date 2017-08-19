#include "SceneDrawer.h"
#include "Source/Units/Unit.h"

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
			camera {PerspectiveCamera(90.0f, aspectRatio, 0.01f, 1e15f), ScaledOrthoCamera(aspectRatio, 1.0f, 0.1f, 10e15)},
			activeCamera(&camera.perspective), grid(data, GetActiveCam(), gridRes, smallToBig), lineTrails(data->size(), GetActiveCam()), simData(GetActiveCam()), test(GetActiveCam())
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

			lineTrails.Draw(data);

			//Make small grid to be 1AU big
			auto ratio = data.RatioOfDistTo(PhysUnits::AU);
			Vec2f scale {float(ratio), float(ratio)};
			//Draw it, convert small grid scale back to meters
			if (gridEnabled)
				gridScale = grid.Draw(data, GetActiveCam(), gridPlane, scale, gridOffset)*ratio*PhysUnits::AU;

			simData.Draw(data);
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
