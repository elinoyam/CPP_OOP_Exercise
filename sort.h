#pragma once

/*
This is a template class for a sort of dynamic arrays for any kind.
The operator() need to get the array and a compare function.
FOR THE TEMPLATE NEEDED PART OF THE PROJECT.
*/

#include <iostream>
using namespace std;
#include <vector>

class Sort {
public:
	template<class T, class O>
	void operator()(T& lst, const O& compare) {
		int size = lst.size();
		auto begin = (lst.begin());
		auto end = --(lst.end());

		for (auto itrI = begin; itrI != end; ++itrI) {
			for (auto itrJ = end; itrJ != itrI; --itrJ) {
				if (compare(*itrI, *itrJ)) { //true if arr[i] < arr[j]
					auto temp = *itrI;
					*itrI = *itrJ;
					*itrJ = temp;
				}
			}
		}
	}

};