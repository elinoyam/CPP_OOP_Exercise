
#include "abstractDistrict.h"
#include <fstream>

// ** constractor and distractor ** //

int AbstractDistrict::runningNumber = 0;

AbstractDistrict::AbstractDistrict(const string str, int numOfDlgts) :name(str), percentageOfVoters(0), canVote(0), numberOfReps(numOfDlgts), numOfVotes(0), vs(5), voters(5)
{
	serialNumber = runningNumber;
	runningNumber++;
}

AbstractDistrict::AbstractDistrict(ifstream& in, int repsNum) : numberOfReps(repsNum) {
	int votesPS, votersPS;
	in.read(rCastC(&serialNumber), sizeof(serialNumber)); // update the serial number of the district
	if (serialNumber >= runningNumber)
		runningNumber = serialNumber + 1; // update the running number to avoide two districts with the same serial number
	int pSize; // for reading purposes
	in.read(rCastC(&pSize), sizeof(pSize)); // read the length of the district name
	name.resize(pSize); //= new char[pSize]; // allocation for the name of the district
	in.read(rCastC(&name), sizeof(name)); // read the name of the district // include the '\0'
	in.read(rCastC(&votersPS), sizeof(votersPS));
	voters.resize(votersPS);
	canVote = 0; // will automaticlly updated by adding the citizens from the file to this district in the electiom round
	in.read(rCastC(&numOfVotes), sizeof(numOfVotes)); // save the number of citizens that already voted in this distrcit
	percentageOfVoters = 0; // will automaticlly update when the user ask to see the election results // I'M NOT SURE
	in.read(rCastC(&votesPS), sizeof(votesPS)); //get the number of partys who got votes in the district
	vs.resize(votesPS);
	if (!in.good()) throw ios::failure("Error reading from file. ");
}

// ***	Methods Implementation ** //

void FairDistrict::showElectionResultInMe(const Votes& vs) {

	this->sortPartiesByDels();
	cout << "The district is giving it's electors by the following order:" << endl;
	for (int i = 0; i < this->getNumOfVotedParties(); i++) { //show each voted party candidates and its amount of electors in the ditrict.
		try {
			if (this->getvotesArrIn(i).getRepNum() > 0) {
				cout << this->getvotesArrIn(i).getParty().getName() << ": " << this->getvotesArrIn(i).getRepNum() << " electors." << endl;
				this->getvotesArrIn(i).getParty().sumElectors(this->getvotesArrIn(i).getRepNum());
			}
		}
		catch (invalid_argument& ia) {
			cout << ia.what() << "abstract district cpp, line 54"<<endl;
		}
	}
}

void District::showElectionResultInMe(const Votes& vs) {

	const_cast<Votes&>(vs).getParty().sumElectors(this->getNumOfReps()); // "Give" winner party in dist all it's delegates.
	cout << "The district is giving it's electors to: " << const_cast<Votes&>(vs).getParty().getHead().getName() << endl; // Show the head candidate of the winning party in this district

}

void AbstractDistrict::save(ofstream& out) const {
	int votesSize = vs.size(), votersLS=voters.size();
	out.write(rCastCC(&numberOfReps), sizeof(numberOfReps)); // save the number of representatives in the district
	out.write(rCastCC(&serialNumber), sizeof(serialNumber)); // save the serial number of the district
	int nameSize = name.size();
	out.write(rCastCC(&nameSize), sizeof(nameSize)); // save the logical size of the name
	out.write(rCastCC(&name), sizeof(name)); // save the name of the district
	out.write(rCastCC(&votersLS), sizeof(votersLS)); // save the number of citizens in the district
	out.write(rCastCC(&numOfVotes), sizeof(numOfVotes)); // save the number of votes that already done in the district
	out.write(rCastCC(&(votesSize)), sizeof(vs.size())); // save the number of partys who got votes in the district
	if(!out.good()) throw ios::failure("Error in writing of file");
}

void AbstractDistrict::calcPercOfVoters()
{
	if (canVote == 0) // no citizen in this district
		percentageOfVoters = 0;
	else
		percentageOfVoters = (numOfVotes / canVote) * 100;
}

void AbstractDistrict::setName(const string str) {
	name = str;
}

float AbstractDistrict::getPercOfVoters() {
	this->calcPercOfVoters();
	return percentageOfVoters;
}

void AbstractDistrict::addVoter(Person* voter)
{
	voters.push_back(voter);
}

void AbstractDistrict::addVote(Party& party)
{
	DynamicArray<Votes>::iterator itr(vs.begin());
	DynamicArray<Votes>::iterator itrEnd = vs.end();

	for (; itr != itrEnd; ++itr) {
		if ((*itr).party == &party) { //found party
			(*itr).count++;
			break;
		}
	}
	if(itr==itrEnd) //not found
	{
		Votes newV;
		newV.party = &party;
		newV.count++;
		vs.push_back(newV);
	}

	numOfVotes++;			//increase general votes in district
	party.increaseVotesCounter(); // increase the total votes to the party
}

//** Votes Method **// - sub-division of district - 

DynamicArray<Person*>* Votes::getChosenRepsOfParty(AbstractDistrict& dist) {
	
	return party->getChosenRepsOfParty(dist, reps); // call for an assistance function that will return the list of representatives
	 
}

int AbstractDistrict::findMaxRemainder()
{
	int maxI = 0;
	for (int i = 0; i < vs.size(); i++) {
		if (vs[i].remainder > vs[maxI].remainder)
			maxI = i;
		else if ((i != maxI) && (vs[i].remainder == vs[maxI].remainder)) {		// IF TIE
			if (vs[maxI].party->getPartyNum() > vs[i].party->getPartyNum()) {	// WINER IS WITH LOWER SERIAL NUMBER
				maxI = i;
			}
		}
	}
	return maxI;
}

int AbstractDistrict::checkForTieOrChange(int maxI)
{
	for (int i = 0; i < vs.size(); i++) {
		if (vs[i].reps > vs[maxI].reps)
			maxI = i;		// IF CHANGE
		else if ((i != maxI) && (vs[i].reps == vs[maxI].reps)) {		// IF TIE
			if (vs[maxI].party->getPartyNum() > vs[i].party->getPartyNum()) {	// WINER IS WITH LOWER SERIAL NUMBER
				maxI = i;
			}
		}
	}
	return maxI;	// The index in votesArr who's party's supposed to win
}

int AbstractDistrict::findMaxReps(int& remainingReps, float rep)
{
	int maxRepsIdx = 0;

	for (int i = 0; i < vs.size(); i++) {
		vs[i].reps = (vs[i].count) / rep; // first distribution of representatives
		if (vs[i].reps > vs[maxRepsIdx].reps) // if the current party won maximum representatives till now
			maxRepsIdx = i;
		remainingReps -= vs[i].reps; // update the remaining reps to distribute
		vs[i].setPerc((static_cast<float> (vs[i].count) / numOfVotes) * 100); // calc and set the percentage of votes to the party in the district
	}

	return maxRepsIdx;
}

Votes& AbstractDistrict::calcWinInDist()
{
	int remainingReps = numberOfReps;
	float rep = (static_cast<float>(numOfVotes) / numberOfReps); // votes cost of each representative in this district
	int maxRepsIdx = findMaxReps(remainingReps, rep); // finding the party with the most reps after the first distribution

	int winI = 0; // the zero is just for initialization

	if (remainingReps == 0) {	// Need to check for tie
		winI = checkForTieOrChange(maxRepsIdx);
		return vs[winI];
	}

	while (remainingReps > 0) { // if there are more reps to split..
		winI = findMaxRemainder(); // finding the party with maximu remainder of votes
		vs[winI].reps++;
		vs[winI].remainder = 0;
		remainingReps--;
	}

	winI = checkForTieOrChange(winI); // check which party won in this district

	return vs[winI];	 // return the data of the party with the most electors
}