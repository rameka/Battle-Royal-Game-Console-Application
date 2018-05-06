#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <list>
#include <algorithm>
using namespace std;

ofstream out("data1.txt");

class card
{
public:
	int number;
	int suit;
	static int uniquenumber;
	card();
	void showcard();
	card * next;
};


class collection
{
public:
	card * start;
	int numberofcards;
	collection(int x);
	collection() { start = nullptr; numberofcards = 0; }
	void showcollection();
	void shuffle();
	card deal();
	void add(card a);
};
card::card()
{
	number = uniquenumber % 13;
	suit = (uniquenumber / 13);
	uniquenumber++;
	next = nullptr;
}

void card::showcard()
{

	switch (number)
	{
	case 0: out << "A"; break;  // Ace is low
	case 10: out << "J"; break;
	case 11: out << "Q"; break;
	case 12: out << "K"; break;
	default: out << (number + 1); break;
	}

	switch (suit) {
	case 0: out << "S "; break;
	case 1: out << "H "; break;
	case 2: out << "D "; break;
	case 3: out << "C "; break;
	}
}

collection::collection(int x)
{

	start = nullptr;

	numberofcards = 0;
	for (int i = 0; i<x; i++)
	{
		card temp;
		add(temp);
	}
}
void collection::showcollection()
{
	int i;
	card * temp;
	temp = start;
	for (i = 0; i<numberofcards; i++)
	{
		temp->showcard();
		temp = temp->next;
	}

}


void collection::add(card a)
{
	/*
	Add a card to the front of the linked list.
	*/
	card *node = new card();
	node->number = a.number;
	node->suit = a.suit;
	node->next = start;
	node->uniquenumber -= 1;
	start = node;
	numberofcards = numberofcards + 1;
}

void collection::shuffle()
{
	/*
	Randomize the sequence of cards in the deck.
	You can use my shuffle
	*/
	card *temp2, *temp3;
	temp2 = start;
	temp3 = temp2;
	int i, j, k;
	char c1;
	int num;
	for (i = numberofcards; i>1; i--) {
		j = rand() % i;
		for (k = 0; k <= j - 1; k++)
			temp3 = temp3->next;
		c1 = temp2->suit;
		num = temp2->number;
		temp2->suit = temp3->suit;
		temp2->number = temp3->number;
		temp3->suit = c1;
		temp3->number = num;
		temp2 = temp2->next;
		temp3 = temp2;
	}

	// Reorganize cards in array randomly
}
card collection::deal()
{
	// Remove a card from the front of the linked list, and return the card
	card * temp = new card();
	card *del = start;
	(*temp).number = del->number;
	(*temp).suit = del->suit;
	start = start->next;
	numberofcards = numberofcards - 1;
	delete del;
	return *temp;
}



int card::uniquenumber = 0;

void deal_cards(int k, int d, collection &deck, collection *hand);
void battle(int k, collection *hand, collection *table);
void Enter_Num_players_Dealer(int &k_players, int &dealer);
void Show_Initial_Deck_Hands(int &k_players, int &dealer, collection &deck, collection * hand, collection * table);
void showHand(int numplayers, collection * hands);
void showTable(int numplayers, collection * tables);

int main() {

	collection *hand, *table;
	int num_players, dealer, i, winner = 0;
	srand(time(0));
	Enter_Num_players_Dealer(num_players, dealer);
	collection deck(52);
	hand = new collection[num_players];
	table = new collection[num_players];
	Show_Initial_Deck_Hands(num_players, dealer, deck, hand, table);
	battle(num_players, hand, table);
	return 0;
}

void Show_Initial_Deck_Hands(int &k_players, int &dealer, collection &deck, collection * hand, collection * table) {
	out << "\n\n*** initial deck of cards before shuffle *\n\n";
	deck.showcollection();
	deck.shuffle(); // Shuffle cards in deck
	out << "\n\n*** after shuffle ***\n\n";
	deck.showcollection();
	out << "\n\n*** after cards are dealt ***\n\n";
	deal_cards(k_players, dealer, deck, hand);
	for (int i = 0; i< k_players; i++)
	{
		out << "\ncards for player " << i + 1 << "\n";
		hand[i].showcollection();
	}
}
void Enter_Num_players_Dealer(int &num_players, int &dealer) {

	do {
		cout << "Enter the number (2-10) of players ";
		cin >> num_players;
		if (num_players > 10 || num_players < 2)
			out << "Illegal player number. Enter again\n\n";
	} while (num_players >10 || num_players < 2);

	do {
		cout << "Enter initial dealer (1-k)";
		cin >> dealer;
		if (dealer < 1 || dealer > num_players)
			out << "Illegal dealer ID. Enter again\n\n";
	} while (dealer < 1 || dealer > num_players);
}


void battle(int k, collection *hand, collection *table) { //k is num_players
														  /*
														  function battle is the heart of the program.  It implements the card battle operaitons and decides outcomes of card battel according to rules.
														  */
	int winningIndex = -1;//lastly assigning the winning index
	
	// This loop will run until the winner is found
	while (true) {
		int minimumNumber = INT_MAX;
		int roundWinner = INT_MIN;
		bool oneWinner = false;
		showHand(k, hand);
		//Each player play's one card and finding the minimum number
		for (int i = 0; i < k; i++) {
			if (hand[i].numberofcards != 0) {
				minimumNumber = std::min(minimumNumber, hand[i].start->number);
				table[i].add(hand[i].deal());
			}
		}

		showTable(k, table);
		//Finding number of players having the same card value
		list <int> tiedPlayers;
		for (int i = 0; i < k; i++) {
			if (table[i].numberofcards != 0 && table[i].start->number == minimumNumber) {
				tiedPlayers.push_back(i);
			}
		}
		if (tiedPlayers.size() == 1) {
			//Only one winner in the round
			roundWinner = tiedPlayers.front();
			oneWinner = true;
		}
		else {
			//More than one winner. So, engaing the tied players to series of tied battles as per rules
			oneWinner = false;
			int playerIndex = -1;
			list<int> newTiedPlayers;
			while (!oneWinner)
			{
				minimumNumber = INT_MAX;
				showHand(k, hand);
				list <int> ::iterator it;
				for (it = tiedPlayers.begin(); it != tiedPlayers.end(); ++it) {
					playerIndex = *it;
					if (hand[playerIndex].numberofcards >= 2) {
						//Dropping two cards and finding the minimum number of the cards dropped by all players
						table[playerIndex].add(hand[playerIndex].deal());
						minimumNumber = std::min(minimumNumber, hand[playerIndex].start->number);
						table[playerIndex].add(hand[playerIndex].deal());
					}
				}
				showTable(k, table);
				newTiedPlayers.clear();
				for (it = tiedPlayers.begin(); it != tiedPlayers.end(); ++it) {
					playerIndex = *it;
					if (table[playerIndex].start->number == minimumNumber) {
						newTiedPlayers.push_back(playerIndex);
					}
				}
				if (newTiedPlayers.size() == 1) {
					roundWinner = newTiedPlayers.front();
					oneWinner = true;
				}
				else
				{
					tiedPlayers.clear();
					tiedPlayers = newTiedPlayers;
				}
			}
		}
		// Adding the cards to winner logic
		for (int i = 0; i < k; i++) {

			// Adding the table cards to the round winner
			while (table[i].numberofcards > 0) {
				hand[roundWinner].add(table[i].deal());
			}

			//Ignoring the round winner cards for addition
			if (i == roundWinner) continue;
			//Adding the cards of the players if they loose to the round winner
			if (hand[i].numberofcards < 5 && hand[i].numberofcards>0) {
				while (hand[i].numberofcards > 0) {
					hand[roundWinner].add(hand[i].deal());
				}
			}
		}

		//Shuffling the cards that the player has won
		hand[roundWinner].shuffle();

		//Checking number of players left after each round
		int count = 0;
		for (int i = 0; i < k; i++) {
			if (hand[i].numberofcards != 0)
			{
				count++;
				winningIndex = i;
			}
		}
		if (count == 1) {
			out << "Game Over!! The winner is player: " << winningIndex + 1 << endl;
			for (int i = 0; i < k; i++) {
				out << "\Final Hand :" << i + 1 << "\n";
				hand[i].showcollection();
				out << endl;
			}
			break; //Ending the while loop when the item is found
		}
	}//End of While loop

}// End of battle function

 //Displaying hand cards
void showHand(int numplayers, collection * hands) {
	out << endl;
	for (int i = 0; i < numplayers; i++) {
		out << " Hand " << i + 1 << endl;
		hands[i].showcollection();
		out << endl;
	}

}
// Displaying table cards
void showTable(int numplayers, collection * tables) {
	out << endl;
	for (int i = 0; i < numplayers; i++) {
		out << " Table " << i + 1 << endl;
		tables[i].showcollection();
		out << endl;
	}
}

void deal_cards(int k, int d, collection &deck, collection *hand) {
	//deal cards to each player
	//Your code

	int val = deck.numberofcards;
	int index = (d%k);
	while (val != 0) {
		hand[index++].add(deck.deal());
		index = index % k;
		val--;
	}
}