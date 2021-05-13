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
		virtual void update() = 0;
		virtual Behaviour* clone() const {
			throw new NotSupposedToCopyException();
		};
		virtual ~Behaviour() {};
		void setGameObject(BaseGameObject* parentGameObject)
		{
			gameObject = parentGameObject;
		}
	};
}
