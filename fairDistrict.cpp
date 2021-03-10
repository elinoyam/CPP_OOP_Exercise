#include "abstractDistrict.h"
#include "sort.h"

void FairDistrict::sortPartiesByDels()
{
	Sort()(vs, [](Votes& a, Votes& b) {if (a.getRepNum() < b.getRepNum()) return true; else return false; });
}

ostream& operator<<(ostream& os, const FairDistrict& d) {
	os << d.name << ", this district is divided, Serial number of district: " << d.serialNumber << ", type of district: divided district, and amount of representatives: " << d.numberOfReps << endl;
	return os;
}