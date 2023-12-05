#include "ray_generation.hpp"
#include "../lib/shader_graph/node/binary_operator.hpp"
#include "../lib/shader_graph/node/constant.hpp"
#include "../lib/shader_graph/node/inputs.hpp"
#include "../lib/shader_graph/node/normalize.hpp"
#include "../lib/shader_graph/node/outputs.hpp"
#include "../lib/shader_graph/node/intersect.hpp"
#include "../lib/shader_graph/node/store_pixel.hpp"
#include "../lib/shader_graph/node/fetch_primitive.hpp"

shadergraph::ShaderGraph create_ray_gen_program()
{
	shadergraph::ShaderGraph graph;
	size_t thread_id = graph.add_node(std::make_unique<shadergraph::ThreadIdInput>());
	size_t thread_count = graph.add_node(std::make_unique<shadergraph::ThreadCountInput>());
	size_t div = graph.add_node(std::make_unique<shadergraph::BinaryOperatorNode<instructions::Div>>());

	graph.connect(shadergraph::ShaderGraph::Connection{ thread_id, 0, div, 0 });
	graph.connect(shadergraph::ShaderGraph::Connection{ thread_count, 0, div, 1 });

	size_t scale = graph.add_node(std::make_unique<shadergraph::ConstantNode>());
	graph.set_default_value(scale, 0, Value{ Float3(2.0f, 2.0f, 1.0f) });
	size_t mul = graph.add_node(std::make_unique<shadergraph::BinaryOperatorNode<instructions::Mul>>());

	graph.connect(shadergraph::ShaderGraph::Connection{ div, 0, mul, 0 });
	graph.connect(shadergraph::ShaderGraph::Connection{ scale, 0, mul, 1 });

	size_t offset = graph.add_node(std::make_unique<shadergraph::ConstantNode>());
	graph.set_default_value(offset, 0, Value{ Float3(1.f, 1.f, 0.f) });

	size_t sub_node = graph.add_node(std::make_unique<shadergraph::BinaryOperatorNode<instructions::Sub>>());
	graph.connect(shadergraph::ShaderGraph::Connection{ mul, 0, sub_node, 0 });
	graph.connect(shadergraph::ShaderGraph::Connection{ offset, 0, sub_node, 1 });

	size_t origin = graph.add_node(std::make_unique<shadergraph::ConstantNode>());
	graph.set_default_value(origin, 0, Float3(0, 2, -10));

	size_t direction_vector = graph.add_node(std::make_unique<shadergraph::BinaryOperatorNode<instructions::Sub>>());
	graph.connect(shadergraph::ShaderGraph::Connection{ sub_node, 0, direction_vector, 0 });
	graph.connect(shadergraph::ShaderGraph::Connection{ origin, 0, direction_vector, 1 });

	size_t direction = graph.add_node(std::make_unique<shadergraph::NormalizeNode>());
	graph.connect(shadergraph::ShaderGraph::Connection{ direction_vector, 0, direction, 0 });

	size_t intersect = graph.add_node(std::make_unique<shadergraph::Intersect>());
	graph.connect(shadergraph::ShaderGraph::Connection{ origin, 0, intersect, 0 });
	graph.connect(shadergraph::ShaderGraph::Connection{ direction, 0, intersect, 1 });
	size_t index = graph.add_node(std::make_unique<shadergraph::ConstantNode>());
	graph.set_default_value(index, 0, Float3(0, 0, 0));
	graph.connect(shadergraph::ShaderGraph::Connection{ index, 0, intersect, 2 });

	return graph;
}

shadergraph::ShaderGraph create_closest_hit_program(size_t next_free_register)
{
	shadergraph::ShaderGraph graph(next_free_register);

	size_t buffer_index = graph.add_node(std::make_unique<shadergraph::ConstantNode>());
	graph.set_default_value(buffer_index, 0, Float3(0, 0, 0));
	size_t fetch_primitive = graph.add_node(std::make_unique<shadergraph::FetchPrimitive>());
	graph.connect(shadergraph::ShaderGraph::Connection{ buffer_index, 0, fetch_primitive, 0 });

	size_t edge0 = graph.add_node(std::make_unique<shadergraph::BinaryOperatorNode<instructions::Sub>>());
	graph.connect(shadergraph::ShaderGraph::Connection{ fetch_primitive, 0, edge0, 0 });
	graph.connect(shadergraph::ShaderGraph::Connection{ fetch_primitive, 1, edge0, 1 });

	size_t edge1 = graph.add_node(std::make_unique<shadergraph::BinaryOperatorNode<instructions::Sub>>());
	graph.connect(shadergraph::ShaderGraph::Connection{ fetch_primitive, 0, edge1, 0 });
	graph.connect(shadergraph::ShaderGraph::Connection{ fetch_primitive, 2, edge1, 1 });

	size_t normal = graph.add_node(std::make_unique<shadergraph::BinaryOperatorNode<instructions::Cross>>());
	graph.connect(shadergraph::ShaderGraph::Connection{ edge0, 0, normal, 0 });
	graph.connect(shadergraph::ShaderGraph::Connection{ edge1, 0, normal, 1 });

	size_t normalize = graph.add_node(std::make_unique<shadergraph::NormalizeNode>());
	graph.connect(shadergraph::ShaderGraph::Connection{ normal, 0, normalize, 0 });

	size_t light_direction = graph.add_node(std::make_unique<shadergraph::ConstantNode>());
	graph.set_default_value(light_direction, 0, Float3(0, 1, 0));

	size_t dot = graph.add_node(std::make_unique<shadergraph::BinaryOperatorNode<instructions::Dot>>());
	graph.connect(shadergraph::ShaderGraph::Connection{ normalize, 0, dot, 0 });
	graph.connect(shadergraph::ShaderGraph::Connection{ light_direction, 0, dot, 1 });

	size_t barycentric_coordinates = graph.add_node(std::make_unique<shadergraph::BarycentricCoordinateInput>());

	size_t store_pixel = graph.add_node(std::make_unique<shadergraph::StorePixel>());
	size_t image_index = graph.add_node(std::make_unique<shadergraph::ConstantNode>());
	graph.set_default_value(image_index, 0, Float3(0, 0, 0));
	graph.connect(shadergraph::ShaderGraph::Connection{ image_index, 0, store_pixel, 0 });
	size_t thread_id = graph.add_node(std::make_unique<shadergraph::ThreadIdInput>());
	graph.connect(shadergraph::ShaderGraph::Connection{ thread_id, 0, store_pixel, 1 });
	graph.connect(shadergraph::ShaderGraph::Connection{ barycentric_coordinates, 0, store_pixel, 2 });

	return graph;
}
