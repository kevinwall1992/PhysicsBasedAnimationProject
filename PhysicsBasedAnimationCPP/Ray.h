#ifndef __RHWS_RAY__
#define __RHWS_RAY__

#include "Vector.h"

namespace Geometry
{
	template<class T, int dimension_count>
	struct Ray
	{
		Vector<T, dimension_count> position, direction;

		Ray()
		{

		}

		Ray(Vector<T, dimension_count> position, Vector<T, dimension_count> direction)
		{
			this->position= position;
			this->direction= direction;
		}

		Vector<T, dimension_count> GetHead()
		{
			return position+ direction;
		}

		float GetLength()
		{
			return direction.Magnitude();
		}
	};

	typedef Ray<float, 2> Ray2f;
	typedef Ray<float, 3> Ray3f;
}

#endif