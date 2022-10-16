#include <SFML/Graphics.hpp>
#include "map.h"
#include "View.h"
#include "Game.h"
using namespace sf;
int main()
{
	RenderWindow window(VideoMode(1600, 920), "WORKS!", Style::Close);
	setValues();
	
	//игра
	setsounds();
	maintheme.play();
	gameState = inmenu;
	play(window, levelNumber);
	saveValues();

	return 0;
}
