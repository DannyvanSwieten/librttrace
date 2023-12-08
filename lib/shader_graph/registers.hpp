#pragma once

#include <array>
#include "../float3.hpp"

constexpr size_t REGISTER_COUNT = 32;
using Registers = std::array<Float4, REGISTER_COUNT>;