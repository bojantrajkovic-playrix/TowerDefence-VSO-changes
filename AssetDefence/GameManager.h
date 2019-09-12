#pragma once
#include "Actor.h"
#include "Visual/Behaviour/LocatorBehaviour.h"

namespace AssetDefence
{
	class EventArgsCoinsChange
	{
	public:
		int coins;
	};

	class EventArgsLivesChange
	{
	public:
		int lives;
	};

	class GameManager :public Singleton<GameManager>
	{
		friend class Singleton<GameManager>;

		GameManager()
		{
			_delegateEnemyDie = Delegate<EnemyDieEventArgs>::Create<GameManager, &GameManager::OnEnemyDied>(this);
			EnemyActor::OnEnemyDie += _delegateEnemyDie;
			_isLoading = false;
		}

		~GameManager()
		{
			EnemyActor::OnEnemyDie -= _delegateEnemyDie;
		}

	public:

		void Initialize();
		void Update(float dt);

		inline bool IsLoading() { return _isLoading; }
		inline int GetCoins() { return _coins; }
		inline int GetLives() { return _lives; }

		void RemoveCoins(int amount);
		void RemoveLife();

		//Events
		Event<EventArgsCoinsChange> OnCoinsChange;
		Event<EventArgsLivesChange> OnLivesChange;

		//Incoming Delegates
		void OnEnemyDied(const void *sender, EnemyDieEventArgs &e);
		Delegate<EnemyDieEventArgs> _delegateEnemyDie;

	private:

		int _lives;
		int _coins;
		bool _isLoading = false;
	};
}