//---------------------------------------------------------------------------

#ifndef EvalCriteriaWeightsH
#define EvalCriteriaWeightsH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <VCLTee.TeCanvas.hpp>

#include <vector>
#include "boost/regex.hpp"
#include "ahpsolver.cpp"
#include "matrix.cpp"
#include "Project.cpp"
#include "ProjectManager.cpp"
#include "Processor.cpp"
#include "UIManager.cpp"
//---------------------------------------------------------------------------

using std::vector;
using boost::wregex;

class TEvalCriteriaWeightsForm : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *CriteriaEstimates;
	TButton *BackButton;
	TButton *NextButton;
	TLabel *ConsistencLabel;
	TStringGrid *PairWiseGrid;
	TLabel *Label2;
	TLabel *Label1;
	TLabel *PrioritiesLabel;
	TLabel *Label3;
	TLabel *Label4;
	TMemo *ExplanationLabel;
	void __fastcall CriteriaEstimatesDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall CriteriaEstimatesSetEditText(TObject *Sender, int ACol, int ARow,
          const UnicodeString Value);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall NextButtonClick(TObject *Sender);
	void __fastcall BackButtonClick(TObject *Sender);
	void __fastcall CriteriaEstimatesGetEditText(TObject *Sender, int ACol, int ARow,
          UnicodeString &Value);
	void __fastcall CriteriaEstimatesSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect);
	void __fastcall FormHide(TObject *Sender);
private:	// User declarations
	void drawFixedColNames(TObject *Sender, int ACol, int ARow, TRect &Rect);
	void setRowHeight(TObject *Sender, UnicodeString &str);
	void setColWidth(TObject *Sender, UnicodeString &str, int col = 0);
	void setData();
	bool isDataFilled();
	void eval();

	vector<UnicodeString> *gridNames;
	double consistency;
	const double maxConsistency;
	Estimates *estimates;
	const wregex gridRegex;
	int step;
	bool *isBack;
	Project *currentProject;
	Processor processor;
    TColor emptyCellBG;
public:		// User declarations
	__fastcall TEvalCriteriaWeightsForm(TComponent* Owner);
	void setBackPointer(bool *back);
};
//---------------------------------------------------------------------------
extern PACKAGE TEvalCriteriaWeightsForm *EvalCriteriaWeightsForm;
//---------------------------------------------------------------------------
#endif
