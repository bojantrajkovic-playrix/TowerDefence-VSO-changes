#include "TowerActor.h"
#include <Gdb/G_SOCollection.h>
#include "Bullet.h"

void AssetDefence::TowerActor::Update(float dt)
{
	if (_data == nullptr)
	{
		return;
	}

	if (_target != nullptr)
	{
		_currentRateOfFire -= dt;

		if (_currentRateOfFire < 0)
		{
			Shoot();
			_currentRateOfFire = _towerAssetData.Get().get()->rateOfFire;
		}

		return;
	}

	if (_target == nullptr && _enemiesInRange.size() > 0)
	{
		_target = _enemiesInRange[0];
	}
}

void AssetDefence::TowerActor::OnCollision(Actor* other)
{
	EnemyActor* enemy = dynamic_cast<EnemyActor*>(other);

	if (enemy != nullptr)
	{
		_enemiesInRange.emplace_back(enemy);
	}
}

void AssetDefence::TowerActor::OnCollisionExit(Actor* other)
{
	EnemyActor* enemy = dynamic_cast<EnemyActor*>(other);

	if (enemy != nullptr)
	{
		RemoveEnemyFromList(enemy);
	}
}

void AssetDefence::TowerActor::RemoveEnemyFromList(EnemyActor* enemy)
{
	for (std::vector<EnemyActor*>::iterator iter = _enemiesInRange.begin(); iter != _enemiesInRange.end(); ++iter)
	{
		if (*iter == enemy)
		{
			if (_target == enemy)
			{
				_target = nullptr;
			}

			_enemiesInRange.erase(iter);
			break;
		}
	}
}

void AssetDefence::TowerActor::EnemyDied(const void *sender, EnemyDieEventArgs &e)
{
	EnemyActor* enemy = const_cast<EnemyActor*>(static_cast<const EnemyActor*>(sender));
	RemoveEnemyFromList(enemy);
}

void AssetDefence::TowerActor::Shoot()
{
	auto bullet = GetScene()->InstantiateToScene(_towerAssetData.Get()->visual);
	bullet.GetRoot().GetTransform().SetWorldPosition2D(_shootPosition.Get().GetTransform().GetWorldPosition2D());
	math::Vector3 direction = _target.get()->GetTransform().GetWorldPosition() - _shootPosition.Get().GetTransform().GetWorldPosition();

	bullet.GetRoot().GetBehaviour<AssetDefence::BulletActor>().get()->SetDirection(direction);
}

void AssetDefence::TowerActor::SetData(const AssetDefence::ActorData::Ptr &data)
{
}