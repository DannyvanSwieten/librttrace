#pragma once
#include <map>
#include <memory>
#include <vector>
#include "overload.hpp"
#include "node/constructor.hpp"
namespace shadergraph {
	class ConstructorCache
	{
	public:
		~ConstructorCache();
		void register_ctor(std::unique_ptr<Constructor> ctor);
		Node* construct(const std::string& ctor_uid, const Overload& overload) const;

		struct Entry
		{
			std::vector<Overload> m_overloads;
			std::unique_ptr<Constructor> m_ctor;
		};

		const Entry* get_entry(const std::string& ctor_uid) const;

	private:
		std::map<std::string, std::unique_ptr<Entry>> m_constructors;
	};
}   // namespace shadergraph