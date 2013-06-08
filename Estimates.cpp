//---------------------------------------------------------------------------

#pragma hdrstop

#include "Estimates.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

Estimates::Estimates(UnicodeString name)
{
    this->name = name;
}

const UnicodeString & Estimates::getName() const {
	return name;
}

void Estimates::setName(UnicodeString name) {
    this->name = name;
}

vector<double> & Estimates::getEstimates() {
    return estimates;
}

vector< vector<int> > & Estimates::getRates() {
	return rates;
}
