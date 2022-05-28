#ifndef BARRACKS_H
#define BARRACKS_H

#include "Building.h"

class Barracks : public Building
{
public:
	Barracks(std::string _name, int _team, int _type);
	~Barracks();

private:
	int hp = 180;
	float bT = 5.5f;
	int c = 300;

	void onInit();
	void onHit();
	void onDeath();
};

#endif // !BARRACKS_H

