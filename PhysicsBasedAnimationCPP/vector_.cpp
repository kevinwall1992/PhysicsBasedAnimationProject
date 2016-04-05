#include "Vector.h"

namespace Geometry
{
	namespace Make
	{
		Vector4f Vector4f_(float x, float y, float z, float w)
		{
			return Vector4<float>(x, y, z, w);
		}

		Vector4i Vector4i_(int x, int y, int z, int w)
		{
			return Vector4<int>(x, y, z, w);
		}

		Vector4ui Vector4ui_(unsigned int x, unsigned int y, unsigned int z, unsigned int w)
		{
			return Vector4<unsigned int>(x, y, z, w);
		}

		Vector3f Vector3f_(float x, float y, float z)
		{
			return Vector3<float>(x, y, z);
		}

		Vector3i Vector3i_(int x, int y, int z)
		{
			return Vector3<int>(x, y, z);
		}

		Vector3ui Vector3ui_(unsigned int x, unsigned int y, unsigned int z)
		{
			return Vector3<unsigned int>(x, y, z);
		}

		Vector2f Vector2f_(float x, float y)
		{
			return Vector2<float>(x, y);
		}

		Vector2i Vector2i_(int x, int y)
		{
			return Vector2<int>(x, y);
		}

		Vector2ui Vector2ui_(unsigned int x, unsigned int y)
		{
			return Vector2<unsigned int>(x, y);
		}
	}
}