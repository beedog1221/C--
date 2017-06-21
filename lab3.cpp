/* Specification:
Braylin Jones - mcmullen  
Lab 3 Problem 1
program lets user work recursive problem  */

#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

int main(void) {
	

int one;
int two;
int first;
int second;
int fill;
int n;
int exp;
int exp2;

cout << "Enter a numeric value for A(0):\n";
cin >> one;
cout << endl;
cout << "Enter a numeric value for A(1):\n";
cin >> two;
cout << endl;


cout << "N = " << 0 << "\t A(N) = 1\t"  << "\t 2^N = 1\t" <<  "\t 2.1^N = 1\n";
cout << "N = " << 1 << "\t A(N) = 2\t"  << "\t 2^N = 2\t" <<  "\t 2.1^N = 2\n";

first = one;
second = two;


for (n=2; n<19; n++) {
	fill = (4 * second) - (4 * first);
	exp = pow (2,n);
	exp2 = pow (2.1,n);
	cout << "N = " << n << "\t A(N) = " << fill << "\t" << "\t 2^N = " << exp << "\t" << "\t 2.1^N = " << exp2 << endl;
	
	first = second;
	second = fill;
}


}