//Vector.h
//Defines an n dimensional vector of any type
//This was designed for another project, which is why it's so extensive

#include "Math_.h"

#ifndef __RHWS_VECTOR__
#define __RHWS_VECTOR__

//TODO: 
//Encapsulate dimensions access
//Create Iterator for this class

namespace Geometry
{
	template <class T, int dimension_count>
	struct Vector
	{
		T d[dimension_count];

		Vector<T, dimension_count>()
		{
			for(int i= 0; i< dimension_count; i++)
				this->d[i]= 0;
		}

		Vector<T, dimension_count>(T value)
		{
			for(int i= 0; i< dimension_count; i++)
				this->d[i]= value;
		}

		Vector<T, dimension_count>(const T dimensions[dimension_count])
		{
			for(int i= 0; i< dimension_count; i++)
				this->d[i]= dimensions[i];
		}

		T & operator[](const int index)
		{
			return d[index];
		}

		const T & operator[](const int index) const
		{
			return d[index];
		}

		Vector<T, dimension_count> operator+(const Vector<T, dimension_count> &other) const
		{
			Vector<T, dimension_count> sum;

			for(int i= 0; i< dimension_count; i++)
				sum[i]= this->d[i]+ other[i];

			return sum;
		}

		Vector<T, dimension_count> operator-(const Vector<T, dimension_count> &other) const
		{
			Vector<T, dimension_count> difference;

			for(int i= 0; i< dimension_count; i++)
				difference[i]= this->d[i]- other[i];

			return difference;
		}

		Vector<T, dimension_count> operator*(const Vector<T, dimension_count> &other) const
		{
			Vector<T, dimension_count> vector_product;

			for(int i= 0; i< dimension_count; i++)
				vector_product[i]= this->d[i]* other[i];

			return vector_product;
		}

		Vector<T, dimension_count> & operator*=(const Vector<T, dimension_count> &other)
		{
			for(int i= 0; i< dimension_count; i++)
				this->d[i]= this->d[i]* other[i];

			return *this;
		}

		template<class U>
		Vector<T, dimension_count> operator*(const U &scalar) const
		{
			Vector<T, dimension_count> scaled;

			for(int i= 0; i< dimension_count; i++)
				scaled[i]= (this->d[i])* scalar;

			return scaled;
		}

		template<class U>
		Vector<T, dimension_count> & operator*=(const U &scalar)
		{
			for(int i= 0; i< dimension_count; i++)
				this->d[i]= this->d[i]* scalar;

			return *this;
		}

		Vector<T, dimension_count> operator/(const Vector<T, dimension_count> &other) const
		{
			Vector<T, dimension_count> vector_quotient;

			for(int i= 0; i< dimension_count; i++)
				vector_quotient[i]= this->d[i]/ other[i];

			return vector_quotient;
		}

		Vector<T, dimension_count> & operator/=(const Vector<T, dimension_count> &other)
		{
			for(int i= 0; i< dimension_count; i++)
				this->d[i]= this->d[i]/ other[i];

			return *this;
		}

		template<class U>
		Vector<T, dimension_count> operator/(const U &scalar) const
		{
			Vector<T, dimension_count> scaled;

			for(int i= 0; i< dimension_count; i++)
				scaled[i]= this->d[i]/ scalar;

			return scaled;
		}

		template<class U>
		Vector<T, dimension_count> & operator/=(const U &scalar)
		{
			for(int i= 0; i< dimension_count; i++)
				this->d[i]= this->d[i]/ scalar;

			return *this;
		}

		bool operator==(const Vector<T, dimension_count> &other) const 
		{
			for(unsigned int i= 0; i< dimension_count; i++)
				if((*this)[i]!= other[i])
					return false;

			return true;
		}

		//the implicitness of the "operator operator Vector<U, dimension_count>" version caused precision problems
		//Vector<int> + Vector<float> ->
		//Vector<int> + Vector<int>
		template <class U>
		Vector<U, dimension_count> ExplicitlyConvertTo()
		{
			U converted_dimensions[dimension_count];

			for(int i= 0; i< dimension_count; i++)
				converted_dimensions[i]= (U)this->d[i];

			return Vector<U, dimension_count>(converted_dimensions);
		}

		float Magnitude() const
		{
			float magnitude_squared= 0;

			for(int i= 0; i< dimension_count; i++)
				magnitude_squared+= pow((float)d[i], 2);

			return sqrt(magnitude_squared);
		}

		Vector<T, dimension_count> GetOffsetOf(const Vector &other)
		{
			return other- *this;
		}

		float Distance(const Vector &other)
		{
			return GetOffsetOf(other).Magnitude();
		}

		T GetHighestDimensionValue() const
		{
			return d[dimension_count- 1];
		}

		Vector<T, dimension_count+ 1> AddDimension(T value) const
		{
			Vector<T, dimension_count+ 1> higher_vector;

			for(int i= 0; i< dimension_count; i++)
				higher_vector[i]= this->d[i];

			higher_vector[(dimension_count+ 1)- 1]= value;

			return higher_vector;
		}

		Vector<T, dimension_count- 1> RemoveDimension() const
		{
			Vector<T, dimension_count- 1> lower_vector;

			for(int i= 0; i< dimension_count- 1; i++)
				lower_vector[i]= this->d[i];

			return lower_vector;
		}

		Vector<T, dimension_count- 1> RemoveDimension(T &value) const
		{
			value= GetHighestDimensionValue();

			return RemoveDimension();
		}

		Vector<T, dimension_count> GetNormalized() const
		{
			return (*this)/ this->Magnitude();
		}

		T Dot(const Vector<T, dimension_count> &other)
		{
			T dot_product= 0;

			for(int i= 0; i< dimension_count; i++)
				dot_product+= this->d[i]* other.d[i];

			return dot_product;
		}

		float AngleBetween(const Vector<T, dimension_count> &other)
		{
			return acos(this->GetNormalized().Dot(other.GetNormalized()));
		}
	};

	//maybe replace with specialized version
	template<class U>
	static U GetScalarVectorValue(Vector<U, 1> &scalar_vector)
	{
		return scalar_vector[0];
	}

	template<class U>
	static void SetScalarVectorValue(Vector<U, 1> &scalar_vector, U value)
	{
		scalar_vector[0]= value;
	}

	typedef Vector<int, 4> Vector4i;
	typedef Vector<unsigned int, 4> Vector4ui;
	typedef Vector<float, 4> Vector4f;

	typedef Vector<int, 3> Vector3i;
	typedef Vector<unsigned int, 3> Vector3ui;
	typedef Vector<float, 3> Vector3f;

	typedef Vector<int, 2> Vector2i;
	typedef Vector<unsigned int, 2> Vector2ui;
	typedef Vector<float, 2> Vector2f;

	template<class T>
	Vector<T, 3> Cross(Vector<T, 3> a, Vector<T, 3> b)
	{
		Vector<T, 3> cross_product;

		cross_product[0]= a[1]* b[2]- a[2]* b[1];
		cross_product[1]= a[2]* b[0]- a[0]* b[2];
		cross_product[2]= a[0]* b[1]- a[1]* b[0];

		return cross_product;
	}

	namespace Make
	{
		template<class T>
		Vector<T, 4> Vector4(T x, T y, T z, T w)
		{
			Vector<T, 4> vector;

			vector[0]= x;
			vector[1]= y;
			vector[2]= z;
			vector[3]= w;

			return vector;
		}

		Vector4f Vector4f_(float x, float y, float z, float w);
		Vector4i Vector4i_(int x, int y, int z, int w);
		Vector4ui Vector4ui_(unsigned int x, unsigned int y, unsigned int z, unsigned int w);

		template<class T>
		Vector<T, 3> Vector3(T x, T y, T z)
		{
			Vector<T, 3> vector;

			vector[0]= x;
			vector[1]= y;
			vector[2]= z;

			return vector;
		}

		Vector3f Vector3f_(float x, float y, float z);
		Vector3i Vector3i_(int x, int y, int z);
		Vector3ui Vector3ui_(unsigned int x, unsigned int y, unsigned int z);

		template<class T>
		Vector<T, 2> Vector2(T x, T y)
		{
			Vector<T, 2> vector;

			vector[0]= x;
			vector[1]= y;

			return vector;
		}

		Vector2f Vector2f_(float x, float y);
		Vector2i Vector2i_(int x, int y);
		Vector2ui Vector2ui_(unsigned int x, unsigned int y);
	}

	template<class T>
	float GetDirection(Vector<T, 2> vector)
	{
		Vector<T, 2> polar_axis;
		polar_axis[0]= 1;
		polar_axis[1]= 0;

		return vector[1] >= 0 ? vector.AngleBetween(polar_axis) : (float)(2* M_PI)- vector.AngleBetween(polar_axis);
	}

	template<class T, int dimension_count>
	float PercentError(Vector<T, dimension_count> a, Vector<T, dimension_count> b)
	{
		float percent_error= PercentError(a[0], b[0]);

		for(unsigned int i= 1; i< dimension_count; i++)
			percent_error= std::max(percent_error, PercentError(a[i], b[i]));

		return percent_error;
	}
}

using Geometry::Vector4i;
using Geometry::Vector4ui;
using Geometry::Vector4f;

using Geometry::Vector3i;
using Geometry::Vector3ui;
using Geometry::Vector3f;

using Geometry::Vector2i;
using Geometry::Vector2ui;
using Geometry::Vector2f;

#endif