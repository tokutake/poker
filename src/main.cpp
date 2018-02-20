//
//  main.cpp
//  hoge
//
//  Created by Hokuto Tokutake on 07/02/2018.
//  Copyright © 2018 Hokuto Tokutake. All rights reserved.
//

#include <stdio.h>
#include <vector>

enum Suit {
    Spade,
    Clover,
    Heart,
    Dia,
};

enum Action {
    NONE,
    FOLD,
    CHECK,
    BET,
    RAISE,
};

enum Stage {
    PRE_FLOP,
    FLOP,
    RIVER,
    TURN,
};

class Hand {
public:
    enum Kind {
        HIGH_CARD,
        ONE_PAIR,
        TWO_PAIR,
        THREE_OF_A_KIND,
        STRAIGHT,
        FLASH,
        FULL_HOUSE,
        FOUR_OF_A_KIND,
        STRAIGHT_FLASH,
        ROYAL_STRAIGHT_FLASH,
    };
    
    Kind kind;
    std::vector<int> ranks;
};

class Card {
public:
    int rank;
    Suit suit;
};

class Player {
public:
    Player()
    : stock{0}
    , amountOfBet{0}
    , hasDealButton{false}
    , action{NONE}
    {
    }
    std::vector<Card> holdCards;
    int stock;
    int amountOfBet;
    bool hasDealButton;
    Action action;
};

int myrand(int i) {
    return std::rand() %i;
}

class Deck {
public:
    Deck() {
        shuffle();
    }

    void shuffle() {
        cards.clear();
        for (int i = 1; i <= 13; i++) {
            for (int j = 0; j < 4; j++) {
                Card card;
                card.rank = i;
                card.suit = static_cast<Suit>(j);
                cards.push_back(card);
            }
        }
        std::random_shuffle(cards.begin(), cards.end(), myrand);
    }
    
    Card draw() {
        Card card = cards.back();
        cards.pop_back();
        return card;
    }
    
    Card draw(Card aCard) {
        for (auto card = cards.begin(); card < cards.end(); card++) {
            if (card->rank == aCard.rank && card->suit == aCard.suit) {
                cards.erase(card);
                return aCard;
            }
        }
        return aCard;
    }
    
    std::vector<Card> cards;
};

class Game {
public:
    Game()
    : stage{PRE_FLOP}
    , pot{0}
    {
    }

    void initializePlayers(int numberOfPlayer) {
        for (int i = 0; i < numberOfPlayer; i++) {
            players.push_back(new Player());
        }
    }
    
    void shuffle() {
        deck.shuffle();
    }
    
    void showdown() {
        
    }
    
    int compareHand(Player *p1, Player *p2) {
        std::vector<Card> pCards[2];
        for (auto card : p1->holdCards) {
            pCards[0].push_back(card);
        }
        for (auto card : p2->holdCards) {
            pCards[1].push_back(card);
        }
        for (auto card : communityCards) {
            for (auto pCards : pCards) {
                pCards.push_back(card);
            }
        }
        return 0;
    }
    
    void dealCards() {
        for (auto player : players) {
            player->holdCards.clear();
        }
        for (int i = 0; i < 2; i++) {
            for (auto player : players) {
                player->holdCards.push_back(deck.draw());
            }
        }
    }

    Deck deck;
    std::vector<Player*> players;
    std::vector<Card> communityCards;
    Stage stage;
    int pot;
};

bool isFourCard(std::vector<Card> cards)
{

    for (int i = 1; i <= 13; i++) {
        int numberOfSameCard = 0;
        for (auto card : cards) {
            if (card.rank == i) {
                numberOfSameCard++;
            }
            if (numberOfSameCard == 4) {
                return true;
            }
        }
    }
    return false;
}

bool isOnePair(std::vector<Card> cards)
{
    for (int i = 1; i <= 13; i++) {
        int numberOfSameCard = 0;
        for (auto card : cards) {
            if (card.rank == i) {
                numberOfSameCard++;
            }
            if (numberOfSameCard >= 2) {
                return true;
            }
        }
    }
    return false;
}

bool isTwoPair(std::vector<Card> cards)
{
    int numberOfPair = 0;
    for (int i = 1; i <= 13; i++) {
        int numberOfSameCard = 0;
        for (auto card : cards) {
            if (card.rank == i) {
                numberOfSameCard++;
            }
        }
        if (numberOfSameCard >= 2) {
            numberOfPair++;
        }
    }
    if (numberOfPair >= 2) {
        return true;
    }
    return false;
}

bool isStraight(std::vector<Card> cards)
{
    for (int i = 1; i <= 10; i++) {
        for (int j = i; j < i + 5; j++) {
            int rank = j;
            if (rank == 14) {
                rank = 1;
            }
            bool hasRank = false;
            for (auto card : cards) {
                if (card.rank == rank) {
                    hasRank = true;
                }
            }
            if (!hasRank) {
                break;
            }
            if (j - i == 4) {
                return true;
            }
        }
    }
    return false;
}

bool isFlash(std::vector<Card> cards)
{
    for (int i = 0; i < 4; i++) {
        int numberOfSameSuit = 0;
        for (auto card : cards) {
            if (card.suit == static_cast<Suit>(i)) {
                numberOfSameSuit++;
            }
        }
        if (numberOfSameSuit >= 5) {
            return true;
        }
    }
    return false;
}

void printFlashDrawPercentage()
{
    int numberOfHands = 10000;
    int numberOfFlash = 0;
    for (int i = 0; i < numberOfHands; i++) {
        Deck deck;
        std::vector<Card> cards;
        cards.push_back(deck.draw({1, Dia}));
        cards.push_back(deck.draw({2, Dia}));
        cards.push_back(deck.draw({3, Dia}));
        cards.push_back(deck.draw({4, Dia}));
        cards.push_back(deck.draw({4, Spade}));
        for (int j = 0; j < 2; j++) {
            cards.push_back(deck.draw());
        }
        if (isFlash(cards)) {
            numberOfFlash++;
        }
    }
    printf("percentage of flash draw:%d%%\n", numberOfFlash * 100 / numberOfHands);
}

void printStraightDrawPercentage()
{
    int numberOfHands = 10000;
    int numberOfStraight = 0;
    for (int i = 0; i < numberOfHands; i++) {
        Deck deck;
        std::vector<Card> cards;
        cards.push_back(deck.draw({2, Dia}));
        cards.push_back(deck.draw({3, Dia}));
        cards.push_back(deck.draw({4, Dia}));
        cards.push_back(deck.draw({4, Spade}));
        cards.push_back(deck.draw({5, Spade}));
        for (int j = 0; j < 2; j++) {
            cards.push_back(deck.draw());
        }
        if (isStraight(cards)) {
            numberOfStraight++;
        }
    }
    printf("percentage of straight:%d%%\n", numberOfStraight * 100 / numberOfHands);
}


int main(int argc, const char * argv[]) {
    srand(static_cast<unsigned int>(time(nullptr)));
    int numberOfFourCards = 0;
    int numberOfFlash = 0;
    int numberOfOnePair = 0;
    int numberOfTwoPair = 0;
    int numberOfStraight = 0;
    int numberOfHands = 10000;
    for (int i = 0; i < numberOfHands; i++) {
        Deck deck;
        std::vector<Card> cards;
        for (int j = 0; j < 7; j++) {
            cards.push_back(deck.draw());
        }
        if (isFourCard(cards)) {
            numberOfFourCards++;
        }
        if (isFlash(cards)) {
            numberOfFlash++;
        }
        if (isOnePair(cards)) {
            numberOfOnePair++;
        }
        if (isTwoPair(cards)) {
            numberOfTwoPair++;
        }
        if (isStraight(cards)) {
            numberOfStraight++;
        }
    }
    printf("four of a kind:%d%%\n", numberOfFourCards * 100 / numberOfHands);
    printf("flash:%d%%\n", numberOfFlash * 100 / numberOfHands);
    printf("one pair:%d%%\n", numberOfOnePair * 100 / numberOfHands);
    printf("two pari:%d%%\n", numberOfTwoPair * 100 / numberOfHands);
    printf("straight:%d%%\n", numberOfStraight * 100 / numberOfHands);
    printFlashDrawPercentage();
    printStraightDrawPercentage();
    return 0;
}

