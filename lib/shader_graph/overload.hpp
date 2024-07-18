#pragma once
#include "type.hpp"
#include <vector>
#include <span>
#include <variant>

namespace shadergraph {
	using Argument = std::variant<Type, int>;
	class Overload
	{
	public:
		struct Return
		{
			Type type;
			bool operator==(const Return& other) const = default;
		};

		Overload() = default;

		template <typename... Args>
		Overload& with_args(Args&&... args)
		{
			(m_args.emplace_back(std::forward<Args>(args)), ...);
			return *this;
		}

		Overload& with_return(Type type)
		{
			m_return.type = type;
			return *this;
		}

		bool operator==(const Overload& other) const;

		void add_argument(const Argument& argument)
		{
			m_args.emplace_back(argument);
		}

		void add_argument(const Type type)
		{
			m_args.emplace_back(Argument{ type });
		}

		void set_return(const Return& ret)
		{
			m_return = ret;
		}

		void set_type(size_t index, Type type)
		{
			m_args[index] = type;
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
