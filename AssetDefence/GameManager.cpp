#include "GameManager.h"
#include "Visual/SceneManager.h"

void AssetDefence::GameManager::RemoveLife()
{
	--_lives;
	EventArgsLivesChange args;
	args.lives = _lives;
	OnLivesChange.Invoke(this, args);

	if (_lives <= 0)
	{
		//Gameover
		_isLoading = true;
	}
}

void AssetDefence::GameManager::Initialize()
{
	_lives = 3;
	_coins = 2;
	_isLoading = false;
}

void AssetDefence::GameManager::Update(float dt)
{
	if (_isLoading)
	{
		Visual::SceneManager::Instance().ReloadScene(Visual::SceneManager::Instance().GetMainSceneName());
		_isLoading = false;
	}
}

void AssetDefence::GameManager::RemoveCoins(int amount)
{
	_coins -= amount;

	if (_coins < 0)
	{
		_coins = 0;
	}

	EventArgsCoinsChange args;
	args.coins = _coins;
	OnCoinsChange.Invoke(this, args);
}

void AssetDefence::GameManager::OnEnemyDied(const void * sender, EnemyDieEventArgs& e)
{
	_coins += e.coins;

	EventArgsCoinsChange args;
	args.coins = _coins;
	OnCoinsChange.Invoke(this, args);
}