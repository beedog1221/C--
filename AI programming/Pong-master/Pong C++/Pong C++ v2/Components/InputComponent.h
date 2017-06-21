#include <SDL.h>

class InputComponent
{
public:
	/// <summary>
	/// Handles the input.
	/// </summary>
	/// <param name="currentKeyStates">The current key states.</param>
	virtual void handleInput(const Uint8* currentKeyStates) = 0;
};