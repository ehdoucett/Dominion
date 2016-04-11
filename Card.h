#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>

using namespace std;

class Card
{
	public:
		Card();
		Card(string name, int cost, int victory_points, int value, string type);
		string GetName();
		string GetType();
		int GetValue();
		int GetCost();
		int GetVictory();
		int GetDraw();
		int GetActions();
		void PlayCard(bool yn);
		bool Played_YN();
	private:
		string name;
		int cost;
		int victory_points;
		int value;
		string type;
		int draw;
		int actions;
		bool played_yn;
};

#endif //CARD_H
