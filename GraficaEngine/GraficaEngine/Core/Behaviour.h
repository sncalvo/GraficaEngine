#pragma once

#include "../Exceptions/NotSupposedToCopyException.h"

namespace Engine
{
	class BaseGameObject;
	
	class Behaviour
	{
	protected:
		BaseGameObject* gameObject;

	public:
		virtual void update() {};
		virtual Behaviour* clone() const = 0;
		virtual ~Behaviour() {};
		void setGameObject(BaseGameObject* parentGameObject)
		{
			gameObject = parentGameObject;
		}
		virtual void start() {};
	};
}
