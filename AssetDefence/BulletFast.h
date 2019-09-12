#pragma once

#include "Visual/GameObject.h"
#include "Bullet.h"

namespace AssetDefence
{
	class BulletFast : public BulletActor
	{
		VISUAL_CLASS(BulletActor)

	public:
		
		// Inherited via BulletActor
		void Update(float dt) override;
		void OnCollision(Actor * other) override;
		void Die() override;
		void SetDirection(const math::Vector3 direction) override;
		void SetData(const AssetDefence::ActorData::Ptr &data);
	};
}