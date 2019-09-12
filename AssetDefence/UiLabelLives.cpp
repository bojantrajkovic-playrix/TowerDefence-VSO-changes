#include "UiLabelLives.h"

void AssetDefence::UiLabelLives::OnInit()
{
	if (_label == nullptr)
	{
		_label = GetNode().GetComponent<Visual::UI::LabelComponent>();
	}
}

void AssetDefence::UiLabelLives::OnEnabled()
{
	_onLivesChange=Delegate<EventArgsLivesChange>::Create<UiLabelLives, &UiLabelLives::ChangeLives>(this);
	GameManager::Instance().OnLivesChange += _onLivesChange;

	_label.get()->SetText("Lives:");
	_label.get()->SetText("Lives:" + GameManager::Instance().GetLives());
}

void AssetDefence::UiLabelLives::OnDisabled()
{
	GameManager::Instance().OnLivesChange -= _onLivesChange;
}

void AssetDefence::UiLabelLives::ChangeLives(const void *sender, EventArgsLivesChange &e)
{
	Assert2(_label != nullptr, "label component not found");
	_label.get()->SetText("Lives:" + std::to_string(e.lives));
}