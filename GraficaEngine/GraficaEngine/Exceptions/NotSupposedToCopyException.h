#pragma once

#include <exception>

namespace Engine
{
	class NotSupposedToCopyException : public std::exception
	{
		const char* what() const throw ()
		{
			return "This Behaviour is not supposed to be copied";
		}
	};
}