#pragma once

#include <string>
#include <vector>

#include "Behaviour.h"
#include "Transform.h"

namespace Engine
{
	class Scene;
	class Collider;

	class BaseGameObject
	{
	private:
		std::vector<std::string> _tags;
		std::vector<Behaviour *> _behaviours;

	protected:
		Scene *_scene;
		BaseGameObject *_parent;
		std::vector<BaseGameObject *> _children; // TODO: Create local transform
	public:
		BaseGameObject();
		BaseGameObject(BaseGameObject *);
		virtual ~BaseGameObject();
		Transform transform;
		void update();
		void addBehaviour(Behaviour *);
		std::vector<Behaviour *> getBehaviours() const;
		void addChild(BaseGameObject *);
		BaseGameObject *getParent() const;
		bool hasParent() const;
		void setParent(BaseGameObject *);
		void deleteChild(BaseGameObject *);
		void setScene(Scene *);
		Scene *getScene() const;
		virtual Collider *getCollider() const;
		virtual void draw() const;
		void addTag(std::string tag);
		bool hasTag(std::string tag) const;

		template<class BehaviourType> BehaviourType* getComponent() const
		{
			for (Behaviour* behaviour : _behaviours)
			{
				BehaviourType* component = dynamic_cast<BehaviourType*>(behaviour);
				if (component != nullptr)
				{
					return component;
				}
			}

			return nullptr;
		}
	};
}
