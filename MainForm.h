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

#include <vector>

using std::vector;
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *N1;
	TMenuItem *N2;
	TMenuItem *N3;
	TMenuItem *N4;
	TMenuItem *N6;
	TMenuItem *N7;
	TMenuItem *N8;
	TMenuItem *N11;
	TMenuItem *N13;
	TMenuItem *N9;
	TMenuItem *N10;
	TStringGrid *InputDataStringGrid;
	TToolBar *ToolBar2;
	TValueListEditor *Params;
	TRichEdit *ResultRichEdit;
	TMenuItem *N14;
	TPanel *Panel1;
	TSpeedButton *SpeedButton1;
	TSpeedButton *SpeedButton2;
	TSpeedButton *SpeedButton3;
	TSpeedButton *SpeedButton4;
	TSpeedButton *SpeedButton5;
	TComboBox *MethodComboBox;
	TPopupMenu *ResultsPopupMenu;
	TMenuItem *N5;
	void __fastcall InputDataStringGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall InputDataStringGridSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect);
	void __fastcall Memo1Change(TObject *Sender);
	void __fastcall Memo1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Memo1MouseEnter(TObject *Sender);
	void __fastcall N14Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ParamsValidate(TObject *Sender, int ACol, int ARow, const UnicodeString KeyName,
          const UnicodeString KeyValue);
	void __fastcall N5Click(TObject *Sender);
	void __fastcall N8Click(TObject *Sender);
	void __fastcall SpeedButton1Click(TObject *Sender);
	void __fastcall InputDataStringGridDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
	void __fastcall InputDataStringGridFixedCellClick(TObject *Sender, int ACol, int ARow);
	void __fastcall InputDataStringGridDblClick(TObject *Sender);




private:	// User declarations
	int getCriteriaCount();
	int getObjectsCount();
	TGridCoord activeCell;
	int fixedCols, fixedRows;
	UnicodeString lastParam;
	void evalAHP();
	void evalWS();
	void initGrid();
	void fillFixedCols();
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	vector<UnicodeString> *colNames, *rowNames;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
