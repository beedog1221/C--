#include <SDL.h>

//A circle stucture
struct Circle
{
	int x, y;
	int r;
};

class Dot
{
public:
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;

	static const int DOT_VEL = 1;

	Dot(int x, int y);
	~Dot();

	void move(SDL_Rect& square, Circle& circle);
	void render();
	Circle& getCollider();
	
private:
	// the x and y offsets of the dot
	int mPosX, mPosY;

	//the velocity of the dot
	int mVelX, mVelY;
	
	//Dots collision circle
	Circle mCollider;

	//MOve the collision circle relative to the dots offset
	void shiftColliders();
};

