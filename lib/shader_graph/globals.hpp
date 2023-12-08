#pragma once
#include <array>

#include "global.hpp"
#include "../float3.hpp"
namespace globals {
	static constexpr Global THREAD_ID{ 0 };
	static constexpr Global THREAD_COUNT{ 1 };
	static constexpr Global RAY_ORIGIN{ 2 };
	static constexpr Global RAY_DIRECTION{ 3 };
	static constexpr Global BARYCENTRIC_COORDINATE{ 4 };
	static constexpr Global HIT_DISTANCE{ 5 };
	static constexpr Global INSTANCE_ID{ 6 };
	static constexpr Global PRIMITIVE_ID{ 7 };
	static constexpr Global WORLD_POSITION{ 8 };
	static constexpr size_t GLOBALS_COUNT = 9;

	using Globals = std::array<Float3, GLOBALS_COUNT>;

}   // namespace globals