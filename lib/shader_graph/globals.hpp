#pragma once
#include <array>

#include "global.hpp"
#include "../float3.hpp"
namespace globals {
	static constexpr Global THREAD_ID{ 0, Type::Float2 };
	static constexpr Global THREAD_COUNT{ 1, Type::Float2 };
	static constexpr Global RAY_ORIGIN{ 2, Type::Float3 };
	static constexpr Global RAY_DIRECTION{ 3, Type::Float3 };
	static constexpr Global BARYCENTRIC_COORDINATE{ 4, Type::Float2 };
	static constexpr Global HIT_DISTANCE{ 5, Type::Float };
	static constexpr Global INSTANCE_ID{ 6, Type::Int };
	static constexpr Global PRIMITIVE_ID{ 7, Type::Int };
	static constexpr Global WORLD_POSITION{ 8 };
	static constexpr size_t GLOBALS_COUNT = 9;

	using Globals = std::array<Float3, GLOBALS_COUNT>;
}   // namespace globals