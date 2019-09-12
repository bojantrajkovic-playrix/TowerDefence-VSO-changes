#include "stdafx.h"
#include "Actor.h"
#include "Visual/Sorting/SortingStrategy.h"
#include "Visual/Scene.h"
#include "ActorCollisionHandler.h"
#include "Visual/Assets/SOCollection.h"
#include "Bullet.h"
#include "GameManager.h"

Event<AssetDefence::EnemyDieEventArgs> AssetDefence::EnemyActor::OnEnemyDie;

void AssetDefence::Actor::OnEnabled()
{
	auto node = GetNode();
	auto sorting = node.GetSorting();
	if (sorting) {
		const auto priority = static_cast<int>(-node.GetTransform().GetWorldPosition2D().y);
		sorting->SetPriority(priority);
	}

	node.GetOrCreateBehaviour<ActorCollisionHandler>();
}

void AssetDefence::EnemyActor::SetData(const ActorData::Ptr& data)
{
	_data = boost::dynamic_pointer_cast<EnemyActorData>(data);

	_health = _data->health;
}

void AssetDefence::EnemyActor::Update(float dt)
{
	Assert(_data);

	auto&& node = GetNode();

	if (!_isAlive)
	{
		node.GetOwner().RemoveFromScene();
		return;
	}

	float distance = _pathPoints->at(_currentPointIndex).GetDistanceTo(node.GetTransform().GetWorldPosition2D());

	if (distance < 2.0f)
	{
		if ((unsigned int)_currentPointIndex < _pathPoints->size() - 1)
		{
			++_currentPointIndex;
		}
	}

	FPoint direction = _pathPoints->at(_currentPointIndex) - node.GetTransform().GetWorldPosition2D();
	direction.Normalize();

	node.GetTransform().SetWorldPosition2D(node.GetTransform().GetWorldPosition2D() + direction * _data->speed * dt);
}

void AssetDefence::EnemyActor::OnCollision(Actor* other)
{
	const auto& defender = dynamic_cast<DefenceActor*>(other);
	if (defender)
	{
		Die();
		GameManager::Instance().RemoveLife();
		return;
	}

	const auto& bullet = dynamic_cast<BulletActor*>(other);
	if (bullet)
	{
		TakeDamage(bullet->GetDamage());
	}
}

void AssetDefence::EnemyActor::Die()
{
	// Spawn die effect
	if (!_data->explosion.empty() && Visual::SOCollection::IsAssetExist(_data->explosion)) {
		auto node = GetNode();
		auto pos = Visual::TransformParams().SetPos(node.GetTransform().GetWorldPosition());
		auto obj = GetScene()->InstantiateToScene(_data->explosion, pos);
	}
	// Mark as die to destroy on next frame
	_isAlive = false;

	EnemyDieEventArgs dieArgs;
	dieArgs.coins = _data.get()->coins;
	OnEnemyDie.Invoke(this, dieArgs);
}

void AssetDefence::EnemyActor::SetPath(std::vector<FPoint>& pathPoints)
{
	_pathPoints = &pathPoints;
	_currentPointIndex = 0;
}

void AssetDefence::EnemyActor::TakeDamage(const float damage)
{
	if (!_isAlive)
	{
		return;
	}

	_health -= damage;

	if (_health <= 0)
	{
		Die();
	}
}