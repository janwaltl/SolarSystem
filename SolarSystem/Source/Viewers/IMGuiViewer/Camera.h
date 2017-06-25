#ifndef IMGUIVIEWER_CAMERA_HEADER
#define IMGUIVIEWER_CAMERA_HEADER

#include "Source/Math/Math.h"
namespace solar
{
	namespace openGL
	{
		class Shader;
	}
	//Represents OpenGL camera
	//Each shader that wants to use it must call Subscribe
	//If there are more than one camera object, then Bind() should be called before drawing.
	class Camera
	{
	public:
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
		//
		Camera& LookAt(const Vec3d& camPos, const Vec3d& targetPos, const Vec3d& upDir = Vec3d(0.0, 1.0, 0.0));
		//
		Camera& Look(const Vec3d& camPos, const Vec3d& dir, const Vec3d& upDir = Vec3d(0.0, 1.0, 0.0));
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
		Vec3d CamPos();
		Vec3d TargetPos();
		Vec3d UpDir();
		Vec3d RightDir();
		Mat4f ViewMatrix();
		void ViewMatrix(const Mat4f& newView);
	private:
		void LazyInit();
		void SubmitMatrices();
		uint32_t UBO;
		Mat4f projection, view;
		Vec3d camPos, targetPos, upDir,rightDir;
	};
}

#endif