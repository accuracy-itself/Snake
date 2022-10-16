#include <SFML/Graphics.hpp>
#include <fstream>

std::ifstream fin;
sf::String Map[60];
int map_height;
int map_width;
void createLevel(int level){
	switch (level)
	{
	case 1: fin.open("files/level 1.txt", std::ios::in); break;
	case 2: fin.open("files/level 2.txt", std::ios::in); break;
	case 3: fin.open("files/level 3.txt", std::ios::in); break;
	default: fin.open("files/level 3.txt", std::ios::in); break;//тут должен быть последний уровень!
	break;
	}

	char ss[181];
	int k = -1, l;
	while (fin){
		fin.getline(ss, 181, '\n');
		if (strlen(ss) != 0) l = strlen(ss);
		
		k++;
		Map[k] = ss;
	}
	map_width = l;
	map_height = k;
	fin.close();
}

