#ifndef WALL_H
#define WALL_H

#include "Building.h"

class Wall : public Building
{
public:
	Wall(std::string _name, int _team, int _type);
	~Wall();

private:
	int hp = 120;
	float bT = 2.3f;
	int c = 80;

	void onInit();
	void onHit();
	void onDeath();
};

#endif // !WALL_H

