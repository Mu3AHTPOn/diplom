//---------------------------------------------------------------------------

#ifndef ProjectH
#define ProjectH
//---------------------------------------------------------------------------

#include <vcl.h>
#include <vector>
#include "matrix.cpp"
#include "AlternativeEstimates.cpp"

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
		inline vector<AlternativeEstimates> & getAlternativeEstimates();

	private:
		vector<UnicodeString> criteriaNames;
		UnicodeString name;
		vector<AlternativeEstimates> alternativeEstimates;
};
#endif
