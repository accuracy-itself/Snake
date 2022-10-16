#include <SFML/Audio.hpp>
sf::SoundBuffer hitbuff, hissbuff, firebuff, bonusbuff, overbuff, doorbuff;
sf::Music maintheme;
sf::Sound hitsound, hisssound, firesound, bonussound, doorsound, oversound;
void setsounds(){
	hitbuff.loadFromFile("files/hit.ogg");
	hissbuff.loadFromFile("files/hiss1.ogg");
	firebuff.loadFromFile("files/fire.ogg");
	bonusbuff.loadFromFile("files/bonus.ogg");
	overbuff.loadFromFile("files/over.ogg");
	doorbuff.loadFromFile("files/door.ogg");

	hitsound.setBuffer(hitbuff);
	hisssound.setBuffer(hissbuff);
	firesound.setBuffer(firebuff);
	bonussound.setBuffer(bonusbuff);
	oversound.setBuffer(overbuff);
	doorsound.setBuffer(doorbuff);

	maintheme.openFromFile("files/maintheme.ogg");
	maintheme.setLoop(true);
	//maintheme.setVolume(50);
}