#pragma once

#include "Visual/FrameCounter.h"

namespace Visual
{
	class Collider;

	struct Collision
	{
		intrusive_weak_ptr<Collider> first;
		intrusive_weak_ptr<Collider> second;
		float duration = 0;
		math::Vector3 hitPosition;
		FrameNum frameNumber;
	};
}
