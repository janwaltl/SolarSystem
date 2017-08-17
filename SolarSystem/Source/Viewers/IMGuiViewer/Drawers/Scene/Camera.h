#ifndef IMGUIVIEWER_CAMERA_HEADER
#define IMGUIVIEWER_CAMERA_HEADER

#include <limits>
#include "Source/Math/Math.h"
#include "Source/Units/Unit.h"

namespace solar
{
	namespace openGL
	{
		class Shader;
	}
	//Represents OpenGL camera - Needs OpenGL context when constructed.
	//Each shader that wants to use it must call Subscribe
	//If there are more than one camera object, then Bind() should be called before drawing.
	class Camera
	{
	public:
		enum type :uint8_t
		{
			ortho = 0,
			perspective = 1
		};
		static constexpr size_t noTarget = std::numeric_limits<size_t>::max();

		//Construct ortographic camera with MakeOrtho(1.0f, 1.0f, 1.f, 10.0f);
		Camera();
		//Copying disabled for now
		// - Policy on copying of UBO?
		Camera(const Camera&) = delete;
		Camera& operator=(const Camera&) = delete;
		Camera(Camera&&) = default;
		Camera& operator=(Camera&&) = default;
		~Camera();
		//Makes this camera use an orthogonal projection matrix
		Camera& MakeOrtho(float width, float height, float near, float far);
		//Makes this camera use a perspective projection matrix
		Camera& MakePerspective(float FOV, float AR, float near, float far);

		type GetType() { return camType; }
		//Set camera's target as object in simData[index]. Use Camera::noTarget to stop following
		//To update matrices Update(simData) method must be called frequently.
		void FollowObject(const size_t index);
		size_t GetFollowedObjectIndex() { return targetObjectIndex; }
		bool IsFollowing() { return targetObjectIndex != noTarget; }
		//Updates camera if its targeted at some object.
		///In future it might be used to animate its movement
		void Update(const SimData& data);
		//
		Camera& LookAt(const Vec3d& newCamPos, const Vec3d& newTargetPos, const Vec3d& newUpDir = Vec3d(0.0, 1.0, 0.0));
		//
		Camera& Look(const Vec3d& newCamPos, const Vec3d& newViewDir, const Vec3d& newUpDir = Vec3d(0.0, 1.0, 0.0));
		//If shader wants to draw from this camera's point of view, then it must contain following interface block:
		/*		std140 uniform **blockName**
				{
						mat4 projection;
						mat4 view;
						mat4 projView;
						mat4 invProj;
						mat4 invView;
						mat4 invProjView;
				} **instanceName**;*/
		const Camera& Subscribe(const openGL::Shader& shader, const std::string& blockName = "CameraMatrices") const;
		//Binds this camera for next drawing.
		//Should be used before drawing if there are more than one camera object.
		//IMPROVE: UBOs can be indexed by uniqued index based on refCount.
		//			That would allow multiple bound cameras at same time. Minimum binding points is 24 which is more than enough for all camera which will probably be the only use for UBOs anyway.
		//			Might get messy. With OpenGLSM and with shader remembering its UBO it can bind correct camera with its own Bind() which is already called before draw calls.
		Camera& Bind();
		const Vec3d& CamPos() const;
		const Vec3d& TargetPos() const;
		const Vec3d& UpDir() const;
		const Vec3d& RightDir() const;
		const Mat4f& ViewMatrix() const;
		const Mat4f& ProjMatrix() const;
		const Mat4f& ProjViewMatrix() const;
		double GetDistToTarget() const;
	private:
		void SubmitMatrices();
		type camType;
		uint32_t UBO;
		Mat4f projection, view, projView;
		Vec3d camPos, targetPos, upDir, rightDir;
		//Index in simData array of the camera's target, 'noTarget' for none
		size_t targetObjectIndex;

	};
}

#endif