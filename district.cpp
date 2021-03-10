#include "district.h"
#include "electionRound.h"

// ** constractor and distractor ** //

int District::runningNumber = 0;

District::District(char* str, int numOfDlgts) :percentageOfVoters(0), canVote(0), numberOfReps(numOfDlgts), numOfVotes(0) 
{
	voters = new Person * [votersPS];
	votesArr = new Votes[votesPS];
	setName(str);
	serialNumber = runningNumber;
	runningNumber++;
}

District::~District()
{
	if(name !=nullptr)
		delete[] name;
	if(voters !=nullptr)
		delete[]voters;
	if(votesArr !=nullptr)
		delete[] votesArr;

}

// ***	Methods Implementation ** //

void District::calcPercOfVoters() 
{ 
	if (canVote == 0) // no citizen in this district
		percentageOfVoters = 0;
	else
		percentageOfVoters = (numOfVotes / canVote) * 100; 
}

bool District::setName(char* str) {
	if (name != nullptr)
		delete[] name;

	name = new char[strlen(str)+1];
	lSize = strlen(str);
	pSize = lSize + 1;

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

float District::getPercOfVoters() { 
	this->calcPercOfVoters(); 
	return percentageOfVoters; 
}

void District::reallocVoters()
{
	if (votersLS >= votersPS) // if the logical size is equal to the physical size
		votersPS *= 2;
	else
		return;
	Person** temp = new Person * [votersPS];
	for (int i = 0; i < votersLS; i++) // copy the data till now 
		temp[i] = voters[i];
	delete[]voters;
	voters = temp;
}

void District::addVoter(Person* voter)
{
	reallocVoters();
	voters[votersLS] = voter;
	votersLS++;
}

void District::reallocVotes()
{
	if (votesLS >= votesPS)
		votesPS *= 2;
	else
		return;
	Votes* temp = new Votes [votesPS];
	for (int i = 0; i < votesLS; i++) // copy the data till now
		temp[i] = votesArr[i];
	delete[]votesArr;
	votesArr = temp;
}

void District::addVote(Party& party)
{
	int i = 0;
	while ((i < votesLS) && (votesArr[i].party != &party))  // find the Votes structure with the wanted party
		i++;
	if (i >= votesLS)	// first vote for this party in the district
	{
		reallocVotes();
		votesArr[votesLS].party = &party;
		votesLS++;
	}
	votesArr[i].count++;	//icrease votes for THIS party in this district
	numOfVotes++;			//increase general votes in district
	party.increaseVotesCounter(); // increase the total votes to the party
}

int District::findMaxRemainder()
{
	int maxI = 0;
	for (int i = 0; i < votesLS; i++) {
		if (votesArr[i].remainder > votesArr[maxI].remainder)
			maxI = i;
		else if ((i != maxI) && (votesArr[i].remainder == votesArr[maxI].remainder)) {		// IF TIE
			if (votesArr[maxI].party->getPartyNum() > votesArr[i].party->getPartyNum()) {	// WINER IS WITH LOWER SERIAL NUMBER
				maxI = i;
			}
		}
	}
	return maxI;
}

int District::checkForTieOrChange(int maxI)
{
	for (int i = 0; i < votesLS; i++) {
		if (votesArr[i].reps > votesArr[maxI].reps)
			maxI = i;		// IF CHANGE
		else if ((i!=maxI)&&(votesArr[i].reps == votesArr[maxI].reps)) {		// IF TIE
			if (votesArr[maxI].party->getPartyNum() > votesArr[i].party->getPartyNum()) {	// WINER IS WITH LOWER SERIAL NUMBER
				maxI = i;
			}
		}
	}
	return maxI;	// The index in votesArr who's party's supposed to win
}

int District::findMaxReps(int &remainingReps, float rep)
{
	int maxRepsIdx = 0;

	for (int i = 0; i < votesLS; i++) {
		votesArr[i].reps = (votesArr[i].count) / rep; // first distribution of representatives
		if (votesArr[i].reps > votesArr[maxRepsIdx].reps) // if the current party won maximum representatives till now
			maxRepsIdx = i;
		remainingReps -= votesArr[i].reps; // update the remaining reps to distribute
		votesArr[i].setPerc((static_cast<float> (votesArr[i].count) / numOfVotes) * 100); // calc and set the percentage of votes to the party in the district
	}

	return maxRepsIdx;
}

Votes& District::calcWinInDist()
{
	int remainingReps = numberOfReps;
	float rep = (static_cast<float>(numOfVotes) / numberOfReps); // votes cost of each representative in this district
	int maxRepsIdx = findMaxReps(remainingReps, rep); // finding the party with the most reps after the first distribution

	int winI = 0; // the zero is just for initialization

	if (remainingReps == 0)	{	// Need to check for tie
		winI = checkForTieOrChange(maxRepsIdx);
		return votesArr[winI];
	}

	while (remainingReps > 0) { // if there are more reps to split..
		winI = findMaxRemainder(); // finding the party with maximu remainder of votes
		votesArr[winI].reps++;
		votesArr[winI].remainder = 0;
		remainingReps--;
	}
	
	winI = checkForTieOrChange(winI); // check which party won in this district

	return votesArr[winI];	 // return the data of the party who won in this district
}

//** Votes Method **// - sub-division of district - 

Person** Votes::getChosenRepsOfParty(District& dist) {
	Person** resRepList; 
	resRepList = party->getChosenRepsOfParty(dist, reps); // call for an assistance function that will return the list of representatives
	return resRepList;
}