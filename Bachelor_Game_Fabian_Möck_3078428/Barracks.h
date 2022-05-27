#ifndef BARRACKS_H
#define BARRACKS_H

#include "Building.h"

class Barracks : public Building
{
public:
	Barracks(std::string _name, int _team, int _type);
	~Barracks();

private:
	int hp = 30;
	float bT = 5.5f;

	void onInit();
	void onHit();
	void onDeath();
};

#endif // !BARRACKS_H

