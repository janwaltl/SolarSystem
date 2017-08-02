#include "Camera.h"

#include <vector>
#include <GL/glew.h>
#include "Source/Viewers/IMGuiViewer/OpenGL/Shader.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/Error.h"
#include <iostream>
namespace solar
{
	namespace
	{
		constexpr size_t matSize = 16 * sizeof(float);
		constexpr size_t bufferSize = 6 * matSize;
		//Cameras' UBOs will use this binding index
		constexpr size_t UBOBinding = 4;
	}
	Camera::Camera() : UBO(0) {}

	Camera::~Camera()
	{
		if (UBO != 0)
			glDeleteBuffers(1, &UBO);
	}

	Camera & Camera::MakeOrtho(float width, float height, float near, float far)
	{
		LazyInit();
		projection = solar::MakeOrtho<float>(width, height, near, far);
		SubmitMatrices();

		return *this;
	}

	Camera & Camera::MakePerspective(float FOV, float AR, float near, float far)
	{
		LazyInit();
		projection = solar::MakePerspective<float>(FOV, AR, near, far);
		SubmitMatrices();
		return *this;
	}

	Camera& Camera::LookAt(const Vec3d& newCamPos, const Vec3d& newTargetPos, const Vec3d& newUpDir)
	{
		LazyInit();
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
		LazyInit();
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

	double Camera::GetDistToTarget() const
	{
		return (targetPos - camPos).Length();
	}

	void Camera::LazyInit()
	{
		if (UBO == 0)
		{
			glGenBuffers(1, &UBO);
			glBindBuffer(GL_UNIFORM_BUFFER, UBO);
			Bind();
			glBufferData(GL_UNIFORM_BUFFER, (GLsizeiptr)bufferSize, nullptr, GL_STREAM_DRAW);
		}
	}

	void Camera::SubmitMatrices()
	{
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
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * matSize, matSize, (projection*view).Data());
		///TODO inverses if needed
		//glBufferSubData(GL_UNIFORM_BUFFER, 3 * matSize, matSize, Inverse(projection).Data());
		//glBufferSubData(GL_UNIFORM_BUFFER, 4 * matSize, matSize, Inverse(view).Data());
		//glBufferSubData(GL_UNIFORM_BUFFER, 5 * matSize, matSize, Inverse(projection*view).Data());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}


}
