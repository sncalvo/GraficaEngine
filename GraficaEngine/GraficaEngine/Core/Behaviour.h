#pragma once

namespace Engine
{
	class GameObject;
	
	class Behaviour
	{
	protected:
		GameObject* gameObject;
	public:
		virtual void update() = 0;
		void setGameObject(GameObject* parentGameObject)
		{
			gameObject = parentGameObject;
		}
	};
}
