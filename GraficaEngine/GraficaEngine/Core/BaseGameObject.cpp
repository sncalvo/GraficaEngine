#include "BaseGameObject.h"

#include <algorithm>

namespace Engine
{
	BaseGameObject::BaseGameObject() : _scene(nullptr), _parent(nullptr)
	{}

	BaseGameObject::~BaseGameObject()
	{
		for (BaseGameObject* child : _children)
		{
			delete child;
		}

		for (Behaviour* behaviour : _behaviours)
		{
			delete behaviour;
		}
	}

	void BaseGameObject::addBehaviour(Behaviour* behaviour)
	{
		behaviour->setGameObject(this);
		_behaviours.push_back(behaviour);
	}

	void BaseGameObject::addChild(BaseGameObject* child)
	{
		child->setScene(_scene);
		child->setParent(this);
		_children.push_back(child);
	}

	BaseGameObject* BaseGameObject::getParent() const
	{
		return _parent;
	}

	void BaseGameObject::setParent(BaseGameObject* parent)
	{
		_parent = parent;
	}

	bool BaseGameObject::hasParent() const
	{
		return _parent != nullptr;
	}

	void BaseGameObject::deleteChild(BaseGameObject* child)
	{
		_children.erase(
			std::remove(
				_children.begin(),
				_children.end(),
				child
			),
			_children.end()
		);

		delete child;
	}

	void BaseGameObject::update()
	{
		for (Behaviour* behaviour : _behaviours)
		{
			behaviour->update();
		}

		for (BaseGameObject* child : _children)
		{
			child->update();
		}
	}

	void BaseGameObject::setScene(Scene* scene)
	{
		_scene = scene;
	}

	Scene* BaseGameObject::getScene() const
	{
		return _scene;
	}

	void BaseGameObject::draw() const
	{
		for (BaseGameObject* child : _children)
		{
			child->draw();
		}
	}

	void BaseGameObject::addTag(std::string tag)
	{
		_tags.push_back(tag);
	}

	bool BaseGameObject::hasTag(std::string tagToFind) const
	{
		for (std::string tag : _tags)
		{
			if (tag == tagToFind)
			{
				return true;
			}
		}
		return false;
	}
}
