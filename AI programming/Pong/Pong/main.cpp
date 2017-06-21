#include <iostream>
#include <string>
#include <vector>
#include "movement.h"
#include "state machine.h"


float ballSpeedX = BSPEED;
float ballSpeedY = Bspeed;



int main(){

	// sim loop
	while (window.isOpen()){
		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed){
				window.close();
			}
			if (event.type == sf::Event::KeyPressed){
				// Press escape to close the window
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
			}
		}

	}

	moveBall();

	if (ballSpeedX > 0)
		player[1].chase();
		player[2].wander();
	if (


		return 0;
}

void moveBall(){

	ball.setPosition(sf::Vector2f(ball.getPosition().x + ballSpeedX, ball.getPosition().y + ballSpeedY));



	// Ball -> Wall

	if (ball.getPosition().x < 0){

		enemyPoints++;

		gameOver = true;

		return;

	}

	if (ball.getPosition().x + BALLW > window.getSize().x){

		playerPoints++;

		gameOver = true;

		return;

	}

	if (ball.getPosition().y < 0 || ball.getPosition().y + BALLH > window.getSize().y){

		ballSpeedY *= -1;

		return;

	}



	// Ball -> Both paddles (Side)

	if (intersect(sf::Vector2f(ballSpeedX, ballSpeedY), sf::Vector2f(player.getPosition().x + PADDLEW, player.getPosition().y),

		sf::Vector2f(player.getPosition().x + PADDLEW, player.getPosition().y + PADDLEH)) ||

		intersect(sf::Vector2f(ballSpeedX, ballSpeedY), enemy.getPosition(),

		sf::Vector2f(enemy.getPosition().x, enemy.getPosition().y + PADDLEH)))

	{

		ballSpeedX *= -1;

		ball.setFillColor(sf::Color(rand() % 156 + 100, rand() % 156 + 100, rand() % 156 + 100));

		increaseSpeed();

		return;

	}



	// Player - Top

	if (intersect(sf::Vector2f(ballSpeedX, ballSpeedY), player.getPosition(),

		sf::Vector2f(player.getPosition().x + PADDLEW, player.getPosition().y)))

	{

		ballSpeedY += 0.02f;

		if (ballSpeedX < 0)

			ballSpeedX *= -1;

		if (ballSpeedY > 0)

			ballSpeedY *= -1;

		return;

	}



	// Player - Bottom

	if (intersect(sf::Vector2f(ballSpeedX, ballSpeedY),

		sf::Vector2f(player.getPosition().x, player.getPosition().y + PADDLEH),

		sf::Vector2f(player.getPosition().x + PADDLEW, player.getPosition().y + PADDLEH)))

	{

		if (ballSpeedX < 0)

			ballSpeedX *= -1;

		if (ballSpeedY < 0)

			ballSpeedY *= -1;

		ballSpeedY += 0.02f;

	}

}