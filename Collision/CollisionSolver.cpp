#include "stdafx.h"
#include "CollisionSolver.h"
#include "CollisionSettings.h"
#include "Visual/Scene.h"
#include "Visual/SceneObjectImpl.h"

void Visual::CollisionSolver::AddCollider(Collider* collider)
{
	_colliders.emplace_back(collider);
}

void Visual::CollisionSolver::RemoveCollider(Collider* collider)
{
	const auto it = std::remove(_colliders.begin(), _colliders.end(), Collider::WeakPtr(collider));
	_colliders.erase(it, _colliders.end());
}

bool Visual::CollisionSolver::CheckCollision(const Collider::Ptr& first, const Collider::Ptr& second, math::Vector3& hitPoint)
{
	return first->CheckCollision(second, hitPoint);
}

void Visual::CollisionSolver::Update(float dt)
{
	Solve(dt, GetScene());
}

Visual::CollisionSolver::CollisionCollection::iterator Visual::CollisionSolver::FindCollision(const Collider::Ptr& first, const Collider::Ptr& second)
{
	const auto it = _collisions.find(std::make_pair(first.get(), second.get()));
	if (it != _collisions.end()) {
		return it;
	}

	return _collisions.end();
}

void Visual::CollisionSolver::Solve(float dt, Scene* scene)
{
	const auto frameNumber = FrameCounter::GetGlobalFrameNum();

	const auto settings = scene->GetSettings<CollisionSettings>();

	// I don't care about performance because number of colliders are to low to wary about
	for (size_t i = 0; (i + 1) < _colliders.size(); ++i) {
		auto first = _colliders[i].lock();
		for (size_t j = i + 1; j < _colliders.size(); ++j) {
			auto second = _colliders[j].lock();
			// Skip if collision between layers disabled
			if (!settings->IsLayersCollide(first->GetLayer(), second->GetLayer())) {
				continue;
			}

			math::Vector3 hitPoint;
			if (CheckCollision(first, second, hitPoint)) {
				// Is collision exists?
				auto it = FindCollision(first, second);
				if (it != _collisions.end()) {
					// Just update duration and frame number
					it->second.duration += dt;
					it->second.frameNumber = frameNumber;
					OnCollisionStay(it->second);
				}
				else {
					// Create collision
					Collision collision;
					collision.frameNumber = frameNumber;
					collision.first = first;
					collision.second = second;
					collision.hitPosition = hitPoint;
					OnCollisionEnter(collision);
					_collisions.emplace(std::make_pair(first.get(), second.get()), std::move(collision));
				}
			}
		}
	}
	// Process old collisions
	for (auto it = _collisions.begin(); it != _collisions.end();) {
		if (it->second.frameNumber != frameNumber) {
			OnCollisionExit(it->second);
			it = _collisions.erase(it);
		}
		else {
			++it;
		}
	}
}

void Visual::CollisionSolver::OnCollisionEnter(const Collision& collision) const
{
	if (!collision.first || !collision.second) {
		return;
	}
	Notify(collision, collision.first, &CollisionHandler::OnCollisionEnter);
	Notify(collision, collision.second, &CollisionHandler::OnCollisionEnter);
}

void Visual::CollisionSolver::OnCollisionStay(const Collision& collision) const
{
	if (!collision.first || !collision.second) {
		return;
	}
	Notify(collision, collision.first, &CollisionHandler::OnCollisionStay);
	Notify(collision, collision.second, &CollisionHandler::OnCollisionStay);
}

void Visual::CollisionSolver::OnCollisionExit(const Collision& collision) const
{
	if (!collision.first || !collision.second) {
		return;
	}
	Notify(collision, collision.first, &CollisionHandler::OnCollisionExit);
	Notify(collision, collision.second, &CollisionHandler::OnCollisionExit);
}

boost::intrusive_ptr<Visual::CollisionSolver> Visual::CollisionSolver::FindSolver(Scene* scene)
{
	constexpr auto OBJECT_NAME = "_collision_";

	auto obj = scene->FindObject(OBJECT_NAME);
	if (!obj) {
		obj = SceneObject(SceneObjectImpl::CreateWithRoot(scene));
		obj.SetName(OBJECT_NAME);
		auto root = obj.GetRoot();
		return root.AddBehaviour<CollisionSolver>();
	}
	return obj.GetRoot().GetBehaviour<CollisionSolver>();
}

void Visual::CollisionSolver::Notify(const Collision& collision, const Collider::WeakPtr& collider, void (CollisionHandler::*method)(const Collision&)) const
{
	auto node = collider.lock()->GetNode();
	while (node) {
		for (auto&& behaviour : node.GetBehaviours()) {
			const auto handler = dynamic_cast<CollisionHandler*>(behaviour);
			if (handler) {
				auto& obj = *handler;
				(obj.*method)(collision);
			}
		}

		node = node.GetParent();
	}
}
