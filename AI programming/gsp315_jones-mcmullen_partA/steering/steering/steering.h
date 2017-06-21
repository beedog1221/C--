class movement
{
public:
	Location *character;
	real maxSpeed;
	virtual void steering(steeringOutput* output) const = 0;
};

class targetMovement : public movement
{
public:
	Vector3 *target;
};

class seek : public targetMovement
{
public:
	virtual void steering(steeringOutput* output) const;
};

class flee : public seek
{
public:
	virtual void steering(steeringOutput* output) const;
};

class arrive : public targetMovement
{
public:
	real timeToTarget;
	real radius;
	virtual void steering(steeringOutput* output) const;
};

class wander : public movement
{
public:
	real maxRotation;
	virtual void steering(steeringOutput* output) const;
};

class avoid : public movement
{
public:
	real lookAhead;
	virtual void steering(steeringOutput* output) const;
};

class blendedWeight : public movement
{
public:
	real behavior;
	virtual void steering(steeringOutput* output) const;
};