#ifndef MATH_MAT4_537283275208_HEADER
#define MATH_MAT4_537283275208_HEADER

#include "Common.h"
#include "Vec3.h"

namespace solar
{

	//Column-major 4x4matrix.
	//access to individual elements given by operator[0...15]
	template<typename T>
	class Mat4
	{
	public:
		Mat4()
		{
			for (int i = 0; i < 16; ++i)
				e[i] = 0;
		}
		Mat4(const Mat4<T>&) = default;
		Mat4(Mat4<T>&&) = default;
		Mat4& operator=(const Mat4<T>&) = default;
		Mat4& operator=(Mat4<T>&&) = default;
		~Mat4() = default;

		Mat4<T>& operator+=(T val)
		{
			for (int i = 0; i < 16; ++i)
				e[i] += val;

			return *this;
		}
		Mat4<T>& operator-=(T val)
		{
			for (int i = 0; i < 16; ++i)
				e[i] -= val;

			return *this;
		}
		Mat4<T>& operator*=(T val)
		{
			for (int i = 0; i < 16; ++i)
				e[i] *= val;

			return *this;
		}
		Mat4<T>& operator/=(T val)
		{
			if (abs(val) < epsilon<T>)
				throw Exception("Cannot divide by zero.");
			for (int i = 0; i < 16; ++i)
				e[i] /= val;

			return *this;
		}
		Mat4<T>& operator+=(const Mat4<T>& other)
		{
			for (int i = 0; i < 16; ++i)
				e[i] += other.e[i];

			return *this;
		}
		Mat4<T>& operator-=(const Mat4<T>& other)
		{
			for (int i = 0; i < 16; ++i)
				e[i] += other.e[i];
			type = mType::unknown;
			return *this;
		}
		Mat4<T>& operator*=(const Mat4<T>& other)
		{
			Mat4<T> temp(*this);
			*this = temp*other;
			return *this;
		}
		T& operator[](const size_t index)
		{
			return e[index];
		}
		const T& operator[](const size_t index) const
		{
			return e[index];
		}
		//Access to elements' array. Can be passed to OpenGL
		T* Data()
		{
			return &e[0];
		}
		const T* Data() const
		{
			return &e[0];
		}
	private:
		T e[16];//Elements
	};

	template<typename T>
	Mat4<T> operator*(const Mat4<T>& a, const Mat4<T>& b)
	{
		Mat4<T> res;

		for (int r = 0; r < 4; ++r)
			for (int c = 0; c < 4; ++c)
				for (int k = 0; k < 4; ++k)
					res[r + 4 * c] = a[r + 4 * k] * b[k + 4 * c];
		return res;
	}

	template<typename T>
	Mat4<T> Inverse(const Mat4<T>& matrix)
	{
		Mat4<T> res;

		//Using adjugate formula:
		///TODO
		assert(0);
		return res;
	}
	template<typename T>
	Mat4<T> MakeIdentity()
	{
		Mat4<T> res;
		res[0] = res[5] = res[10] = res[15] = static_cast<T>(1);
		return res;
	}
	template<typename T>
	Mat4<T> MakeScale(T x, T y, T z)
	{
		Mat4<T> res;

		res[0] = x;
		res[5] = y;
		res[10] = z;
		res[15] = static_cast<T>(1);
		return res;
	}
	template<typename T>
	Mat4<T> MakeScale(const Vec3<T>& scale)
	{
		Mat4<T> res;

		res[0] = scale.x;
		res[5] = scale.y;
		res[10] = scale.z;
		res[15] = static_cast<T>(1);
		return res;
	}
	template<typename T>
	Mat4<T> MakeTranslation(T x, T y, T z)
	{
		Mat4<T> res;

		res[0] = 1;
		res[5] = 1;
		res[10] = 1;
		res[12] = x;
		res[13] = y;
		res[14] = z;
		return res;
	}
	template<typename T>
	Mat4<T> MakeTranslation(const Vec3<T>& dir)
	{
		Mat4<T> res;

		res[0] = 1;
		res[5] = 1;
		res[10] = 1;
		res[12] = dir.x;
		res[13] = dir.y;
		res[14] = dir.z;
		return res;
	}
	//Rotation matrix
	template<typename T>
	Mat4<T> MakeRotation(T x, T y, T z, T angle)
	{
		T c = cos(angle);
		T s = sin(angle);

		Mat4<T> res;

		//From: https://stackoverflow.com/questions/5188876/rotation-matrix-given-angle-and-point-in-x-y-z
		// Which uses this paper: https://sites.google.com/site/glennmurray/Home/rotation-matrices-and-formulas
		res[0] = x*x + (1 - x*x)*c;			res[4] = x*y*(1 - c) - z*s;			res[8] = x*z*(1 - c) + y*s;
		res[1] = x*y*(1 - c) + z*s;			res[5] = y*y + (1 - y*y)*c;			res[9] = y*z*(1 - c) - x*s;
		res[2] = x*z*(1 - c) - y*s;			res[6] = y*z*(1 - c) + x*s;			res[10] = z*z + (1 - z*z)*c;
		res[15] = 1;

		return res;
	}
	template<typename T>
	Mat4<T> MakeRotation(const Vec3<T>& axis, T angle)
	{
		return MakeRotation(axis.x, axis.y, axis.z, angle);
	}
	//Perspective projection matrix
	//Horizontal field of view in degrees, aspect ratio(w/h)
	//near<far
	template<typename T>
	Mat4<T> MakePerspective(T FOV, T AR, T near, T far)
	{
		T halfW = near*tan(DegToRad(FOV / T(2)));
		T halfH = halfW / AR;
		//From: http://www.songho.ca/opengl/gl_projectionmatrix.html
		Mat4<T> res;
		res[0] = near / halfW;
		res[5] = near / halfH;
		res[10] = -(far + near) / (far - near);
		res[11] = -static_cast<T>(1);
		res[14] = -2 * far*near / (far - near);
	}
	template<typename T>
	Mat4<T> MakePerspective(T top, T bottom, T left, T right, T near, T far)
	{
		T height = top - bottom;
		T width = right - left;
		//From: http://www.songho.ca/opengl/gl_projectionmatrix.html
		Mat4<T> res;
		res[0] = T(2)*near / width;
		res[5] = T(2)*near / height;
		res[8] = (left + right) / width;
		res[9] = (top + left) / height;
		res[10] = -(far + near) / (far - near);
		res[11] = -static_cast<T>(1);
		res[14] = -2 * far*near / (far - near);
	}
	//Orthogonal projection matrix
	//near<far
	template<typename T>
	Mat4<T> MakeOrtho(T width, T height, T near, T far)
	{
		//From: http://www.songho.ca/opengl/gl_projectionmatrix.html
		Mat4<T> res;
		res[0] = static_cast<T>(1) / width;
		res[5] = static_cast<T>(1) / height;
		res[10] = static_cast<T>(-2)*(far - near);
		res[14] = -(far + near) / (far - near);
		res[15] = static_cast<T>(1);

		return res;
	}
	//Orthogonal projection matrix
	//near<far
	template<typename T>
	Mat4<T> MakeOrtho(T top, T bottom, T left, T right, T near, T far)
	{
		T width = right - left;
		T height = top - bottom;
		//From: http://www.songho.ca/opengl/gl_projectionmatrix.html
		Mat4<T> res;
		res[0] = static_cast<T>(2) / width;		  //Normalize
		res[5] = static_cast<T>(2) / height;	  //Normalize
		res[10] = static_cast<T>(-2)*(far - near);//Normalize
		res[12] = -(right + left) / width;	   //Move to center
		res[13] = -(top + bottom) / height;	   //Move to center
		res[14] = -(far + near) / (far - near);//Move to center
		res[15] = static_cast<T>(1);
		return res;
	}
}

#endif