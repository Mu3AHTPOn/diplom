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
		inline vector<UnicodeString> &getAlternativeNames();
		inline vector<UnicodeString> &getCriteriaNames();
		inline vector<Estimates> & getAlternativeEstimates();
		inline Estimates &getCriteriaEstimates();
		inline int getMethod();
		inline void setMethod(int method);

		enum EstimateType { ALTERNATIVE, CRITERIA };

		void removeEstimate(int index, EstimateType type);
		void addEstimate(int index, UnicodeString name, EstimateType type);
		void moveEstimate(int from, int to, EstimateType type);

	private:
		void resetEstimates();
		vector<UnicodeString> criteriaNames;
		vector<UnicodeString> alternativeNames;
		UnicodeString name;
		vector<Estimates> alternativeEstimates;
		Estimates criteriaEstimates;
		int method;
};
#endif
