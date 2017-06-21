#include <time.h>
#include <iostream>

class timer {
	private:
		unsigned long begTime;
	public:
		void start() {
			begTime = clock();
		}

		unsigned long elapsedTime() {
			return ((unsigned long) clock() - begTime) / CLOCKS_PER_SEC;
		}

		bool isTimeout(unsigned long seconds) {
			return seconds >= elapsedTime();
		}
};

unsigned long bSeconds = 5;
unsigned long dSeconds = 8;
timer t;
t.start();


public function birth() :void{
		while (true) {
			if(t.elapsedTime() >= bSeconds){
				execute();
			}
			else {
				break;
			}
}

public function death() :void{
	while (true){
		if (t.elapsedTime() >= dSeconds){
			enter()
			exit();
		}
}