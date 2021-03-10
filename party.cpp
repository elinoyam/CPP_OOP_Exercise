#include "party.h"


// For setting serial numbers
int Party::runningNum = 0;

DynamicArray<Person*>* Party::getChosenRepsOfParty(AbstractDistrict& dist, int numOfReps) {
	int i;
	bool found = false;
	DynamicArray<Person*>* resRepList; 
	int curDistNum;
	int wantedDistNum = dist.getSerialNumber();
	for (i = 0; i < allReps.size() && !found; i++) { // to find the representatives for dist
		curDistNum = allReps[i].getDistrict().getSerialNumber();
		if (curDistNum == wantedDistNum) // search in allReps for the district, to acces the reps array.
			found = true;	//FOUND!
	}
	if (!found) // the party don't have any representative in dist
		throw logic_error("the party don't have any representative in the district");	// NOT ENOUGH REPS IN PARTY FOR ELECTORS IT GOT
		
	resRepList = allReps[i-1].getChosenRepsOfParty(numOfReps);
	return resRepList;

}

 ostream& operator<<(ostream& os, const Party& p) {
	
	os << p.name << ", Serial number of party: " << p.partyNumber << ", Head candidate of the party: " << p.headCandidate->getName() <<endl;
	os << "The representatives of the party:" << endl;
	for (int i = 0; i < p.allReps.size(); i++)
		os << p.allReps[i] <<endl;
	
	return os;
}

Party::Party(const string str, Person& head): headCandidate(&head), name(str), allReps(5)
{
	partyNumber = runningNum;
	runningNum++;
}

void Party::addRep(Person *rep, AbstractDistrict *dist)
{
	int i;
	
	for (i = 0; i < allReps.size(); i++) {
		if (allReps[i].getDistrict().getSerialNumber() == dist->getSerialNumber()) {
			allReps[i].addRep(rep);
			return;
		}
	}
	if (i >= allReps.size()) // there wasn't any representative in this district
	{
		repsList rl;
		rl.setDistrict(dist);
		rl.addRep(rep);
		allReps.push_back(rl);
	}
}

void Party::setName(const string str){
	
	name = str;
}

void Party::setHeadCandidate(Person& p) {
	headCandidate = &p;
}

//////////// methods of repsList

ostream& operator<<(ostream& os, const repsList& list) {
	os << "In District " << list.dist->getName() << ":" << endl;
	int i;
	for (i = 0; i < list.getLogSize(); i++) {
		os << list.reps[i]->getName();
		if ((i + 1) != list.getLogSize()) os<< ", ";
	}
	os << endl;
	return os;
}

void repsList::addRep(Person* rep) {
	
	reps.push_back(rep);
}



// repsList Method:

DynamicArray<Person*>* repsList::getChosenRepsOfParty(int numOfReps)
{
	int i;
	DynamicArray<Person*>* resRepList;
	resRepList = new DynamicArray<Person*>(numOfReps);
	
	for (i = 0; i < getLogSize() && i < numOfReps; i++)
		(*resRepList)[i] = reps[i];
	if (i < numOfReps) { // there were not enough representatives for the party in the district
		throw (logic_error("not enough representatives for the party in the district"));
	}
	return resRepList;
}

// ** methods to work with file ** //
void Party::save(ofstream& out) const {
	int headID = headCandidate->getPersonID();
	out.write(rCastCC(&headID), sizeof(headID)); // save the id of the head candidate
	out.write(rCastCC(&partyNumber), sizeof(partyNumber)); // save the party serial number
	int nameSize = name.size();
	out.write(rCastCC(&nameSize), sizeof(nameSize)); // save the logical size of the name of the party
	out.write(rCastCC(&name), sizeof(name)); // save the name of the party
	int repsSize = allReps.size();
	out.write(rCastCC(&repsSize), sizeof(repsSize)); // save the number of district in which the party have representatives
	out.write(rCastCC(&countVotes), sizeof(countVotes));
	if(! out.good())
		throw ios::failure("Error in writing to file");
}

Party::Party(ifstream& in, Person& head) :headCandidate(&head),electorsCount(0){
	in.read(rCastC(&partyNumber), sizeof(partyNumber));
	if (runningNum <= partyNumber)
		runningNum = partyNumber + 1;
	int nameSize;
	in.read(rCastC(&nameSize), sizeof(nameSize));
	name.resize(nameSize); 
	in.read(rCastC(&name), sizeof(name));
	int delPS;
	in.read(rCastC(&delPS), sizeof(delPS));
	allReps.resize(++delPS); //= new repsList[++delPS];
	in.read(rCastC(&countVotes), sizeof(countVotes));
	if (!in.good()) throw ios::failure("Error reading from file. ");
}
