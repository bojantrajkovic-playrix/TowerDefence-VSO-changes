#include "stdafx.h"
#include "ActorCollisionHandler.h"
#include "Collision/Collision.h"
#include "Collision/Collider.h"
#include "Actor.h"

void AssetDefence::ActorCollisionHandler::OnCollisionEnter(const Visual::Collision& collision)
{
	OwnerCheck(collision);

	auto selfActor = _self.GetRoot().GetBehaviour<Actor>();
	auto otherActor = _other.GetRoot().GetBehaviour<Actor>();

	if (selfActor && otherActor)
	{
		selfActor->OnCollision(otherActor.get());
	}
}

void AssetDefence::ActorCollisionHandler::OnCollisionStay(const Visual::Collision & collision)
{
	OwnerCheck(collision);

	auto selfActor = _self.GetRoot().GetBehaviour<Actor>();
	auto otherActor = _other.GetRoot().GetBehaviour<Actor>();

	if (selfActor && otherActor)
	{
		selfActor->OnCollisionStay(otherActor.get());
	}
}

void AssetDefence::ActorCollisionHandler::OnCollisionExit(const Visual::Collision & collision)
{
	OwnerCheck(collision);

	auto selfActor = _self.GetRoot().GetBehaviour<Actor>();
	auto otherActor = _other.GetRoot().GetBehaviour<Actor>();

	if (selfActor && otherActor)
	{
		selfActor->OnCollisionExit(otherActor.get());
	}
}

void AssetDefence::ActorCollisionHandler::OwnerCheck(const Visual::Collision& collision)
{
	_self = collision.first.lock()->GetOwner();
	_other = collision.second.lock()->GetOwner();

	auto test = GetNode();

	if (GetOwner() != _self) {
		std::swap(_self, _other);
	}
}
