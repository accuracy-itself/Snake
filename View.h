#include <SFML/Graphics.hpp>
using namespace sf;

View view;
void setviewcoordinates(float x, float y){
	float tx = x, ty = y;
	if (x > (map_width - 16) * 40) tx = (map_width - 16) * 40;
	if (x < 640) tx = 640;
	if (y >(map_height - 12) * 40) ty = (map_height - 12) * 40 + 20;
	if (y < 460) ty = 460;

	view.setCenter(tx, ty);
}

void scrollwasd(float time){
	if (Keyboard::isKeyPressed(Keyboard::A)){

		view.move(-0.1 * time, 0);
	}
	if (Keyboard::isKeyPressed(Keyboard::W)){
		view.move(0, -0.1 * time);
	}
	if (Keyboard::isKeyPressed(Keyboard::S)){
		view.move(0, 0.1 * time);
	}
	if (Keyboard::isKeyPressed(Keyboard::D)){
		view.move(0.1 * time, 0);
	}
}