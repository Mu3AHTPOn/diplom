//---------------------------------------------------------------------------

#pragma hdrstop

#include "Estimates.h"
//---------------------------------------------------------------------------

#pragma once

vector<double> & Estimates::getPriorities() {
    return priorities;
}

vector< vector<int> > & Estimates::getRates() {
	return rates;
}
