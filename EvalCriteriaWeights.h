//---------------------------------------------------------------------------

#ifndef EvalCriteriaWeightsH
#define EvalCriteriaWeightsH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>

#include <vector>
#include "boost/regex.hpp"
#include "ahpsolver.cpp"
#include "matrix.cpp"
#include "Project.cpp"
#include "ProjectManager.cpp"
//---------------------------------------------------------------------------

using std::vector;
using boost::wregex;

class TEvalCriteriaWeightsForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TStringGrid *CriteriaEstimates;
	TButton *BackButton;
	TButton *NextButton;
	TLabel *StepLabel;
	TLabel *ConsistencLabel;
	TStringGrid *PairWiseGrid;
	TLabel *Label2;
	void __fastcall CriteriaEstimatesDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall CriteriaEstimatesSetEditText(TObject *Sender, int ACol, int ARow,
          const UnicodeString Value);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall NextButtonClick(TObject *Sender);
	void __fastcall BackButtonClick(TObject *Sender);
private:	// User declarations
	void drawFixedColNames(int ACol, int ARow, TRect &Rect);
	void setRowHeight(UnicodeString &str);
	void setColWidth(UnicodeString &str);

	vector<UnicodeString> *gridNames;
	const wregex gridRegex;
	int step;
	bool *isBack;
	Project *currentProject;

public:		// User declarations
	__fastcall TEvalCriteriaWeightsForm(TComponent* Owner);
	void setBackPointer(bool *back);
};
//---------------------------------------------------------------------------
extern PACKAGE TEvalCriteriaWeightsForm *EvalCriteriaWeightsForm;
//---------------------------------------------------------------------------
#endif
