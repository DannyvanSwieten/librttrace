#include "shaders.hpp"
#include "../lib/shader_graph/node/binary_operator.hpp"
#include "../lib/shader_graph/node/constant.hpp"
#include "../lib/shader_graph/node/inputs.hpp"
#include "../lib/shader_graph/node/normalize.hpp"
#include "../lib/shader_graph/node/outputs.hpp"
#include "../lib/shader_graph/node/intersect.hpp"
#include "../lib/shader_graph/node/store_pixel.hpp"
#include "../lib/shader_graph/node/fetch_primitive.hpp"
#include "../lib/shader_graph/node/mix.hpp"
#include "../lib/shader_graph/node/smooth_step.hpp"

using namespace shadergraph;

shadergraph::ShaderGraph create_ray_gen_program()
{
	shadergraph::ShaderGraph graph;
	// Current Pixel
	size_t thread_id = graph.add_node(std::make_unique<shadergraph::ThreadIdInput>());
	// Resolution
	size_t thread_count = graph.add_node(std::make_unique<shadergraph::ThreadCountInput>());
	// Divide them to get the normalized pixel position
	size_t div = graph.add_node(std::make_unique<shadergraph::BinaryOperatorNode<Div>>());

	graph.connect(shadergraph::Connection{ thread_id, 0, div, 0 });
	graph.connect(shadergraph::Connection{ thread_count, 0, div, 1 });

	// scale it by 2 to get to the range [0 - 2]
	size_t scale = graph.add_node(std::make_unique<shadergraph::ConstantNode>());
	graph.set_default_value(scale, 0, Operand{ Float3(2.0f, 2.0f, 1.0f) });
	size_t mul = graph.add_node(std::make_unique<shadergraph::BinaryOperatorNode<Mul>>());

	graph.connect(shadergraph::Connection{ div, 0, mul, 0 });
	graph.connect(shadergraph::Connection{ scale, 0, mul, 1 });

	// subtract 1 to get to the range [-1 - 1]
	size_t offset = graph.add_node(std::make_unique<shadergraph::ConstantNode>());
	graph.set_default_value(offset, 0, Operand{ Float3(1.f, 1.f, 0.f) });

	size_t sub_node = graph.add_node(std::make_unique<shadergraph::BinaryOperatorNode<Sub>>());
	graph.connect(shadergraph::Connection{ mul, 0, sub_node, 0 });
	graph.connect(shadergraph::Connection{ offset, 0, sub_node, 1 });

	// Camera position
	size_t origin = graph.add_node(std::make_unique<shadergraph::ConstantNode>());
	graph.set_default_value(origin, 0, Float3(0, 0, -10));

	// Direction vector target - origin
	size_t direction_vector = graph.add_node(std::make_unique<shadergraph::BinaryOperatorNode<Sub>>());
	graph.connect(shadergraph::Connection{ sub_node, 0, direction_vector, 0 });
	graph.connect(shadergraph::Connection{ origin, 0, direction_vector, 1 });

	// normalize the direction vector
	size_t direction = graph.add_node(std::make_unique<shadergraph::NormalizeNode>());
	graph.connect(shadergraph::Connection{ direction_vector, 0, direction, 0 });

	// Intersect with the scene ( This will trigger either a hit or a miss shader, See below)
	size_t intersect = graph.add_node(std::make_unique<shadergraph::IntersectNode>());
	graph.connect(shadergraph::Connection{ origin, 0, intersect, 0 });
	graph.connect(shadergraph::Connection{ direction, 0, intersect, 1 });
	size_t index = graph.add_node(std::make_unique<shadergraph::ConstantNode>());
	graph.set_default_value(index, 0, Float3(0, 0, 0));
	graph.connect(shadergraph::Connection{ index, 0, intersect, 2 });

	return graph;
}

shadergraph::ShaderGraph create_closest_hit_program()
{
	shadergraph::ShaderGraph graph;

	// Buffer index that contains our position data
	size_t buffer_index = graph.add_node(std::make_unique<shadergraph::ConstantNode>());
	graph.set_default_value(buffer_index, 0, Float3(0, 0, 0));
	size_t fetch_primitive = graph.add_node(std::make_unique<shadergraph::FetchPrimitiveNode>());
	graph.connect(shadergraph::Connection{ buffer_index, 0, fetch_primitive, 0 });

	// Calculate the normal, first subtract v1 from v0
	size_t edge0 = graph.add_node(std::make_unique<shadergraph::BinaryOperatorNode<Sub>>());
	graph.connect(shadergraph::Connection{ fetch_primitive, 0, edge0, 0 });
	graph.connect(shadergraph::Connection{ fetch_primitive, 1, edge0, 1 });

	// Ten subtract v2 from v0
	size_t edge1 = graph.add_node(std::make_unique<shadergraph::BinaryOperatorNode<Sub>>());
	graph.connect(shadergraph::Connection{ fetch_primitive, 0, edge1, 0 });
	graph.connect(shadergraph::Connection{ fetch_primitive, 2, edge1, 1 });

	// Cross product of the two edges
	size_t normal = graph.add_node(std::make_unique<shadergraph::BinaryOperatorNode<Cross>>());
	graph.connect(shadergraph::Connection{ edge0, 0, normal, 0 });
	graph.connect(shadergraph::Connection{ edge1, 0, normal, 1 });

	// Normalize the normal
	size_t normalize = graph.add_node(std::make_unique<shadergraph::NormalizeNode>());
	graph.connect(shadergraph::Connection{ normal, 0, normalize, 0 });

	// Create a directional light
	size_t light_direction = graph.add_node(std::make_unique<shadergraph::ConstantNode>());
	graph.set_default_value(light_direction, 0, Float3(0, 1, 1));

	// Calculate the dot product between the normal and the light direction
	size_t dot = graph.add_node(std::make_unique<shadergraph::BinaryOperatorNode<Dot>>());
	graph.connect(shadergraph::Connection{ normalize, 0, dot, 0 });
	graph.connect(shadergraph::Connection{ light_direction, 0, dot, 1 });

	// Fetch the barycentric coordinates
	size_t barycentric_coordinates = graph.add_node(std::make_unique<shadergraph::BarycentricCoordinateInput>());

	// Multiply with the light intensity
	size_t mul = graph.add_node(std::make_unique<shadergraph::BinaryOperatorNode<Mul>>());
	graph.connect(shadergraph::Connection{ dot, 0, mul, 0 });
	graph.connect(shadergraph::Connection{ barycentric_coordinates, 0, mul, 1 });

	// Store the pixel
	size_t store_pixel = graph.add_node(std::make_unique<shadergraph::StorePixelNode>());
	// At image location 0
	size_t image_index = graph.add_node(std::make_unique<shadergraph::ConstantNode>());
	graph.set_default_value(image_index, 0, Float3(0, 0, 0));
	graph.connect(shadergraph::Connection{ image_index, 0, store_pixel, 0 });
	size_t thread_id = graph.add_node(std::make_unique<shadergraph::ThreadIdInput>());
	graph.connect(shadergraph::Connection{ thread_id, 0, store_pixel, 1 });
	graph.connect(shadergraph::Connection{ barycentric_coordinates, 0, store_pixel, 2 });

	return graph;
}

shadergraph::ShaderGraph create_miss_program()
{
	shadergraph::ShaderGraph graph;

	// Fetch the ray direction
	size_t ray_dir = graph.add_node(std::make_unique<shadergraph::RayDirectionInput>());
	// add an offset and scale it a bit
	size_t add = graph.add_node(std::make_unique<shadergraph::BinaryOperatorNode<Add>>());
	graph.connect(shadergraph::Connection{ ray_dir, 0, add, 0 });
	size_t constant = graph.add_node(std::make_unique<shadergraph::ConstantNode>());
	graph.set_default_value(constant, 0, Float3(0, 1, 0));
	graph.connect(shadergraph::Connection{ constant, 0, add, 1 });
	size_t scale = graph.add_node(std::make_unique<shadergraph::ConstantNode>());
	graph.set_default_value(scale, 0, Float3(0.5, 0.5, 0.5));
	size_t mul = graph.add_node(std::make_unique<shadergraph::BinaryOperatorNode<Mul>>());
	graph.connect(shadergraph::Connection{ add, 0, mul, 0 });
	graph.connect(shadergraph::Connection{ scale, 0, mul, 1 });

	// Interpolate between white and sky color based on the y coordinate of the ray direction
	size_t mix = graph.add_node(std::make_unique<shadergraph::MixNode>());
	graph.connect(shadergraph::Connection{ scale, 0, mix, 2 });

	// Store the pixel
	size_t store_pixel = graph.add_node(std::make_unique<shadergraph::StorePixelNode>());
	size_t image_index = graph.add_node(std::make_unique<shadergraph::ConstantNode>());
	graph.set_default_value(image_index, 0, Float3(0, 0, 0));
	graph.connect(shadergraph::Connection{ image_index, 0, store_pixel, 0 });
	size_t thread_id = graph.add_node(std::make_unique<shadergraph::ThreadIdInput>());
	graph.connect(shadergraph::Connection{ thread_id, 0, store_pixel, 1 });
	graph.connect(shadergraph::Connection{ mix, 0, store_pixel, 2 });
	return graph;
}