#include "BulletFast.h"
#include "Visual/Assets/SOCollection.h"
#include "MathExtends.h"

void AssetDefence::BulletFast::Update(float dt)
{
	if (_data.get() == nullptr)
	{
		return;
	}

	if (!_isAlive)
	{
		GetOwner().RemoveFromScene();
		return;
	}

	GetTransform().SetLocalPosition2D(GetTransform().GetLocalPosition2D() + FPoint(_direction.x, _direction.y) * _data->speed * dt);
}

void AssetDefence::BulletFast::OnCollision(Actor * other)
{
	Die();
}

void AssetDefence::BulletFast::Die()
{
	// Spawn die effect
	if (!_data->explosion.empty() && Visual::SOCollection::IsAssetExist(_data->explosion)) {

		auto&& obj = GetScene()->InstantiateToScene(_data->explosion);
		obj.GetRoot().GetTransform().SetWorldPosition2D(GetNode().GetTransform().GetWorldPosition2D());
	}
	// Mark as die to destroy on next frame
	_isAlive = false;
}

void AssetDefence::BulletFast::SetDirection(const math::Vector3 direction)
{
	_direction = direction;
	_direction.Normalize();
	math::RotateToDirection(GetTransform(), _direction);
}

void AssetDefence::BulletFast::SetData(const AssetDefence::ActorData::Ptr &data)
{
}
