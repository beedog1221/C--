#include <iostream>

using namespace std;

class NGramPredictor;
int data;
int nValue;

registerSequence(actions)
{
	key = actions[0:nValue];
	value = actions[nValue];
};

if(key != NULL)
{
	data[key] = new KeyDataRecord();
	keyData = data[key];
};

if (value != keyData.counts)
{
	keyData.counts[value] = 0;
	++keyData.counts[value];
	++keyData.total;
}

getMostLikely(actions)
{
	keyData = data[actions];
	highestValue = 0;
	bestAction = none;
	actins = keyData.counts.getKeys();
}

for( actions = true)
{
	if(keyData.counts[action] > highestValue)
	{
		highestValue = keyData.counts[action];
		bestAction = action;
	}
}

return bestAction;
