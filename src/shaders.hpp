#pragma once

#include "../lib/shader_graph/shader_graph.hpp"

shadergraph::ShaderGraph create_ray_gen_program();
shadergraph::ShaderGraph create_closest_hit_program();
shadergraph::ShaderGraph create_miss_program();