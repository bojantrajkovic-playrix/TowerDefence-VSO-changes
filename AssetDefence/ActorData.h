//◦ Playrix ◦
#pragma once
#include "Visual/Assets/Asset.h"

namespace AssetDefence
{
	/// @gdb(name=Actors)
	class ActorData : public Visual::Asset
	{
		VISUAL_CLASS(Visual::Asset)

	public:
		using Ptr = boost::intrusive_ptr<ActorData>;

		/// @property
		std::string actorType;
		/// @property @assetType(SceneObject)
		std::string visual;
		/// @property @assetType(SceneObject)
		std::string explosion;
	};

	class EnemyActorData : public ActorData
	{
		VISUAL_CLASS(ActorData)

	public:
		using Ptr = boost::intrusive_ptr<EnemyActorData>;

		/// @property
		float speed = 1;
		/// @property
		float health = 10;
		/// @property
		float damage = 2;
		/// @property
		float coins = 1;
	};

	class DefenceActorData : public ActorData
	{
		VISUAL_CLASS(ActorData)

	public:
		using Ptr = boost::intrusive_ptr<DefenceActorData>;

		/// @property
		float health = 10;
		/// @property
		float damage = 2;
		/// @property
		float cooldown = 1;
	};

	class BulletActorData : public ActorData
	{
		VISUAL_CLASS(ActorData)

	public:
		using Ptr = boost::intrusive_ptr<BulletActorData>;

		/// @property
		float speed = 10;
		/// @property
		float damage = 2;
	};

	class TowerActorData : public ActorData
	{
		VISUAL_CLASS(ActorData)

	public:
		using Ptr = boost::intrusive_ptr<TowerActorData>;

		/// @property
		float cost;
		/// @property
		float range;
		/// @property
		float rateOfFire;
		/// @property
		float bonnusDamage = 2;
	};
}