#pragma once

#include "Engine/Core/Input.h"

namespace Engine {
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;

		virtual bool IsKeyReleasedImpl(int keycode) override;

		virtual bool IsMouseButtonPressedImpl(int button) override;

		virtual std::pair<float, float> GetMousePositionImpl() override;
	};
}
