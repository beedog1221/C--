/* Specification:
First Name Last Name  
Lab 3 Exercise 3
This program does guessing game  */

      //Demonstrate how to generate random numbers
//The random number will be greater than or equal to the lower bound
//And less than or equal to the upper bound  
#include <iostream>
#include <time.h>

using namespace std;

int main(void) 
{
      srand((int)time(NULL)); //seed the random function

      int upperLimit = 10;  //defining the upper and lower bounds
      int lowerLimit = 1;
	  int guess = 0;
	  int guessCount = 0;
	  char userChoice = 'n';


  
            int myRandomNumber = 1 + rand() % (upperLimit - lowerLimit + 1);  //generate the random number
           
			do {
				cout << "guess the number ";
				cin >> guess;
			guessCount = guessCount + 1;
			if (guess > myRandomNumber)
				cout << "Too high, try again!" ;
			if (guess < myRandomNumber)
				cout << "too low, try again!" ;
			if (guess == myRandomNumber)
				cout << "You've guessed wisely!" ;
			if (guess == 1)
				cout << " end game" << endl;
			}
			cout << "play again? ";
			cin >> userChoice;

			while ((userChoice == 'y') || (userChoice == 'Y'));
}