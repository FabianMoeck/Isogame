#ifndef FACTORY_H
#define FACTORY_H

#include "Building.h"

class Factory : public Building
{
public:
	Factory(std::string _name, int _team, int _type);
	~Factory();

private:
	int hp = 200;
	float bT = 7.0f;
	int c = 500;

	void onInit();
	void onHit();
	void onDeath();

};

#endif // !FACTORY_H

