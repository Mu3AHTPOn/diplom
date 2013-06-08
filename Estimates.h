//---------------------------------------------------------------------------

#ifndef EstimatesH
#define EstimatesH
//---------------------------------------------------------------------------

#include <vcl.h>
#include <vector>

using std::vector;

class Estimates {
	public:
		Estimates() {}
		~Estimates() {}
		Estimates(UnicodeString name);
		inline const UnicodeString & getName() const;
		inline void setName(UnicodeString name);
		inline vector<double> & getEstimates();
		inline vector< vector<int> > & getRates();

	private:
		vector< vector<int> > rates;
		vector<double> estimates;
		UnicodeString name;
};

#endif
