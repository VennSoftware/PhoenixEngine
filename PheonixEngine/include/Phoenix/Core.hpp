#pragma once

#include <vector>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <set>
#include <fstream>
#include <cstdio>

#define PHNX_LOG(fmt, ...) do { printf("\x1b[32m[Phoenix::LOG] - "##fmt##"\x1b[39m\n", ##__VA_ARGS__); } while(0)
#define PHNX_DEBUG(fmt, ...) do { printf("\x1b[36m[Phoenix::DEBUG] - "##fmt##"\x1b[39m\n", ##__VA_ARGS__); } while(0)
#define PHNX_WARN(fmt, ...) do { printf("\x1b[33m[Phoenix::WARN] - "##fmt##"\x1b[39m\n", ##__VA_ARGS__); } while(0)
#define PHNX_FATAL(fmt, ...) do { printf("\x1b[37;41m[Phoenix::FATAL] - "##fmt##"\x1b[0m\n", ##__VA_ARGS__); exit(-1); } while(0)


#if defined(PHNX_USE_OPENGL)
#include <Phoenix/gl/OpenGL.hpp>
#endif


namespace phoenix {
}


#include "RendererAPI.hpp"