#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <Bullet/btBulletDynamicsCommon.h>

#include "../Core/Camera.h"
#include "DebugDrawer.h"

namespace Engine {
	class PhysicsManager {
	private:
		btAlignedObjectArray<btCollisionShape*> _collisionShapes;
		btBroadphaseInterface* _broadphase;
		btCollisionDispatcher* _dispatcher;
		btConstraintSolver* _constraintSolver;
		btDefaultCollisionConfiguration* _collisionConfiguration;
		DebugDrawer* _debugDrawer;
		btDiscreteDynamicsWorld* _dynamicsWorld;
		Camera* _camera;
		PhysicsManager();
		~PhysicsManager();
	public:
		static PhysicsManager& getInstance()
		{
			static PhysicsManager _instance;
			return _instance;
		}
		void stepSimulation(float deltaTime);
		void renderDebug();
		void setCamera(Camera*);
		btRigidBody* createRigidBody(
			float mass,
			const glm::vec3& transform,
			btCollisionShape* shape,
			const btVector4& color = btVector4(1, 0, 0, 1)
		);
	};
}
