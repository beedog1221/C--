#include <iostream>

using namespace std;
int main(int argc, char *argv[]) {
	double mem1 = 0.6;
	double mem2 = 0.4;
	int sysOutput1 = 20;
	int sysOutput2 = 50;
	int weightedAvg;
	
	if ((sysOutput1 != 0) || (sysOutput2 != 0))
	{
		weightedAvg = ((mem1 * sysOutput1) + (mem2 * sysOutput2)) / (mem1 + mem2);
		cout << weightedAvg;
		cout << endl;
	}
}