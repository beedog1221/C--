/* Specification:
First Name Last Name  
Lab 3 Exercise 2
This program does for loop  */

#include <iostream> 
using namespace std;

int main() 
{
			  int startVal = 0;
			  int endVal = 0;
			  int currentRow = 0;
			  int sum = 0;

			  cout << "**Create a Multiplication Table**\n ";
			  cout << "Enter the starting value: ";
			  cin >> startVal;
			  cout << "Enter the ending value: ";
			  cin >> endVal;

		cout << "\t";

		for (int theColmn = startVal; theColmn <= endVal; theColmn++) {
			 cout << theColmn << "\t"; //print the column headings
		}

		cout << endl;

		while (currentRow <= 5) {
			 cout << currentRow << "|\t" ; //print the row headings
			 for (int theColmn = startVal; theColmn <= endVal; theColmn++) 
			 {
				  cout << theColmn * currentRow << "\t"; //print the multiplication
			 }

			 cout << endl;
			 currentRow +=1; //go to the next row
		}

		cout << endl;


		return 0;

}
