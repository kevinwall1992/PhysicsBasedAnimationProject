//Point.h
//Basically just a convenience wrapper for the Vector class, 
//allows you to construct instances without having to use an array

#ifndef __RHWS_POINT__
#define __RHWS_POINT__

#include "Vector.h"

namespace Geometry
{
	template <class T>
	struct Size2;

	template <class T>
	struct Size3;


	#define POINT_2_DIMENSION_COUNT 2

	template <class T>
	struct Point2: public Vector<T, POINT_2_DIMENSION_COUNT>
	{
		//x property
		T GetX() {return this->d[Axis::X];}
		void SetX(T value) {this->d[Axis::X]= value;}
		//__declspec(property(get= GetX, put= SetX)) T X;

		//y property
		T GetY() {return this->d[Axis::Y];}
		void SetY(T value) {this->d[Axis::Y]= value;}
		//__declspec(property(get= GetY, put= SetY)) T Y;

		Point2()
		{

		}

		Point2(T x, T y)
		{
			this->d[Axis::X]= x;
			this->d[Axis::Y]= y;
		}

		Point2(Vector<T, 2> other)
		{
			this->SetX(other[Axis::X]);
			this->SetY(other[Axis::Y]);
		}

		template <class U>
		void Point2_Assignment(const Vector<U, POINT_2_DIMENSION_COUNT> &other)
		{
			this->d[Axis::X]= (T)other[Axis::X];
			this->d[Axis::Y]= (T)other[Axis::Y];
		}

		template <class U>
		void Point3_Assignment(const U &x, const U &y)
		{
			this->d[Axis::X]= (T)x;
			this->d[Axis::Y]= (T)y;
		}

		template<class U>
		Point2(const Vector<U, POINT_2_DIMENSION_COUNT> &other)
		{
			Point2_Assignment(other);
		}

		template<class U>
		Point2<T> & operator=(const Vector<U, POINT_2_DIMENSION_COUNT> &other)
		{
			Point2_Assignment(other);

			return *this;
		}


		//Conversion operators for sister classes

		operator Size2<T>()
		{
			return Size2<T>(GetX(), GetY());
		}
	};

	typedef Point2<int> Point2i;
	typedef Point2<float> Point2f;

	#undef POINT_2_DIMENSION_COUNT


	#define POINT_3_DIMENSION_COUNT 3

	template <class T>
	struct Point3: public Vector<T, POINT_3_DIMENSION_COUNT>
	{
		//x property
		T GetX() const {return this->d[Axis::X];}
		void SetX(T value) {this->d[Axis::X]= value;}
		//__declspec(property(get= GetX, put= SetX)) T X;

		//y property
		T GetY() const {return this->d[Axis::Y];}
		void SetY(T value) {this->d[Axis::Y]= value;}
		//__declspec(property(get= GetY, put= SetY)) T Y;

		//z property
		T GetZ() const {return this->d[Axis::Z];}
		void SetZ(T value) {this->d[Axis::Z]= value;}
		//__declspec(property(get= GetZ, put= SetZ)) T Z;

		Point3()
		{

		}

		Point3(T value)
		{
			SetX(value);
			SetY(value);
			SetZ(value);
		}

		Point3(T x, T y, T z)
		{
			this->d[Axis::X]= x;
			this->d[Axis::Y]= y;
			this->d[Axis::Z]= z;
		}

		template <class U>
		void Point3_Assignment(const Vector<U, POINT_3_DIMENSION_COUNT> &other)
		{
			this->d[Axis::X]= (T)other[Axis::X];
			this->d[Axis::Y]= (T)other[Axis::Y];
			this->d[Axis::Z]= (T)other[Axis::Z];
		}

		template <class U>
		void Point3_Assignment(const U &x, const U &y, const U &z)
		{
			this->d[Axis::X]= (T)x;
			this->d[Axis::Y]= (T)y;
			this->d[Axis::Z]= (T)z;
		}

		template<class U>
		Point3(const Vector<U, POINT_3_DIMENSION_COUNT> &other)
		{
			Point3_Assignment(other);
		}

		template<class U>
		Point3<T> & operator=(const Vector<U, POINT_3_DIMENSION_COUNT> &other)
		{
			Point3_Assignment(other);

			return *this;
		}


		//Conversion operators for sister classes

		operator Size3<T>()
		{
			return Size3<T>(GetX(), GetY(), GetZ());
		}
	};

	typedef Point3<int> Point3i;
	typedef Point3<float> Point3f;

	#undef POINT_3_DIMENSION_COUNT
}

using Geometry::Point2i;
using Geometry::Point2f;

using Geometry::Point3i;
using Geometry::Point3f;

#endif