#pragma once

#include <string>
#include <vector>

#include "Behaviour.h"
#include "Transform.h"

#include "../Renderer/MeshRenderer.h"
#include "../Renderer/ShadowRenderer.h"
#include "../Renderer/TextRenderer.h"

#include "../Utils/UUID.h"

namespace Engine
{
	class Scene;

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
		std::string uuid = getUUID();

		BaseGameObject();
		BaseGameObject(const BaseGameObject *);
		virtual BaseGameObject *clone() const;
		virtual ~BaseGameObject();
		Transform transform;
		void update();
		void start();
		void addBehaviour(Behaviour *);
		std::vector<Behaviour *> getBehaviours() const;
		void addChild(BaseGameObject *);
		BaseGameObject *getParent() const;
		bool hasParent() const;
		void setParent(BaseGameObject *);
		void deleteChild(BaseGameObject *);
		void setScene(Scene *);
		Scene *getScene() const;
		virtual void draw(Shader *) const;
		virtual std::tuple<std::vector<std::shared_ptr<MeshRenderer>>, std::vector<std::shared_ptr<ShadowRenderer>>, std::vector<std::shared_ptr<TextRenderer>>> getRenderers();
		void addTag(std::string tag);
		bool hasTag(std::string tag) const;
		BaseGameObject *getChildWithTag(std::string tag) const;
		virtual void syncTransformWithRigidBody();

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

		template<class BehaviourType> std::vector<BehaviourType*> getComponents() const
		{
			std::vector<BehaviourType*> behaviours;
			for (Behaviour* behaviour : _behaviours)
			{
				BehaviourType* component = dynamic_cast<BehaviourType*>(behaviour);
				if (component != nullptr)
				{
					behaviours.push_back(component);
				}
			}

			return behaviours;
		}
	};
}
