#include "stateMachine.h"

int made;
int killed;


public function enter() :void{
}

public function execute() :void{
	string message;
	cout << "enter message: ";
	cin >> message;
	cout << "sending message.....";
}

public function exit() :void{
	create_entity().new Entity
		++made;
	delete_entity()
		++killed;
}

public function update() :void{
}