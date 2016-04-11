#include <vector>
#include <iostream>
#include <string>
#include <map>
#include "Card.h"
#include "Player.h"
#include <fstream>
#include <sstream>

using namespace std;

// Initialize Globals
map<string, Card> kingdom;
map<string, int> size_kingdom;

// Build Kingdom Cards
void BuildKingdom()
{
	// The basics
	Card* estate = new Card("Estate", 2, 1, 0, "Victory");
	Card* copper = new Card("Copper", 0, 0, 1, "Treasure");
	Card* silver = new Card("Silver", 3, 0, 2, "Treasure");
	Card* gold = new Card("Gold", 6, 0, 3, "Treasure");
	Card* duchy = new Card("Duchy", 5, 3, 0, "Victory");
	Card* provence = new Card("Provence", 8, 6, 0, "Treasure");

	// Other kingdom cards
	Card* smithy = new Card("Smithy", 4, 0, 0, "Action");
	Card* village = new Card("Village", 3, 0, 0, "Action");
	Card* lab = new Card("Laboratory", 5, 0, 0, "Action");

	// Kingdom Array to hold all cards
	kingdom["Estate"] = *estate;
	kingdom["Copper"] = *copper;
	kingdom["Silver"] = *silver;
	kingdom["Gold"] = *gold;
	kingdom["Duchy"] = *duchy;
	kingdom["Provence"] = *provence;

	// Size Kingdom Array holds amounts in each pile
	size_kingdom["Estate"] = 5;
	size_kingdom["Copper"] = 30;
	size_kingdom["Silver"] = 30;
	size_kingdom["Gold"] = 30;
	size_kingdom["Duchy"] = 5;
	size_kingdom["Provence"] = 5;

	// Additional cards
	kingdom["Smithy"] = *smithy;
	size_kingdom["Smithy"] = 10;
	kingdom["Village"] = *village;
	size_kingdom["Village"] = 10;
	kingdom["Laboratory"] = *lab;
	size_kingdom["Laboratory"] = 10;
}

// Read in file containing amounts of each card to buy
// currently file is three numbers separated by commas
// first value is number of villages, 
// second is number of smithys,
// third is number of labs
vector<string> getStrategy() {

	vector<string> strategy;
	ifstream infile; //input file stream object
	infile.open("strategy.txt");

	char input[10];
	char c[10];
	while (infile) {
		infile.getline(input, 10, '\n');
		if (input[0] == '\0') break;
		istringstream ss(input);
		ss.getline(c, 10);
		strategy.push_back(c);
	}
	return strategy;

	infile.close();
}

int main() {
	
	Card* estate = new Card("Estate", 2, 1, 0, "Victory");
	Card* copper = new Card("Copper", 0, 0, 1, "Treasure");
	
	
	int iterations = 10; // holds the number of times to play a game for each strategy 
	int strategies = 100;   // holds the number of strategies in file
	
	double output[100];

	vector<Card> deck;
	vector<Card> hand;
	vector<Card> discard;

	// build starting deck of 7 copper, 3 estate
	for (int i = 0; i < 10; i++) {
		if (i < 7) discard.push_back(*copper);
		else discard.push_back(*estate);
	}

	vector<string> strategy = getStrategy();
	for (int j = 0; j < strategies; j++) {
		double sum = 0.0;
		for (int i = 1; i <= iterations; i++) {

			Player* player1 = new Player(deck, hand, discard, 1); // start with one player, can add more here

			BuildKingdom();

			player1->Shuffle();					// start every game by shuffling the deck
			player1->DrawHand();				// drawing the first hand
			player1->SetStrategy(strategy[j]);  // grab strategy

			bool end_game = false;
			int counter = 0;
			do
			{
				// turns are player based - will have to update for multi-player
				counter++;
				player1->Turn(kingdom, size_kingdom);					// turn phase
				player1->Cleanup();										// cleanup phase		
				player1->DrawHand();									// draw a new hand
				if (size_kingdom["Provence"] == 0) end_game = true;		// check game-end conditions
			} while (end_game == false);
			sum += counter;
		}

		output[j] = sum / double(iterations);

		cout << "..." << j+1;
	}

	ofstream outfile;
	outfile.open("output.txt");

	// print results to output.txt
	for (int i = 0; i < strategies; i++) {
		outfile << output[i] << endl;
	}

	outfile.close();
	
	cout << endl;

	return 0;
}