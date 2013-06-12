//---------------------------------------------------------------------------

#ifndef MainFormH
#define MainFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.ValEdit.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
#include "ahpsolver.cpp"
#include "matrix.cpp"
#include <Vcl.Dialogs.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include "boost/regex.hpp"
#include <VCLTee.Series.hpp>
#include <vector>
#include "ProjectManager.cpp"
#include "Processor.cpp"

using std::vector;
using boost::wregex;
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *N1;
	TMenuItem *MMNewProject;
	TMenuItem *MMOPentProject;
	TMenuItem *MMCloseProject;
	TMenuItem *N6;
	TMenuItem *N8;
	TStringGrid *InputDataStringGrid;
	TRichEdit *ResultRichEdit;
	TMenuItem *MMCloseApp;
	TPanel *Panel1;
	TPopupMenu *ResultsPopupMenu;
	TMenuItem *N5;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	TMenuItem *MMEditProject;
	TMenuItem *MMSaveProject;
	TChart *Chart1;
	TBarSeries *Series1;
	TPanel *Panel2;
	TSpeedButton *NewProjectButton;
	TSpeedButton *SpeedButton3;
	TSpeedButton *SpeedButton2;
	TSpeedButton *SpeedButton5;
	TComboBox *MethodComboBox;
	TSpeedButton *SpeedButton1;
	void __fastcall InputDataStringGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Memo1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Memo1MouseEnter(TObject *Sender);
	void __fastcall MMCloseAppClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall N5Click(TObject *Sender);
	void __fastcall N8Click(TObject *Sender);
	void __fastcall SpeedButton1Click(TObject *Sender);
	void __fastcall InputDataStringGridDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
	void __fastcall InputDataStringGridDblClick(TObject *Sender);
	void __fastcall MMNewProjectClick(TObject *Sender);
	void __fastcall SpeedButton2Click(TObject *Sender);
	void __fastcall SpeedButton3Click(TObject *Sender);
	void __fastcall NewProjectButtonClick(TObject *Sender);
	void __fastcall SpeedButton5Click(TObject *Sender);
	void __fastcall MMCloseProjectClick(TObject *Sender);
	void __fastcall MMOPentProjectClick(TObject *Sender);
	void __fastcall MMSaveProjectClick(TObject *Sender);
	void __fastcall MMEditProjectClick(TObject *Sender);
	void __fastcall InputDataStringGridSetEditText(TObject *Sender, int ACol, int ARow,
          const UnicodeString Value);
	void __fastcall Chart1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall Chart1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall Chart1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall MethodComboBoxSelect(TObject *Sender);


private:	// User declarations
	void evalAHP();
	void evalWS();
	void showResultAtChart(Matrix<double> &result);
	void initGrid();
	void drawFixedColNames(int ACol, int ARow, TRect &Rect);
	void setRowHeight(UnicodeString &str);
	void setColWidth(UnicodeString &str, int col = 0);
	bool showSaveDialog();
	bool isDataValid();
	void isOnChartBorder(int X, int Y);
	void changeCursor(int X, int Y);
	inline int getCriteriaCount();
	inline int getAlternativesCount();
	void showCurrentProject();
	void showResult(Matrix<double> &result, UnicodeString method);

	void newProject();
	void loadProject();
	void saveProject();
	bool closeProject();

	int fixedCols, fixedRows;
	UnicodeString lastParam;
	const wregex gridRegex, floatGridRegex;
	ProjectManager &projectManager;
	bool isOnChartButtonPresssed;
	TPoint lastChartMousePoint;
	bool isLeft, isRight, isTop, isBottom;
	Processor processor;
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
