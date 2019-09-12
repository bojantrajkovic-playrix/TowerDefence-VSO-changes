#pragma once

#include "Collider.h"
#include "Collision.h"
#include "CollisionHandler.h"

namespace Visual
{
	class Scene;

	class CollisionSolver : public Visual::Behaviour
	{
		VISUAL_CLASS(Visual::Behaviour)

	public:
		void AddCollider(Collider* collider);
		void RemoveCollider(Collider* collider);
		void Solve(float dt, Scene* scene);

		void OnCollisionEnter(const Collision& collision) const;
		void OnCollisionStay(const Collision& collision) const;
		void OnCollisionExit(const Collision& collision) const;

		static boost::intrusive_ptr<CollisionSolver> FindSolver(Scene* scene);
		static bool CheckCollision(const Collider::Ptr& first, const Collider::Ptr& second, math::Vector3& hitPoint);

	protected:
		void Update(float dt) override;

	private:
		void Notify(const Collision& collision, const Collider::WeakPtr& collider, void (CollisionHandler::*method)(const Collision&)) const;

		using CollisionCollection = std::map<std::pair<Collider*, Collider*>, Collision>;
		CollisionCollection::iterator FindCollision(const Collider::Ptr& first, const Collider::Ptr& second);

	private:
		std::vector<Collider::WeakPtr> _colliders;
		CollisionCollection _collisions;
	};
}
