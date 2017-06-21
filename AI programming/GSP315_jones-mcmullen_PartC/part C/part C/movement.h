#ifndef MOVEMENTC_H
#define MOVEMENT_H

namespace aicore
{
    
    class Movement
    {
    public:
     
        Location *ball;

        real maxSpeed;

        virtual void getSteering(SteeringOutput* output) const = 0;
    };

  
    class BallMovement : public Movement
    {
    public:
   
        Vector3 *target;
    };


   
    class Seek : public BallMovement
    {
    public:
       
        virtual void getSteering(SteeringOutput* output) const;
    };

    
     */
    class Wander : public Movement
    {
    public:
       
        real maxRotation;

        virtual void getSteering(SteeringOutput* output) const;
    };

}; 

#endif 
