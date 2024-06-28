#pragma once
#include "type.hpp"
#include <vector>
#include <span>

namespace shadergraph {

	class Overload
	{
	public:
		struct Argument
		{
			Type type;
			bool operator==(const Argument& other) const = default;
		};

		struct Return
		{
			Type type;
			bool operator==(const Return& other) const = default;
		};

		Overload() = default;
		Overload(const std::span<const Type> args, const Type ret);

		bool operator==(const Overload& other) const;

		void add_argument(const Argument& argument)
		{
			m_args.emplace_back(argument);
		}

		void add_argument(const Type type)
		{
			m_args.emplace_back(Argument{ .type = type });
		}

		void set_return(const Return& ret)
		{
			m_return = ret;
		}

		void set_type(size_t index, Type type)
		{
			m_args[index].type = type;
		}

		Type return_type() const
		{
			return m_return.type;
		}

		const auto& args() const
		{
			return m_args;
		}

	private:
		std::vector<Argument> m_args;
		Return m_return;
	};
}   // namespace shadergraph
