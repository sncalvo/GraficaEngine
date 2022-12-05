#include "PhysicsManager.h"

namespace Engine
{
	PhysicsManager::PhysicsManager(): _camera(nullptr)
	{
		_collisionConfiguration = new btDefaultCollisionConfiguration();
		_dispatcher = new btCollisionDispatcher(_collisionConfiguration);
		_broadphase = new btDbvtBroadphase();
		_constraintSolver = new btSequentialImpulseConstraintSolver();
		_dynamicsWorld = new btDiscreteDynamicsWorld(
			_dispatcher,
			_broadphase,
			_constraintSolver,
			_collisionConfiguration
		);
		_dynamicsWorld->setGravity(btVector3(0, -10, 0));
		_debugDrawer = new DebugDrawer();
		_dynamicsWorld->setDebugDrawer((btIDebugDraw*) _debugDrawer);
		_debugDrawer->setDebugMode(
			btIDebugDraw::DBG_DrawWireframe |
			btIDebugDraw::DBG_DrawContactPoints
		);
	}

	void PhysicsManager::renderDebug()
	{
		if (_dynamicsWorld)
		{
			_dynamicsWorld->debugDrawWorld();
			glm::mat4 viewMatrix = _camera->getViewMatrix();
			glm::mat4 projectionMatrix = _camera->getProjectionMatrix();
			_debugDrawer->setMatrices(viewMatrix, projectionMatrix);
			_debugDrawer->doDebugDraw();
		}
	}

	void PhysicsManager::stepSimulation(float deltaTime)
	{
		if (_dynamicsWorld)
		{
			_dynamicsWorld->stepSimulation(deltaTime);
		}
	}

	void PhysicsManager::setCamera(Camera* camera)
	{
		_camera = camera;
	}

	btRigidBody* PhysicsManager::createRigidBody(
		float mass,
		const glm::vec3& position,
		btCollisionShape* shape
	)
	{
		btTransform rigidBodyTransform;
		rigidBodyTransform.setIdentity();
		rigidBodyTransform.setOrigin(btVector3(
			position.x,
			position.y,
			position.z
		));
		btVector3 localInertia(0, 0, 0);
		auto isDynamic = (mass != 0.f);
		if (isDynamic)
		{
			shape->calculateLocalInertia(mass, localInertia);
		}
		auto* rigidBody = new btRigidBody(mass, 0, shape, localInertia);
		rigidBody->setWorldTransform(rigidBodyTransform);
		_collisionShapes.push_back(shape);
		_dynamicsWorld->addRigidBody(rigidBody);
		return rigidBody;
	}

	PhysicsManager::~PhysicsManager()
	{
		for (int i = 0; i < _collisionShapes.size(); i++)
		{
			delete _collisionShapes[i];
		}
		_collisionShapes.clear();

		delete _dynamicsWorld;
		delete _constraintSolver;
		delete _broadphase;
		delete _dispatcher;
		delete _collisionConfiguration;
	}
}
