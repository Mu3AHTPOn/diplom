//---------------------------------------------------------------------------

#ifndef ProjectH
#define ProjectH
//---------------------------------------------------------------------------

#include <vcl.h>
#include <vector>

using std::vector;

class Project {
	public:
		Project();
		~Project();
		inline int getCriteriaCount();
		inline int getAlternativesCount();
		UnicodeString & getName();
		void setName(UnicodeString & name);
		inline vector<UnicodeString> & getAlternativeNames();
		inline vector<UnicodeString> & getCriteriaNames();

	private:
		vector<UnicodeString> criteriaNames, alternativeNames;
		UnicodeString name;
};
#endif
