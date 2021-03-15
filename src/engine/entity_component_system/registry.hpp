#pragma once
#include <optional>
#include <vector>
#include <unordered_map>
#include <typeindex>

template<typename T>
concept component_type = std::movable<T>;

struct Entity
{
	uint32_t id;
};

inline bool operator==(const Entity& a, const Entity& b)
{
	return a.id == b.id;
}

inline bool operator<(const Entity& a, const Entity& b)
{
	return a.id < b.id;
}

struct EntityRef
{
	Entity ent;
	uint32_t generation;
};

template<typename Component>
struct componentStruct {
	std::vector<int> sparse;
	std::vector<Entity> entities;
	std::vector<Component> components;
	size_t componentSize = 0;
};

class Registry
{
public:

	Entity create() {
		uint32_t i;
		for (i = 0; i < flags.size(); i++) {
			if (!flags[i]) {
				flags[i] = true;
				generations[i]++;
				return Entity{ i };
			}
		}
		flags.push_back(true);
		generations.push_back(1);
		return Entity{ i };
	}

	void erase(Entity _ent) {
		flags[_ent.id] = false;
		for (auto it = componentMap.begin(); it != componentMap.end(); it++) {
			auto& cS = it->second;
			if (cS.sparse.size() > _ent.id && cS.sparse[_ent.id] != -1) {
				int pos = cS.sparse[_ent.id];
				cS.sparse[_ent.id] = -1;
				cS.entities[pos] = cS.entities[cS.entities.size() - 1];
				cS.entities.pop_back();
				char* remove = cS.components.data() + cS.componentSize * pos;
				char* last = cS.components.data() + cS.componentSize * cS.entities.size();
				memcpy(remove, last, cS.componentSize);
				cS.components.resize(cS.componentSize * (cS.components.size() / cS.componentSize - 1));

				if (pos < cS.entities.size()) {
					cS.sparse[cS.entities[pos].id] = pos;
				}					
			}
		}
	}

	EntityRef getRef(Entity _ent) const {
		return { _ent, generations[_ent.id] };
	}
	std::optional<Entity> getEntity(EntityRef _ent) const {
		if (_ent.generation == generations[_ent.ent.id] && flags[_ent.ent.id]) {
			return _ent.ent;
		}
		return std::nullopt;
	}

	// Add a new component to an existing entity. No changes are done if Component
	// if_ent already has a component of this type.
	// @return A reference to the new component or the already existing component.
	template<component_type Component, typename... Args>
	Component& addComponent(Entity _ent, Args&&... _args) {
		if (componentMap.contains(std::type_index(typeid(Component)))) {
			auto& cS = componentMap[std::type_index(typeid(Component))];
			while (cS.sparse.size() <= _ent.id) {
				cS.sparse.push_back(-1);
			}
			if (cS.sparse[_ent.id] != -1) {
				return *(Component*)(cS.components.data() + cS.componentSize * cS.sparse[_ent.id]);
			}
		}
		else {
			std::vector<int> sparse;
			std::vector<Entity> entities;
			std::vector<char> components;
			size_t componentSize = sizeof(Component);
			componentStruct<char> comStr = { sparse, entities, components, componentSize };

			componentMap[std::type_index(typeid(Component))] = comStr;
		}
		auto& cS = componentMap[std::type_index(typeid(Component))];
		do {
			cS.sparse.push_back(-1);
		} while (cS.sparse.size() <= _ent.id);
		cS.sparse.push_back(-1);
		cS.sparse[_ent.id] = cS.entities.size();
		cS.entities.push_back(_ent);
		cS.components.resize(cS.componentSize * cS.entities.size());
		Component component{ _args... };
		memcpy(cS.components.data() + cS.componentSize * cS.sparse[_ent.id], &component, cS.componentSize);
		return *(Component*)(cS.components.data() + cS.componentSize * cS.sparse[_ent.id]);
	}

	// Remove a component from an existing entity.
	// Does not check whether it exists.
	template<component_type Component>
	void removeComponent(Entity _ent) {
		if (componentMap.contains(std::type_index(typeid(Component)))) {
			auto& cS = componentMap[std::type_index(typeid(Component))];
			if (cS.sparse.size() > _ent.id && cS.sparse[_ent.id] != -1) {
				int pos = cS.sparse[_ent.id];
				cS.sparse[_ent.id] = -1;
				cS.entities[pos] = cS.entities[cS.entities.size() - 1];
				cS.entities.pop_back();
				char* remove = cS.components.data() + cS.componentSize * pos;
				char* last = cS.components.data() + cS.componentSize * cS.entities.size();
				memcpy(remove, last, cS.componentSize);
				cS.components.resize(cS.componentSize * (cS.components.size() / cS.componentSize - 1));

				if (pos < cS.entities.size())
					cS.sparse[cS.entities[pos].id] = pos;
			}
			if (cS.entities.size() == 0) {
				componentMap.erase(std::type_index(typeid(Component)));
			}
		}
	}

	// Retrieve a component associated with an entity.
	// @return The component or nullptr if the entity has no such component.
	template<component_type Component>
	Component* getComponent(Entity _ent) {
		if (componentMap.contains(std::type_index(typeid(Component)))) {
			auto& cS = componentMap[std::type_index(typeid(Component))];
			if (cS.sparse.size() > _ent.id && cS.sparse[_ent.id] != -1) {
				return (Component*)(cS.components.data() + cS.componentSize * cS.sparse[_ent.id]);
			}
		}
		return nullptr;
	}
	template<component_type Component>
	const Component* getComponent(Entity _ent) const {
		if (componentMap.contains(std::type_index(typeid(Component)))) {
			auto& cS = componentMap[std::type_index(typeid(Component))];
			if (cS.sparse.size() > _ent.id && cS.sparse[_ent.id] != -1) {
				return (Component*)(cS.components.data() + cS.componentSize * cS.sparse[_ent.id]);
			}
		}
		return nullptr;
	}

	// Retrieve a component associated with an entity.
	// Does not check whether it exits.
	template<component_type Component>
	Component& getComponentUnsafe(Entity _ent) {
		auto& cS = componentMap[std::type_index(typeid(Component))];
		return *(Component*)(cS.components.data() + cS.componentSize * cS.sparse[_ent.id]);
	}
	template<component_type Component>
	const Component& getComponentUnsafe(Entity _ent) const {
		auto& cS = componentMap[std::type_index(typeid(Component))];
		return *(Component*)(cS.components.data() + cS.componentSize * cS.sparse[_ent.id]);
	}

	// Execute an Action on all entities having the components
	// expected by Action::operator(component_type&...).
	// In addition, the entity itself is provided if
	// the first parameter is of type Entity.
	template<typename... Args, typename Action>
	void execute(const Action& _action) {
		using namespace std;
		vector<std::type_index> comp = { (0, std::type_index(typeid(Args))) ... };
		if (comp.size() == 1 && comp[0] != std::type_index(typeid(Entity))) {		//special case: one component
			auto& cS = componentMap[comp[0]];
			for (auto it = cS.entities.begin(); it != cS.entities.end(); it++) {
				func<Args...>(*it, _action, std::tie());
			}
			return;
		}
		if (comp[0] == std::type_index(typeid(Entity))) {
			comp.erase(comp.begin());
		}
		for (int i = 0; i < comp.size(); i++) {
			if (componentMap.find(comp.at(i)) == componentMap.end()) return;
		}
		vector<Entity> ent = componentMap[comp[0]].entities;
		for (int i = 1; i < comp.size(); i++) {
			for (auto it = ent.begin(); it != ent.end();) {
				if (componentMap[comp[i]].sparse.size() <= it->id || componentMap[comp[i]].sparse[it->id] == -1) {
					it = ent.erase(it);
				}
				else {
					it++;
				}
			}
		}
		for (auto it = ent.begin(); it != ent.end(); it++) {
			func<Args...>(*it, _action, std::tie());
		}
	}

	//template<typename Component, typename... Args, typename Action>
	//void func(Entity ent, const Action& action);
	template<typename Component, typename... Args, typename Action, typename Tuple>
	void func(Entity ent, const Action& action, Tuple tuple) {
		if constexpr (std::is_same<Entity, Component>::value) {
			auto tu = std::tuple_cat(tuple, std::tie(ent));
			if constexpr (sizeof...(Args) > 0) func<Args...>(ent, action, tu);
			else std::apply(action, tu);
		}
		else {
			Component& c = getComponentUnsafe<Component>(ent);
			auto tu = std::tuple_cat(tuple, std::tie(c));
			if constexpr (sizeof...(Args) > 0) func<Args...>(ent, action, tu);
			else std::apply(action, tu);
		}
	}

	std::vector<bool> flags;
	std::vector<uint32_t> generations;
	std::unordered_map<std::type_index, componentStruct<char>> componentMap;
};
