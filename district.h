#pragma once
#include "person.h"
#include "electionRound.h"
#include <iostream>
using namespace std;

class Person;
class Party;

//** sub-division of district **// 
// this structure is responsible to save the voting data to each party in each district // 
class Votes {
	private:
		Party* party; // pointer to the party which the structure will save the votes
		int count=0; // counter of the votes the party won in the district 
		int reps = 0;			// for results calculation - number of reps the party "wins" in the district
		float remainder = 0;	// for results calculation 
		float percOfVotesForParty = 0; // percentage of votes the party won from all the votes in the district
	public:
		Votes() :remainder(0), reps(0), count(0), party(nullptr) ,percOfVotesForParty(0){};
		Party& getParty() { return *party; }
		// Returns the amount of reps the party won (in the district)
		int getRepNum() { return reps; }
		float getPerc()const { return percOfVotesForParty; }
		int getVotesCount() { return count; }
		bool setPerc(float perc) { percOfVotesForParty = perc; return true; }

		// this method returns array of the chosen representatives of a specific party in the wanted district //  
		// if there is not enough representatives to this party in dist the method will return nullptr //
		Person** getChosenRepsOfParty(District& dist);

		friend class District;
	};

class District
{
private:
	char *name= nullptr; // the name of the district
	int pSize, lSize; // sizes of the name of the district
	static int runningNumber; //initialized at the head of the cpp file - for the serial number division to the districts
	int serialNumber;
	const int numberOfReps; //number of representatives in this district
	Person** voters; // array of pointer to the voters that this is their HOME DISTRICT
	int votersLS = 0, votersPS = 5;

	// data about percentage of voters
	float percentageOfVoters; // percentage of how many citizens voted in the district
	float canVote; // it's float so it would be easier to calculate the percentage
	int numOfVotes; // number of how many citizens voted in the district

	// this method calculates the percentage of votes in the district for the total citizens in this district //
	void calcPercOfVoters();

	// this method is responsible to check if the voters array is large enough before adding a new voter // 
	void reallocVoters();
	

	Votes* votesArr; // array of Votes - responsible to save the votes data to each party in the district
	
	int votesLS = 0, votesPS = 5; // sizes of the votesArr

	// help function for calcWinInDist 
	// this method is searching for the party with the maximum remainder of votes in this district *after* the first distribution of representatives //
	int findMaxRemainder();
	
	// help function for calcWinInDist
	// this method is searching for the index of the party who won most representatives // 
	int checkForTieOrChange(int winner);

	// this method is distributing the firsts representatives to all the parties in this district and setting the percentage of votes each party won // 
	int findMaxReps(int &remainingReps, float rep);

	void operator=(const District&) = delete; // cancel the = operator 
	District(const District&) = delete; // cancel the copy ctor

public:
	District(char* str, int numOfDlgts);

	// this method is adding a new voter to the votersBook // 
	void addVoter(Person* voter);
	int getNumOfReps() { return numberOfReps; }
	~District();

	int getNumOfVotedParties() { return votesLS; }
	int getSerialNumber() { return serialNumber; }
	
	// this method is responsible to check and resize if needed the size of the voting data array //
	void reallocVotes();
	void increaseCanVote() { canVote++; }

	// this method is adding to the voting data array a new vote //
	// if it's the first vote to this party in THIS district, the method will add a new Votes structure to votes array // 
	void addVote(Party& party);
	char* getName() {return name; }

	// this method is setting the name of the district // 
	bool setName(char* str);
	// this method first calculeates the percentage of votes and then returns the result //
	float getPercOfVoters();
	// Returns the Votes help-class instance of the district in index idx
	Votes& getvotesArrIn(int idx) { return votesArr[idx]; }
	
	// this method is calculating which party won in this district // 
	//returns the votesArr element that won in the called upon district.
	Votes& calcWinInDist();

	friend ostream& operator<<(ostream& os, const District& d) {
		os << d.name << ", Serial number of district: " << d.serialNumber << ", and amount of representatives: " << d.numberOfReps << endl;
		return os;
	}
	
	friend ostream& operator<<(ostream& os, const Person& p);
	
};

