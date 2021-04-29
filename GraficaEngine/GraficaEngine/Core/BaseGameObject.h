#pragma once

#include <string>
#include <vector>

#include "Behaviour.h"
#include "Transform.h"

namespace Engine
{
	class Scene;

	class BaseGameObject
	{
	private:
		std::vector<std::string> _tags;
		std::vector<Behaviour*> _behaviours;
	protected:
		Scene* _scene;
	public:
		BaseGameObject();
		~BaseGameObject();
		Transform transform;
		void update();
		void addBehaviour(Behaviour*);
		void setScene(Scene*);
		Scene* getScene() const;
		virtual bool isDrawable() const;
		void addTag(std::string tag);
		bool hasTag(std::string tag) const;
	};
}
