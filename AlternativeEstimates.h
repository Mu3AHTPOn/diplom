//---------------------------------------------------------------------------

#ifndef AlternativeEstimatesH
#define AlternativeEstimatesH
//---------------------------------------------------------------------------

#include <vcl.h>
#include <vector>

using std::vector;

class AlternativeEstimates {
	public:
		AlternativeEstimates() {}
		~AlternativeEstimates() {}
		AlternativeEstimates(UnicodeString name);
		inline UnicodeString & getName();
		inline vector<double> & getEstimates();

	private:
		vector<double> estimates;
		UnicodeString name;
};

#endif
