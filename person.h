#pragma once
#include <iostream>
using namespace std;
#include "district.h"

// Has members of types:
class District;
class Party;

class Person
{
private:
	// citizen info
	char *name;
	int lSize=0;				// logical size of name // allocated according to input string length
	const unsigned int id;
	int birthYear;
	District *homeDistrict;

	//vote info:
	bool voted;
	Party *partyV;

	// delegation info
	bool isRep;
	Party *repOfParty;
	District *repOfDistrict;

public:
	Person(const int i, District& dist, int bYear, char* name = nullptr);
	~Person(); 

	char* getName() const { return name; }
	const int getPersonID()const { return id; }
	int getBirthYear() const { return birthYear; }
	District& getPersonDistrict() const { return *homeDistrict; } 
	const Party& getVotedParty() { return *partyV; }
	// Get the party the person is a delegate of:
	const Party& getRepParty();	
	// Get the district the person is a delegate in:
	District& getRepDist(); 
	bool setName(char* str);
	bool setBirthYear(int year) { birthYear = year; return true; }
	bool setDistrict(District& dist) { homeDistrict = &dist; return true; }
	// Sets the members of this person as a party delegate:
	bool setRep(Party& party, District& district); 
	// Returns true if this person is a delegate (for any party):
	bool getIsRep() { return isRep; }	
	// sets this person vote for party:
	bool setVote(Party& party); 
	//operator<< for party:
	friend ostream& operator<<(ostream& os, const Person& p);

};

