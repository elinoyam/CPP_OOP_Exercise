#include "party.h"

// For setting serial numbers
int Party::runningNum = 0;

Person** Party::getChosenRepsOfParty(District& dist, int numOfReps) {
	int i;
	bool found = false;
	Person** resRepList; 
	int curDistNum;
	int wantedDistNum = dist.getSerialNumber();
	for (i = 0; i < delLS && !found; i++) { // to find the representatives for dist
		curDistNum = allReps[i].getDistrict().getSerialNumber();
		if (curDistNum == wantedDistNum) // search in allReps for the district, to acces the reps array.
			found = true;	//FOUND!
	}
	if (!found) // the party don't have any representative in dist
		return nullptr;	// NOT ENOUGH REPS IN PARTY FOR ELECTORS IT GOT
		
	resRepList = allReps[i-1].getChosenRepsOfParty(numOfReps);
	return resRepList;

}

 ostream& operator<<(ostream& os, const Party& p) {
	
	os << p.name << ", Serial number of party: " << p.partyNumber << ", Head candidate of the party: " << p.headCandidate->getName() <<endl;
	os << "The representatives of the party:" << endl;
	for (int i = 0; i < p.delLS; i++)
		os << p.allReps[i] <<endl;
	
	return os;
}

Party::Party(char* str, Person& head): headCandidate(&head) 
{
	allReps = new repsList [delPS];
	partyNumber = runningNum;
	runningNum++;
	setName(str);
}

void Party::reallocAllReps()
{
	if (delLS >= delPS) {
		delPS *= 2;
		repsList* temp = new repsList [delPS];
		for (int i = 0; i < delLS; i++)
			temp[i] = allReps[i];
		delete[]allReps;
		allReps = temp;
	}
}

void Party::addRep(Person *rep, District *dist)
{
	int i;
	
	for (i = 0; i < delLS; i++) {
		if (allReps[i].getDistrict().getSerialNumber() == dist->getSerialNumber()) {
			allReps[i].addRep(rep);
			return;
		}
	}
	if (i >= delLS) // there wasn't any representative in this district
	{
		reallocAllReps();
		allReps[delLS].setDistrict(dist);
		allReps[delLS].addRep(rep);
		delLS++;
	}
}

Party::~Party()
{
	if(name !=nullptr)
		delete[] name;
	if (allReps != nullptr)
		delete[] allReps;
}

bool Party::setName(char* str){
	
	if (name != nullptr)
		delete[] name;

	name = new char[strlen(str)+1];
	nameLS = strlen(str);
	namePS = nameLS + 1;

	// copy the string into the name -  can't use strdup or strcpy
	int read = 0, write = 0;

	while (str[read] != '\0') {
		name[write] = str[read];
		write++;
		read++;
	}
	name[write] = '\0'; // sign end of string
	return true;
}

bool Party::setHeadCandidate(Person& p) {
	headCandidate = &p;
	return true;
}

//////////// methods of repsList

ostream& operator<<(ostream& os, const repsList& list) {
	os << "In District " << list.dist->getName() << ":" << endl;
	int i;
	for (i = 0; i < list.logSize-1; i++)
		os << list.reps[i]->getName() << ", ";
	if (i != 0)
		os << list.reps[i]->getName();
	os << endl;
	return os;
}

bool repsList::addRep(Person* rep) {
	reallocReps();
	reps[logSize] = rep;
	logSize++;
	return true;
}


void repsList::reallocReps() {
	if (logSize >= phySize) {
		phySize *= 2;
		Person** temp = new Person * [phySize];
		for (int i = 0; i < logSize; i++) 
			temp[i] = reps[i];
		delete[] reps;
		reps = temp;
	}
}

// repsList Method:

Person** repsList::getChosenRepsOfParty(int numOfReps) const 
{
	int i;
	Person** resRepList = new Person * [numOfReps];
	for (i = 0; i < logSize && i < numOfReps; i++)
		resRepList[i] = reps[i];
	if (i < numOfReps) { // there were not enough representatives for the party in the district
		delete[] resRepList;
		resRepList = nullptr;
	}
	return resRepList;
}