#pragma once

#include <cinttypes>
#include <vector>
#include <unordered_map>

#include "containers/ring-vector.hpp"

namespace containers {

template<
	class Key,
	class T,
	class Hash = std::hash<Key>,
	class KeyEqual = std::equal_to<Key>,
	class HashContainer = std::unordered_map<Key, T, Hash, KeyEqual>,
	class RingContainer = ring_vector<Key>>
class ring_hash_map {
public:
	explicit ring_hash_map(uint64_t max_size)
		: m_key_order(max_size), m_max_size(max_size)
	{
		m_hash.reserve(m_max_size + 1);
	}

	template<class ... Ts>
	auto insert_or_assign(Ts&&...args)
	{
		auto [it, inserted] = m_hash.insert_or_assign(
			std::forward<Ts>(args)...
		);

		std::optional<std::pair<Key, T>> removed = std::nullopt;

		if(inserted) {
			auto opt = m_key_order.push_back(it->first);
			if(opt.has_value()) {
				auto to_rem = m_hash.find(opt.value());
				removed.emplace(to_rem->first, to_rem->second);
				m_hash.erase(to_rem);
			}
		}

		return std::make_tuple(it, inserted, removed);
	}

	[[nodiscard]] auto size() const
	{
		return m_hash.size();
	}

	[[nodiscard]] auto max_size() const
	{
		return m_max_size;
	}

	[[nodiscard]] auto is_full() const
	{
		return size() == max_size();
	}

	template<class SomeKeyType>
	[[nodiscard]] auto find(SomeKeyType&& k)
	{
		return m_hash.find(std::forward<SomeKeyType>(k));
	}

	template<class SomeKeyType>
	[[nodiscard]] auto find(SomeKeyType&& k) const
	{
		return m_hash.find(std::forward<SomeKeyType>(k));
	}

	template<class SomeKeyType>
	[[nodiscard]] bool contains(SomeKeyType&& k) const
	{
		return m_hash.contains(std::forward<SomeKeyType>(k));
	}

	[[nodiscard]] auto begin()
	{
		return m_hash.begin();
	}

	[[nodiscard]] auto end()
	{
		return m_hash.end();
	}

	[[nodiscard]] auto begin() const
	{
		return m_hash.begin();
	}

	[[nodiscard]] auto end() const
	{
		return m_hash.end();
	}

	[[nodiscard]] auto cbegin() const
	{
		return m_hash.cbegin();
	}

	[[nodiscard]] auto cend() const
	{
		return m_hash.end();
	}

	template<class SomeKeyType>
	auto& operator[](SomeKeyType&& k)
	{
		return find(std::forward<SomeKeyType>(k))->second;
	}

	template<class SomeKeyType>
	auto const& operator[](SomeKeyType&& k) const
	{
		return find(std::forward<SomeKeyType>(k))->second;
	}

private:
	HashContainer m_hash;
	RingContainer m_key_order;
	uint64_t m_max_size;
};

} // end of namespace containers
