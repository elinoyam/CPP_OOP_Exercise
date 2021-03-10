#include "electionRound.h"


ostream& operator<<(ostream& os, const ElectionRound& elect) 
{
	int i, j,k;
	District *dist;
	Party *party;
	Person** electors;

	int res = elect.checkElectionValidation(&party,&dist); // check if there is an unvalid result from the votes in this election
	if (res == -1) {
		os << "Error!" << endl << "The Represntatives listed for " << party->getName() << " party in " << dist->getName() << " district do not suffice," << endl << "the party won more electors than it can provide." << endl;
		return os;
	}
	// else
	// for every district, show all the wanted data of this election round
	os << "The results:" << endl << endl;
	for (i = 0; i < elect.getAmountOfDistricts(); i++) {
		dist = elect.listOfDistricts[i];
		os << *dist << endl; // show the name, the serialNumber and the number of representative of the district
		if (dist->getNumOfVotedParties() == 0) { // No votes in dist. Automatic winner in dist is the party with 0 serial number.
			elect.getParty(elect.findParty(0)).sumElectors(dist->getNumOfReps()); // "Give" winner party in dist all it's delegates.
			os << "The district is giving it's electors to: " << elect.getParty(elect.findParty(0)).getHead().getName() << endl;
		}
		else {
			Votes& vs = dist->calcWinInDist();	// Calculate election results in district
			vs.getParty().sumElectors(dist->getNumOfReps()); // "Give" winner party in dist all it's delegates.
			os << "The district is giving it's electors to: " << vs.getParty().getHead().getName() << endl; // Show the head candidate of the winning party in this district
		}

		for (j = 0; j < dist->getNumOfVotedParties(); j++) { // Running on all parties who got votes in the district
			party = &dist->getvotesArrIn(j).getParty();
			electors = dist->getvotesArrIn(j).getChosenRepsOfParty(*dist); // Ret the electors of each party in this district
			os << "The representatives in the district of the party " << party->getName() << " are: ";
			for (k = 0; k < dist->getvotesArrIn(j).getRepNum(); k++) { // Running on all the delegates the party got in the district
				os << electors[k]->getName();
				if(k< dist->getvotesArrIn(j).getRepNum() - 1) // For Aesthetics in output... :)
					os << " , ";
			}
			os << endl;
			
			os << "The party " << party->getName() << " got " << dist->getvotesArrIn(j).getVotesCount() << " votes, " << endl; // Shows the number of votes for the party in this district
			os << "And got " << dist->getvotesArrIn(j).getPerc() << "% of the votes in this district. " << endl; // Shows the percentage of votes for the party in this district
		}
		os << "In this district, " << dist->getPercOfVoters() << "% of the citizens voted " << endl<<endl; // Shows the percantage of votes in this district
	}

	os<<"The Parties:"<<endl;

	// Show every party all the wanted data of this election round
	elect.sortPartiesByElectors(); // Sort the parties according to the number of electors the party got in total
	for(i=0;i<elect.getAmountOfParties();i++)
	{
		party=elect.listOfParties[i];
		os<<party->getName()<<":"<<endl<<"Candidate for head of the country is: "<<party->getHead().getName()<<endl;
		os<<"The amount of electors the party won is "<<party->getElectorsCount()<<endl;
		os<<"The amount of votes the party won is "<<party->getVotesCounter()<<endl;
	}

	os << "That's all folks!" << endl;
	// After all the values were added to os, now that we don't use them,
	// we'd like to reset electors in all the parties, for possible next election round.
	for (int i = 0; i < elect.partyLS; i++)
		elect.listOfParties[i]->sumElectors((elect.listOfParties[i]->getElectorsCount()) * (-1));
	
	return os;
}

void ElectionRound::sortPartiesByElectors() const
{
	int i, j;
	for (i = 0; i < partyLS - 1; i++)
	{
		for (j = partyLS - 1; j > i; j--)
		{
		// if the number of electors is smaller in the "left" of the array from the numbers of the electors in the "right" of the array
			if (listOfParties[i]->getElectorsCount() < listOfParties[j]->getElectorsCount()) 
			{
				Party* temp = listOfParties[i];
				listOfParties[i] = listOfParties[j];
				listOfParties[j] = temp;
			}
		}
	}
}

int ElectionRound::checkElectionValidation(Party** resParty, District** resDist) const 
{
	int i, j;
	District* dist;
	Party* party;
	Person** electors;

	for (i = 0; i < getAmountOfDistricts(); i++)
	{
		dist = listOfDistricts[i];
		if(dist->getNumOfVotedParties()!=0)
			Votes& vs = dist->calcWinInDist(); // set the number of representatives each party won in the district

		for (j = 0; j < dist->getNumOfVotedParties(); j++)
		{
			party = &dist->getvotesArrIn(j).getParty();
			electors = dist->getvotesArrIn(j).getChosenRepsOfParty(*dist); // get the list of electors
			if (electors == nullptr) // not enough represetatives of the party in the district
			{
				*resParty = party; // returns ref of the party whos' short of representatives
				*resDist = dist; // returns ref of the district in which the party is in short of representatives
				return -1;
			}
		}
	}
	return 0;
}

ElectionRound::ElectionRound(int day, int month, int year)
{
	setDate(year,month,day);
	listOfParties = new Party * [partyPS]; // first allocation of parties list
	listOfDistricts = new District * [distPhySize]; // first allocation of districts list
	voters = new Person * [voterPhyS]; // fisrt allocation of citizen book
}

ElectionRound::~ElectionRound()
{
	int i=0;
	if (listOfParties != nullptr) {
		Party* pTemp;
		while (i < partyLS) { // delete all the parties in the parties list
			pTemp = listOfParties[i];
			i++;
			delete pTemp;
		}
		delete[] listOfParties;
	}
	if (listOfDistricts != nullptr) { // delete all the districts in the districts list
		i = 0;
		District* dTemp;
		while (i < distLogS) {
			dTemp = listOfDistricts[i];
			i++;
			delete dTemp;
		}
		delete[] listOfDistricts;
	}
	if (voters != nullptr) { // delete all the citizens in the citizens book
		i = 0;
		Person* perTemp;
		while (i < votersLogS) {
			perTemp = voters[i];
			i++;
			delete perTemp;
		}
		delete[] voters;
	}
}

void ElectionRound::setDate(int year, int month, int day)
{
	date[0] = day;
	date[1] = month;
	date[2] = year;
}

void ElectionRound::reallocVoters()
{
	if (votersLogS >= voterPhyS)
		voterPhyS *= 2;
	else
		return;
	Person** temp = new Person * [voterPhyS];
	for (int i = 0; i < votersLogS; i++) // copy the data until now
		temp[i] = voters[i];
	delete[]voters;
	voters = temp;
}

int ElectionRound::addVoter(char* name, int id, int birthYear, int district)
{
	int distI = findDistrict(district); // check if there is a district with the same serial number
	if (distI < 0)
		return -1;	// DISTRICT DOESN'T EXIST!

	//else...

	Person* v = new Person(id, *listOfDistricts[distI], birthYear, name);
	
	reallocVoters();
	voters[votersLogS] = v; // Add voter to general voters book
	votersLogS++;

	listOfDistricts[distI]->increaseCanVote();	// Increase amount of voters in district
	listOfDistricts[distI]->addVoter(v);	// Add voter to voters book of district

	return 0;
}

void ElectionRound::reallocPartys()
{
	if (partyLS >= partyPS)
		partyPS*= 2;
	else
		return;
	Party** temp = new Party * [partyPS];
	for (int i = 0; i < partyLS; i++) // copy the data until now
		temp[i] = listOfParties[i];
	delete[]listOfParties;
	listOfParties = temp;
}

int ElectionRound::addParty(char* str, int id) 
{
	reallocPartys(); // check if needed reallcation

	int personI = findPerson(id); // check if there is a person with this ID
	if (personI < 0)
		return -1;		//Person not listed in system
	Person& head = getPerson(id);  // get reference to the head candidate
	Party* party = new Party(str, head);
	listOfParties[partyLS] = party;
	partyLS++;

	return 0;
}

Person& ElectionRound::getPerson(int id) const{ //returns the person in the id index in the voters book
	int personI = findPerson(id); //we alway check before that the person exists
	return *voters[personI];
}

int ElectionRound::findPerson(int id) const
{
	int i = 0;
	while ((i<votersLogS)&&(voters[i]->getPersonID() != id)) // searching for person with the same ID
		i++;
	if (i >= votersLogS)
		return -1; //Person isn't listed (doesn't exist)
	return i;
}

int ElectionRound::findDistrict(int dist) const
{
	int i = 0;
	while ((i<distLogS)&&(listOfDistricts[i]->getSerialNumber() != dist)) // searching for district with the given serial number
		i++;
	if (i >= distLogS)
		return -1; //Person isn't listed (doesn't exist)
	return i;
}
 
int ElectionRound::addRep(int id, int party, int dist)
{
	int distI, partyI, personI;

	if ((personI = findPerson(id)) < 0) return -1;		// No such person
	if ((partyI = findParty(party)) < 0) return -2;		// No such party
	if ((distI = findDistrict(dist)) < 0) return -3;	// No such district
	
	Person* rep = voters[personI];
	if (rep->getIsRep())
		return -4;									// Person is already deligate

	rep->setRep(*listOfParties[partyI], *listOfDistricts[distI]); // updating the data of the person
	
	listOfParties[partyI]->addRep(rep, listOfDistricts[distI]); // adding the person to the representative list of the party in the right district

	return 0;
}

void ElectionRound::addDistrict(District* district)
{
	reallocDistricts(); // check if reallocation is needed
	listOfDistricts[distLogS] = district;
	distLogS++;
}

void ElectionRound::reallocDistricts(){

	if (distLogS >= distPhySize)	{ // if the logical size is equal or bigger then the pyhsical size
	distPhySize *= 2;
	District** temp = new District * [distPhySize];
	for (int i = 0; i < votersLogS; i++) // copy the data until now
		temp[i] = listOfDistricts[i];
	delete[]listOfDistricts;
	listOfDistricts = temp;
	}
}

int ElectionRound::findParty(int party) const
{
	int i = 0;
	while ((i<partyLS)&&(listOfParties[i]->getPartyNum() != party)) // searching for party with the given serial number
		i++;
	if (i >= partyLS)
		return -1; //Party isn't listed (doesn't exist)
	return i;
}


int ElectionRound::addVote(int id, int party)	
{
	int voterI = findPerson(id); // finds the citizen with the given ID // return -1 if the ID doesn't exist
	if (voterI<0)
		return -1;	// person doesn't exist

	Person* voter = voters[voterI];	//right person who wishes to vote! 

	int partyI = findParty(party); // finds the party with the given serial number // return -1 if the serial number doesn't exist
	if (partyI<0)
		return -3;				// No such Party

	if (!voter->setVote(*listOfParties[partyI])) //already voted! (set vote returns false if the person already voted)
		return -2;
	

	District& district = voter->getPersonDistrict(); // get the home district of the citizen

	district.addVote(*listOfParties[partyI]);		//update the array of votes in the district, in the party's index.

	return 0;
}


