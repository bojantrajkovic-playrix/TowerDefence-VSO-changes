#pragma once
#include "Visual/Behaviour/Behaviour.h"
#include "Actor.h"
#include "Visual/Assets/AssetRef.h"

namespace AssetDefence
{
	class BulletActor : public Actor
	{
		VISUAL_CLASS(Visual::Behaviour)

	public:

		inline void OnEnabled() override { _data = _bulletAssetData.Get(); };
		void SetData(const ActorData::Ptr& data) override = 0;
		void Update(float dt) override = 0;
		void OnCollision(Actor* other) override = 0;

		inline float GetDamage() const { return _data->damage; }
		virtual void Die() = 0;
		virtual void SetDirection(const math::Vector3 direction) = 0;

	protected:

		/// @property @header(GDB - asset refs)
		Visual::AssetRef<BulletActorData> _bulletAssetData;

		BulletActorData::Ptr _data;
		math::Vector3 _direction;
	};
}