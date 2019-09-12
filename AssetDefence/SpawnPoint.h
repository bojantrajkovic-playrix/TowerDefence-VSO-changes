//◦ Playrix ◦
#pragma once
#include "Visual/Behaviour/Behaviour.h"
#include "Visual/Assets/AssetRef.h"
#include "ActorData.h"

namespace AssetDefence
{
	struct SpawnInfo : public Visual::Serializable
	{
		VISUAL_STRUCT();

		/// @property
		float interval = 10.0f;

		/// @property
		Visual::AssetRef<ActorData> _asset;
	};

	class SpawnPoint : public Visual::Behaviour
	{
		VISUAL_CLASS(Visual::Behaviour)

	protected:
		void Spawn();
		void InitializePath();
		void Update(float dt) override;
		
	private:
		/// @property
		std::vector<SpawnInfo> _spawns;
		/// @property
		FPoint _spawnRange;
		/// @property
		bool _isActive;
		/// @property @range(0.1f, 100.0f)
		float _intensity = 1.0f;

		float _curInterval = 0;

		/// @property
		Visual::NodeRef _path;

		bool _pathInitialized;

		std::vector<FPoint> _pathPoints;
	};
}
