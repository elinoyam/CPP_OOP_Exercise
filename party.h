#pragma once
#include <iostream>
#include "person.h"
#include "dynamicArray.h"
using namespace std;

#define rCastCC reinterpret_cast<const char*>
#define rCastC reinterpret_cast<char *>

// Has members of types:
class Person;
class fairDistrict;
class District;
class AbstractDistrict;

// Sub-division of district:
// Each instance contains a district, and all the representatives of the party for that district.
class repsList {
private:
	AbstractDistrict* dist;
	DynamicArray<Person*> reps;
	
public:
	repsList() :dist(nullptr) {}
	~repsList() {}                      //CHECKING!
	// Adds rep to the array of reps
	void addRep(Person* rep);
	AbstractDistrict& getDistrict() const { return *dist; }
	void setDistrict(AbstractDistrict* _dist) { dist = _dist; }
	friend  ostream& operator<<(ostream& os, const repsList& list);
	// Help function for Party::getChosenRepsOfParty
	//returns array of ptrs to the delegates of the party who "won seats" in the district
	DynamicArray<Person*>* getChosenRepsOfParty(int numOfReps);
	int getLogSize()const { return reps.size(); }
	Person*  getRepInIndex(int index) const {  return reps.at(index); }

	void operator=(const repsList& other) { dist = other.dist; reps = other.reps; }  
	repsList(const repsList& other) { *this = other; } 

};

class Party
{
private:
	static int runningNum;	// Initialized at the head of the cpp file // for setting the serial numbers.
	int partyNumber;		// Serial number
	string name;
	Person *headCandidate; 
	DynamicArray<repsList> allReps; // Array of "repsLists", for each distict's representatives array.

	int electorsCount = 0;	// num of reps that the party won ALL IN ALL
	int countVotes = 0;


	void operator=(const Party&) = delete; // cancel the = operator 
	Party(const Party&) = delete;

public:
	Party(const string str, Person& head);
	~Party() {}
	Party(ifstream& in, Person& head);
	
	int getPartyNum() const { return partyNumber; }
	int getLenOfRepsList()const { return allReps.size(); }
	repsList* getRepsListIndex(int index) { return &allReps.at(index); }
	Person& getHead() const { return *headCandidate; }
	// Returns how many electors the party won ALL IN ALL (in all districts)
	int getElectorsCount() { return electorsCount; }
	string getName() { return name; }
	//// Returns how many votes the party got ALL IN ALL (in all districts)
	int getVotesCounter() { return countVotes; }	
	// Help function for Votes::getChosenRepsOfParty
	// Gets district ref and amount of reps the party got in the district,
	// and return array of ptrs to delegates of the party who got the "seats" in the district dist.
	DynamicArray<Person*>* getChosenRepsOfParty(AbstractDistrict& dist, int numOfReps);	// returns list of thr delegates the party got in dist district (after votes calculation)

	// Gets ptr to citizen, and a pointer to district,
	// Calls RepsList::addRep in the right index (with dist district)
	// (Adds a delegate to the party's district's array of delegates)
	void addRep(Person* rep, AbstractDistrict* dist);		// Add elegate of the party
	void setName(const string str); 
	void setHeadCandidate(Person& p);
	// Icreases sum of electors by addition
	void sumElectors(int addition) { electorsCount += addition; }	// for summing all the electors the party won (after votes calculation)
	void increaseVotesCounter() { countVotes++; }
	
	friend ostream& operator<<(ostream& os, const Party& p);
	void save(ofstream& out) const;
};

