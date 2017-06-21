/* Specification:
First Name Last Name  
Lab 3 Exercise 1
This program does while loop  */

#include <iostream> 

using namespace std;

int main() 
{
      double games = 0.0;

	  int totalGames = 0;
	  

	  cout << "enter game amount: ";
	  cin >> games;

	  while (games >= 0.0)
	  {
		  totalGames = totalGames + games;

		  cout << "you have " << totalGames << " in stock.\n ";

		  cout << "next game amount: ";
		  cin >> games;
	  }  //end while
	
return 0;

}
