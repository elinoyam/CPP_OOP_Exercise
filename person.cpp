#include "person.h"

ostream& operator<<(ostream& os, const Person& p) {

	os << "Citizen's name is: " << p.name << ", ID is: " << p.id << ", birth year: " << p.birthYear << ", and belong to the district: " << (p.homeDistrict)->getName() << endl; // << p.getPersonDistrict().getName() << endl;  // << dName << endl;
	return os;
}

// ** constractor and distractor ** //
Person::Person(const int i, District& dist, int bYear, char* name) :id(i), birthYear(bYear), homeDistrict(&dist),voted(false), partyV(nullptr), isRep(false), repOfParty(nullptr), repOfDistrict(nullptr) {
	setName(name);
}

Person::~Person() {
	if(name != nullptr)		
		delete[] name;		
}							
						
// ***	Methods Implementation ** //

const Party& Person::getRepParty() { return *repOfParty; }

District& Person::getRepDist() { return *repOfDistrict; }
					
bool Person::setName(char* str) {
	if (name != nullptr)
		delete[] name;

	name = new char[strlen(str)+1];

	// copy the string into the name -  can't use strdup or strcpy
	int read = 0, write = 0;

	while(str[read] != '\0') {
		name[write] = str[read];
		write++;
		read++;
	}
	name[write] = '\0'; // sign end of string
	lSize = write;
	return true;
}

bool Person::setRep(Party& party, District& district) {
	if (isRep)
		return false; // already is a representative of a party
	//else
	isRep = true;
	repOfParty = &party;
	repOfDistrict = &district;
	return true;
}
							
bool Person::setVote(Party& party) {
	if (voted == true)
		return false; // person can't vote twice - return that the vote wasn't valid
	else {
		voted = true;
		partyV = &party;
	}
	return true;  //vote is ok
}