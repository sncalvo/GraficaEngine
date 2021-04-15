#pragma once

#include "Model.h"
#include "Material.h"
#include "Transform.h"

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