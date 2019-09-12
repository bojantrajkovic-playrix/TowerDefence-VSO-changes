#pragma once
#include "Visual/Behaviour/Behaviour.h"
#include "Actor.h"
#include "Visual/Assets/AssetRef.h"
#include "Collision/Collider.h"

namespace AssetDefence
{
	class TowerActor : public Actor
	{
		VISUAL_CLASS(Actor)

	public:

		using Ptr = boost::intrusive_ptr<TowerActor>;

		inline void OnEnabled() override
		{
			_data = _towerAssetData.Get();
			if (_data != nullptr)
			{
				_rangeCollider.Get().get()->Radius = _data.get()->range;
			}

			EnemyActor::OnEnemyDie += _enemyDieDelegate.Create<TowerActor, &TowerActor::EnemyDied>(this);
		};

		inline void OnDisabled() override
		{
			EnemyActor::OnEnemyDie.Remove(_enemyDieDelegate);
		};

		inline int GetCost() { return _towerAssetData.Get().get()->cost; }

		void SetData(const AssetDefence::ActorData::Ptr &data) override;
		void Update(float dt) override;
		void OnCollision(Actor* other) override;
		void OnCollisionExit(Actor* other) override;

		void EnemyDied(const void *sender, EnemyDieEventArgs &e);
		void RemoveEnemyFromList(EnemyActor* enemy);
		void Shoot();

	private:

		///@property
		Visual::NodeRef _shootPosition;
		///@property
		Visual::RefWrapper<Visual::CircleCollider> _rangeCollider;

		/// @property @header(GDB - asset refs)
		Visual::AssetRef<TowerActorData> _towerAssetData;

		Delegate<EnemyDieEventArgs> _enemyDieDelegate;
		TowerActorData::Ptr _data;
		EnemyActor::Ptr _target;
		std::vector<EnemyActor*> _enemiesInRange;
		float _currentRateOfFire;
	};
}