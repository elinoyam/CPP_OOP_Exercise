#include "person.h"


ostream& operator<<(ostream& os, const Person& p) {

	os << "Citizen's name is: " << p.name << ", ID is: " << p.id << ", birth year: " << p.birthYear << ", and belong to the district: " << (p.homeDistrict)->getName() << endl; // << p.getPersonDistrict().getName() << endl;  // << dName << endl;
	return os;
}

// ** constractor and distractor ** //
Person::Person(const int i, AbstractDistrict& dist, int bYear, const string str) :id(i), name(str), birthYear(bYear), homeDistrict(&dist),voted(false), partyV(nullptr), isRep(false), repOfParty(nullptr), repOfDistrict(nullptr) {}

Person::~Person() {}						

void Person::loadPerson(ifstream& in) {

	int lSize;
	in.read(rCastC(&lSize), sizeof(lSize));
	if (!in.good()) throw ios::failure("Error in reading of file");
	name.resize(lSize);
	in.read(rCastC(&name), sizeof(name));
	in.read(rCastC(&birthYear), sizeof(birthYear));
	in.read(rCastC(&voted), sizeof(voted));
	if (!in.good()) throw ios::failure("Error in reading of file");
	
}

Person::Person(const int i) :id(i), birthYear(0), homeDistrict(nullptr), voted(false), partyV(nullptr), isRep(false), repOfParty(nullptr), repOfDistrict(nullptr) {}

// ***	Methods Implementation ** //

void Person::save(ofstream& out) const {
	out.write(rCastCC(&id), sizeof(id)); // saves the id of the citizen
	int nameSize = name.size();
	out.write(rCastCC(&nameSize), sizeof(nameSize)); // saves the length of the name // not include the '\0'
	out.write(rCastCC(&name), sizeof(name)); // save the name of the citizen
	out.write(rCastCC(&birthYear), sizeof(birthYear)); // save the birth year of the citizen
	out.write(rCastCC(&voted), sizeof(voted)); // save if the citizen already voted in this election round
	int homeDistrictSerialNumber = homeDistrict->getSerialNumber(); // temporary integer
	out.write(rCastCC(&homeDistrictSerialNumber), sizeof(homeDistrictSerialNumber)); // save the serial number of the home district of the citizen
	if(!out.good())
		throw ios::failure("Error in writing to file");
}

const Party& Person::getRepParty() { return *repOfParty; }

AbstractDistrict& Person::getRepDist() { return *repOfDistrict; }
					
void Person::setName(const string str) {
	name = str;
}

void Person::setRep(Party& party, AbstractDistrict& district) {
	if (isRep)
		throw logic_error("This person is already a representetive of a party. ");
		
	//else
	isRep = true;
	repOfParty = &party;
	repOfDistrict = &district;
}
							
void Person::setVote(Party& party) {
	if (voted == true)
		throw logic_error("This person already voted. One citizen can't vote twice. "); // person can't vote twice - return that the vote wasn't valid
	
	else {
		voted = true;
		partyV = &party;
	}
}