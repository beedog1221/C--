#ifndef ENTITYBOX_UUID_H
#define ENTITYBOX_UUID_H
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
namespace eb {
	class Entity : public boost::uuids::uuid {
		public:
			Entity();
			explicit Entity(boost::uuids::uuid const& u);
	
			operator boost::uuids::uuid();
			operator boost::uuids::uuid() const;
	};
}
#endif