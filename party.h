#pragma once
#include <iostream>
#include "person.h"
using namespace std;

// Has members of types:
class Person;
class District;

// Sub-division of district:
// Each instance contains a district, and all the representatives of the party for that district.
class repsList {
private:
	District* dist;
	Person** reps;
	int logSize = 0, phySize = 5;
public:
	repsList() :dist(nullptr) { reps = new Person * [phySize]; };
	~repsList() { delete[]reps; }
	// Adds rep to the array of reps
	bool addRep(Person* rep);
	void reallocReps();
	District& getDistrict() const { return *dist; }
	bool setDistrict(District* _dist) { dist = _dist; return true; }
	friend  ostream& operator<<(ostream& os, const repsList& list);
	// Help function for Party::getChosenRepsOfParty
	//returns array of ptrs to the delegates of the party who "won seats" in the district
	Person** getChosenRepsOfParty(int numOfReps) const;

};

class Party
{
private:
	static int runningNum;	// Initialized at the head of the cpp file // for setting the serial numbers.
	int partyNumber;		// Serial number
	char* name = nullptr;
	int nameLS, namePS;
	Person *headCandidate; 
	repsList* allReps;		// Array of "repsList"s, for each distict's representatives array.
	int delLS = 0, delPS = 5;

	int electorsCount = 0;	// num of reps that the party won ALL IN ALL
	int countVotes = 0;


	void operator=(const Party&) = delete; // cancel the = operator 
	Party(const Party&) = delete;

public:
	Party(char* str, Person& head);
	~Party();
	
	int getPartyNum() const { return partyNumber; }
	Person& getHead() const { return *headCandidate; }
	// Returns how many electors the party won ALL IN ALL (in all districts)
	int getElectorsCount() { return electorsCount; }
	char* getName() { return name; }
	//// Returns how many votes the party got ALL IN ALL (in all districts)
	int getVotesCounter() { return countVotes; }	
	// Help function for Votes::getChosenRepsOfParty
	// Gets district ref and amount of reps the party got in the district,
	// and return array of ptrs to delegates of the party who got the "seats" in the district dist.
	Person** getChosenRepsOfParty(District& dist, int numOfReps);	// returns list of thr delegates the party got in dist district (after votes calculation)
	void reallocAllReps();
	// Gets ptr to citizen, and a pointer to district,
	// Calls RepsList::addRep in the right index (with dist district)
	// (Adds a delegate to the party's district's array of delegates)
	void addRep(Person* rep, District* dist);		// Add elegate of the party
	bool setName(char* str); 
	bool setHeadCandidate(Person& p);
	// Icreases sum of electors by addition
	void sumElectors(int addition) { electorsCount += addition; }	// for summing all the electors the party won (after votes calculation)
	void increaseVotesCounter() { countVotes++; }
	
	
	friend ostream& operator<<(ostream& os, const Party& p);

};

