//---------------------------------------------------------------------------

#pragma hdrstop

#include "Estimates.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

vector<double> & Estimates::getPriorities() {
    return priorities;
}

vector< vector<int> > & Estimates::getRates() {
	return rates;
}
