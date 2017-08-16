#include "Camera.h"

#include <vector>
#include <GL/glew.h>
#include "Source/Viewers/IMGuiViewer/OpenGL/Shader.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/Error.h"

namespace solar
{
	namespace
	{
		constexpr size_t matSize = 16 * sizeof(float);
		constexpr size_t bufferSize = 6 * matSize;
		//Cameras' UBOs will use this binding index
		constexpr size_t UBOBinding = 4;
	}
	Camera::Camera() :targetObjectIndex(noTarget)
	{
		glGenBuffers(1, &UBO);
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferData(GL_UNIFORM_BUFFER, (GLsizeiptr)bufferSize, nullptr, GL_STREAM_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		Bind();

		MakeOrtho(1.0f, 1.0f, 1.0f, 10.0f);
	}

	Camera::~Camera()
	{
		glDeleteBuffers(1, &UBO);
	}

	Camera & Camera::MakeOrtho(float width, float height, float near, float far)
	{
		projection = solar::MakeOrtho<float>(width, height, near, far);
		camType = ortho;
		SubmitMatrices();

		return *this;
	}

	Camera & Camera::MakePerspective(float FOV, float AR, float near, float far)
	{
		projection = solar::MakePerspective<float>(FOV, AR, near, far);
		camType = perspective;
		SubmitMatrices();
		return *this;
	}

	void Camera::FollowObject(const size_t index)
	{
		targetObjectIndex = index;
	}

	void Camera::Update(const SimData & data)
	{
		if (targetObjectIndex != noTarget)
			LookAt(camPos, data[targetObjectIndex].pos, upDir);
	}

	Camera& Camera::LookAt(const Vec3d& newCamPos, const Vec3d& newTargetPos, const Vec3d& newUpDir)
	{
		camPos = newCamPos;
		targetPos = newTargetPos;
		auto dir = (camPos - targetPos).Normalize();

		this->rightDir = CrossProduct(newUpDir, dir).Normalize();
		this->upDir = CrossProduct(dir, rightDir);
		view[0] = float(rightDir.x); view[4] = float(rightDir.y);	view[8] = float(rightDir.z);	view[12] = -float(DotProduct(camPos, rightDir));
		view[1] = float(upDir.x);	 view[5] = float(upDir.y);		view[9] = float(upDir.z);		view[13] = -float(DotProduct(camPos, upDir));
		view[2] = float(dir.x);		 view[6] = float(dir.y);		view[10] = float(dir.z);		view[14] = -float(DotProduct(camPos, dir));
		view[3] = 0;				 view[7] = 0;					view[11] = 0;					view[15] = 1;

		SubmitMatrices();
		return *this;
	}

	Camera& Camera::Look(const Vec3d& newCamPos, const Vec3d& newViewDir, const Vec3d& newUpDir)
	{
		LookAt(newCamPos, newCamPos + newViewDir, newUpDir);
		return *this;
	}

	const Camera & Camera::Subscribe(const openGL::Shader & shader, const std::string & blockName) const
	{
		size_t blockIndex = shader.GetUniformBlockIndex(blockName);
		//Bind it to correct index
		glUniformBlockBinding(shader.GetID(), blockIndex, UBOBinding);
		return *this;
	}

	Camera & Camera::Bind()
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, UBOBinding, UBO);
		return *this;
	}

	const Vec3d& Camera::CamPos() const
	{
		return camPos;
	}

	const Vec3d& Camera::TargetPos() const
	{
		return targetPos;
	}

	const Vec3d& Camera::UpDir() const
	{
		return upDir;
	}

	const Vec3d& Camera::RightDir() const
	{
		return rightDir;
	}

	const Mat4f & Camera::ViewMatrix() const
	{
		return view;
	}

	const Mat4f & Camera::ProjMatrix() const
	{
		return projection;
	}

	const Mat4f & Camera::ProjViewMatrix() const
	{
		return projView;
	}

	double Camera::GetDistToTarget() const
	{
		return (targetPos - camPos).Length();
	}

	void Camera::SubmitMatrices()
	{
		projView = projection*view;
		//This is layout of buffer:
		/*
				mat4 projection;
				mat4 view;
				mat4 projView;
				mat4 invProj;
				mat4 invView;
				mat4 invProjView;
		*/
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0 * matSize, matSize, projection.Data());
		glBufferSubData(GL_UNIFORM_BUFFER, 1 * matSize, matSize, view.Data());
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * matSize, matSize, projView.Data());
		///TODO inverses if needed
		//glBufferSubData(GL_UNIFORM_BUFFER, 3 * matSize, matSize, Inverse(projection).Data());
		//glBufferSubData(GL_UNIFORM_BUFFER, 4 * matSize, matSize, Inverse(view).Data());
		//glBufferSubData(GL_UNIFORM_BUFFER, 5 * matSize, matSize, Inverse(projection*view).Data());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}


}
