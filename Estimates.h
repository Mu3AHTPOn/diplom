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
		inline vector<double> & getPriorities();
		inline vector< vector<int> > & getRates();

	private:
		vector< vector<int> > rates;
		vector<double> priorities;
};

#endif
