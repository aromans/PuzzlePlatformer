#pragma once

#ifdef ENG_PLATFORM_WINDOWS
#if ENG_DYNAMIC_LINK
	#ifdef ENG_BUILD_DLL
		#define ENG_API __declspec(dllexport)
	#else
		#define ENG_API __declspec(dllimport)
	#endif
#else
	#define ENG_API
#endif
#else 
	#error Engine Only Supports Windows!
#endif

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
#define BIND_EVENT_FN_TO(x,y) std::bind(&x, y, std::placeholders::_1)