#pragma once

#include <map>
#include <memory>
#include <string>

#include "ug/graphics/component.hpp"

namespace ug::graphics{

using component_ptr = std::weak_ptr<component>;
using component_container = std::map<uint64_t, component_ptr>;

/**
  * this class is responsible for managing a set of weak pointers to
  * components
  */
class component_manager{
public:

	void add_component(
		std::shared_ptr<component> ptr,
		uint64_t id = next_id()
	);

	size_t size() const;
	component_ptr at(uint64_t id) const;
	void remove_expired_components();

	component_container::iterator begin();
	component_container::const_iterator begin() const;

	component_container::iterator end();
	component_container::const_iterator end() const;

protected:

	static uint64_t next_id();

	component_container m_components;
};

} // end of namespace graphics
