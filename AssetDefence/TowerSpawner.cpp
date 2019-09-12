#include "TowerSpawner.h"
#include "GameManager.h"
#include "Visual/Assets/SOCollection.h"

void AssetDefence::TowerSpawner::OnEnabled()
{
	GameManager::Instance().Initialize();

	_prototype = Visual::SOCollection::Find(_towerPrefab);
	_assetTowerObj = _prototype.get()->GetRoot().get()->GetBehaviour<AssetDefence::TowerActor>();
}

void AssetDefence::TowerSpawner::Update(float dt)
{
	GameManager::Instance().Update(dt);
}

void AssetDefence::TowerSpawner::OnPointerUp(Visual::UI::PointerEvent& e)
{	
	if (AssetDefence::GameManager::Instance().GetCoins() >= _assetTowerObj.get()->GetCost())
	{
		e.SetCurrentCamera(GetScene()->GetMainCameraPtr());
		auto&& tower = GetScene()->InstantiateToScene(_towerPrefab);
		tower.GetRoot().GetTransform().SetWorldPosition(e.GetPointerWorldPosition());

		GameManager::Instance().RemoveCoins(_assetTowerObj.get()->GetCost());
	}
}