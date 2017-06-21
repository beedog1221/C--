#include "movement.h"



    void Seek::getSteering(SteeringOutput* output) const

    {
        output->linear = *target;
        output->linear -= character->position;

        if (output->linear.squareMagnitude() > 0)
        {
            output->linear.normalise();
            output->linear *= maxSpeed;
        }

    }

	void Wander::getSteering(SteeringOutput* output) const

    {
        output->linear = character->getOrientationAsVector();
        output->linear *= maxSpeed;

        real change = randomBinomial();
        output->angular = change * maxRotation;

    }