#include <iostream>

using namespace std;
int main(int argc, char *argv[]) {
	
	int input;
	
	cout << "enter a number 1-10 to see how hungry you are: ";
	cin >> input;
	
	if ((input <= 0) && (input <=2 ))
	{
		cout << "no I'm not that hungry. ";
	}
	
	else if ((input >= 2.1) && (input <= 4))
	{
		cout << "well.... I could use a bite. ";
	}
	
	else if ((input >= 4.1) && (input <= 7.5 ))
	{
		cout << "grab me a plate, I'm really hungry.";
	}
	
	else if ((input >= 7.6) && (input <= 10))
	{
		cout << "DUDE I'M DYING OVER HERE!!!";
	}
}