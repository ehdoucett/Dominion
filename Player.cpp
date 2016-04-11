#include "Player.h"

/*****************************************************************************************
* Player private variables per turn include
*  - actions - start with 1
*  - treasures - start with 0
*  - buys - start with 1
*
* Order of operations:
*  - player finds actions in hand
*  - orders action by priority (village = 1, lab = 1, smithy = 2)
*  - plays all priority 1 actions, followed by priority 2 and so on
*  - player counts treasures in hand
*  - buy sequence initiated (only one buy assumed for now)
*****************************************************************************************/

Player::Player(vector<Card> deck, vector<Card> hand, vector<Card> discard, int order)
{
	this->deck = deck;
	this->hand = hand;
	this->discard = discard;
	this->order = order;
}

vector<Card> Player::GetDeck()
{
	return deck;
}

vector<Card> Player::GetHand()
{
	return hand;
}

vector<Card> Player::GetDiscard()
{
	return discard;
}

// Shuffle method to shuffle arbitrary number of cards
void Player::Shuffle() {
	srand(time(NULL)); // initialize random seed
	int size = discard.size();
	vector<Card> new_deck;
	// Loop to move cards from discard pile into new deck
	if (size > 0) {
		do {
			int get_card = rand() % size;
			new_deck.push_back(discard[get_card]);
			discard.erase(discard.begin() + get_card);
			size--;
		} while (size > 0);

		for (int i = 0; i < new_deck.size(); i++) {
			deck.push_back(new_deck[i]);
		}
	}
}


// Draws amount of cards from argument - 5 for new hand, 3 for smithy, 2 for lab, etc.
void Player::DrawCard(int draw)
{
	if (deck.size() < draw) Shuffle();
	if (deck.size() < draw) draw = deck.size();

	// build hand
	for (int i = 0; i < draw; i++) {
		hand.push_back(deck[i]);
	}
	// remove from deck
	deck.erase(deck.begin(), deck.begin() + draw);
}

void Player::DrawHand()
{
	DrawCard(5);

	// set private variables for start of turn
	actions = 1;
	treasures = 0;
	buys = 1;
}

void Player::Turn(map<string, Card> &kingdom, map<string, int> &size_kingdom)
{
	bool canplay = true;

	do {
		// continue action sequence as long as you have action cards to play and have actions to play them
		// check for action to play
		int card_to_play = -1;
		for (int i = 0; i < hand.size(); i++) {
			string type = hand[i].GetType();
			bool played = hand[i].Played_YN(); // keeps track of whether or not the card has been played
			if (type == "Action" && played == false) {
				/******************************************************************
				* this is the main area where the decision is made to play a card *
				*	this will get very large when working in other types of cards *
 				*   if nothing is done to improve this.                           *
				*******************************************************************/
				string name = hand[i].GetName();
				if (name == "Village") card_to_play = i;
				if (name == "Laboratory" && card_to_play == -1) card_to_play = i; // if no village play lab
				if (name == "Smithy" && card_to_play == -1) card_to_play = i;     // if nothing else, play smithy

			}
		}
		// play action
		if (card_to_play != -1) Action(card_to_play);

		// check for conditions to allow you to play another action
		if (actions == 0) canplay = false;
		if (card_to_play == -1) canplay = false;
	
	} while (canplay);

	// go to buy phase
	Buy(kingdom, size_kingdom);
}

void Player::Action(int card_to_play)
{
	// get number of cards to draw
	int draw = hand[card_to_play].GetDraw();

	// get number of actions to add
	int act = hand[card_to_play].GetActions();
	
	// mark card as played
	hand[card_to_play].PlayCard(1);

	// deduct an action from available actions
	actions--;

	DrawCard(draw);	// all cards have draw = 0 by default - safe to call here with no logic
	actions += act;
}

/****************************************************************************
* The BuyLogic method counts the number of treasures in hand				*
*	then checks the number of cards available to buy						*
*	then buys according to deterministic logic - another area to improve	*
*   for when additional cards are added										*
*****************************************************************************/

string Player::BuyLogic(map<string, Card> &kingdom, map<string, int> &size_kingdom)
{
	// Get treasures
	for (int i = 0; i < hand.size(); i++) {
		string type = hand[i].GetType();
		if (type == "Treasure") treasures += hand[i].GetValue();
	}

	// check amount of cards available - strategy depends on what's been bought already
	// will need another solution when adding more cards - probably a player private variable
	int num_smithy = 10 - size_kingdom["Smithy"];
	int num_village = 10 - size_kingdom["Village"];
	int num_laboratory = 10 - size_kingdom["Laboratory"];
	int num_prov = size_kingdom["Provence"];
	int num_gold = size_kingdom["Gold"];
	int num_silver = size_kingdom["Silver"];

	// actual buying logic
	if (treasures >= 8 && num_prov > 0) return "Provence";								// always buy Provence if you can
	else if (treasures >= 6 && num_gold > 0) return "Gold";								// otherwise get a gold if enough
	else if (treasures >= 5 && num_laboratory < laboratory_buy) return "Laboratory";	// else get according to strategy
	else if (treasures >= 3) {
		if (num_village < village_buy) return "Village";
		else if (treasures > 3 && num_smithy < smithy_buy) return "Smithy";
		else return "Silver";															// silver is default if nothing else
	}
	
	// can't afford anything
	return "Nothing";

}

void Player::Buy(map<string, Card> &kingdom, map<string, int> &size_kingdom)
{
	// method that actually buys the card returned by BuyLogic and adds to discard
	string cardtobuy = BuyLogic(kingdom, size_kingdom);

	if (cardtobuy != "Nothing"){
		discard.push_back(kingdom[cardtobuy]);
		size_kingdom[cardtobuy]--;
	}
	buys--;
}

void Player::Cleanup()
{
	for (int i = 0; i < hand.size(); i++) {
		hand[i].PlayCard(0); // make all cards unplayed
		discard.push_back(hand[i]);
	}
	hand.erase(hand.begin(), hand.end());


}

// converts ascii character to actual number
void Player::SetStrategy(string strat)
{
	village_buy = strat[0] - 48;
	smithy_buy = strat[2] - 48;
	laboratory_buy = strat[4] - 48;
}
