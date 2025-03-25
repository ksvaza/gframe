#pragma once
#include <SFML/Graphics.hpp>



class InputManager
{
public:
	bool IsSpriteClicked(/*sprite*/, /*button*/, /*window*/);
	bool IsSpriteHovered(/*sprite*/, /*window*/);
	sf::Vector2i GetMousePosition(/*window*/);
};