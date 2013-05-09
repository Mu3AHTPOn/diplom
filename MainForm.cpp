//---------------------------------------------------------------------------

#include <vcl.h>
#include <Clipbrd.hpp>

#pragma hdrstop

#include "MainForm.h"
#include "ahpsolver.cpp"
#include "matrix.cpp"
//---------------------------------------------------------------------------

#pragma resource "*.dfm"

// [\d]*(.|,)[\d]+
TForm1 *Form1;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::InputDataStringGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Shift.Contains(ssCtrl)&& Key == 86U) {      //paste event
		TClipboard *cb = Clipboard();
		wchar_t *str = new wchar_t[0];
		int len = cb->GetTextBuf(str, 0);
		str = new wchar_t[len];
		cb->GetTextBuf(str, len);
		UnicodeString uStr(str, len);

	}

	if (Shift.Contains(ssCtrl) && Key == 67U) { //copy event

	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
	activeCell.X = 1;
	activeCell.Y = 1;

	InputDataStringGrid->Cells[1][1] = "3";
	InputDataStringGrid->Cells[2][1] = "9";
	InputDataStringGrid->Cells[3][1] = "5";
	InputDataStringGrid->Cells[4][1] = "7";
	InputDataStringGrid->Cells[5][1] = "5";
	InputDataStringGrid->Cells[6][1] = "7";

	InputDataStringGrid->Cells[1][1+1] = "4";
	InputDataStringGrid->Cells[1][2+1] = "9";
	InputDataStringGrid->Cells[1][3+1] = "8";
	InputDataStringGrid->Cells[2][1+1] = "5";
	InputDataStringGrid->Cells[2][1+2] = "5";
	InputDataStringGrid->Cells[2][1+3] = "5";
	InputDataStringGrid->Cells[3][1+1] = "5";
	InputDataStringGrid->Cells[3][1+2] = "7";
	InputDataStringGrid->Cells[3][1+3] = "8";
	InputDataStringGrid->Cells[4][1+1] = "5";
	InputDataStringGrid->Cells[4][1+2] = "7";
	InputDataStringGrid->Cells[4][1+3] = "8";
	InputDataStringGrid->Cells[5][1+1] = "5";
	InputDataStringGrid->Cells[5][1+2] = "6";
	InputDataStringGrid->Cells[5][1+3] = "9";
	InputDataStringGrid->Cells[6][1+1] = "5";
	InputDataStringGrid->Cells[6][1+2] = "3";
	InputDataStringGrid->Cells[6][1+3] = "9";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::InputDataStringGridSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect)
{
	activeCell.X = ACol;
	activeCell.Y = ARow;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N8Click(TObject *Sender)
{
//создаём множество объектов ahpsolver для оценок важности критериев и для
//каждого критерия в частности

	const int criteriaCount = InputDataStringGrid->ColCount - 1;		//TODO create methods, responds for this
	const int objectCount = InputDataStringGrid->RowCount - 2;
	
	Matrix<int> criteriaEstimates(criteriaCount);
	
	Matrix<int> *objectEstimates = new Matrix<int>[criteriaCount];
	AHPSolver<int> *ahpObjects = new AHPSolver<int>[criteriaCount];
	
	for (int i = 0; i < criteriaCount; ++i) {
		criteriaEstimates[i][0] = InputDataStringGrid->Cells[i+1][1].ToInt();
		objectEstimates[i] = *(new Matrix<int>(objectCount));
	}

	AHPSolver<int> ahpCriteriaEstimates(criteriaEstimates);
	
	//fill objectEstimates[
	for (int i = 0; i < criteriaCount; ++i) {
		Matrix<int> &curr = objectEstimates[i];
		for (int j = 0; j < objectCount; ++j) {
			curr[j][0] = InputDataStringGrid->Cells[i+1][j+2].ToInt();
		}

		ahpObjects[i] = *(new AHPSolver<int>(curr));
	}

	Matrix<double> test(ahpObjects[0].getMaxEigenVectors());
	for(int i = 1; i < criteriaCount; ++i)
	{
		test.append(ahpObjects[i].getMaxEigenVectors());
	}

	Matrix<double> result = test * ahpCriteriaEstimates.getMaxEigenVectors();
	UnicodeString resultStr;
	
	
	for (int i = 0; i < result.getHeight(); ++i) {
		resultStr += FloatToStr(result[i][0]) + "\n";
	}

	MessageDlg(resultStr, mtInformation, mbOKCancel, 0);
}
//---------------------------------------------------------------------------

