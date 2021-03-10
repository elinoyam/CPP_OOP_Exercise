#pragma once
#include <iostream>
#include <fstream>
using namespace std;
#include "abstractDistrict.h"

#define rCastCC reinterpret_cast<const char*>
#define rCastC reinterpret_cast<char *>

// Has members of types:
class AbstractDistrict;
class District;
class Party;

class Person
{
private:
	// citizen info
	string name;
	const unsigned int id;
	int birthYear;
	AbstractDistrict* homeDistrict;

	//vote info:
	bool voted;
	Party* partyV;

	// delegation info
	bool isRep;
	Party* repOfParty;
	AbstractDistrict* repOfDistrict;

public:
	Person(const int i, AbstractDistrict& dist, int bYear, const string str = nullptr);
	Person(const int i); // defulte c-tor for loading citizen from a file
	~Person();
	void loadPerson(ifstream& in);

	void save(ofstream& out) const;
	const string getName() const { return name; }
	const int getPersonID()const { return id; }
	int getBirthYear() const { return birthYear; }
	AbstractDistrict& getPersonDistrict() const { return *homeDistrict; }
	const Party& getVotedParty() { return *partyV; }
	// Get the party the person is a delegate of:
	const Party& getRepParty();
	// Get the district the person is a delegate in:
	AbstractDistrict& getRepDist();
	void setName(const string str);
	void setBirthYear(int year) { birthYear = year;}
	void setDistrict(AbstractDistrict& dist) { homeDistrict = &dist; }
	// Sets the members of this person as a party delegate:
	void setRep(Party& party, AbstractDistrict& district);
	// Returns true if this person is a delegate (for any party):
	bool getIsRep() { return isRep; }
	// sets this person vote for party:
	void setVote(Party& party);
	//operator<< for party:
	friend ostream& operator<<(ostream& os, const Person& p);
	void operator=(const Person&) = delete; // cancel the = operator 
	Person(const Person&) = delete; // cancel the copy ctor

};

