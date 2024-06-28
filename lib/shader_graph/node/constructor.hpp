#pragma once
#include <vector>
#include <string_view>
#include "../value.hpp"
namespace shadergraph {
	class Node;
	class Overload;
	class IOContext;
	class Constructor
	{
	public:
		virtual ~Constructor() = default;
		virtual std::string_view uid() const = 0;
		virtual std::string_view display_name() const = 0;
		virtual std::vector<Overload> overloads() const = 0;
		virtual size_t default_overload() const = 0;
		virtual Node* construct(const Overload& overload) const = 0;
		virtual std::string_view input_name(const Overload& overload, size_t input_index) const = 0;
		virtual std::string_view output_name(const Overload& overload) const = 0;
		virtual Operand input_default(const Overload& overload, size_t input_index) const = 0;

		void add_io(IOContext& ctx, const Overload& overload) const;
	};
}   // namespace shadergraph