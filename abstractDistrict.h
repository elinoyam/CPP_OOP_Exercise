//All district types here
#pragma once
#include "person.h"
#include "electionRound.h"
#include "dynamicArray.h"
#include <iostream>
#include <string>
using namespace std;

#define rCastCC reinterpret_cast<const char*>
#define rCastC reinterpret_cast<char *>

class Person;
class Party;
class AbstractDistrict;


//** sub-division of district **// 
// this structure is responsible to save the voting data to each party in each district // 
class Votes {
private:
	Party* party; // pointer to the party which the structure will save the votes for
	int count = 0; // counter of the votes the party won in the district 
	int reps = 0;			// for results calculation - number of reps the party "wins" in the district
	float remainder = 0;	// for results calculation 
	float percOfVotesForParty = 0; // percentage of votes the party won from all the votes in the district
public:
	Votes() :remainder(0), reps(0), count(0), party(nullptr), percOfVotesForParty(0) {};
	Votes(Votes& vs) { *this = vs; }
	void operator=(const Votes& vs) {
		party = vs.party;
		count = vs.count;
		reps = vs.reps;
		remainder = vs.remainder;
		percOfVotesForParty = vs.percOfVotesForParty;
	}
	~Votes() {}
	Party& getParty() { return *party; }
	// Returns the amount of reps the party won (in the district)
	int getRepNum() { return reps; }
	float getPerc()const { return percOfVotesForParty; }
	int getVotesCount() { return count; }
	void setPerc(float perc) { percOfVotesForParty = perc; }
	void setParty(Party* pty) { party = pty; }
	void setCount(int addition) { count = addition; }

	// this method returns array of the chosen representatives of a specific party in the wanted district //  
	// if there is not enough representatives to this party in dist the method will return nullptr //
	DynamicArray<Person*>* getChosenRepsOfParty(AbstractDistrict& dist);

	friend class AbstractDistrict;
	friend class District; //NEEDED?
	friend class FairDistrict; //NEEDED?
};


class AbstractDistrict {

protected:
	string name; // the name of the district
	static int runningNumber; //initialized at the head of the cpp file - for the serial number division to the districts
	int serialNumber;
	const int numberOfReps; //number of representatives in this district
	//DYNAMIC ARRAY
	DynamicArray<Person*> voters;

	// data about percentage of voters
	float percentageOfVoters; // percentage of how many citizens voted in the district
	float canVote; // it's float so it would be easier to calculate the percentage
	int numOfVotes; // number of how many citizens voted in the district

	//DYNAMIC ARRAY:
	DynamicArray<Votes> vs;

	void operator=(const AbstractDistrict&) = delete; // cancel the = operator 
	AbstractDistrict(const AbstractDistrict&) = delete; // cancel the copy ctor

	// help function for calcWinInDist 
	// this method is searching for the party with the maximum remainder of votes in this district *after* the first distribution of representatives //
	int findMaxRemainder();

	// help function for calcWinInDist
	// this method is searching for the index of the party who won most representatives // 
	int checkForTieOrChange(int winner);

	// this method is distributing the firsts representatives to all the parties in this district and setting the percentage of votes each party won // 
	int findMaxReps(int& remainingReps, float rep);

public:

	AbstractDistrict(const string str, int numOfDlgts);
	//Builds abstract district out of file
	AbstractDistrict(ifstream& in, int);
	virtual void save(ofstream& out) const;  
	
	// this method is adding a new voter to the votersBook // 
	void addVoter(Person* voter);
	int getNumOfReps() { return numberOfReps; }
	virtual ~AbstractDistrict() {}

	int getNumOfVotedParties() { return vs.size(); }
	int getSerialNumber() { return serialNumber; }
	void increaseCanVote() { canVote++; }

	// this method calculates the percentage of votes in the district for the total citizens in this district //
	void calcPercOfVoters();

	// this method is adding to the voting data array a new vote //
	// if it's the first vote to this party in THIS district, the method will add a new Votes structure to votes array // 
	void addVote(Party& party);
	const string getName() const { return name; }

	void pushVotes(Votes& vts) { vs.push_back(vts); }

	// this method is setting the name of the district // 
	void setName(const string str);
	// this method first calculeates the percentage of votes and then returns the result //
	float getPercOfVoters();
	// Returns the Votes help-class instance of the district in index idx
	Votes& getvotesArrIn(int idx) { return vs.at(idx); }

	//returns the votesArr element that won in the called upon district.
	// each district will write this method as he wants to part is representatives - abstract method
	Votes& calcWinInDist();
	virtual void showMe() = 0;
	virtual void showElectionResultInMe(const Votes& vs) = 0;

};

class District : public AbstractDistrict 
{
protected:

	void operator=(const District&) = delete; // cancel the = operator 
	District(const District&) = delete; // cancel the copy ctor

public:
	District(const string str, int numOfDlgts) : AbstractDistrict(str, numOfDlgts) {};
	virtual ~District() {};
	District(ifstream& in,int reps):AbstractDistrict(in,reps) {};
	// writes 0 to the file before all data, to indicate- unified district 
	virtual void save(ofstream& out) const { int type = 0; out.write(rCastCC(&type), sizeof(type)); AbstractDistrict::save(out); }
	friend ostream& operator<<(ostream& os, const District& d);
	virtual void showMe() override { cout << *this; } // this method is for polimorfizem output
	virtual void showElectionResultInMe(const Votes& vs) override;
};

class FairDistrict : public AbstractDistrict
{
private:

public:
	FairDistrict(const string str, int numOfDlgts) : AbstractDistrict(str, numOfDlgts) {};
	virtual ~FairDistrict() {};
	FairDistrict(ifstream& in, int reps) :AbstractDistrict(in,reps) {};

	// writes 1 to the file before all data, to indicate- divided district 
	virtual void save(ofstream& out) const { int type = 1; out.write(rCastCC(&type), sizeof(type)); AbstractDistrict::save(out); }
	// Meant to be called after calculations; sorts votesArr for curr (fair and divided)district by amount of delegates each got.
	void sortPartiesByDels();
	friend ostream& operator<<(ostream& os, const FairDistrict& d);
	virtual void showMe() override { cout << *this; } // this method is for polimorfizem output
	virtual void showElectionResultInMe(const Votes& vs) override;
};
