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
		}
		SceneDrawer::SceneDrawer(const SimData & data) :
			grid(data, cam, gridRes, smallToBig), lineTrails(data->size(), cam), simData(cam), test(cam)
		{
			//cam.MakeOrtho(17 * 2, 10 * 2, 0.1f, 10e15);
			//cam.MakePerspective(90, 1.7f, 0.01f, 10000.0f);
			cam.MakePerspective(90.0f, 1.7f, 0.01f, 1e15f);
			cam.LookAt(Vec3d(0, 0, 1.0), Vec3d(0, 0, 0));
		}
		void SceneDrawer::Draw(const SimData & data)
		{
			lineTrails.Draw(data);
			auto ratio = data.RatioOfDistTo(PhysUnits::AU);
			Vec2f scale {float(ratio), float(ratio)};
			grid.Draw(data, cam, grid.XY, scale, -0.1f);
			simData.Draw(data);
		}
	}
}
