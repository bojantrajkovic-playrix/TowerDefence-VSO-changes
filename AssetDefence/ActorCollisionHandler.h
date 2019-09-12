#pragma once
#include "Collision/CollisionHandler.h"

namespace AssetDefence
{
	class ActorCollisionHandler : public Visual::CollisionHandler
	{
		VISUAL_CLASS(CollisionHandler)

	public:
		void OnCollisionEnter(const Visual::Collision& collision) override;
		void OnCollisionStay(const Visual::Collision& collision) override;
		void OnCollisionExit(const Visual::Collision& collision) override;
	private:

		void OwnerCheck(const Visual::Collision& collision);

		Visual::SceneObject _self;
		Visual::SceneObject _other;
	};
}
