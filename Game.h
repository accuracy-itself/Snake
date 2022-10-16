#include <list>;
#include "menu.h"
#include "Music.h"

//всякие глобальности
bool rshift;
//с уровнями всякое
int levelNumber = 1;
const int kolLevel = 3;
const int specialLevel = 3;
//

class Creature{
public:
	float x, y;
	float w, h, dx, dy, speed, movetime, ch = 0;
	enum { left, right, jump, up, down, stay } state;
	bool shoots;
	int shootingtime;
	bool life, moves, onground;
	int health;
	Texture texture;
	Sprite sprite;
	String name;
	Creature(Image &image, float X, float Y, float W, float H, String Name){
		x = X;
		y = Y;
		w = W;
		h = H;
		name = Name;
		movetime = 0;
		shootingtime = 0;
		ch = 0;
		dx = 0;
		dy = 0;
		speed = 0;
		moves = false;
		onground = false;
		life = true;
		health = 100;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}

	FloatRect getRect(){
		return FloatRect(x, y, w, h);
	}

	virtual void update(float time) = 0;
};

class Player :public Creature{
public:
	float colx = 0, coly = 0, timeColor, sped = 0.22, injuredTime = 0, invulnTime = 0;
	int score = 0;
	bool injured = false, canclose = false, invuln = false;
	enum spriteState { left, right, jump, up, down, stay };
	spriteState state;
	//int colx = 48 * 6, coly = 0; 
	float CurrentFrame = 0;
	bool oldonground = onground, onplatf = false;
	int strangev = 17;


	Player(Image &image, float X, float Y, float W, float H, String Name) :Creature(image, X, Y, W, H, Name){
		colx = gcolx;
		coly = gcoly;
		timeColor = 0;
		state = stay;
		score = 0;
		health = hp; 
		Name = "Player";
		sprite.setTextureRect(IntRect(0 + colx, 0 + coly, w, h));
		speed = sped;
		//onground = true;
	}

	void interactWithMap(float Dx, float Dy){
		int i, j;

		for (i = x / 40; i < (x + w) / 40; i++)
		for (j = y / 40; j < (y + h) / 40; j++){
			if (Map[j][i] == '0'){
				if (Dx < 0) { x = i * 40 + 40; dx = 0; }
				if (Dx > 0) { x = i * 40 - w; dx = 0; }
				if (Dy > 0) { y = j * 40 - h; dy = 0; oldonground = onground; onground = true; if (!oldonground) sped = 0.22; }
				if (Dy < 0) { y = j * 40 + 40; dy = 0; }
			}

			if (Map[j][i] == 'm'){
				Map[j][i] = ' ';
				score++;
				bonussound.play();
			}

			if (Map[j][i] == 'n'){
				Map[j][i] = ' ';
				score++;
				bonussound.play();
			}

			if (Map[j][i] == 'f'){
				Map[j][i] = ' ';
				if ((!invuln)) { health -= 1; hisssound.play(); }
				if (!invuln) injured = true;
			}

			if (Map[j][i] == 'h'){
				Map[j][i] = ' ';
				health++;
				bonussound.play();
			}

			if (Map[j][i] == 'd'){
				canclose = true;
				doorsound.play();
			}

			if (Map[j][i] == 'z'){
				Map[j][i] = ' ';
				invuln = true;
				bonussound.play();
			}
		}
	
	}

	void control(float time){
		if (colx == 6 * 48) { strangev = 10; }
		else { strangev = 17; }

		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			speed = sped;
			state = right;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			sprite.setTextureRect(IntRect(48 * int(CurrentFrame) + colx, 96 + coly + strangev, 48, 48 - strangev));
			h = 25;
			w = 40;
		}

		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			speed = sped;
			state = left;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			sprite.setTextureRect(IntRect(48 * int(CurrentFrame) + colx, 48 + coly + strangev, 48, 48 - strangev));
			h = 25;
			w = 40;
		}

		if ((Keyboard::isKeyPressed(Keyboard::Up)) && (onground)) {
			dy = -0.9;
			if (!onplatf) sped = 0.26;
			state = jump;
			onground = false;
			
		}

		if (life){
			if (Keyboard::isKeyPressed(Keyboard::F)){

				if (timeColor > 0.5){
					colx += 48 * 3;
					if (colx >= 48 * 12){
						colx = 0;
						coly += 48 * 4;
						if (coly >= 48 * 4 * 2) coly = 0;
						gcolx = colx; gcoly = coly;
					}
					timeColor = 0;
				}	
			}

		}
	}

	void update(float time){
		if (life) control(time);
		else sprite.setColor(Color::Red);
		switch (state){
		case right: dx = speed; break;
		case left: dx = -speed; break;
		case stay: break;
		case jump: break;
		case up: break;
		case down: break;
		}

		sprite.setOrigin(w / 2, h / 2);

		x += dx * time;
		interactWithMap(dx, 0);
		y += dy * time;
		interactWithMap(0, dy);

		sprite.setPosition(x + w / 2, y + h / 2);
		if (health <= 0){ oversound.play(); life = false; }
		if (!moves) 
			speed = 0;
		else speed = sped;

		if (life) setviewcoordinates(x, y);
		timeColor += time * 0.001;
		if (life)
		{
			if (injured){
				injuredTime += time * 0.001;
				if ((int)(injuredTime * 8) % 2) sprite.setColor(Color::White);
				else sprite.setColor(Color::Red);
				if (injuredTime > 1.5){
					injuredTime = 0;
					injured = false;
					sprite.setColor(Color::White);
				};
			}

			if (invuln){
				invulnTime += time * 0.001;
				if ((int)(invulnTime * 4) % 2) sprite.setColor(Color::White);
				else sprite.setColor(Color(59, 88, 252));
				if (invulnTime > 5){
					invulnTime = 0;
					invuln = false;
					sprite.setColor(Color::White);
				};
			}
		}

		dy = dy + 0.0025*time;
	}
};

class Enemy :public Creature{
public:
	float ch = 0;

	Enemy(Image &image, float X, float Y, float W, float H, String Name) :Creature(image, X, Y, W, H, Name){
		ch = 0;
		if (name == "easy"){
			sprite.setTextureRect(IntRect(0, 0, w, h));
			ch = 0.1;
			w *= ch * 0.8;
			h *= ch * 0.8;
			sprite.setScale(-ch, ch);
			dx = 0.15;
			speed = 0.15;
			state = right;
		}

		if (name == "easyb"){
			sprite.setTextureRect(IntRect(0, 0, w, h));
			ch = 0.1;
			w *= ch * 0.8;
			h *= ch * 0.8;
			sprite.setScale(-ch, ch);
			dx = 0.05;
			speed = 0.05;
			state = right;
			shoots = false;
			sprite.setColor(Color(168, 104, 70));
		}
	}

	void interactionWithMap(float Dx, float Dy){
		for (int i = x / 40; i < std::min((x + w) / 40, (float)map_width); i++)
		for (int j = y / 40; j < std::min((y + h) / 40, (float)map_height); j++){
			if (Map[j][i] == '0'){
				if (Dx < 0) { x = i * 40 + 40; dx = speed; state = right; sprite.setScale(-ch, ch); }
				if (Dx > 0) { x = i * 40 - w; dx = -speed; state = left; sprite.setScale(ch, ch); }
				if (Dy > 0) { y = j * 40 - h; dy = 0; }
				if (Dy < 0) { y = j * 40 + 40; dy = 0; }
			}
		}
	}

	void update(float time){
		if ((name == "easy") || (name == "easyb")){
			x += dx * time;
			interactionWithMap(dx, 0);
			y += dy * time;
			interactionWithMap(0, dy);
			if (life) sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0) life = false;
			dy = dy + 0.0025*time;
		}
	}
};

class MovingPlatform : public Creature{
public:
	int hightime = 2000;
	MovingPlatform(Image &image, float X, float Y, float W, float H, String Name) :Creature(image, X, Y, W, H, Name){
		w *= 0.2353;
		h *= 0.2353 / 4;
		sprite.setScale(0.2353, 0.2353);
		sprite.setTextureRect(IntRect(402, 26, W, H));
		if (name == "longmovingPlatform") hightime = 6170;
		dx = 0.08;
		sprite.setOrigin(w / 2, h / 2);
	}

	void update(float time){
		x += dx* time;
		movetime += time;
		if (movetime > hightime){
			movetime = 0;
			dx *= -1;
		}
		sprite.setPosition(x + w / 2, y + h / 2);
	}
	FloatRect getRect(){
		if (name == "longmovingPlatform") return FloatRect(x, y, w + 10, h);
		return FloatRect(x, y, w + 20, h);
	}
};

class Bullet :public Creature{
public:
	int direction;

	Bullet(Image &image, float X, float Y, float W, float H, String Name, int dir) :Creature(image, X, Y, W, H, Name){
		x = X;
		y = Y;
		direction = dir;
		speed = 0.15;
		w = 16;
		h = 7;
		life = true;
	}


	void update(float time)
	{
		switch (direction)
		{
		case 1: dx = speed; dy = 0;   sprite.setScale(1, 1); break;//state == right
		case 0: dx = -speed; dy = 0;  sprite.setScale(-1, 1); break;//

		}

		x += dx*time;//само движение пули по х

		if (x <= 0) x = 1;
		if (y <= 0) y = 1;
		if (x >= map_width * 40) x = map_width * 40 - 2;
		if (y >= map_height * 40) y = map_height * 40 - 2;


		for (int i = x / 40; i < (x + w) / 40; i++)
		for (int j = y / 40; j < (y + h) / 40; j++) {
			if (Map[j][i] == '0') if (FloatRect(i * 40, j * 40, 40, 40).intersects(getRect())) //если этот объект столкнулся с пулей,
			{
				life = false;// то пуля умирает
			}
		}

		sprite.setPosition(x + w / 2, y + h / 2);//задается позицию пуле
	}
};

bool Game(RenderWindow &window, int &levelNumber){
	rshift = false;
	//menu(window);
	//RenderWindow window(VideoMode(1600, 920), "WORKS!"/*, Style::Fullscreen*/); 
	
	view.reset(FloatRect(0, 0, 1280, 920));

	createLevel(levelNumber);

	//для фона и
	Image map_image, map_image2;
	map_image.loadFromFile("images/text.png");
	map_image.createMaskFromColor(Color::White);
	map_image2.loadFromFile("images/text4.png");
	
	Texture map, map2, mainBackground;
	map.loadFromImage(map_image);
	map2.loadFromImage(map_image2);
	if(levelNumber!= specialLevel) mainBackground.loadFromFile("images/mainBg.png");
	else mainBackground.loadFromFile("images/specialBg.png");
	
	Sprite s_map, s_map2, mainBg(mainBackground);
	s_map.setTexture(map);
	s_map2.setTexture(map2);
	
	//if (levelNumber != specialLevel) 
	

	//шрифты
	Font font;
	font.loadFromFile("files/CyrilicOld.TTF");
	Text text("", font, 30);
	text.setFillColor(Color::White);
	Text text2("", font, 90);
	text2.setFillColor(Color::Red);

	//игрок
	
	int colx = 0, coly = 0, jumpBool = 0, change = 0, px = 0, py = 0;
	Image pimage;
	pimage.loadFromFile("images/snakes.png");
	for (int i = 0; i < map_height; i++)
	for (int j = 0; j < map_width; j++){
		if (Map[i][j] == 's'){ px = j * 40; py = i * 40; break; } //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}
	Player p(pimage, px, py, 48, 48 - 5, "p");
	setviewcoordinates(p.sprite.getPosition().x, p.sprite.getPosition().y);
	mainBg.setPosition(view.getCenter().x - 640, view.getCenter().y - 460);
	//враги
	Image eimage;
	eimage.loadFromFile("images/rat3.png");
	eimage.createMaskFromColor(Color::White);
	int kolEnemies = 0;
	int kolKilled = 0;

	//платформы 
	Image platfImage;
	platfImage = map_image2;

	//пули
	Image bulletImage;
	bulletImage.loadFromFile("images/bullet.png");
	bulletImage.createMaskFromColor(Color::Black);
	
	
	//звуки
	//setsounds(); 

	//тут со списком врагов
	std::list <Creature*> enemies;
	std::list<Creature*>::iterator list_i;
	for (int i = 0; i < map_height; i++)
	for (int j = 0; j < map_width; j++){
		if (Map[i][j] == 'e'){
			enemies.push_back(new Enemy(eimage, j * 40, i * 40, 450, 430, "easy"));
			kolEnemies++;
		}

		if (Map[i][j] == 'b'){
			enemies.push_back(new Enemy(eimage, j * 40, i * 40, 450, 430, "easyb"));
			kolEnemies++;
		}
	}

	std::list <MovingPlatform*> platforms;
	std::list<MovingPlatform*>::iterator listp_i;
	for (int i = 0; i < map_height; i++)
	for (int j = 0; j < map_width; j++){
		if (Map[i][j] == 'p'){
			platforms.push_back(new MovingPlatform(platfImage, j * 40, i * 40, 340, 170, "movingPlatform"));
		}

		if (Map[i][j] == 'l'){
			platforms.push_back(new MovingPlatform(platfImage, j * 40, i * 40, 170, 170, "longmovingPlatform"));
		}
	}

	std::list <Bullet*> bullets;
	std::list<Bullet*>::iterator listb_i;

	//для концовки
	Clock clock2;
	float wholeTime = 0, sx, sy;
	Image santaImage;
	santaImage.loadFromFile("images/santaall.png");
	santaImage.createMaskFromColor(Color(166, 202, 240));
	Texture santaTexture;
	santaTexture.loadFromImage(santaImage);
	Sprite santaSprite(santaTexture);
	santaSprite.setTextureRect(IntRect(0, 0, 680, 476));
	santaSprite.setScale(0.5, 0.5);
	sx = view.getCenter().x - 1000;
	sy = view.getCenter().y + 440;
	//для

	//время
	Clock clock, gameClock;
	int gameTime = 0;

	setviewcoordinates(p.x, p.y);
	while (window.isOpen())
	{

		/////////////////////////////////////////для концовки
		if ((levelNumber > kolLevel) && (totalscore >= 15)){
			//p.speed = 0;
			p.life = false;
			santaSprite.setPosition(sx, sy);
			window.draw(santaSprite);
				//clock2.restart();
				if (wholeTime < 6){

					float time2 = clock.getElapsedTime().asMilliseconds();
					wholeTime += time2 / 1000;
					sx += 0.1 * time2;
					santaSprite.setPosition(sx, sy);
					
				}

				if ((wholeTime >= 6) && (wholeTime < 13)){
					santaSprite.setTextureRect(IntRect(680, 0, 680, 476));
					float time2 = clock.getElapsedTime().asMilliseconds();
					wholeTime += time2 / 1000;
				}
				if (wholeTime >= 13) return false;
			
		}
		//////////////////////////////////////////////////////////

		mainBg.setPosition(view.getCenter().x - 640, view.getCenter().y - 460);
		float time = clock.getElapsedTime().asMicroseconds();
		if (p.life) gameTime = gameClock.getElapsedTime().asSeconds();
		else { view.move(0.1, 0); mainBg.setPosition(view.getCenter().x - 640, view.getCenter().y - 460);; /*continue;*/ }
		clock.restart();
		time = time / 1000;

		//Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed){
				window.close();
				//hp = p.health;
				return false;
			}

		}

		//закрываем, если у двери
		if (p.canclose == true) { gcolx = p.colx; gcoly = p.coly; hp = p.health; totalscore += p.score; return true; }
		gcolx = p.colx; gcoly = p.coly;/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (!p.life) scrollwasd(time);

		//удаляем лишних врагов из списка
		for (list_i = enemies.begin(); list_i != enemies.end();){
			(*list_i)->update(time);

			if (!(*list_i)->life) list_i = enemies.erase(list_i);
			else {
				if ((*list_i)->name == "easyb"){
					if ((((*list_i)->x - p.x < 400) && ((*list_i)->x - p.x > 0) && ((*list_i)->state == 0)) || ((-(*list_i)->x + p.x < 400) && (-(*list_i)->x + p.x > 0) && ((*list_i)->state == 1))){


						//if ((*list_i)->state == 1) std::cout << "right";


						if ((*list_i)->shootingtime == 0) (*list_i)->shoots = true;
						(*list_i)->shootingtime += time;
						if ((*list_i)->shootingtime > 1500){ (*list_i)->shootingtime = 0; }
						if ((*list_i)->shoots){
							firesound.play();
							bullets.push_back(new Bullet(bulletImage, (*list_i)->x, (*list_i)->y + 19, 16, 7, "bullet", (*list_i)->state));
							(*list_i)->shoots = false;
						}
					}
				}
				list_i++;
			}
		}

		//пули
		for (listb_i = bullets.begin(); listb_i != bullets.end();){
			(*listb_i)->update(time);
			//std::cout << "here";
			if (!(*listb_i)->life) listb_i = bullets.erase(listb_i);
			else listb_i++;
		}
		for (listb_i = bullets.begin(); listb_i != bullets.end(); listb_i++){
			if ((*(*listb_i)).getRect().intersects(p.getRect())){
				if ((*listb_i)->name == "bullet"){
					if ((!p.injured) && (!p.invuln)) { p.health -= 1; hisssound.play(); }
					if(!p.invuln)p.injured = true;
					(*listb_i)->life = false;

				}
			}
		}


		//платформы
		for (listp_i = platforms.begin(); listp_i != platforms.end();){
			(*listp_i)->update(time);
			if (!(*listp_i)->life) listp_i = platforms.erase(listp_i);
			else listp_i++;
		}

		//проверяем столкновения с врагами															//((int)p.y / 40 == (int) (*list_i)->y / 40 - 1))
		for (list_i = enemies.begin(); list_i != enemies.end(); list_i++){
			if ((*(*list_i)).getRect().intersects(p.getRect())){
				if (((*list_i)->name == "easy") || ((*list_i)->name == "easyb")){
					if ((p.dy > 0) && ((p.onground == false) || (p.y + p.h < (*list_i)->y + (*list_i)->h))){
						//(*list_i)->dx = 0;
						p.dy = -0.6;
						(*list_i)->health = 0;
						kolKilled++;
						hitsound.play();
					}
					else {
						if ((!p.injured) && (!p.invuln)) p.health -= 1;
						if (!p.invuln){ p.injured = true; hisssound.play(); }
					}
				}
			}
		}

		//столкновения с платформами
		for (listp_i = platforms.begin(); listp_i != platforms.end(); listp_i++){
			if ((*(*listp_i)).getRect().intersects(p.getRect())){
				if (((*listp_i)->name == "movingPlatform") || ((*listp_i)->name == "longmovingPlatform")){
					if ((p.y - (*listp_i)->y < -p.h / 2) && (p.dy > 0) && ((p.sprite.getPosition().x + 20 - (*listp_i)->sprite.getPosition().x > 0))){
						p.dy = 0;
						p.x += (*listp_i)->dx * time;
						p.onground = true;
						p.onplatf = true;
					}
					else {
						/*if ((!p.injured) && (!p.invuln)) p.health -= 1;
					if(!p.invuln)p.injured = true;*/
						p.onplatf = false;
					}
				}
				else p.onplatf = false;
			}
			else p.onplatf = false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		//рисую карту
		//рисую всё, что только рисую
		window.setView(view);
		window.clear(Color(238, 226, 206));
		window.draw(mainBg);
		for (int i = 0; i < map_height; i++)
		for (int j = 0; j < map_width; j++){
			if ((Map[i][j] == '0') || (Map[i][j] == '9')) {
				s_map2.setScale(0.2353, 0.2353);
				s_map2.setTextureRect(IntRect(402, 405, 170, 170)); s_map2.setPosition(j * 40, i * 40);
				window.draw(s_map2); continue;
			}
			//if (Map[i][j] == '0') { s_map.setTextureRect(IntRect(0, 0, 40, 40)); }
			if (Map[i][j] == 'f') { s_map.setTextureRect(IntRect(120, 40, 40, 40)); }
			if (Map[i][j] == 'm') s_map.setTextureRect(IntRect(80, 40, 40, 40));
			if (Map[i][j] == 'g') s_map.setTextureRect(IntRect(0, 200, 40, 40));
			if (Map[i][j] == ' ') s_map.setTextureRect(IntRect(120, 320, 40, 40));
			if (Map[i][j] == 'h') s_map.setTextureRect(IntRect(120, 0, 40, 40));
			if (Map[i][j] == 'd') s_map.setTextureRect(IntRect(119, 80, 23, 40));
			if (Map[i][j] == 'z') s_map.setTextureRect(IntRect(120, 160, 40, 40));
			if (Map[i][j] == 'n') s_map.setTextureRect(IntRect(120, 120, 40, 40));

			s_map.setPosition(j * 40, i * 40);
			window.draw(s_map);
			s_map.setScale(1, 1);
		}

		//рисую сердечки 
		s_map.setTextureRect(IntRect(120, 0, 40, 40));
		int dx = 0;
		for (int i = 0; i < p.health; i++){
			s_map.setPosition(view.getCenter().x + 600 - dx, view.getCenter().y + 375);
			window.draw(s_map);
			dx += 40;
		}

		//рисую прогресс в плане мышек
		if (levelNumber != specialLevel) s_map.setTextureRect(IntRect(80, 40, 40, 40));
		else s_map.setTextureRect(IntRect(120, 120, 40, 40));
		dx = 0;
		for (int i = 0; i < p.score; i++){
			s_map.setPosition(view.getCenter().x + 600 - dx, view.getCenter().y + 335);
			window.draw(s_map);
			dx += 40;
		}

		//рисую текст времени
		char st[10], sst[30] = "Время в игре: ", s2t[30] = "Всего очков:", s3t[25] = "RShift - сбросить кожу", s4t[20] = "Esc - Выйти в меню";
		_itoa_s(gameTime, st, 10);
		strcat_s(sst, st);
		text.setString(sst);
		text.setPosition(view.getCenter().x + 390, view.getCenter().y + 415);
		window.draw(text);
		//и текст очков
		_itoa_s(totalscore, st, 10);
		strcat_s(s2t, st);
		text.setString(s2t);
		text.setPosition(view.getCenter().x - 630, view.getCenter().y + 415);
		window.draw(text);
		//и управление
		text.setString(s4t);
		text.setPosition(view.getCenter().x - 630, view.getCenter().y - 460);
		window.draw(text);
		text.setString(s3t);
		text.setPosition(view.getCenter().x + 330, view.getCenter().y - 460);
		window.draw(text);
		//и текст убитых врагов
		char senemies[30] = "Убито ", svsp[4], senemies2[10] = " врагов", sslash[2] = "/";
		_itoa_s(kolKilled, svsp, 10);
		strcat_s(senemies, svsp);
		strcpy_s(svsp, sslash);
		strcat_s(senemies, svsp);
		_itoa_s(kolEnemies, svsp, 10);
		strcat_s(senemies, svsp);
		strcat_s(senemies, senemies2);
		text.setString(senemies);
		text.setPosition(view.getCenter().x - 630, view.getCenter().y + 375);
		window.draw(text);


		for (list_i = enemies.begin(); list_i != enemies.end(); list_i++){
			window.draw((*list_i)->sprite);
		}

		for (listp_i = platforms.begin(); listp_i != platforms.end(); listp_i++){
			window.draw((*listp_i)->sprite);
		}

		for (listb_i = bullets.begin(); listb_i != bullets.end(); listb_i++){
			window.draw((*listb_i)->sprite);
		}

		//текст концовки
		/////////////////для концовки
		if ((levelNumber > kolLevel) && (totalscore < 15)){
			p.life = false;
			char stroka[35] = "МОГЛИ НАБРАТЬ \n   И ПОБОЛЬШЕ!!!";
			text2.setString(stroka);
			text2.setPosition(view.getCenter().x - 350, view.getCenter().y - 140);
			window.draw(text2);
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) { gameState = inmenu;/*menu(window);*/ return false; }
		if (Keyboard::isKeyPressed(Keyboard::RShift)) { /*hp = p.health;*/ rshift = true; return true; }
		if(p.life) p.update(time);
		//рисую змейку и отображаю всё
		window.draw(p.sprite);

		for (int i = 0; i < map_height; i++)
		for (int j = 0; j < map_width; j++){
			if ((Map[i][j] == '9')) {
				s_map2.setScale(0.2353, 0.2353);
				s_map2.setTextureRect(IntRect(402, 405, 170, 170)); s_map2.setPosition(j * 40, i * 40);
				window.draw(s_map2);
			}
		}

		//window.draw(mainBg);
		if (levelNumber > kolLevel) window.draw(santaSprite);
		window.display(); 
	}
	return false;
}

void setValues(){
	std::ifstream fin;
	fin.open("files/score.txt", std::ios::in);
	fin >> hp;
	fin >> totalscore;
	fin >> levelNumber;
	fin >> gcolx;
	fin >> gcoly;
	fin.close();
	//тут из файла собираем значения
}

void saveValues(){
	std::ofstream fout;
	fout.open("files/score.txt", std::ios::out | std::ios::trunc);
	fout << hp << ' ';
	fout << totalscore << ' ';
	fout << levelNumber<< ' ' << gcolx << ' ' << gcoly;
	fout.close();
	//тут из файла собираем значения
}

void play(RenderWindow & window, int &levelNumber){
	while (window.isOpen()){
		switch (gameState){
			case inmenu: 
				menu(window); 
				break;
			
			case inrest: 
				levelNumber = 1;
				totalscore = 0;
				hp = 10;
				gameState = ingame;
				break;

			case ingame: 
				if (levelNumber > kolLevel) { 
					Music specMusic;
					specMusic.openFromFile("files/specMusic.ogg");
					if (totalscore >= 15) {
						specMusic.play();
						maintheme.setVolume(20);
					}
					Game(window, levelNumber); 
					if (totalscore >= 15){
						specMusic.stop();
						maintheme.setVolume(100);
					}
					gameState = inmenu;
				}
				else if (Game(window, levelNumber)){
					if (!rshift) { 
						levelNumber++;
					}
					if (levelNumber > kolLevel) {  
						Music specMusic;
						if (totalscore >= 15) {
							maintheme.setVolume(20);
							specMusic.openFromFile("files/specMusic.ogg");
							specMusic.play();
						}
						Game(window, levelNumber);  
						if (totalscore >= 15) {
							specMusic.stop();
							maintheme.setVolume(100);
						}
						gameState = inmenu;
					}
					else play(window, levelNumber);
				}
				break;

			case inend: 
				saveValues();
				window.close();
				break;
			}
	}
}
	
	
