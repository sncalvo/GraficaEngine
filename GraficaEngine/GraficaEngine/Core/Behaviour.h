#pragma once

namespace Engine
{
	class GameObject;
	
	class Behaviour
	{
	public:
		virtual void update(GameObject*) = 0;
	};
}