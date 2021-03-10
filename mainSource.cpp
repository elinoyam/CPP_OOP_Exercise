#include <iostream>
#include <fstream>
#include "electionRound.h"
#include <typeinfo>
using namespace std;

void mainSelection(int sel, ElectionRound** elect);
void loadElection(ElectionRound** elect);
void saveElection(ElectionRound& elect);
void inputElection(ElectionRound** elect);
void printMenu();
void getSelection(int, ElectionRound**);
void inputDistrcit(ElectionRound& elect);
void inputPerson(ElectionRound& elect);
void inputParty(ElectionRound& elect);
void inputRep(ElectionRound& elect);
void inputVote(ElectionRound& elect);
void showAllCitizens(ElectionRound& elect);

int main() {
	ElectionRound* elect = nullptr;
	int sel = 0;
	int in = 0;

	cout << "Choose which election round you want: 1: new round; 2: load an exited round from file; 3: exit the program" << endl;
	cin >> sel;
	mainSelection(sel, &elect);

	while (in != 10) {
		system("CLS");
		printMenu();
		sel = in;
		cout << endl;
		getSelection(sel, &elect);

		cout << endl << "Please select your next action:" << endl;
		cin >> in;
	}
	getSelection(in, &elect);
	
	return 0;
}

void inputElection(ElectionRound** elect)
{// if input is 0--> regular, if input is 1-->simple
	int day, month, year, type;
	cout << "Welcome!" << endl << "Please enter day, month, year: ";
	cin >> day >> month >> year;
	cout << endl << "Great. Now, please enter type of election-" << endl << "For regular election round enter 0, for simple- 1, or any bigger number than 0." << endl;
	cin >> type;
	try {
		switch (type) {
		case 0:

			*elect = new ElectionRound(day, month, year);
			break;
		default:
			int reps;
			cout << "Please enter total number of representatives:";
			cin >> reps;
			if (reps < 0) {
				cout << "Invalid input." << endl;
				return;
			}
			*elect = new SimpleElectionRND(reps, day, month, year);
			break;
		}
	}
	catch (out_of_range& oor) { // if the entered date is not valid
		cout << "Error! " << oor.what() << endl;
		cout << "We will restart the program. " << endl;
		delete* elect;
		cout << "Choose which election round you want: 1: new round; 2: load an exited round from file; 3: exit the program" << endl;
		int sel;
		cin >> sel;
		mainSelection(sel, elect);

	}
	catch (exception& e) {
		cout << "Error! " << e.what() << endl;
		cout << "The program will exit now." << endl;
		delete* elect;
		exit(1);
	}
	catch (...) {
		cout << "There was error in the middle of the program. The program will terminate." << endl;
		delete* elect;
		exit(1);
	}
}

//Here we take care of date+type (before the class load method is called)
void loadElection(ElectionRound** elect)
{
	string fileName;
	cout << "Please enter the name of the file to load from: ";
	cin >> fileName;
	cout << endl;

	ifstream in;
	in.open(fileName, ios::binary);
	if (!in) {
		cout << "Error with openning file. Program termination" << endl;
		exit(-1);
	}
	int type = 0;
	in.read(rCastC(&type), sizeof(type));
	int date[3], numOfReps = 0;
	in.read(rCastC(&date[0]), sizeof(date[0]));
	in.read(rCastC(&date[1]), sizeof(date[1]));
	in.read(rCastC(&date[2]), sizeof(date[2]));
	if (!in.good()) { cout << "Error reading file. Program termination" << endl; exit(0); }
	delete* elect;
	switch (type) {
	case 1:
		in.read(rCastC(&numOfReps), sizeof(numOfReps));
		if (!in.good()) { cout << "Error reading file. Program termination" << endl; exit(0); }
		*elect = new SimpleElectionRND(numOfReps, date[0], date[1], date[2], 1);
		break;
	default: //-->0
		*elect = new ElectionRound(date[0], date[1], date[2]);
		break;
	}
	try {
		(*elect)->loadElectionRound(in);
		cout << "Current election round loaded successfully." << endl;
		in.close();
	}
	catch (ios::failure& f) {
		cout << "Error reading file. Program termination" << endl;
		delete* elect;
		exit(1);
	}
	catch (bad_alloc& ba) {
		cout << ba.what()<<endl;
		cout << "The program will end now." << endl;
		delete* elect;
		exit(1);
	}
}

void saveElection(ElectionRound& elect)
{
	string fileName;
	cout << "Please enter name for file: ";
	cin >> fileName;
	ofstream out(fileName, ios::binary);
	if (!out) { cout << "Error with file. Program termination" << endl; exit(-1); }
	try {
		elect.saveElectionRound(out); 
		cout << "Current election round was succesfully written to file" << endl;
		out.close();
	}
	catch (ios::failure& f) {
		cout << "Error with writing to file. Program termination" << endl;
		delete &elect;
		exit(0);
	}
	
	
	
}

void mainSelection(int sel, ElectionRound** elect) {
	switch (sel) {
	case 1:
		inputElection(elect);
		break;
	case 2:
		// load existed election round from a file
		loadElection(elect);
		break;
	case 3:
		cout << "You wish to exit the program. Farewell and goodbye!" << endl;
		exit(1);
		break;
	}
}

void printMenu() {
	cout << "Please select your action:" << endl;
	cout << 1 << ".  Add District" << endl;
	cout << 2 << ".  Add Citizen" << endl;
	cout << 3 << ".  Add Party" << endl;
	cout << 4 << ".  Add Representative to a party" << endl;
	cout << 5 << ".  Show all the districts" << endl;
	cout << 6 << ".  Show all the citizens" << endl;
	cout << 7 << ".  Show all the parties" << endl;
	cout << 8 << ".  Add Vote" << endl;
	cout << 9 << ".  Show the election round results" << endl;
	cout << 10 << ". Exit the program" << endl;
	cout << 11 << ". Save current election round" << endl;
	cout << 12 << ". Load election round from file" << endl;
}

void getSelection(int sel, ElectionRound** elect) {
	switch (sel) {
	case 1:
		inputDistrcit(**elect);
		break;
	case 2:
		inputPerson(**elect);
		break;
	case 3:
		inputParty(**elect);
		break;
	case 4:
		inputRep(**elect);
		break;
	case 5:
		(**elect).showAllDistricts();
		break;
	case 6:
		showAllCitizens(**elect);
		break;
	case 7:
		(**elect).showAllParties();
		break;
	case 8:
		inputVote(**elect);
		break;
	case 9:
		(*elect)->showMe();
		break;
	case 10:
		system("CLS");
		cout << "Thank you for using our program, we hope you had a good election round. " << endl;
		cout << "Bye bye! We'll see you next time.. " << endl;
		delete *elect;
		exit(0); // exit the program
		break;
	case 11:
		saveElection(**elect);
		break;
	case 12:
		loadElection(elect);
		break;
	}
}

void inputDistrcit(ElectionRound& elect) {

	if (typeid(elect) == typeid(SimpleElectionRND)) {
		cout << "NO DISTRICTS HERE MATE. we are in simple election round mode. " << endl;
		return;
	}

	string name;
	int deli;
	int whichDist = 0;

	cout << "Let's start to add the new district.." << endl;
	cout << "First- which kind of ditrict do you want to list?" << endl << "Enter 0 for unified, or 1 for divided:" << endl;

	cin >> whichDist;

	if (whichDist > 1 || whichDist < 0) {
		cout << "Invalid input." << endl;
		return;
	}

	cout << "Please enter the name of the district: ";

	cin.ignore();
	cin >> name;

	cout << endl << "Now, enter the number of deligate in this district: ";
	cin >> deli;

	if (deli < 0) {
		cout << "Invalid amount of deligates." << endl;
		return;
	}
	try{
		if (whichDist == 0) { //unified
			District* dist = new District(name, deli);
			elect.addDistrict(dist);
		}
		else { //divided
			FairDistrict* dist = new FairDistrict(name, deli);
			elect.addDistrict(dist);
		}
	}
	catch (bad_alloc& ba) {
		cout << "There is a problem with allocation. the program will end now."<<endl;
		delete &elect;
		exit(1);
	}
	cout << "District added to the system." << endl;
}

void inputRep(ElectionRound& elect) {
	int idRep, partyNum, distNum;

	cout << "Let's start to add the new representative.." << endl;
	cout << "Please enter the ID of the representative: ";
	cin >> idRep;
	cout << endl;

	cout << "Now, please enter the number of the party: ";
	cin >> partyNum;
	cout << endl;

	cout << "And last, enter the number of ditrict: ";
	cin >> distNum;
	cout << endl;

	try {
		elect.addRep(idRep, partyNum, distNum);
		cout << "The representative was registered successfully. Good Luck!" << endl;
	}
	catch (logic_error le) {
		cout << "Error. " << le.what() << endl;
	}
	catch (invalid_argument& ia) {
		cout << "Error! " << ia.what() << endl;
	}
	catch (bad_alloc& ba) {
		cout << "There is a problem with allocation. the program will end now." << endl;
		delete& elect;
		exit(1);
	}
}


void showAllCitizens(ElectionRound& elect) {
	cout << "The Holy Voters Book: " << endl;

	for (auto itr = elect.getVotersBook().begin(); itr != elect.getVotersBook().end(); ++itr)
		cout << *(itr->second);

}

void inputVote(ElectionRound& elect) // VOTE INPUT MAIN FUNCTION
{
	int id = 0, party = 0;
	cout << "Happy election day!" << endl << "Please enter your ID: ";
	cin >> id;
	cout << endl << "Please enter the number of the party you wish would win: ";
	cin >> party;
	try {
		elect.addVote(id, party);
		cout << endl << "Thank you for being a responsible citizen!" << endl << "Your vote is being proccesed. Great day!" << endl;
	}
	catch (invalid_argument& ia) {
		cout << "Error! " << ia.what() << endl;
		cout << "This vote doesn't count..." << endl;
	}
	catch (logic_error& le) {
		cout << "Error! " << le.what() << endl;
		cout << "This vote doesn't count..." << endl;
	}
	
}

void inputPerson(ElectionRound& elect)	//PERSON INPUT MAIN FUNCTION
{
	string name;
	int id, birthYear, dist;

	cout << "To the voters book!" << endl << "Please Enter your name: ";
	cin.ignore();
	cin >> name;

	cout << endl << "Your ID: " << endl;
	cin >> id;
	cout << endl << "Your birth year: " << endl;
	cin >> birthYear;
	if (birthYear > 2021)
		cout << "Dear time traveller! We welcome you." << endl;
	cout << endl << "And finally, the number of the district you live in: " << endl;
	cin >> dist;
	try {
		elect.addVoter(name, id, birthYear, dist);
		cout << "Voter added to the system." << endl;
	}
	catch (invalid_argument arg) {
		cout << arg.what() << endl;
	}
	catch (bad_alloc& ba) {
		cout << "There is a problem with allocation. the program will end now." << endl;
		delete& elect;
		exit(1);
	}
	
		
}

void inputParty(ElectionRound& elect)
{
	string name;
	int candidateId;

	cout << "Please enter the name of the party you wish to list:" << endl;
	cin.ignore();
	cin >> name; 

	cout << endl << "Now, please enter the ID of the party's candidate for head of country:" << endl;
	cin >> candidateId;
	try {
		elect.addParty(name, candidateId);
		cout << "Party added to the system." << endl;
	}
	catch (invalid_argument& msg) {
		cout << "A person with this ID does not exist in the system." << endl << "Party not added..." << endl;
	}	
	catch (bad_alloc& ba) {
		cout << "There is a problem with allocation. the program will end now." << endl;
		delete& elect;
		exit(1);
	}
}
