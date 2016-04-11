#include "Card.h"

Card::Card()
{
}

Card::Card(string name, int cost, int victory_points, int value, string type)
{
	this->name = name;
	this->cost = cost;
	this->victory_points = victory_points;
	this->value = value;
	this->type = type;
	this->played_yn = false;

	if (name == "Smithy") draw = 3;
	else if (name == "Village") draw = 1;
	else if (name == "Laboratory") draw = 2;
	else draw = 0;

	if (name == "Village") actions = 2;
	else if (name == "Laboratory") actions = 1;
	else actions = 0;

}
void Card::PlayCard(bool yn)
{
	if (yn == true) played_yn = true;
	else played_yn = false;
}

bool Card::Played_YN()
{
	return played_yn;
}

int Card::GetDraw()
{
	return draw;
}

string Card::GetName()
{
	return name;
}

string Card::GetType()
{
	return type;
}

int Card::GetValue()
{
	return value;
}

int Card::GetCost()
{
	return cost;
}

int Card::GetVictory()
{
	return victory_points;
}

int Card::GetActions()
{
	return actions;
}
