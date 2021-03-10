using namespace std;
#include <iostream>
#include "electionRound.h"
using namespace std;

void printMenu();
void getSelection(int,ElectionRound&);
void inputDistrcit(ElectionRound &elect);
void inputPerson(ElectionRound& elect);
void inputParty(ElectionRound& elect);
void inputRep(ElectionRound &elect);
void inputVote(ElectionRound& elect);
void showAllDistricts(ElectionRound& elect);
void showAllParties(ElectionRound& elect);
void showAllCitizens(ElectionRound& elect);
void showElectionResults(ElectionRound& elect);

int main() {
	ElectionRound elect(8, 12, 2020);
	int sel=0;
	int in = 0;

	while(in!=10){
		system("CLS");
		printMenu();
		sel = in;
		cout << endl;
		getSelection(sel,elect);
		
		cout << endl << "Please select your next action:" << endl;
		cin >> in;
	}
	getSelection(in, elect);
	
	return 0;
}

void printMenu(){
	cout << "Please select your action:" << endl;
	cout << 1 << ". Add District" << endl;
	cout << 2 << ". Add Citizen" << endl;
	cout << 3 << ". Add Party" << endl;
	cout << 4 << ". Add Representative to a party" << endl;
	cout << 5 << ". Show all the districts" << endl;
	cout << 6 << ". Show all the citizens" << endl; 
	cout << 7 << ". Show all the parties" << endl;
	cout << 8 << ". Add Vote" << endl;
	cout << 9 << ". Show the election round results" << endl;
	cout << 10 << ". Exit the program" << endl;
}

void getSelection(int sel, ElectionRound& elect){
	switch (sel) {
	case 1:
		inputDistrcit(elect);
		break;
	case 2:
		inputPerson(elect);
		break;
	case 3:
		inputParty(elect);
		break;
	case 4:
		inputRep(elect);
		break;
	case 5:
		showAllDistricts(elect);
		break;
	case 6:
		showAllCitizens(elect);
		break;
	case 7:
		showAllParties(elect);
		break;
	case 8:
		inputVote(elect);
		break;
	case 9:
		showElectionResults(elect);
		break;
	case 10:
		cout << "Thank you for using our program, we hope you had a good election round. " << endl;
		cout << "Bye bye! We'll see you next time.. " << endl;
		exit; // exit the progrem
		break;
	}
}

void inputDistrcit(ElectionRound &elect){
	char name[100];
	int deli;

	cout << "Let's start to add the new district.." << endl;
	cout << "First, please enter the name of the district: ";

	cin.ignore();
	cin.getline(name, 100); //Assuming no name longer than 100 characters...
	
	cout << endl << "Now, enter the number of deligate in this district: ";
	cin >> deli;
	
	District* dist = new District(name, deli);
	elect.addDistrict(dist);

	cout<<"District added to the system."<<endl;
}

void inputRep(ElectionRound &elect){
	int idRep, partyNum, distNum;
	
	cout << "Let's start to add the new representative.."<< endl;
	cout << "Please enter the ID of the representative: ";
	cin >> idRep;
	cout << endl;

	cout << "Now, please enter the number of the party: ";
	cin >> partyNum;
	cout << endl;

	cout << "And last, enter the number of ditrict: ";
	cin >> distNum;
	cout << endl;

	int check = elect.addRep(idRep, partyNum, distNum);

	if (check == -1)
		cout << "There is no person with this ID. You need to register to the citizen book first (option 2 in the main menu)." << endl;
	else if (check == -2)
		cout << "There is no such party with this serial number." << endl;
	else if (check == -3)
		cout << "There is no such district with this serial number." << endl;
	else if (check == -4)
		cout << "This person is already a representative of a Party." << endl;
	else
		cout << "The representative was registered successfully. Good Luck!" << endl;

}

void showAllDistricts(ElectionRound& elect) {
	cout << "The Districts:" << endl;
	int size = elect.getAmountOfDistricts();
	for (int i = 0; i < size; i++)
		cout << elect.getDistrict(i) << endl;
}

void showAllParties(ElectionRound& elect) {
	cout << "The Parties: " << endl;
	int size = elect.getAmountOfParties();
	for (int i = 0; i < size; i++) 
		cout << elect.getParty(i);
}

void showAllCitizens(ElectionRound& elect) {
	cout << "The Holy Voters Book: "<<endl;
	
	for (int i = 0; i < elect.getAmountOfCitizens(); i++)
		cout << (*(elect.getVotersBook()[i]));

}

void inputVote(ElectionRound& elect) // VOTE INPUT MAIN FUNCTION
{
	int id = 0, party = 0;
	cout << "Happy election day!" << endl << "Please enter your ID: ";
	cin >> id;
	cout << endl << "Please enter the number of the party you wish would win: ";
	cin >> party;
	int res=elect.addVote(id, party);
	if (res == -1)
		cout << endl << "Sorry dear, there is no person in the system with the id: " << id << endl;
	else if (res == -2)
		cout << "This person has already voted!" << endl << "Do not cheat." << endl;
	else if (res == -3)
		cout << "This party isn't listed in our system. Hence, invalid vote." << endl;
	else // res=0
		cout <<endl<< "Thank you for being a responsible citizen!" << endl << "Your vote is being proccesed. Great day!" << endl;
}

void inputPerson(ElectionRound& elect)	//PERSON INPUT MAIN FUNCTION
{
	char name[100];
	int id, birthYear, dist;

	cout << "To the voters book!" << endl << "Please Enter your name: ";
	cin.ignore();
	cin.getline(name, 100); //Assuming no name longer than 100 characters...

	cout << endl << "Your ID: " << endl;
	cin >> id;
	cout << endl << "Your birth year: " << endl;
	cin >> birthYear;
	if (birthYear > 2020)
		cout << "Dear time traveller! We welcome you." << endl;
	cout << endl << "And finally, the number of the district you live in: " << endl;
	cin >> dist;

	if (elect.addVoter(name, id, birthYear, dist) != 0)	//No matching district
		cout << "There is no such district in our system. Hence, Voter is invalid." << endl;
	else
		cout << "Voter added to the system." << endl;
}

void inputParty(ElectionRound& elect)
{
	char name[100];
	int candidateId;

	cout << "Please enter the name of the party you wish to list:" << endl;
	cin.ignore();
	cin.getline(name, 100); //Assuming no name longer than 100 characters...

	cout << endl << "Now, please enter the ID of the party's candidate for head of country:" << endl;
	cin >> candidateId;

	if (elect.addParty(name, candidateId) != 0)
		cout << "A person with this ID does not exist in the system." << endl << "Party not added..." << endl;

	else
		cout << "Party added to the system." << endl;
}

void showElectionResults(ElectionRound& elect)
{
	cout<< endl <<elect;
}