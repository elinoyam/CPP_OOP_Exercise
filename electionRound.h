//Both election round types here
#pragma once
#include "AbstractDistrict.h"
#include "party.h"
#include "person.h"
#include <typeinfo>
#include <iostream>
#include <fstream>
#include <list>
#include "dynamicArray.h"
#include <map>
using namespace std;

class AbstractDistrict;
class FairDistrict;
class District;
class Person;
class Party;


class ElectionRound
{
protected:
	int date[3]; // date of the election
	list<Party*> listOfParties; // pointers array to all the parties in this election
	
	map<int, Person*> voters;
	ElectionRound(int numOfReps, int day, int month, int year, int loaded=0); //overload for simple
	list<AbstractDistrict *> listOfDistricts;  // pointers array to all the districts in this election
	
	ElectionRound(const ElectionRound&) = delete; // cancel the copy ctor

public:
	ElectionRound(int day, int month, int year);
	virtual ~ElectionRound();

	void setDate(int year = 2021, int month = 1, int day = 1);
	// calls all needed function to make a new citizen in the election // 
	// checks if any of the given data is illegal //
	virtual void addVoter(const string name, int id, int birthYear, int district);

	// calls all needed set a new representative to a given party //
	// checks if any of the given data is illegal //
	virtual void addRep(int id, int party, int dist);

	// this method is making a new party and inserting the new party to the parties array//
	void addParty(const string str, int id);

	// this method is making a new district and insert the new district to the districts array //
	void addDistrict(AbstractDistrict* district);

	//Calls all needed function to set a vote-
	//checks if any of the parameters is illegal,
	//if everything's fine-
	//calls Person::addVote
	//otherwise, returns values beneath zero for each case.
	void addVote(int id, int party);

	// Returns pointer to the party in the listOfParties with the given serial number 
	// check if there is a party with the same serial number - if not throws invalied argument error
	Party * findParty(int party) const;
	// Returns pointer to person in votersBook, with person id.
	Person* findPerson(int id);
	// Returns pointer to the district in the listOfDistricts with the given serial number 
	// check if there is a district with the same serial number - if not throws invalied argument error
	AbstractDistrict * findDistrict(int dist) const;
		// Returns array of ptrs to all People who listed
	const map<int, Person*>& getVotersBook() const { return voters;}
	// Returns amount of listed districts
	int getAmountOfDistricts() const { return listOfDistricts.size(); }
	// Returns amount of listed parties 
	int getAmountOfParties() const { return listOfParties.size(); }
	// Returns amount of listed people (citizens)
	int getAmountOfCitizens() const { return voters.size(); }
	// Returns person referrence by index in votersBook
	Person* getCitizenInIndex(int index) { return (voters[index]); }
	// Gets Party** and District** to update, and:
	// calls calcWinInDist, and checks if any of the parties voted to in the distict
	// doesn't have enough delegates to fill in the "seats" it got.
	void checkElectionValidation( Party** resParty, AbstractDistrict** resDist) const;
	// os << ....ALL THE INFO TO BE PRINTED ABOUT ELECTION..... (9)
	friend ostream& operator<<(ostream& os, const ElectionRound& elect);

	const ElectionRound& operator=(const ElectionRound&);

	void showAllDistricts()const;
	void showAllParties() const;
	virtual void showMe() const {
		cout << endl << *this << endl;
	}

	// methods for files:
	void loadVoter(ifstream& in);
	void loadParty(ifstream& in);
	void loadDistrict(ifstream& in);
	void loadRepsOfAll(ifstream& in);
	void saveAllReps(ofstream& out);
	void loadAllVotesData(ifstream& in);
	void saveVotesData(ofstream& out);
	void loadElectionRound(ifstream& in);
	void saveElectionRound(ofstream& out);
};

class SimpleElectionRND : public ElectionRound
{
private:
public:
	// ctor for the simple election round
	// need to make only one district
	SimpleElectionRND(int numberOfReps, int day, int month, int year, int loaded=0) :ElectionRound(numberOfReps, day, month, year, loaded) {};
	virtual ~SimpleElectionRND() {} // calling to the base d-tor
	AbstractDistrict& getDistrict(int index) const = delete; // unabling the base function for simple election round

	virtual void addVoter(const string name, int id, int birthYear, int dist); // we don't care which number of district we got
	virtual void addRep(int id, int party, int dist); // we don't care which number of district we got
	friend ostream& operator<<(ostream& os, const SimpleElectionRND& elect); // Different output operator here
	virtual void showMe() const override { cout << endl << *this << endl; }
};