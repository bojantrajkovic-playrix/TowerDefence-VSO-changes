#pragma once
#include "Visual/Behaviour/Behaviour.h"
#include "ActorData.h"

namespace AssetDefence
{
	class EnemyDieEventArgs : public EventArgs
	{
	public:
		int coins;
	};

	class Actor : public Visual::Behaviour
	{
		VISUAL_CLASS(Visual::Behaviour)

	public:
		virtual void SetData(const ActorData::Ptr& data) {};
		virtual void OnCollision(Actor* other) {};
		virtual void OnCollisionStay(Actor* other) {};
		virtual void OnCollisionExit(Actor* other) {};

	private:
		void OnEnabled() override;

	protected:
		bool _isAlive = true;
	};

	class EnemyActor : public Actor
	{
		VISUAL_CLASS(Actor)

	public:
		using Ptr = boost::intrusive_ptr<EnemyActor>;

		void SetData(const ActorData::Ptr& data) override;
		void Update(float dt) override;
		void OnCollision(Actor* other) override;

		float GetDamage() const { return _data->damage; }
		void Die();
		void SetPath(std::vector<FPoint>& pathPoints);
		void TakeDamage(const float damage);

		static Event<EnemyDieEventArgs> OnEnemyDie;

	private:
		EnemyActorData::Ptr _data;
		std::vector<FPoint>* _pathPoints;

		float _health = 0;
		int _currentPointIndex;
	};

	class DefenceActor : public Actor
	{
		VISUAL_CLASS(Actor)

	private:

		DefenceActorData::Ptr _data;
	};
}