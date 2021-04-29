#pragma once

#include <vector>

#include "../Renderer/Model.h"
#include "../Renderer/Material.h"

#include "BaseGameObject.h"
#include "Scene.h"

namespace Engine
{
	class GameObject : public BaseGameObject
	{
	private:
		Model* _model;
		MaterialObject _material;
		// GameObject[] _children;
	public:
		GameObject(Model* model, MaterialObject material);
		~GameObject();
		void draw() const;
		virtual bool isDrawable() const;
	};
}
