#pragma once

#include "Visual/Behaviour/Behaviour.h"

namespace Visual
{
	ENGINE_ENUM(ColliderTypes, uint32_t, Circle, Box);

	/// @abstract
	class Collider : public Behaviour
	{
		VISUAL_CLASS(Behaviour)

	public:
		using Ptr = boost::intrusive_ptr<Collider>;
		using WeakPtr = intrusive_weak_ptr<Collider>;

		ColliderTypes ColliderType;

		const std::string& GetLayer() const { return _layer; }

		virtual bool CheckCollision(const boost::intrusive_ptr<Collider>& other, math::Vector3& hitPoint) const = 0;

	protected:
		/// @property
		std::string _layer;
	};

	class CircleCollider : public Collider
	{
		VISUAL_CLASS(Collider)

	public:

		void OnDrawGizmos(Visual::GizmosSelection) override;

		bool CheckCollision(const boost::intrusive_ptr<Collider>& other, math::Vector3& hitPoint) const override;

		/// @property
		float Radius = 0;

	protected:
		void OnEnabled() override;
		void OnDisabled() override;
	};

	class BoxCollider : public Collider
	{
		VISUAL_CLASS(Collider)

	public:

		void OnDrawGizmos(Visual::GizmosSelection) override;

		inline SceneGraph::AxisAlignedBoundingBox GetAABB() const { return _boundingBox; }

		bool CheckCollision(const boost::intrusive_ptr<Collider>& other, math::Vector3& hitPoint) const override;

	protected:
		void OnEnabled() override;
		void OnDisabled() override;
	private:

		/// @property
		math::Vector3 _minEdge;

		/// @property
		math::Vector3 _maxEdge;

		SceneGraph::AxisAlignedBoundingBox _boundingBox;
	};

	static class CollisionCheck final
	{
	public:

		static bool CheckCollisionCircleCircle(const CircleCollider& first, const CircleCollider& second, math::Vector3& hitPoint);
		static bool CheckCollisionBoxBox(const BoxCollider& first, const BoxCollider& second, math::Vector3& hitPoint);
		static bool CheckCollisionBoxCircle(const BoxCollider& first, const CircleCollider& second, math::Vector3& hitPoint);

	private:

		CollisionCheck() = default;
		CollisionCheck(CollisionCheck&) = delete;
	};
}
