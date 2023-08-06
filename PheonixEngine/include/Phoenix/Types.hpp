#pragma once

#include <cstdint>
#include <memory>
#include <map>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef int8_t I8;
typedef int16_t I16;
typedef int32_t I32;
typedef int64_t I64;

typedef float F32;
typedef double F64;

template<typename T>
class Ref {
public:
	T* operator->() {
		return m_item.get();
	}

	T& operator *() {
		return *m_item;
	}

	Ref(T* item) { m_item.reset(item); }
	Ref(std::shared_ptr<T> item) { m_item = item; }

	Ref<T>& operator=(T* other) {
		m_item.reset(other);
		return *this;
	}

private:
	std::shared_ptr<T> m_item;
};


template <class T, class... A>
Ref<T> make_ref(A&&... args) {
	return Ref<T>(T(std::forward<A>(args)));
}