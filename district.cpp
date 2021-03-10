#include "abstractDistrict.h"

ostream& operator<<(ostream& os, const District& d) {
	os << d.name << ", this district is unified, Serial number of district: " << d.serialNumber << ", type of district: unified district, and amount of representatives: " << d.numberOfReps << endl;
	return os;
}