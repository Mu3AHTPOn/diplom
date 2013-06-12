//---------------------------------------------------------------------------

#ifndef ProcessorH
#define ProcessorH
//---------------------------------------------------------------------------

#include "matrix.cpp"
#include "Project.cpp"
#include "ProjectManager.cpp"
#include "Estimates.cpp"
#include "ahpsolver.cpp"
#include <vector>

using std::vector;

class Processor {
	public:
		enum MathMethods { WS, AHP };
		AHPSolver<int> *evalAHP(Estimates &estimates);
		Matrix<double> *evalPairwiseMatrix(vector< vector<int> > &rates);
		double evalConsistency(Estimates &estimates);
		Matrix<double> &evalWS();
};
#endif
