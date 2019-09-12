#pragma once
#include "Visual/Behaviour/Behaviour.h"
#include "Visual/UI/Text/LabelComponent.h"
#include "GameManager.h"

namespace AssetDefence
{
	class UiLabelCoins :public Visual::Behaviour
	{
		VISUAL_CLASS(Visual::Behaviour)

	public:

		void OnInit() override;
		void OnEnabled() override;
		void OnDisabled() override;
		void OnStart() override;

		void ChangeLabel(const void *sender, EventArgsCoinsChange &e);

	private:

		Delegate<EventArgsCoinsChange> _onCoinsChange;
		Visual::UI::LabelComponent::Ptr _label;
	};
}