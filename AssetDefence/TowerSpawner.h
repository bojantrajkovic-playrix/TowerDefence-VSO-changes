#pragma once
#include "Visual/Behaviour/Behaviour.h"
#include "Visual/UI/Input/PointerEvent.h"
#include "TowerActor.h"

namespace AssetDefence
{
	class TowerSpawner : public Visual::Behaviour
	{
		VISUAL_CLASS(Visual::Behaviour)

	public:
		using Ptr = boost::intrusive_ptr<TowerSpawner>;

		void OnEnabled() override;
		void Update(float dt) override;
		void OnPointerUp(Visual::UI::PointerEvent& e);

	private:
		/// @property @assetType(SceneObject)
		std::string _towerPrefab;

		boost::intrusive_ptr<Visual::SceneObjectImpl> _prototype;
		boost::intrusive_ptr<AssetDefence::TowerActor> _assetTowerObj;
	};
}