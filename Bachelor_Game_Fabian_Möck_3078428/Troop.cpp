#include "Troop.h"

ITroop::ITroop(std::string _name, int _team, int _type) : IUnit(_name, _team, _type)
{
}

ITroop::~ITroop()
{
}