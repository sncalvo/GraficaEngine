#pragma once

namespace Engine
{
	class BaseGameObject;
	
	class Behaviour
	{
	protected:
		BaseGameObject* gameObject;
	public:
		virtual void update() = 0;
		void setGameObject(BaseGameObject* parentGameObject)
		{
			gameObject = parentGameObject;
		}
	};
}
