Event event;
enum { inmenu, inrest, ingame, inauth, inend, instory } gameState;
int hp = 10, totalscore = 0;
int gcolx = 0, gcoly = 0;
#include <fstream>

void menu(RenderWindow & window) {
	Image menuIm;
	menuIm.loadFromFile("images/rmenu.png");
	menuIm.createMaskFromColor(Color(237, 28, 36));
	Texture menuTexture1, menuTexture2, menuTexture3, menuTexture4, menuTexture5, menuBackground, storyText;
	menuTexture1.loadFromImage(menuIm);
	menuTexture2.loadFromImage(menuIm);
	menuTexture3.loadFromImage(menuIm);
	menuTexture4.loadFromImage(menuIm);
	menuTexture5.loadFromImage(menuIm);

	menuBackground.loadFromFile("images/des1.png");
	storyText.loadFromFile("images/story.png");
	
	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), menu4(menuTexture4), menu5(menuTexture5), menuBg(menuBackground), storySprite(storyText);
	bool isMenu = 1;
	int menuNum = 0;

	menuBg.setPosition(0, 0);

	menu1.setPosition(600, 150);
	menu1.setTextureRect(IntRect(0, 333, 382, 111));
	menu2.setPosition(600, 271);
	menu2.setTextureRect(IntRect(0, 0, 382, 111));
	menu3.setPosition(600, 392);
	menu3.setTextureRect(IntRect(0, 111, 382, 111));
	menu4.setPosition(600, 513);
	menu4.setTextureRect(IntRect(0, 222, 382, 111));
	
	window.setView(window.getDefaultView());
	while (isMenu)
	{
		window.clear(Color(238, 226, 206));
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menu3.setColor(Color::White);
		menu4.setColor(Color::White);

		menuNum = 0;
		if (IntRect(menu1.getPosition().x, menu1.getPosition().y, 382, 111).contains(Mouse::getPosition(window))) { menu1.setColor(Color(251, 148, 45)); menuNum = 1; }
		if (IntRect(menu2.getPosition().x, menu2.getPosition().y, 382, 111).contains(Mouse::getPosition(window))) { menu2.setColor(Color(251, 148, 45)); menuNum = 2; }
		if (IntRect(menu3.getPosition().x, menu3.getPosition().y, 382, 111).contains(Mouse::getPosition(window))) { menu3.setColor(Color(251, 148, 45)); menuNum = 3; }
		if (IntRect(menu4.getPosition().x, menu4.getPosition().y, 382, 111).contains(Mouse::getPosition(window))) { menu4.setColor(Color(251, 148, 45)); menuNum = 4; }

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
				isMenu = false;
			}
		}

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			switch (menuNum){
			case 1:
				window.draw(storySprite); 
				window.display(); 
				while (!Keyboard::isKeyPressed(Keyboard::Escape)); 
				break;

			case 2:
				isMenu = false;
				gameState = inrest;
				break;

			case 3:
				gameState = ingame;
				isMenu = false;
				break;


			case 4:
				isMenu = false;
				gameState = inend;
				break;
			}
				
		}

			window.draw(menuBg);
			window.draw(menu1);
			window.draw(menu2);
			window.draw(menu3);
			window.draw(menu4);

			window.display();
		}
}
