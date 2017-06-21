#include "Entity.h"

using namespace eb;

Entity::Entity() 
	: boost::uuids::uuid(boost::uuids::random_generator()())
{}

Entity::Entity(boost::uuids::uuid const& u)
	: boost::uuids::uuid(u)
{}

Entity::operator boost::uuids::uuid() {
	return static_cast<boost::uuids::uuid&>(*this);
}

Entity::operator boost::uuids::uuid() const {
	return static_cast<boost::uuids::uuid const&>(*this);
}