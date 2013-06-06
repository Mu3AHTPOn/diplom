//---------------------------------------------------------------------------

#pragma hdrstop

#include "AlternativeEstimates.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

AlternativeEstimates::AlternativeEstimates(UnicodeString name)
{
    this->name = name;
}

UnicodeString & AlternativeEstimates::getName() {
	return name;
}

vector<double> & AlternativeEstimates::getEstimates() {
    return estimates;
}
