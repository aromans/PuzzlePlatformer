#pragma once

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
#define BIND_EVENT_FN_TO(x,y) std::bind(&x, y, std::placeholders::_1)