#include <iostream>

class Ball

{

static inHand<Ball> ball;


int position_x, position_y;

int velocity_x, velocity_y;


Ball(int pos_x, int pos_y, int vel_x, int vel_y)

{

position_x = pos_x;

position_y = pos_y;

velocity_x = vel_x;

velocity_y = vel_y;

}


static void Initialize()

{

ball = new inHand<ball>;

}


static void Throw(int pos_x, int pos_y, int vel_x, int vel_y)

{

ball.Add(new Ball(pos_x, pos_y, vel_x, vel_y);



static void Update()

    {

        for (int i = 0; i < 1; i++)

        {

            ball[i].position_x += ball[i].velocity_x;

            ​ball[i].position_y += ball[i].velocity_y;

        }

    }

}

