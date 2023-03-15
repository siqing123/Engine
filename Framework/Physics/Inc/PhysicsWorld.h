#pragma once
#include "Constraints.h"
#include "Particle.h"

namespace Storm::Physics
{
	struct Tetrahedron
	{
		Particle* a, * b, * c, * d;

		Math::Vector3 GetLook() const;
		Math::Vector3 GetRight() const;
		Math::Vector3 GetUp() const;
		Math::Vector3 GetPosition() const;

		void SetPosition(const Math::Vector3& pos);
		void SetVelocity(const Math::Vector3& velocity);

		Math::Matrix4 GetWorldMatrix() const;

	};
	using ParticleID = size_t;

	class PhysicsWorld
	{
	public:
		struct Settings
		{
			Math::Vector3 gravity{ 0.0f,-9.8f,0.0f };
			float timeStep = 1.0f / 60.0f;
			float drag = 0.0f;
			int iterations = 1;
		};
		void Initialize(Settings settings = {});

		void Update(float deltaTime);
		void DebugDraw() const;
		void DebugUI();

		template < class... Args>
		Particle* AddParticle(Args&&... args)
		{
			return mParticles.emplace_back(std::make_unique<Particle>(std::forward<Args>(args)...)).get();
		}
		/*ParticleID AddParticle(Args&&...args)
		{
			size_t index = mParticles.size();
			ParticleID id = (mGeneration << 50) | index;
			mParticles.emplace_back(std::make_unique<Particle>(std::forward<Args>(args)...)).get();
		}
		Particle* GetPatricle(ParticleID id)
		{
			if (id >> 50 == mGeneration)
			{
				return mParticles[id &(0x1<<51)-1]
			}
			return nullptr;
		}*/

		Tetrahedron AddTetrahedron();

		void AddPlane(const Math::Plane& plane);
		void AddOBB(const Math::OBB& obb);
		void Clear(bool dynamicOnly = false);


		template <class ConstraintType, class... Args> //Variadic Template Argument
		ConstraintType* AddConstraint(Args&&... args)//Forwarding Reference
		{
			auto& newConstraint = mConstraints.emplace_back(std::make_unique<ConstraintType>(std::forward<Args>(args)...));//Perfect Forwarding
			return static_cast<ConstraintType*>(newConstraint.get());
		}

		//void Clear();

	private:
		void AccumulateForces();
		void Integrate();
		void SatisfyConstraints();

		std::vector<std::unique_ptr<Constraint>> mConstraints;
		std::vector<std::unique_ptr<Particle>> mParticles;
		std::vector<std::unique_ptr<Tetrahedron>> mTetrahedron;
		std::vector<Math::Plane> mPlanes;
		std::vector<Math::OBB> mOBBs;

		Settings mSettings;
		float mTimer = 0.0f;
		bool mPuse = false;
	};

}