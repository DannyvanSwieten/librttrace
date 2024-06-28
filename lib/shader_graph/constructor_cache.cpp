#include "constructor_cache.hpp"
#include "node/constructor.hpp"
namespace shadergraph {
	ConstructorCache::~ConstructorCache()
	{}

	void ConstructorCache::register_ctor(std::unique_ptr<Constructor> ctor)
	{
		const auto uid = ctor->uid();
		const auto overloads = ctor->overloads();
		m_constructors.emplace(uid, std::make_unique<Entry>(overloads, std::move(ctor)));
	}
	Node* ConstructorCache::construct(const std::string& ctor_uid, const Overload& overload) const
	{
		auto it = m_constructors.find(ctor_uid);
		if (it == m_constructors.end())
			return nullptr;
		return it->second->m_ctor->construct(overload);
	}
	const ConstructorCache::Entry* ConstructorCache::get_entry(const std::string& ctor_uid) const
	{
		const auto it = m_constructors.find(ctor_uid);
		if (it == m_constructors.end())
			return nullptr;

		return it->second.get();
	}
}   // namespace shadergraph
