#include "stdafx.h"
#include "Collider.h"
#include "Visual/Scene.h"
#include "CollisionSolver.h"
#include "Visual/Utils/VisualRenderHelper.h"

void Visual::BoxCollider::OnEnabled()
{
	Collider::OnEnabled();

	_boundingBox = SceneGraph::AxisAlignedBoundingBox(math::Vector3::One, math::Vector3::One);
	Collider::ColliderType = ColliderTypes::Box;

	CollisionSolver::FindSolver(GetScene())->AddCollider(this);
}

void Visual::BoxCollider::OnDisabled()
{
	Collider::OnDisabled();

	CollisionSolver::FindSolver(GetScene())->RemoveCollider(this);
}

bool Visual::BoxCollider::CheckCollision(const boost::intrusive_ptr<Collider>& otherCollider, math::Vector3& hitPoint) const
{
	if (otherCollider.get()->ColliderType == ColliderTypes::Box)
	{
		const auto& box = boost::dynamic_pointer_cast<BoxCollider>(otherCollider);
		return CollisionCheck::CheckCollisionBoxBox(*this, *box, hitPoint);
	}

	if (otherCollider.get()->ColliderType == ColliderTypes::Circle)
	{
		const auto& circle = boost::dynamic_pointer_cast<CircleCollider>(otherCollider);
		return CollisionCheck::CheckCollisionBoxCircle(*this, *circle, hitPoint);
	}

	return false;
}

void Visual::BoxCollider::OnDrawGizmos(Visual::GizmosSelection selection)
{
	if ((selection & GizmosSelection::Node) == GizmosSelection::Node)
	{
		_boundingBox.GetMinEdge().TransformCoord(math::Matrix4::Compose(math::Vector3::One, math::Quaternion::Identity, _minEdge));
		_boundingBox.GetMaxEdge().TransformCoord(math::Matrix4::Compose(math::Vector3::One, math::Quaternion::Identity, _maxEdge));

		_boundingBox = _boundingBox.Transform(GetNode().GetTransform().GetLocalTransform());

		Visual::VisualRenderHelper::GetLineDrawer().DrawBoundingBox(_boundingBox, Color::GREEN);
	}
}

void Visual::CircleCollider::OnDrawGizmos(Visual::GizmosSelection selection)
{
	if ((selection & GizmosSelection::Node) == GizmosSelection::Node) {
		Visual::VisualRenderHelper::DrawCircle(GetTransform().GetWorldPosition2D(), Radius, Color::GREEN);
	}
}

bool Visual::CircleCollider::CheckCollision(const boost::intrusive_ptr<Collider>& otherCollider, math::Vector3& hitPoint) const
{
	if (otherCollider.get()->ColliderType == ColliderTypes::Circle)
	{
		const auto& circle = boost::static_pointer_cast<CircleCollider>(otherCollider);
		return CollisionCheck::CheckCollisionCircleCircle(*this, *circle, hitPoint);
	}

	if (otherCollider.get()->ColliderType == ColliderTypes::Box)
	{
		const auto& box = boost::static_pointer_cast<BoxCollider>(otherCollider);
		return CollisionCheck::CheckCollisionBoxCircle(*box.get(), *this, hitPoint);
	}

	return false;
}

void Visual::CircleCollider::OnEnabled()
{
	Collider::OnEnabled();
	Collider::ColliderType = ColliderTypes::Circle;
	CollisionSolver::FindSolver(GetScene())->AddCollider(this);
}

void Visual::CircleCollider::OnDisabled()
{
	Collider::OnDisabled();

	CollisionSolver::FindSolver(GetScene())->RemoveCollider(this);
}

bool Visual::CollisionCheck::CheckCollisionCircleCircle(const CircleCollider& first, const CircleCollider& second, math::Vector3 & hitPoint)
{
	const auto path = second.GetTransform().GetWorldPosition() - first.GetTransform().GetWorldPosition();
	const float distance = path.Length();
	const float radiusSum = second.Radius + first.Radius;

	if (distance <= radiusSum)
	{
		hitPoint = first.GetTransform().GetWorldPosition() + path * (first.Radius / radiusSum);
		return true;
	}

	return false;
}

bool Visual::CollisionCheck::CheckCollisionBoxBox(const BoxCollider& first, const BoxCollider& second, math::Vector3& hitPoint)
{
	if (first.GetAABB().Intersects(second.GetAABB()))
		return true;

	return false;
}

bool Visual::CollisionCheck::CheckCollisionBoxCircle(const BoxCollider& first, const CircleCollider& second, math::Vector3& hitPoint)
{
	float rectWidth = first.GetAABB().GetMaxEdge().x - first.GetAABB().GetMinEdge().x;
	float rectHeight = first.GetAABB().GetMaxEdge().y - first.GetAABB().GetMinEdge().y;

	float nearestX = math::max(first.GetAABB().GetMinEdge().x, math::min(second.GetTransform().GetWorldPosition().x, first.GetAABB().GetMinEdge().x + rectWidth));
	float nearestY = math::max(first.GetAABB().GetMinEdge().y, math::min(second.GetTransform().GetWorldPosition().y, first.GetAABB().GetMinEdge().y + rectHeight));

	float deltaX = second.GetTransform().GetWorldPosition().x - nearestX;
	float deltaY = second.GetTransform().GetWorldPosition().y - nearestY;

	if ((deltaX * deltaX + deltaY * deltaY) < (second.Radius * second.Radius))
	{
		hitPoint = math::Vector3(nearestX, nearestY, 0);
		return true;
	}

	return false;
}
