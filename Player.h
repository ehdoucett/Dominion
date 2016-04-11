#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <random>
#include <iostream>
#include <time.h>
#include "Card.h"
#include <map>

using namespace std; 

class Player
{
public:
	Player(vector<Card> deck, vector<Card> hand, vector<Card> discard, int order);
	vector<Card> GetDeck();
	vector<Card> GetHand();
	vector<Card> GetDiscard();
	void Shuffle();
	void DrawHand();
	void Turn(map<string, Card> &kingdom, map<string, int> &size_kingdom);
	void Buy(map<string, Card> &kingdom, map<string, int> &size_kingdom);
	void Cleanup();
	void Action(int card_to_play);
	void DrawCard(int draw);
	void SetStrategy(string strat);
	string BuyLogic(map<string, Card> &kingdom, map<string, int> &size_kingdom);
private:
	vector<Card> deck;
	vector<Card> hand;
	vector<Card> discard;
	int order;
	int actions;
	int treasures;
	int buys;

	int smithy_buy;
	int village_buy;
	int laboratory_buy;
};

#endif //PLAYER_H