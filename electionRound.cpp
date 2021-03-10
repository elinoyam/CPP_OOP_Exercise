#include "electionRound.h"
#include "sort.h"
#include <list>
using namespace std;

ostream& operator<<(ostream& os, const SimpleElectionRND& elect) // OVERLOAD! For simpleElecrtionRound..
{
	bool noVotes = false;
	Party* party;
	DynamicArray< Person*>* electors = nullptr;
	repsList* repsList = nullptr;
	AbstractDistrict* dist;
	int i, j;
	try{
	elect.checkElectionValidation(&party, &dist); // check if there is an unvalid result from the votes in this election
	}
	catch(logic_error& l) {
		if(party!=nullptr)
			os << "Error!" << endl << "The Represntatives listed for " << party->getName() << " party, in district " << dist->getName() <<" do not suffice." << endl << "the party won more electors than it can provide." << endl;
		else
			os << "No party was listed. Anarchy!" << endl;
		return os;
	}


	dist = elect.listOfDistricts.front();
	os << elect.date[0] << "/" << elect.date[1] << "/" << elect.date[2] << endl << endl << "The results:" << endl << endl;

	os << dist->getPercOfVoters() << "% of the citizens voted." << endl;


	Votes& vs = dist->calcWinInDist();	// Calculate election
	if(dist->getNumOfVotedParties()!=0)
		dynamic_cast<FairDistrict*>(dist)->sortPartiesByDels(); 

	for (i = 0; (i < dist->getNumOfVotedParties())||((i==0)&&(dist->getNumOfVotedParties()==0)); i++) {
		if (dist->getNumOfVotedParties() == 0) { // No votes in dist. Automatic winner in dist is the party with 0 serial number.
			noVotes = true;
			elect.findParty(0)->sumElectors(dist->getNumOfReps()); // "Give" winner party in dist all it's delegates.
			party = elect.findParty(0);
			repsList = party->getRepsListIndex(0);
			os << party->getName() << " has won " << party->getElectorsCount() << " electors, and " << party->getVotesCounter() << " votes which are 0% of all votes." << endl;
		}
		else {
			try {
				party = &(dist->getvotesArrIn(i).getParty());
				party->sumElectors(dist->getvotesArrIn(i).getRepNum());
				electors = dist->getvotesArrIn(i).getChosenRepsOfParty(*dist);
			}
			catch (invalid_argument& ia) {
				cout << ia.what() << "election round cpp, line 49";
			}
		}
		if(!noVotes) os << party->getName() << " has won " << party->getElectorsCount() << " electors, and " << party->getVotesCounter() << " votes which are " << dist->getvotesArrIn(i).getPerc() << "% of all votes." << endl;
		os << "The party's elected delegates are: ";
		if(noVotes)
			for (j = 0; j < dist->getNumOfReps(); j++) {
				try {
					os << repsList->getRepInIndex(j)->getName();
					if (j < dist->getNumOfReps() - 1) // For Aesthetics in output... :)
						os << " , ";
				}
				catch (invalid_argument& ia) {
					cout << ia.what() << endl;
				}
			}

		else
			try {
			for (j = 0; j < dist->getvotesArrIn(i).getRepNum(); j++) {
				os << (*electors)[j]->getName();
				if (j < dist->getvotesArrIn(i).getRepNum() - 1) // For Aesthetics in output... :)
					os << " , ";
			}
			
		}
		catch (invalid_argument& ia) {
			cout << ia.what() << "election round cpp, line 75."<< endl;
		}
		os << endl << "And the party's candidate for head of state is " << party->getHead().getName() << endl;
		delete electors;
	}
	os << "That's all folks!" << endl;
	// After all the values were added to os, now that we don't use them,
	// we'd like to reset electors in all the parties, for possible next election round.
	for (auto itr = elect.listOfParties.begin(); itr != elect.listOfParties.end(); ++itr)
		(*itr)->sumElectors((*itr)->getElectorsCount() * (-1));
	
	return os;
}

void SimpleElectionRND::addVoter(const string name, int id, int birthYear, int dist) 
{
	 ElectionRound::addVoter(name, id, birthYear, 0);
}

void SimpleElectionRND::addRep(int id, int party, int dist) 
{
	ElectionRound::addRep(id, party, 0);
}

ElectionRound::ElectionRound(int numOfReps, int day, int month, int year, int loaded) // OVERLOAD! For simpleElecrtionRound..
{
	setDate(year,month,day);
	if(!loaded)
		listOfDistricts.push_back( new FairDistrict(const_cast<char*>("country"), numOfReps));
}

ostream& operator<<(ostream& os, const ElectionRound& elect) 
{
	int i, j, k;
	AbstractDistrict *dist;
	District* uniDist = nullptr;
	FairDistrict* divDist = nullptr;
	Party *party=nullptr;
	DynamicArray< Person*>* electors = nullptr;

	try {
		elect.checkElectionValidation(&party, &dist); // check if there is an unvalid result from the votes in this election

	}	
	catch(logic_error& l) {
		if(party!=nullptr)
			os << "Error!" << endl << "The Represntatives listed for " << party->getName() << " party, in district " << dist->getName() <<" do not suffice." << endl << "the party won more electors than it can provide." << endl;
		else
			os << "No party was listed. Anarchy!" << endl;
		return os;
	}
	// else, for every district, show all the wanted data of this election round
	
	os << elect.date[0] << "/" << elect.date[1] << "/" << elect.date[2] << endl << endl << "The results:" << endl << endl;
	
	auto begin = elect.listOfDistricts.begin();
	auto end = elect.listOfDistricts.end();

	for(auto itr = begin;itr!=end;++itr) {
		dist = (*itr); // for convenience purposes
		dist->showMe();
			
		if (dist->getNumOfVotedParties() == 0) { // No votes in dist. Automatic winner in dist is the party with 0 serial number.
			elect.findParty(0)->sumElectors(dist->getNumOfReps()); // "Give" winner party in dist all it's delegates.
			os << "The district is giving it's electors to: " << elect.findParty(0)->getHead().getName() << endl;
		}
		else {
			Votes& vs = dist->calcWinInDist();	// Calculate election results in district
			dist->showElectionResultInMe(vs);
		}

		for (j = 0; j < dist->getNumOfVotedParties(); j++) { // Running on all parties who got votes in the district
			try {
				party = &dist->getvotesArrIn(j).getParty();
				electors = dist->getvotesArrIn(j).getChosenRepsOfParty(*dist); // Get the electors of each party in this district
				os << "The representatives in the district of the party " << party->getName() << " are: ";
				for (k = 0; k < dist->getvotesArrIn(j).getRepNum(); k++) { // Running on all the delegates the party got in the district
					os << (*electors)[k]->getName();
					if (k < dist->getvotesArrIn(j).getRepNum() - 1) // For Aesthetics in output... :)
						os << " , ";
				}
				os << endl;
			
			
				os << "The party " << party->getName() << " got " << dist->getvotesArrIn(j).getVotesCount() << " votes, " << endl; // Shows the number of votes for the party in this district
				os << "And got " << dist->getvotesArrIn(j).getPerc() << "% of the votes in this district. " << endl; // Shows the percentage of votes for the party in this district
				delete electors;
			}
			catch (invalid_argument& ia) {
				cout << ia.what() << "election round cpp, line 161" << endl;
			}
		}
		os << "In this district, " << dist->getPercOfVoters() << "% of the citizens voted " << endl<<endl; // Shows the percantage of votes in this district
	}

	os<<"The Parties:"<<endl;

	// Show every party all the wanted data of this election round
	//const_cast<ElectionRound &>(elect).sortPartiesByElectors(); // Sort the parties according to the number of electors the party got in total
	Sort()(const_cast<ElectionRound &>(elect).listOfParties, [](Party* x, Party* y) {if (x->getElectorsCount() < y->getElectorsCount()) return true; else return false; });
	for(auto itr = elect.listOfParties.begin();itr!=elect.listOfParties.end();++itr)
	{
		party = *itr;
		os<<party->getName()<<":"<<endl<<"Candidate for head of the country is: "<<party->getHead().getName()<<endl;
		os<<"The amount of electors the party won is "<<party->getElectorsCount()<<endl;
		os<<"The amount of votes the party won is "<<party->getVotesCounter()<<endl;
	}

	os << "That's all folks!" << endl;
	// After all the values were added to os, now that we don't use them,
	// we'd like to reset electors in all the parties, for possible next election round.
	for (auto itr = elect.listOfParties.begin(); itr != elect.listOfParties.end(); ++itr)
		(*itr)->sumElectors((*itr)->getElectorsCount() * (-1));

	
	return os;
}


void ElectionRound::checkElectionValidation(Party** resParty, AbstractDistrict** resDist) const 
{
	if (listOfParties.size() == 0)
		throw logic_error("No party was listed. Anarchy!");
	
	int i=0, j;
	AbstractDistrict* dist = nullptr;
	Party* party = nullptr;
	DynamicArray<Person*>* electors = nullptr;

	auto begin = listOfDistricts.begin();
	auto end = listOfDistricts.end();
	
	for (auto itr = begin; itr != end; ++itr,++i) {
		dist = *itr;
		if ((*itr)->getNumOfVotedParties() != 0)
			Votes& vs = dist->calcWinInDist(); // set the number of representatives each party won in the district
		else { // Winner is serial number 1:
			repsList* currepsL;
			try {
				currepsL = findParty(0)->getRepsListIndex(i);
			}
			catch (invalid_argument ia) { //invlid index (for rep)
				*resParty = findParty(0);
				*resDist = dist;
				throw logic_error("not enough representatives for the party in the district");
			}
			if ((currepsL->getLogSize()) < (dist->getNumOfReps())) { //problem
				*resParty = findParty(0);
				*resDist = dist;
				throw logic_error("not enough representatives for the party in the district");
			}
		}
				

		for (j = 0; j < dist->getNumOfVotedParties(); j++)
		{
			try{
			party = &dist->getvotesArrIn(j).getParty();
			electors = dist->getvotesArrIn(j).getChosenRepsOfParty(*dist); // get the list of electors
			}
			catch (logic_error& l) {
				*resParty = party; // returns ref of the party whos' short of representatives
				*resDist = dist; // returns ref of the district in which the party is in short of representatives
				throw l;
			}
			catch (invalid_argument& ia) {
				cout << ia.what() << "election round cpp, line 255." << endl;
			}
			
		}
	}
}

ElectionRound::ElectionRound(int day, int month, int year)
{
	setDate(year,month,day);
}

ElectionRound::~ElectionRound()  // changed
{
	if (!listOfParties.empty()) 
		listOfParties.erase(listOfParties.begin(), listOfParties.end());
	if (!listOfDistricts.empty()) // delete all the districts in the districts list
		listOfDistricts.erase(listOfDistricts.begin(), listOfDistricts.end());
	if (!voters.empty())
		voters.erase(voters.begin(), voters.end());
}

void ElectionRound::showAllDistricts() const {
	cout << "The Districts:" << endl;
	int size = listOfDistricts.size();

	for (auto itr : listOfDistricts)
		(*itr).showMe();			// the methd will call for the right printing method according to polimorfizem

}

void ElectionRound::showAllParties() const {
	cout << "The Parties: " << endl;

	for (auto itr = listOfParties.begin(); itr != listOfParties.end(); ++itr)
		cout << *(*itr);
}

void ElectionRound::setDate(int year, int month, int day)
{
	if (year <= 0)
		throw out_of_range("The year of the election round can't be negative.");
	if (month <= 0 || month > 12)
		throw out_of_range("Invalid month has been entered to the election round date.");
	if (day < 1)
		throw out_of_range("Invalid day has been entered to the election round date. Day can't be negative.");
	else if (day > 28) {
		if(day>31)
			throw out_of_range("Invalid day has entered to the election round date. There isn't more then 31 days in a month.");
		else if (month == 2)
			throw out_of_range("Invalid day has entered to the election round date. Day isn't compatible with the month.");
		else if((month ==4||month==6||month==9||month==11) && (day>30))
			throw out_of_range("Invalid day has entered to the election round date. Day isn't compatible with the month.");
	}
	date[0] = day;
	date[1] = month;
	date[2] = year;
}

void ElectionRound::addVoter(const string name, int id, int birthYear, int district)
{
	if (!(100000000 <= id && id <= 999999999))
		throw invalid_argument("Invalid ID was given. The id should be 9 digits.");
	if (date[2] - birthYear < 18)
		throw invalid_argument("Invalid birth year. The citizen can't vote unless he is 18 or older.");
	try{
	AbstractDistrict* dist = findDistrict(district); 

	Person* v = new Person(id, *dist, birthYear, name);
	voters[id] = v; // Add voter to general voters book

	dist->increaseCanVote();	// Increase amount of voters in district
	dist->addVoter(v);	// Add voter to voters book of district
	}
	catch (invalid_argument& ia) {
		throw ia; // we want the error to get to the main
	}
	catch (bad_alloc& ba) {
		throw "There was a problem with the allocation";
	}
}

void ElectionRound::addParty(const string str, int id) 
{
	Person *head;
	try {
		head = findPerson(id); // check if there is a person with this ID
	}
	catch (invalid_argument& ia) {
		throw ia;
	}
	
	listOfParties.push_back(new Party(str, *head));
	
}

Person* ElectionRound::findPerson(int id) {
	auto itr = voters.find(id); 
	if (itr == voters.end())
		throw invalid_argument("There is no person with this ID. You need to register to the citizen book first (option 2 in the main menu).");
	return (itr->second);
}

AbstractDistrict* ElectionRound::findDistrict(int dist) const
{
	auto begin = listOfDistricts.begin();
	auto end = listOfDistricts.end();
	for (auto itr = begin; itr != end; ++itr) {
		if ((*itr)->getSerialNumber() == dist)
			return const_cast<AbstractDistrict*>(*itr);
	}
	throw invalid_argument("Invalid District number. There is no such district with this serial number. ");
}
 
void ElectionRound::addRep(int id, int party, int dist)
{
	Person* personI;
	AbstractDistrict* district;
	Party* partyI;

	try {
		personI = findPerson(id);
		partyI = findParty(party);
		district = findDistrict(dist);
	}
	catch (invalid_argument& msg) { throw msg; }

	if (personI->getIsRep())
		throw "This person is already a representative of a party.";								// Person is already deligate

	personI->setRep(*partyI, *district); // updating the data of the person
	
	partyI->addRep(personI, district); // adding the person to the representative list of the party in the right district

}

void ElectionRound::addDistrict(AbstractDistrict* district)
{
	listOfDistricts.push_back( district);
}

Party * ElectionRound::findParty(int party) const
{
	for (auto itr = listOfParties.begin(); itr != listOfParties.end(); ++itr) {
		if ((*itr)->getPartyNum() == party)
			return (*itr);
	}
	throw invalid_argument("Invalid party number.There is no such party with this serial number.");
}

void ElectionRound::addVote(int id, int party)	
{

	Person* voter; 
	Party* partyI;
	try {
		voter = findPerson(id); // finds the citizen with the given ID // throw error if the ID doesn't exist 
		partyI = findParty(party);
		voter->setVote(*partyI);
	}
	catch (invalid_argument& msg) {
		throw msg;
	} // we want the error to go to the main
	catch (logic_error& l) {
		throw l;
	}

	AbstractDistrict& district = voter->getPersonDistrict(); // get the home district of the citizen

	district.addVote(*partyI);		//update the array of votes in the district, in the party's index.

}

const ElectionRound& ElectionRound::operator=(const ElectionRound&)
{
	// Here we need to check for self placement
	return *this;
}

// ** load from file methods ** // 

void ElectionRound::loadVoter(ifstream& in) {
	int i=0;
	in.read(rCastC(&i), sizeof(i));
	if (!in.good()) throw ios::failure("Error in reading of file");
	Person* c;
	try {
		c = new Person(i);
		c->loadPerson(in);
	}
	catch (ios::failure& f) {
		throw f;
	}
	catch (bad_alloc& ba) {
		throw "Error with allocation.";
	}
	
	int distNum;
	in.read(rCastC(&distNum), sizeof(distNum));
	if (!in.good()) throw ios::failure("Error in reading of file");

	AbstractDistrict* dist = findDistrict(distNum); // check if there is a district with the same serial number
	c->setDistrict(*dist);

	voters[c->getPersonID()] = c;
	dist->increaseCanVote();	// Increase amount of voters in district
	dist->addVoter(c);	// Add voter to voters book of district

}

void ElectionRound::loadParty(ifstream& in) {

	int headID;
	in.read(rCastC(&headID), sizeof(headID));
	if (!in.good()) throw ios::failure("Error in reading of file");
	Person& head = *findPerson(headID);  // get reference to the head candidate
	listOfParties.push_back(new Party(in, head));
	
}

void ElectionRound::loadDistrict(ifstream& in)
{
	AbstractDistrict* district;
	int numOfReps, type;
	in.read(rCastC(&type), sizeof(type));
	in.read(rCastC(&numOfReps), sizeof(numOfReps));
	
	if (!in.good()) throw ios::failure("Error in reading of file");
	if(type==0)
		district = new District(in, numOfReps);
	else
		district = new FairDistrict(in, numOfReps);
	
	listOfDistricts.push_back( district);
	
}

void ElectionRound::loadRepsOfAll(ifstream& in){ // In the phase this method is called, ElectionRound::partyLS is already up to date.
	
	int partySerial=0, numOfReps=0;
	int distSerial = 0, repsInDist = 0, repId = 0;
	// for each party - get the number of districts
	for (auto itr = listOfParties.begin(); itr != listOfParties.end(); ++itr){
		in.read(rCastC(&partySerial), sizeof(partySerial));
		in.read(rCastC(&numOfReps), sizeof(numOfReps));
		if (!in.good()) throw ios::failure("Error in reading of file");
		// for each district
		for (int j = 0; j < numOfReps; j++) {
			// get the serial number of the district
			in.read(rCastC(&distSerial), sizeof(distSerial));
			// then get the number of reps
			in.read(rCastC(&repsInDist), sizeof(repsInDist));
			if (!in.good()) throw ios::failure("Error in reading of file");
			for (int k = 0; k < repsInDist; k++) {
				// the id of each rep
				in.read(rCastC(&repId), sizeof(repId));
				if (!in.good()) throw ios::failure("Error in reading of file");
				// call the addRep method
				addRep(repId, partySerial, distSerial);
			}
		}
	}
}

void ElectionRound::saveAllReps(ofstream& out){
	int j, k, partySerial, districtSerial, numofRepsList, repsNum, tempID;
	Party* party;
	repsList* reps;

	for (auto itr = listOfParties.begin(); itr != listOfParties.end(); ++itr) // save the reps for each party
	{
		party = *itr;
		partySerial = party->getPartyNum();
		out.write(rCastCC(&partySerial), sizeof(partySerial)); // save the serial number of the party
		numofRepsList = party->getLenOfRepsList();
		out.write(rCastCC(&numofRepsList), sizeof(numofRepsList)); // save the number of district in which the party have representatives
		for(j=0;j<numofRepsList;j++){
			try {
				reps = party->getRepsListIndex(j);
				districtSerial = reps->getDistrict().getSerialNumber();
				
				out.write(rCastCC(&districtSerial), sizeof(districtSerial)); //  save each district it's serial number
				repsNum = reps->getLogSize();
				out.write(rCastCC(&repsNum), sizeof(repsNum)); // save the number of representatives in the current district
				for (k = 0; k < repsNum; k++) {
					tempID = reps->getRepInIndex(k)->getPersonID();
					out.write(rCastCC(&tempID), sizeof(tempID)); // save the ID of the representative
				}

			}
			catch (invalid_argument& ia) {
				cout << ia.what() << endl;
				cout << "election round cpp, line 561" << endl;
			}
			catch (...) {
				cout << "catch" << endl;
			}
			
		}
	}

	if(!out.good())
		throw ios::failure("Error in writing to file");
}

void ElectionRound::loadAllVotesData(ifstream& in) {
	int numOfDistricts, numOfParties, i, j, districtSerial, partySerial, numOfVotes;
	float percOfVotes;
	Votes vts;
	in.read(rCastC(&numOfDistricts), sizeof(numOfDistricts)); // get the number of districts - need to know how many loops to do
	if (!in.good()) throw ios::failure("Error with reading from file. ");
	for (i = 0; i < numOfDistricts; ++i) { // for each district:
		
		in.read(rCastC(&districtSerial), sizeof(districtSerial)); // get the district serial number
		in.read(rCastC(&numOfParties), sizeof(numOfParties));	// get the number of partys who got voted in current district
		if (!in.good()) throw ios::failure("Error with reading from file. ");
		AbstractDistrict* dist = findDistrict(districtSerial);

		for (j = 0; j < numOfParties; ++j) { // for each party:
			in.read(rCastC(&partySerial), sizeof(partySerial)); // read the serial number of the party
			in.read(rCastC(&numOfVotes), sizeof(numOfVotes)); // read the number of counted votes for the party in this district
			in.read(rCastC(&percOfVotes), sizeof(percOfVotes)); // read the percentage of votes the party got from all the votes in this district - IS IT NEEDED???
			if (!in.good()) throw ios::failure("Error with reading from file. ");
			Party * partyI = findParty(partySerial); // find the index of the party in the parties array
			vts.setParty(partyI); // set the party in the j place in the array
			vts.setCount(numOfVotes); // set the counter of votes in the j place in the array
			vts.setPerc(percOfVotes); // set the percentage of votes in the j place in the array
			dist->pushVotes(vts);
			dist->calcPercOfVoters();
			
		}
	}
}

void ElectionRound::saveVotesData(ofstream& out) {
	int j, distSerial, partySerial, numofParties, counterOfVotes;
	float percOfVotes;

	int distSize = listOfDistricts.size();
	out.write(rCastCC(&distSize), sizeof(distSize)); // save the number of districts

	auto begin = listOfDistricts.begin();
	auto end = listOfDistricts.end();
	for (auto itr = begin; itr != end; ++itr) {
		distSerial = (*itr)->getSerialNumber();
		out.write(rCastCC(&distSerial), sizeof(distSerial)); // save the serial number of the district
		numofParties =(*itr)->getNumOfVotedParties();
		out.write(rCastCC(&numofParties), sizeof(numofParties)); // save the number of voted parties in this district

		for (j = 0; j < numofParties; j++) { // for each party:
			try {
				partySerial = (*itr)->getvotesArrIn(j).getParty().getPartyNum();
				out.write(rCastCC(&partySerial), sizeof(partySerial)); // save the party serial number in the j place in the array
				counterOfVotes = (*itr)->getvotesArrIn(j).getVotesCount();
				out.write(rCastCC(&counterOfVotes), sizeof(counterOfVotes)); // save the number of votes the party got in the district
				percOfVotes = (*itr)->getvotesArrIn(j).getPerc();
				out.write(rCastCC(&percOfVotes), sizeof(percOfVotes)); // get the percentage of the votes the party got from all the votes in the district
			}
			catch (invalid_argument& ia) {
				cout << ia.what() << "election round cpp, line 624" << endl;
			}
		}
	}

	if(!out.good())
		throw ios::failure("Error in writing to file");
}

void ElectionRound::loadElectionRound(ifstream& in) { // Instance is already created (with date (and numOfReps if it's simple))
	int distPhySize;
	try {
		in.read(rCastC(&distPhySize), sizeof(distPhySize));
		if (!in.good()) throw ios::failure("Error in reading of file");
		
		for (int i = 0; i < distPhySize; i++) // enter each district to the district array 
			loadDistrict(in);
		
		int peopleS = 0; // get the number of citizens
		in.read(rCastC(&peopleS), sizeof(peopleS));
		if (!in.good()) throw ios::failure("Error in reading of file");

		for (int i = 0; i < peopleS; i++) // enter each person to the voters book 
			loadVoter(in);

		int partyLs = 0; // get the number of parties
		in.read(rCastC(&partyLs), sizeof(partyLs));
		if (!in.good()) throw ios::failure("Error in reading of file");

		for (int i = 0; i < partyLs; i++) // enter each party to the parties array 
			loadParty(in);

		loadRepsOfAll(in); //  enter each rep to his party and district 
		loadAllVotesData(in);
	}
	catch (ios::failure& f) {
		throw f;
	}
	
}

void ElectionRound::saveElectionRound(ofstream& out) {
	int type = 0;
	if (typeid(*this) == typeid(SimpleElectionRND))
		type = 1;
	try {
		out.write(rCastCC(&type), sizeof(type)); // first- type of election
		out.write(rCastCC(&date[0]), sizeof(date[0])); //day
		out.write(rCastCC(&date[1]), sizeof(date[1])); //month
		out.write(rCastCC(&date[2]), sizeof(date[2])); //year
		if (type != 0) {
			int reps = listOfDistricts.front()->getNumOfReps();
			out.write(rCastCC(&reps), sizeof(reps));
		}
		int distSize = listOfDistricts.size();
		out.write(rCastCC(&(distSize)), sizeof(distSize)); // amount of districts
		auto begin = listOfDistricts.begin();
		auto end = listOfDistricts.end();

		for (auto itr = begin; itr != end; ++itr)
			(*itr)->save(out);

		int votersSize = voters.size();
		out.write(rCastCC(&votersSize), sizeof(votersSize)); // amount of  citizens
		for (auto begin = voters.begin(); begin != voters.end(); ++begin) {
			(*begin).second->save(out);
		}
				
		int partiesSize = listOfParties.size();
		out.write(rCastCC(&partiesSize), sizeof(partiesSize)); // amount of parties
		for (auto itr = listOfParties.begin(); itr != listOfParties.end(); ++itr) // all parties
			(*itr)->save(out);
		saveAllReps(out); // all reps
		saveVotesData(out); // votes data
	}
	catch (ios::failure& f) {
		throw f; // we want the error to get to the main
	}
}