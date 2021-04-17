#pragma once

#include "../Renderer/Model.h"
#include "../Renderer/Material.h"
#include "Transform.h"

namespace Engine
{
	class GameObject
	{
	private:
		Model _model;
		MaterialObject _material;
		// GameObject[] _children;
		// Behaviour[] _behaviours;
	public:
		GameObject(Model model, MaterialObject material);
		Transform transform;
		void draw() const;
	};
}