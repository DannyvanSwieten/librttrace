#include <device_factory.hpp>
#include <device.hpp>
#include <array>

#include "bottom_level_acceleration_structure.hpp"
#include "resource_ctx.hpp"
#include "image.hpp"
#include "compiler/instructions.hpp"
#include "cpu/vm.hpp"

#include "shader_compiler.hpp"

#include "shader_graph/shader_graph.hpp"
#include "shader_graph/node/binary_operator.hpp"
#include "shader_graph/node/constant.hpp"
#include "shader_graph/node/outputs.hpp"
#include "shader_graph/node/inputs.hpp"
#include "shader_graph/node/normalize.hpp"

#include "pipeline.hpp"

constexpr uint32_t width = 512;
constexpr uint32_t height = 512;

std::vector<float> create_cube_vertices()
{
	std::vector<float> vertices;

	vertices.push_back(-0.5f);
	vertices.push_back(-0.5f);
	vertices.push_back(-0.5f);

	vertices.push_back(0.5f);
	vertices.push_back(-0.5f);
	vertices.push_back(-0.5f);

	vertices.push_back(0.5f);
	vertices.push_back(0.5f);
	vertices.push_back(-0.5f);

	vertices.push_back(-0.5f);
	vertices.push_back(0.5f);
	vertices.push_back(-0.5f);

	vertices.push_back(-0.5f);
	vertices.push_back(-0.5f);
	vertices.push_back(0.5f);

	vertices.push_back(0.5f);
	vertices.push_back(-0.5f);
	vertices.push_back(0.5f);

	vertices.push_back(0.5f);
	vertices.push_back(0.5f);
	vertices.push_back(0.5f);

	vertices.push_back(-0.5f);
	vertices.push_back(0.5f);
	vertices.push_back(0.5f);

	return vertices;
}

std::vector<uint32_t> create_cube_indices()
{
	std::vector<uint32_t> indices;

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);

	indices.push_back(1);
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(6);
	indices.push_back(2);
	indices.push_back(1);

	indices.push_back(7);
	indices.push_back(6);
	indices.push_back(5);
	indices.push_back(5);
	indices.push_back(4);
	indices.push_back(7);

	indices.push_back(4);
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(3);
	indices.push_back(7);
	indices.push_back(4);

	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(1);
	indices.push_back(1);
	indices.push_back(0);
	indices.push_back(4);

	indices.push_back(3);
	indices.push_back(2);
	indices.push_back(6);
	indices.push_back(6);
	indices.push_back(7);
	indices.push_back(3);

	return indices;
}

int main()
{
	Api api = Api::CPU;
	DeviceFactory factory;
	auto device = factory.create_device(api).expect("Failed to create device");
	printf("Device created: %s\n", device->vendor_id().value());

	std::vector<float> vertices = create_cube_vertices();
	std::vector<uint32_t> indices = create_cube_indices();

	auto frame_buffer = device->alloc_frame_buffer(PixelFormat::RGBA32SFLOAT, width, height).expect("Failed to create frame buffer");

	const auto vertex_buffer =
		device->alloc_vertex_buffer(reinterpret_cast<const float*>(vertices.data()), vertices.size()).expect("Failed to create vertex buffer");

	const auto index_buffer = device->alloc_index_buffer(indices.data(), indices.size()).expect("Failed to create index buffer");

	const auto acceleration_structure =
		device->alloc_bottom_level_acceleration_structure(vertex_buffer, 3, index_buffer).expect("Failed to create acceleration structure");

	auto command_buffer = device->alloc_command_buffer().expect("Failed to create command buffer");
	command_buffer->build_bottom_level_acceleration_structure(acceleration_structure, vertex_buffer, 3, index_buffer);

	const auto instance = acceleration_structure->create_instance(0, InstanceMask::Opaque, nullptr);

	auto tlas = device->alloc_top_level_acceleration_structure(&instance, 1).expect("Failed to create top level acceleration structure");

	command_buffer->build_top_level_acceleration_structure(tlas, &instance, 1);

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
	graph.set_default_value(origin, 0, Float3(0, 0, -10));

	size_t direction_vector = graph.add_node(std::make_unique<shadergraph::BinaryOperatorNode<instructions::Sub>>());
	graph.connect(shadergraph::ShaderGraph::Connection{ sub_node, 0, direction_vector, 0 });
	graph.connect(shadergraph::ShaderGraph::Connection{ origin, 0, direction_vector, 1 });

	size_t direction = graph.add_node(std::make_unique<shadergraph::NormalizeNode>());
	graph.connect(shadergraph::ShaderGraph::Connection{ direction_vector, 0, direction, 0 });

	size_t origin_output = graph.add_node(std::make_unique<shadergraph::RayOriginOutput>());
	graph.connect(shadergraph::ShaderGraph::Connection{ origin, 0, origin_output, 0 });

	size_t direction_output = graph.add_node(std::make_unique<shadergraph::RayDirectionOutput>());
	graph.connect(shadergraph::ShaderGraph::Connection{ direction, 0, direction_output, 0 });

	const auto ir = graph.generate_ir();
	ShaderCompiler compiler(api);
	auto ray_gen = compiler.compile_ray_generation_program(ir).expect("Failed to compile ray generation program");

	Pipeline pipeline(ray_gen, nullptr, nullptr, 0);

	ResourceContext resource_ctx;
	resource_ctx.add_frame_buffer(0, frame_buffer);
	resource_ctx.add_acceleration_structure(0, tlas);

	command_buffer->trace_rays(&pipeline, &resource_ctx, width, height);

	const auto frame_buffer_data = frame_buffer->data().expect("Failed to get frame buffer data");

	Image image(static_cast<const float*>(frame_buffer_data), width, height, 4);
	image.write("output.hdr");

	return 0;
}