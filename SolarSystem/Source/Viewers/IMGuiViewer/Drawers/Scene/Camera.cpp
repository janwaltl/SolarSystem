#include "Camera.h"

#include <vector>
#include <glad/glad.h>
#include "Source/Viewers/IMGuiViewer/OpenGL/Shader.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/Error.h"

namespace solar
{
	namespace
	{
		constexpr size_t matSize = 16 * sizeof(float);
		constexpr size_t bufferSize = 6 * matSize;
		//Cameras' UBOs will use this binding point
		constexpr size_t UBOBinding = 4;
	}

	Camera::Camera()
	{
		targetObjectIndex = noTarget;
		proj = view = MakeIdentity<float>();
		glGenBuffers(1, &UBO);
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferData(GL_UNIFORM_BUFFER, (GLsizeiptr)bufferSize, nullptr, GL_STREAM_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		Bind();
	}

	Camera::~Camera()
	{
		UnBind();
		glDeleteBuffers(1, &UBO);
	}

	void Camera::LookAt(const Vec3d& newCamPos, const Vec3d& newTargetPos, const Vec3d& newUpDir)
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
	}
	void Camera::Update(const SimData & data)
	{
		if (targetObjectIndex != noTarget)
		{
			assert(targetObjectIndex < data.Get().size());
			LookAt(camPos, data[targetObjectIndex].pos, upDir);
		}
		UpdateCamera();
		SubmitMatrices();
	}

	void Camera::Bind()
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, UBOBinding, UBO);
	}
	void Camera::UnBind()
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, UBOBinding, 0);
	}
	void Camera::Subscribe(const openGL::Shader& shader, const std::string& blockName) const
	{
		size_t blockIndex = shader.GetUniformBlockIndex(blockName);
		//Bind it to correct index
		glUniformBlockBinding(shader.GetID(), blockIndex, UBOBinding);
	}
	void Camera::SubmitMatrices()
	{
		//This is layout of buffer:
		//
		//mat4 projection;
		//mat4 view;
		//mat4 projView;
		//mat4 invProj;
		//mat4 invView;
		//mat4 invProjView;
		//
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0 * matSize, matSize, proj.Data());
		glBufferSubData(GL_UNIFORM_BUFFER, 1 * matSize, matSize, view.Data());
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * matSize, matSize, (proj*view).Data());
		//TODO inverses if needed
		//glBufferSubData(GL_UNIFORM_BUFFER, 3 * matSize, matSize, Inverse(projection).Data());
		//glBufferSubData(GL_UNIFORM_BUFFER, 4 * matSize, matSize, Inverse(view).Data());
		//glBufferSubData(GL_UNIFORM_BUFFER, 5 * matSize, matSize, Inverse(projection*view).Data());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	PerspectiveCamera::PerspectiveCamera(float FOV, float AR, float near, float far)
	{
		proj = MakePerspective(FOV, AR, near, far);
	}

	OrthographicCamera::OrthographicCamera(float width, float height, float near, float far)
	{
		proj = MakeOrtho(width, height, near, far);
	}

	ScaledOrthoCamera::ScaledOrthoCamera(float width, float height, float near, float far) :
		projOriginal(MakeOrtho(width, height, near, far))
	{
		proj = projOriginal;
	}

	void ScaledOrthoCamera::LookAt(const Vec3d & newCamPos, const Vec3d & newTargetPos, const Vec3d & newUpDir)
	{
		Camera::LookAt(newCamPos, newTargetPos, newUpDir);
		proj = projOriginal;
		auto dist = (newTargetPos - newCamPos).Length();
		proj[0] /= static_cast<float>(dist);
		proj[5] /= static_cast<float>(dist);
	}

}
