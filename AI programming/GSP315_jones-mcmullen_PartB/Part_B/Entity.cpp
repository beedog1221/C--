#include "Entity.h"

using namespace eb;

Entity::Entity() 
	: boost::uuids::uuid(boost::uuids::random_generator()())
{}
