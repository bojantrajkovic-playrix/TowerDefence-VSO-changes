#pragma once
#include "Visual/Transform.h"

namespace math
{
	inline static void RotateToDirection(Visual::Transform& transform, const math::Vector3 direction)
	{
		math::Quaternion q;

		float angle = atan2(direction.y, direction.x);
		q.x = 0;
		q.y = 0;
		q.z = 1 * sin(angle / 2.0f);;
		q.w = cos(angle / 2.0f);

		transform.SetLocalQuaternion(q);
	}

	inline static void RotateVectorByQuaternion(const math::Vector3& v, const math::Quaternion& q, math::Vector3& vprime)
	{
		// Extract the vector part of the quaternion
		math::Vector3 u(q.x, q.y, q.z);

		// Extract the scalar part of the quaternion
		float s = q.w;

		// Do the math
		vprime = 2.0f * u.DotProduct(v) * u
			+ (s*s - u.DotProduct(u)) * v
			+ 2.0f * s * u.CrossProduct(v);
	}
}