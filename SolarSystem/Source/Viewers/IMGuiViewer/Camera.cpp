#include "Camera.h"

#include <vector>
#include <GL/glew.h>
#include "OpenGL/Shader.h"
#include "OpenGL/Error.h"

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

	Camera & Camera::LookAt(const Vec3d & camPos, const Vec3d & targetPos, const Vec3d & upDir)
	{
		LazyInit();
		auto dir = (camPos - targetPos).Normalize();

		auto right = CrossProduct(upDir, dir).Normalize();
		auto up = CrossProduct(dir, right);

		view[0] = right.x;		view[4] = right.y;		view[8] = right.z;		view[12] = -DotProduct(camPos, right);
		view[1] = up.x;			view[5] = up.y;			view[9] = up.z;			view[13] = -DotProduct(camPos, up);
		view[2] = dir.x;		view[6] = dir.y;		view[10] = dir.z;		view[14] = -DotProduct(camPos, dir);
		view[3] = 0;			view[7] = 0;			view[11] = 0;			view[15] = 1;

		SubmitMatrices();
		return *this;
	}

	Camera & Camera::Look(const Vec3d & camPos, const Vec3d & dir, const Vec3d & upDir)
	{
		LookAt(camPos, camPos + dir, upDir);
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

	void Camera::LazyInit()
	{
		if (UBO == 0)
		{
			glGenBuffers(1, &UBO);
			glBindBuffer(GL_UNIFORM_BUFFER, UBO);
			glBufferData(GL_UNIFORM_BUFFER, (GLsizeiptr)bufferSize, nullptr, GL_STREAM_DRAW);
			Bind();
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
