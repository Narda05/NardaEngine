#pragma once


#include "Component.h"

namespace NardaEngine
{
	class UIComponent : public Component
	{
	public:
		virtual void Render() = 0;
	};
}
