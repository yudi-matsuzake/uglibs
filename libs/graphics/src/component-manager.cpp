#include "ug/graphics/component-manager.hpp"

namespace graphics{

void component_manager::add_component(
	std::shared_ptr<component> ptr,
	uint64_t id)
{
	m_components.insert({ id, ptr });
}

size_t component_manager::size() const
{
	return m_components.size();
}

void component_manager::remove_expired_components()
{
	for(auto i = std::begin(m_components);
		i != std::end(m_components); ++i){
		if(i->second.expired())
			i = m_components.erase(i);
	}
}


component_container::iterator component_manager::begin()
{
	return std::begin(m_components);
}

component_container::const_iterator component_manager::begin() const
{
	return std::cbegin(m_components);
}

component_container::iterator component_manager::end()
{
	return std::end(m_components);
}

component_container::const_iterator component_manager::end() const
{
	return std::cend(m_components);
}


uint64_t component_manager::next_id()
{
	static uint64_t id = 0;
	return id++;
}

} // end of namespace graphics
