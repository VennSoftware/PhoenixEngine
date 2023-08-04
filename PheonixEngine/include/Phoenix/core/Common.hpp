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
#include <string>

#include <Phoenix/Types.hpp>

#define PHNX_LOG(fmt, ...) do { printf("\x1b[32m[Phoenix::LOG] - "##fmt##"\x1b[39m\n", ##__VA_ARGS__); } while(0)
#define PHNX_DEBUG(fmt, ...) do { printf("\x1b[36m[Phoenix::DEBUG] - "##fmt##"\x1b[39m\n", ##__VA_ARGS__); } while(0)
#define PHNX_WARN(fmt, ...) do { printf("\x1b[33m[Phoenix::WARN] - "##fmt##"\x1b[39m\n", ##__VA_ARGS__); } while(0)
#define PHNX_ERROR(fmt, ...) do { printf("\x1b[31m[Phoenix::ERROR] - "##fmt##"\x1b[39m\n", ##__VA_ARGS__); } while(0)
#define PHNX_FATAL(fmt, ...) do { printf("\x1b[37;41m[Phoenix::FATAL] - "##fmt##"\x1b[0m\n", ##__VA_ARGS__); exit(-1); } while(0)


#ifdef _DEBUG
#define GL_CALL(_CALL)      do { _CALL; GLenum gl_err = glGetError(); if (gl_err != 0) fprintf(stderr, "GL error 0x%x returned from '%s'. (File %s, Line %d)\n", gl_err, #_CALL, __FILE__, __LINE__); } while (0)  // Call with error check
#else
#define GL_CALL(_CALL) _CALL
#endif