#include "UiLabelCoins.h"

void AssetDefence::UiLabelCoins::OnInit()
{
	if (_label == nullptr)
	{
		_label = GetNode().GetComponent<Visual::UI::LabelComponent>();
	}
}

void AssetDefence::UiLabelCoins::OnEnabled()
{
	_onCoinsChange = Delegate<EventArgsCoinsChange>::Create<UiLabelCoins, &UiLabelCoins::ChangeLabel>(this);
	GameManager::Instance().OnCoinsChange += _onCoinsChange;
}

void AssetDefence::UiLabelCoins::OnDisabled()
{
	GameManager::Instance().OnCoinsChange -= _onCoinsChange;
}

void AssetDefence::UiLabelCoins::OnStart()
{
	_label.get()->SetText("Coins:" + GameManager::Instance().GetCoins());
}

void AssetDefence::UiLabelCoins::ChangeLabel(const void *sender, EventArgsCoinsChange &e)
{
	Assert2(_label != nullptr, "label component not found");
	_label.get()->SetText("Coins:" + std::to_string(e.coins));
}
