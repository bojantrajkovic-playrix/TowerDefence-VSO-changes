#pragma once
#include "Visual/Behaviour/Behaviour.h"
#include "Visual/UI/Text/LabelComponent.h"
#include "GameManager.h"

namespace AssetDefence
{
	class UiLabelLives :public Visual::Behaviour
	{
		VISUAL_CLASS(Visual::Behaviour)

	public:

		void OnInit() override;
		void OnEnabled() override;
		void OnDisabled() override;

		void ChangeLives(const void *sender, EventArgsLivesChange &e);

	private:

		Delegate<EventArgsLivesChange> _onLivesChange;
		Visual::UI::LabelComponent::Ptr _label;
	};
}