#pragma once

#include "Visual/Behaviour/Behaviour.h"

namespace Visual
{
	struct Collision;

	/// @abstract
	class CollisionHandler : public Behaviour
	{
		VISUAL_CLASS(Behaviour)

	public:
		using Ptr = boost::intrusive_ptr<CollisionHandler>;
		using WeakPtr = intrusive_weak_ptr<CollisionHandler>;

		virtual void OnCollisionEnter(const Collision& collision);
		virtual void OnCollisionStay(const Collision& collision);
		virtual void OnCollisionExit(const Collision& collision);
	};

}
