#ifndef ENTITYBOX_ENTITY_MANAGER_H
#define ENTITYBOX_ENTITY_MANAGER_H
#include <map>
#include <vector>
#include <memory>
#include <boost/uuid/uuid_generators.hpp>
#include "Entity.h"
#include "TypeInfo.h"
namespace eb {
	class EntityManager {
		private:
			// [TypeInfo] -> [ (Entity -> Component) ]
			// aka: Each Type has a list of Entity -> Component relations
			std::map<TypeInfo, std::map<Entity, std::shared_ptr<void>>> m_entities;
		public:
			~EntityManager();

			// Entity creation function
			Entity createEntity();

			template <typename ComponentType>
			ComponentType& getComponent(Entity entity) {
				ComponentType* data = static_cast<ComponentType*>(
					m_entities[typeid(ComponentType)][entity].get()
				);

				return *data;
			}

			template <typename ComponentType>
			bool hasComponent(Entity entity) {
				bool doesHave = false;

				// Check that the type exists
				auto pair = m_entities.find(typeid(ComponentType));
				if(pair == m_entities.end()) return false;

				// Check if the specified entity has a component of that type
				auto component = pair->second.find(entity);
				if(component == pair->second.end()) return false;

				return true;
			}

			template <typename ComponentType>
			void addComponent(Entity entity, ComponentType* component) {
				std::shared_ptr<void> wrapped_component(component);
				m_entities[typeid(ComponentType)][entity] = wrapped_component;
			}

			template <typename ComponentType>
			std::vector<ComponentType*> getAllComponentsOfType() {
				std::vector<ComponentType*> typed_components;
				std::map<Entity, std::shared_ptr<void>> entity_component_pairs = m_entities[typeid(ComponentType)];

				for(auto pair = entity_component_pairs.begin(); pair != entity_component_pairs.end(); ++pair) {
					typed_components.push_back(static_cast<ComponentType*>(pair->second.get()));
				}

				return typed_components;
			}

			template <typename ComponentType>
			std::vector<Entity> getAllEntitiesWithComponent() {
				std::vector<Entity> entities;
				std::map<Entity, std::shared_ptr<void>> entity_component_pairs = m_entities[typeid(ComponentType)];

				for(auto pair = entity_component_pairs.begin(); pair != entity_component_pairs.end(); ++pair) {
					entities.push_back(pair->first);
				}

				return entities;
			}
	};
}
#endif