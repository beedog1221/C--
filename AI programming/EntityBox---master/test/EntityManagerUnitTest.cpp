#include <string>
#include <memory>
#include "EntityManager.h"
#include "Entity.h"

#define BOOST_TEST_MODULE EntityManagerUnitTest
#include <boost/test/included/unit_test.hpp>

struct WalkComponent {
	int speed;
};

struct SingComponent {
	float pitch;
	std::string song;
};

struct FlyComponent {
	int height;
};

WalkComponent* generateWalkComponent() {
	WalkComponent* walk = new WalkComponent;
	walk->speed = 15;
	return walk;
}

SingComponent* generateSingComponent() {
	SingComponent* sing = new SingComponent;
	sing->pitch = 27.5f;
	sing->song = "Hello World";
	return sing;
}

BOOST_AUTO_TEST_SUITE(entity_manager_unit_test)

BOOST_AUTO_TEST_CASE( has_component_true_false_test) {
	eb::EntityManager manager;

	eb::Entity player = manager.createEntity();

	WalkComponent* walk = generateWalkComponent();

	// hasComponent should return false for both components
	BOOST_CHECK_EQUAL(manager.hasComponent<WalkComponent>(player), false);
	BOOST_CHECK_EQUAL(manager.hasComponent<SingComponent>(player), false);

	manager.addComponent(player, walk);

	// hasComponent should return true for WalkComponent, false for SingComponent
	BOOST_CHECK_EQUAL(manager.hasComponent<WalkComponent>(player), true);
	BOOST_CHECK_EQUAL(manager.hasComponent<SingComponent>(player), false);
}

BOOST_AUTO_TEST_CASE( single_entity_single_component_test ) {
	eb::EntityManager manager;

	eb::Entity player = manager.createEntity();

	WalkComponent* walk = generateWalkComponent();

	// Adding the component shouldn't error as the component exists
	manager.addComponent(player, walk);

	// hasComponent should return true for the walk component
	BOOST_CHECK(manager.hasComponent<WalkComponent>(player));

	// Getting the component back should work
	WalkComponent& outwalk = manager.getComponent<WalkComponent>(player);
	BOOST_CHECK_EQUAL(outwalk.speed, walk->speed);
}

BOOST_AUTO_TEST_CASE( single_entity_multiple_component_test ) {
	eb::EntityManager manager;

	eb::Entity player = manager.createEntity();

	WalkComponent* walk = generateWalkComponent();
	SingComponent* sing = generateSingComponent();

	// Adding the component shouldn't error as the component exists
	manager.addComponent(player, walk);
	manager.addComponent(player, sing);

	// hasComponent should return true for both
	BOOST_CHECK(manager.hasComponent<WalkComponent>(player));
	BOOST_CHECK(manager.hasComponent<SingComponent>(player));

	// Getting the components back should work
	WalkComponent& outwalk = manager.getComponent<WalkComponent>(player);
	SingComponent& outsing = manager.getComponent<SingComponent>(player);

	BOOST_CHECK_EQUAL(outwalk.speed, walk->speed);
	BOOST_CHECK_EQUAL(outsing.pitch, sing->pitch);
	BOOST_CHECK_EQUAL(outsing.song, sing->song);
}

BOOST_AUTO_TEST_CASE( get_all_components_of_type ) {
	eb::EntityManager manager;

	eb::Entity playerA = manager.createEntity();
	eb::Entity playerB = manager.createEntity();

	manager.addComponent(playerA, generateWalkComponent());
	manager.addComponent(playerA, generateSingComponent());
	manager.addComponent(playerB, generateWalkComponent());

	// Getting a list of all WalkComponents should return a list of two
	auto walkList = manager.getAllComponentsOfType<WalkComponent>();
	auto singList = manager.getAllComponentsOfType<SingComponent>();
	auto flyList = manager.getAllComponentsOfType<FlyComponent>();
	BOOST_CHECK_EQUAL(walkList.size(), 2);
	BOOST_CHECK_EQUAL(singList.size(), 1);
	BOOST_CHECK_EQUAL(flyList.size(), 0);

	// Compare the data in the walkList against the reference WalkComponent
	auto walk_reference = std::shared_ptr<WalkComponent>(generateWalkComponent());
	std::for_each(walkList.begin(), walkList.end(), [&walk_reference](WalkComponent*& walk) {
		BOOST_CHECK_EQUAL(walk_reference->speed, walk->speed);
	});

	// Compare the data in the singList against the reference SingComponent
	auto sing_reference = std::shared_ptr<SingComponent>(generateSingComponent());
	std::for_each(singList.begin(), singList.end(), [&sing_reference](SingComponent*& sing) {
		BOOST_CHECK_EQUAL(sing_reference->song, sing->song);
		BOOST_CHECK_EQUAL(sing_reference->pitch, sing->pitch);
	});

	// Attempt to iterate over the flyList, this should just do nothing since the list is empty
	std::for_each(flyList.begin(), flyList.end(), [](FlyComponent*& fly) {
		return boost::exit_test_failure;
	});
}

BOOST_AUTO_TEST_CASE( get_all_entities_with_component) {
	eb::EntityManager manager;

	eb::Entity playerA = manager.createEntity();
	eb::Entity playerB = manager.createEntity();
	eb::Entity playerC = manager.createEntity();

	manager.addComponent(playerA, generateWalkComponent());
	manager.addComponent(playerA, generateSingComponent());
	manager.addComponent(playerB, generateWalkComponent());
	manager.addComponent(playerC, generateWalkComponent());

	auto walkingEntities = manager.getAllEntitiesWithComponent<WalkComponent>();
	auto singingEntities = manager.getAllEntitiesWithComponent<SingComponent>();
	auto flyingEntities = manager.getAllEntitiesWithComponent<FlyComponent>();

	BOOST_CHECK_EQUAL(walkingEntities.size(), 3);
	BOOST_CHECK_EQUAL(singingEntities.size(), 1);
	BOOST_CHECK_EQUAL(flyingEntities.size(), 0);

	// Try doing something with the walking entities
	auto walk_reference = std::shared_ptr<WalkComponent>(generateWalkComponent());
	auto sing_reference = std::shared_ptr<SingComponent>(generateSingComponent());
	std::for_each(walkingEntities.begin(), walkingEntities.end(), [&manager, &walk_reference, &sing_reference](eb::Entity walker) {
		WalkComponent& walk = manager.getComponent<WalkComponent>(walker);
		BOOST_CHECK_EQUAL(walk.speed, walk_reference->speed);

		if(manager.hasComponent<SingComponent>(walker)) {
			SingComponent& sing = manager.getComponent<SingComponent>(walker);
			BOOST_CHECK_EQUAL(sing.pitch, sing_reference->pitch);
			BOOST_CHECK_EQUAL(sing.pitch, sing_reference->pitch);
		}
	});
}

BOOST_AUTO_TEST_SUITE_END()