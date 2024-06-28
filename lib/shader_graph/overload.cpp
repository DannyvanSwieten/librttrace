#include "overload.hpp"

namespace shadergraph {
	Overload::Overload(const std::span<const Type> args, const Type ret) : m_return{ ret }
	{
		for (const auto& arg : args)
		{
			m_args.emplace_back(Argument{ .type = arg });
		}
	}
	bool Overload::operator==(const Overload& other) const
	{
		if (m_args.size() != other.m_args.size())
			return false;
		for (size_t i = 0; i < m_args.size(); ++i)
		{
			if (m_args[i].type != other.m_args[i].type)
				return false;
		}

		return true;
	}
}   // namespace shadergraph