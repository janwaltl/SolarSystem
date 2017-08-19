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

	//Abstract interface for a camera that consists of view and projection matrices stored in UBO and can follow simulated objects around.
	class Camera
	{
	public:
		static constexpr size_t noTarget = std::numeric_limits<size_t>::max();

		Camera();
		//Copying disabled for now
		// - Policy on copying of UBO?
		Camera(const Camera&) = delete;
		Camera& operator=(const Camera&) = delete;
		Camera(Camera&&) = default;
		Camera& operator=(Camera&&) = default;
		virtual ~Camera();
		//Sets camera's view matrix in such way that camera is located at given positon and looks at desired target location.
		virtual void LookAt(const Vec3d& newCamPos, const Vec3d& newTargetPos, const Vec3d& newUpDir = Vec3d(0.0, 1.0, 0.0));
		//Updates camera if its targeted at some object.
		//Calls UpdateCamera() internally
		///In future it might be used to animate its movement
		void Update(const SimData& data);
		//Binds this camera for next drawing.( Updates its UBO with new matrices and binds UBO to binding point associated with all shaders)
		//Should be used before drawing if there are more than one camera object.
		///IMPROVE: UBOs can be indexed by uniqued index based on refCount and bound to multiple binding points.
		///			That would allow multiple bound cameras at same time. Minimum binding points is 24 which is more than enough for all camera which will probably be the only use for UBOs anyway.
		///			Might get messy. With OpenGLSM and with shader remembering its UBO it can bind correct camera with its own Bind() which is already called before draw calls.
		///				- too many hidden dependecies
		void Bind();
		void UnBind();
		//If shader wants to draw from a camera's point of view, then it must contain following interface block:
		/*		std140 uniform **blockName**
		{
		mat4 projection;
		mat4 view;
		mat4 projView;
		mat4 invProj;
		mat4 invView;
		mat4 invProjView;
		} **instanceName**;*/
		void Subscribe(const openGL::Shader& shader, const std::string& blockName = "CameraMatrices") const;
		//Set camera's target as object in simData[index]. Use Camera::noTarget to stop following
		//To update matrices Update(simData) method must be called frequently.
		void FollowObject(const size_t index) { targetObjectIndex = index; }
		size_t GetFollowedObjectIndex()const { return targetObjectIndex; }
		bool IsFollowing()const { return targetObjectIndex != noTarget; }

		const Mat4f& ProjMatrix() const { return proj; }
		const Mat4f& ViewMatrix() const { return view; }
		const Mat4f& ProjViewMatrix() const { return proj*view; }
		const Vec3d& CamPos() const { return camPos; }
		const Vec3d& TargetPos() const { return targetPos; }
		const Vec3d& UpDir() const { return upDir; }
		const Vec3d& RightDir() const { return rightDir; }
		double DistToTarget() const { return (TargetPos() - CamPos()).Length(); }
	protected:
		//Optional behaviour injected into Update()
		virtual void UpdateCamera() {}

		Mat4f proj, view;
		Vec3d camPos, targetPos, upDir,rightDir;
	private:
		void SubmitMatrices();

		uint32_t UBO;
		//Index in simData array of the camera's target, 'noTarget' for none
		size_t targetObjectIndex;

	};

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float FOV, float AR, float near, float far);
	};
	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float width, float height, float near, float far);
	};
	class ScaledOrthoCamera : public Camera
	{
	public:
		//Sets ortho projection for camera one unit from its target
		ScaledOrthoCamera(float width, float height, float near, float far);
		//Calls Camera::LookAt and multiplies the width and height of projection matrix with distance to target
		virtual void LookAt(const Vec3d& newCamPos, const Vec3d& newTargetPos, const Vec3d& newUpDir = Vec3d(0.0, 1.0, 0.0)) override final;
	private:
		const Mat4f projOriginal;
	};
}

#endif