#pragma once

#include "Epch.h"

namespace Engine {
	class Component
	{
	public:
		virtual ~Component() = default;

		virtual void OnAwake() = 0;
		virtual void OnUpdate(double dt) = 0;
		virtual void OnRender() = 0;
	};
}