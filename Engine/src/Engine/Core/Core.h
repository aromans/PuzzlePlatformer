#pragma once

#ifndef ENG_PLATFORM_WINDOWS
	#error Engine only currently supports Windows!
#endif

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
#define BIND_EVENT_FN_TO(x,y) std::bind(&x, y, std::placeholders::_1)