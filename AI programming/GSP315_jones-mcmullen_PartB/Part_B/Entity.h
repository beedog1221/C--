#ifndef ENTITY_H
#define ENTITY_H
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
namespace eb {
	class Entity : public boost::uuids::uuid {
		public:
			Entity();
	};

}
#endif