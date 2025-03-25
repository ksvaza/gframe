#pragma once
#include <SFML/Graphics.hpp>



class InputManager
{
public:

	class hwInput
	{

	};
	bool IsSpriteClicked();
	bool IsSpriteHovered();
	sf::Vector2i GetMousePosition();
};