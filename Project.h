//---------------------------------------------------------------------------

#ifndef ProjectH
#define ProjectH
//---------------------------------------------------------------------------

#include <vcl.h>
#include <vector>
#include "matrix.cpp"
#include "Estimates.cpp"

using std::vector;

class Project {
	public:
		Project();
		~Project();
		inline int getCriteriaCount();
		inline int getAlternativesCount();
		inline const UnicodeString & getName();
		void setName(UnicodeString name);
		inline vector<UnicodeString> getAlternativeNames();
		inline vector<UnicodeString> & getCriteriaNames();
		inline vector<Estimates> & getAlternativeEstimates();
		inline Estimates &getCriteriaEstimates();

	private:
		vector<UnicodeString> criteriaNames;
		UnicodeString name;
		vector<Estimates> alternativeEstimates;
		Estimates criteriaEstimates;
};
#endif
