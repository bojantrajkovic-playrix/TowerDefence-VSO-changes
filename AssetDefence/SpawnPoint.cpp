//◦ Playrix ◦
#include "stdafx.h"
#include "SpawnPoint.h"
#include "Gdb/Actors.h"
#include <Gdb/G_SOCollection.h>
#include "Visual/Factory.h"
#include "Actor.h"

void AssetDefence::SpawnPoint::Spawn()
{
	if (_spawns.empty()) {
		_curInterval = 10;
		return;
	}

	const size_t index = math::random(size_t(0), _spawns.size() - 1);
	auto&& spawn = _spawns[index];

	const auto actorInfo = spawn._asset.Get();

	if (actorInfo.get()->actorType == "enemyActor")
	{
		auto object = GetScene()->InstantiateToScene(actorInfo->visual);
		// Setup position with random offset
		const auto pos = GetTransform().GetWorldPosition2D();
		const auto offset = FPoint(math::random(-_spawnRange.x, _spawnRange.x), math::random(-_spawnRange.y, _spawnRange.y));
		object.GetTransform().SetWorldPosition2D(pos + offset);
		// Create actor (game object)
		if (!actorInfo->actorType.empty()) 
		{
			const auto gameObject = Visual::BehaviourFactory::Instance().Create(actorInfo->actorType);
			Assert2(gameObject, "Actor creation failed! " + actorInfo->actorType);
			if (gameObject) {
				auto actor = boost::dynamic_pointer_cast<EnemyActor>(gameObject);
				actor->SetData(actorInfo);

				actor->SetPath(_pathPoints);

				object.GetRoot().AddBehaviour(actor);
			}
		}
	}
	else
	{
		auto object = GetScene()->InstantiateToScene(actorInfo->visual);
		// Setup position with random offset
		const auto pos = GetTransform().GetWorldPosition2D();
		const auto offset = FPoint(math::random(-_spawnRange.x, _spawnRange.x), math::random(-_spawnRange.y, _spawnRange.y));
		object.GetTransform().SetWorldPosition2D(pos + offset);
		// Create actor (game object)
		if (!actorInfo->actorType.empty()) {
			const auto gameObject = Visual::BehaviourFactory::Instance().Create(actorInfo->actorType);
			Assert2(gameObject, "Actor creation failed! " + actorInfo->actorType);
			if (gameObject) {
				auto actor = boost::dynamic_pointer_cast<Actor>(gameObject);
				actor->SetData(actorInfo);

				object.GetRoot().AddBehaviour(actor);
			}
		}
	}

	_curInterval = spawn.interval;
}

void AssetDefence::SpawnPoint::Update(float dt)
{
	if (_isActive)
	{
		if (!_pathInitialized)
		{
			_pathInitialized = true;
			InitializePath();
		}

		_curInterval -= dt * _intensity;
		if (_curInterval <= 0) {
			Spawn();
		}
	}
}

void AssetDefence::SpawnPoint::InitializePath()
{
	auto children = _path.Get().GetChildren();

	for (auto node : children)
	{
		_pathPoints.emplace_back(node.GetTransform().GetWorldPosition2D());
	}
}