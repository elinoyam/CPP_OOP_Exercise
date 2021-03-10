#pragma once
#include "district.h"
#include "party.h"
#include "person.h"
#include <iostream>
using namespace std;

class District;
class Person;
class Party;


class ElectionRound
{
private:
	int date[3]; // date of the election
	Party** listOfParties = nullptr; // pointers array to all the parties in this election
	void reallocPartys();
	int partyLS = 0, partyPS = 5; // sizes of the parties array
	District** listOfDistricts = nullptr;  // pointers array to all the districts in this election
	int distLogS = 0, distPhySize = 5; // sizes of the districts array
	void reallocDistricts();
	Person** voters=nullptr; // pointers array to all the citizens in this election
	int votersLogS = 0, voterPhyS = 5; // sizes of the citizens array
	void reallocVoters();
	// this method is sorting the parties in the parties array accourding to the number of electors the won // 
	void sortPartiesByElectors() const; 

	void operator=(const ElectionRound&) = delete; // cancel the = operator 
	ElectionRound(const ElectionRound&) = delete; // cancel the copy ctor

public:
	ElectionRound(int day = 1, int month = 1, int year = 2021);
	~ElectionRound();

	void setDate(int year = 2021, int month = 1, int day = 1);
	// calls all needed function to make a new citizen in the election // 
	// checks if any of the given data is illegal //
	// returns -1 if the given district is not exist
	int addVoter(char* name, int id, int birthYear, int district);

	// calls all needed set a new representative to a given party //
	// checks if any of the given data is illegal //
	// return -1 if there isn't such person
	// returns -2 if there isn't such party
	// returns -3 if there isn't such district
	// return -4 if the citizen is already a representative
	int addRep(int id, int party, int dist);

	// this method is making a new party and inserting the new party to the parties array//
	// returns -1 if the given ID is not in the citizens book //
	int addParty(char* str, int id);

	// this method is making a new district and insert the new district to the districts array //
	void addDistrict(District* district);

	//Calls all needed function to set a vote-
	//checks if any of the parameters is illegal,
	//if everything's fine-
	//calls Person::addVote
	//otherwise, returns values beneath zero for each case.
	int addVote(int id, int party);

	// Returns party index in listOfParties, with party serial number.
	int findParty(int party) const;
	// Returns person index in votersBook, with person id.
	int findPerson(int id) const;
	// Returns district index in listOfDistricts, with dist serial number.
	int findDistrict(int dist) const;
	// Returns referrence for person in with id
	Person& getPerson(int id) const;
	// Returns array of ptrs to all People who listed
	Person** getVotersBook() const { return voters;}
	// Returns amount of listed districts
	int getAmountOfDistricts() const { return distLogS; }
	// Returns amount of listed parties 
	int getAmountOfParties() const { return partyLS; }
	// Returns amount of listed people (citizens)
	int getAmountOfCitizens() const { return votersLogS; }
	// Returns district referrence by index in listOfDistricts
	District& getDistrict(int index) const { return *listOfDistricts[index]; }
	// Returns party referrence by index in listOfParties
	Party& getParty(int index) const { return *listOfParties[index]; }
	// Returns person referrence by index in votersBook
	Person& getCitizenInIndex(int index) const { return *voters[index]; }
	// Gets Party** and District** to update, and:
	// calls calcWinInDist, and checks if any of the parties voted to in the distict
	// doesn't have enough delegates to fill in the "seats" it got.
	int checkElectionValidation( Party** resParty, District** resDist) const;
	// os << ....ALL THE INFO TO BE PRINTED ABOUT ELECTION..... (9)
	friend ostream& operator<<(ostream& os, const ElectionRound& elect);
};

