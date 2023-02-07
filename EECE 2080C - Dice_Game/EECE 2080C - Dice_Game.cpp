#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <stdlib.h>
#include <string>
#define MAX_PLAYERS 10
using namespace std;

class Dice {
private:
    int numSides;
public:
    Dice(int numSides) {
        this->numSides = numSides;
        srand((unsigned)time(0));
    }

    int roll() {

        int a = rand() % numSides + 1;
        return a;
    }
};

class Player {
private:
    string name;
    int score;
public:// Getter : Player Name
    Player(string Name, int Score) {   //Constructor
        this->name = Name;
        this->score = Score;

    }
    // Getter : Name
    string getName() {
        return name;

    }
    // Getter : Score
    int getScore() {
        return score;
    }

    void addToScore(int val) {
        score = score + val;;


    }
};

class DiceGame {
private:
    void getNameAndScore(string line, char delimiter, string& name1, int& score1) {//script for getting the name and score from user
        int delimiterIdx = -1;
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == delimiter) {
                delimiterIdx = i;
            }
        }
        name1 = line.substr(0, delimiterIdx);
        score1 = atoi(line.substr(delimiterIdx + 1, delimiterIdx + 2).c_str());
    }

protected:
    Player* players[MAX_PLAYERS];
    Dice* dice;
    int numPlayers, numDice;

public:
    //Pure virtual function
    virtual void play() {};

    void initPlayers() {
        string Name;
        int score = 0;
        int i = 0;
        while (i < numPlayers) {
            cout << "Player's Name? " << endl;
            cin >> Name;
            score = 0;
            players[i] = new Player(Name, score);
            i = i + 1;
        }
    }

    void displayScores() {//to display score
        cout << "\nDisplay Scores:\n";
        for (int i = 0; i < numPlayers; i++)
        {
            cout << players[i]->getName() << ":" << players[i]->getScore() << endl;
        }
    }

    void writeScoresToFile(char* fileName) {
        ofstream textFile(fileName, ios::app);
        if (textFile.is_open())
        {
            for (int i = 0; i < numPlayers; i++)
            {
                textFile << players[i]->getName() << ":" << players[i]->getScore() << endl;
            }
            textFile.close();
        }
        else
        {
            cout << "Unable to open the text file!";
        }
    }

    void findHighestScore(char* fileName) {
        string line, name, maxName;
        int score, maxScore = 0;
        ifstream textFile(fileName);

        if (textFile.is_open())
        {
            while (getline(textFile, line))
            {
                score = 0;
                getNameAndScore(line, ':', name, score);
                if (score > maxScore) {
                    maxScore = score;
                    maxName = name;
                }
            }
            textFile.close();
            cout << "Player with the highest score:\n" << maxName << "\t" << maxScore;
        }
        else {
            cout << "Unable to read from ip file";
        }
    }
};







class KnockOut : public DiceGame {
private:
    int Knock_Out_Score = 0;
    int remainingPlayers = 0;
    int sr = 0;
public:

    KnockOut(int numSides, int numPlayers, int numDice) {
        dice = new Dice(numSides);
        this->numDice = numDice;
        this->numPlayers = numPlayers;
        for (int i = 0; i < numDice; i++) {
            Knock_Out_Score += dice->roll();
        }
    }

    void play()
    {
        int Roll_Sum, Remaining_Players;

        Player* Original_Players[10];
        for (int i = 0; i < numPlayers; i++)
        {
            Original_Players[i] = new Player(players[i]->getName(), 0);

        }

        Remaining_Players = numPlayers;

        while (Remaining_Players > 0)
        {

            for (int i = 0; i < numPlayers; i++)
            {
                Roll_Sum = 0;
                for (int j = 0; j < numDice; j++)
                {
                    Roll_Sum += dice->roll();
                    cout << Knock_Out_Score << "\n";
                    cout << Roll_Sum << "\n";
                }
                Original_Players[i]->addToScore(Roll_Sum);
                if (Roll_Sum == Knock_Out_Score)
                {
                    cout << Original_Players[i]->getName() << " is knocked out";
                    Remaining_Players--;
                    Original_Players[i]->addToScore(-1);
                    break;
                }

            }
        }

        for (int i = 0; i < numPlayers; i++)
        {
            if (Original_Players[i]->getScore() > 0)
            {
                cout << "\nGAME WINNER: \n";

                players[i]->addToScore(Original_Players[i]->getScore());
                cout << players[i]->getName() << ':' << players[i]->getScore() << endl;
                break;
            }
        }

    }
};






class BostonDiceGame : public DiceGame
{
private:
    int maxroll = 0, temp_store;
public:
    BostonDiceGame(int numSides, int numPlayers, int numDice)
    {
        dice = new Dice(numSides);
        this->numPlayers = numPlayers;
        this->numDice = numDice;
    }
    void play()
    {
        int scoret = 0, j = 0;
        for (int i = 0; i < numPlayers; i++) {
            scoret = 0;
            for (j = 0; j < numDice; j++) {
                maxroll = -1;
                for (int k = j; k < numDice; k++) {
                    temp_store = dice->roll();
                    if (maxroll < temp_store) {
                        maxroll = temp_store;
                    }
                }
                scoret += maxroll;
            }
            players[i]->addToScore(scoret);
        }
    }
};


int main()
{
    DiceGame* obj;
    int game, numPlayers, numDice, numSides;
    cout << "ENTER WHICH GAME YOU WOULD LIKE TO PLAY: \n1. Knock Out\n2. Boston Dice Game \n";
    cin >> game;
    cout << "Enter the number of dice ";
    cin >> numDice;
    cout << "Enter the number of sides on the dice";
    cin >> numSides;
    cout << "Enter the number of Players:";
    cin >> numPlayers;
    if (game == 1) {
        KnockOut obj2(numSides, numPlayers, numDice);
        obj = &obj2;

    }
    else {
        BostonDiceGame obj2(numSides, numPlayers, numDice);
        obj = &obj2;
    }

    obj->initPlayers();
    obj->play();
    obj->displayScores();
    obj->writeScoresToFile("scorecard.txt");
    obj->findHighestScore("scorecard.txt");
    return 1;
}