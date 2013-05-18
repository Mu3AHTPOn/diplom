//---------------------------------------------------------------------------

#include <vcl.h>
#include <Clipbrd.hpp>
#include <limits>
#include "boost/regex.hpp"
#include "boost/format.hpp"

#pragma hdrstop

#include "MainForm.h"
#include "ahpsolver.cpp"
#include "matrix.cpp"
#include "UIManager.cpp"
//---------------------------------------------------------------------------

using namespace boost;

#pragma resource "*.dfm"

// [\d]*(.|,)[\d]+
TForm1 *Form1;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	Form1 = this;
	UIManager::getInstance()->addForm(Form1);
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
		int i = 0, j = activeCell.X, k = activeCell.Y;
		UnicodeString pasteString;
		while(str[i])
		{
			if (str[i] == '\t') {
				InputDataStringGrid->Cells[j][k] = pasteString;
				pasteString = "";

				++i;
				++j;
			}
			if (str[i] == '\n') {
				InputDataStringGrid->Cells[j][k] = pasteString;
				pasteString = "";

				++i;
				j = activeCell.X;
				++k;
			}

			pasteString += str[i];
			++i;
		}

		InputDataStringGrid->Cells[j][k] = pasteString;

	} else if (Shift.Contains(ssCtrl) && Key == 67U) { //copy event

	} else if (Key == 46U) {
		InputDataStringGrid->Cells[activeCell.X][activeCell.Y] = "";
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{

	fixedCols = InputDataStringGrid->FixedCols;
	fixedRows = InputDataStringGrid->FixedRows;

	activeCell.X = 1;
	activeCell.Y = 1;

	int gridSize = std::numeric_limits<short int>::max();
	InputDataStringGrid->ColCount = gridSize;
	InputDataStringGrid->RowCount = gridSize;

	InputDataStringGrid->Cells[0][1] = L"Важность критериев";
	int textWidth, temp(0);
	textWidth = InputDataStringGrid->Canvas->TextWidth(InputDataStringGrid->Cells[0][1]);

	for (int i = 1; i < gridSize; i++) {
		UnicodeString rowName = L"Объект " + IntToStr(i);
		InputDataStringGrid->Cells[0][i - 1] = rowName;
		temp = InputDataStringGrid->Canvas->TextWidth(rowName);
		textWidth = temp > textWidth ? temp : textWidth;
	}

	InputDataStringGrid->ColWidths[0] = textWidth + 10;

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

	const int criteriaCount = getCriteriaCount();		//TODO create methods, responds for this
	const int objectCount = getObjectsCount();
	
	Matrix<int> criteriaEstimates(criteriaCount);
	
	Matrix<int> *objectEstimates = new Matrix<int>[criteriaCount];
	AHPSolver<int> *ahpObjects = new AHPSolver<int>[criteriaCount];
	
	for (int i = 0; i < criteriaCount; ++i) {
		criteriaEstimates[i][0] = InputDataStringGrid->Cells[i+fixedCols][fixedRows].ToInt();
		objectEstimates[i] = *(new Matrix<int>(objectCount));
	}

	AHPSolver<int> ahpCriteriaEstimates(criteriaEstimates);
	
	//fill objectEstimates[
	TStringGrid *test = InputDataStringGrid;
	for (int i = 0; i < criteriaCount; ++i) {
		Matrix<int> &curr = objectEstimates[i];
		for (int j = 0; j < objectCount; ++j) {
			curr[j][0] = InputDataStringGrid->Cells[i + fixedCols][j + fixedRows + 1].ToInt();
		}

		ahpObjects[i] = *(new AHPSolver<int>(curr));
	}

	Matrix<double> integrEstimate(ahpObjects[0].getMaxEigenVectors());
	for(int i = 1; i < criteriaCount; ++i)
	{
		integrEstimate.append(ahpObjects[i].getMaxEigenVectors());
	}

	Matrix<double> result = integrEstimate * ahpCriteriaEstimates.getMaxEigenVectors();
	UnicodeString resultStr = L"(";
//	TVarRec *args = new TVarRec[result.getHeight()];

	for (int i = 0; i < result.getHeight() - 1; ++i) {
		resultStr += Format(L"%.3f, ", new TVarRec(result[i][0]), 1);
//		double t = ;
//		args[i] = 0.0;
//		args[i].VExtended[0] = t;
	}

//	args[result.getHeight() - 1] = result[result.getHeight() - 1][0];
	resultStr += Format(L"%.3f)", new TVarRec(result[result.getHeight() - 1][0]), 1);

	UnicodeString resultCaption = L"Расчитанный рейтинг\n";
	ResultRichEdit->Text = resultCaption;
	UnicodeString resultVector = resultStr;
	ResultRichEdit->Text = ResultRichEdit->Text + resultVector;

//	delete [] args;
//	MessageDlg(resultStr, mtInformation, mbOKCancel, 0);
}

int TForm1::getCriteriaCount()
{
	int i(fixedCols);
	while (InputDataStringGrid->Cells[i][fixedRows] != L"")
	{
		++i;
	}

	return i - fixedCols;

}

int TForm1::getObjectsCount()
{
	const int criteriaEstimates = 1;
	int i(fixedRows + criteriaEstimates);
	while (InputDataStringGrid->Cells[fixedCols][i] != L"")
	{
		++i;
	}

	return i - fixedRows - criteriaEstimates;

}
//---------------------------------------------------------------------------




void __fastcall TForm1::Memo1Change(TObject *Sender)
{
//	Memo1->HideSelection = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Memo1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	HideCaret(ResultRichEdit->Handle);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Memo1MouseEnter(TObject *Sender)
{
	   HideCaret(ResultRichEdit->Handle);

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TForm1::N14Click(TObject *Sender)
{
	UIManager::getInstance()->closeApp(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	UIManager::getInstance()->closeApp(this);
}
//---------------------------------------------------------------------------





void __fastcall TForm1::ParamsValidate(TObject *Sender, int ACol, int ARow, const UnicodeString KeyName,
		  const UnicodeString KeyValue)
{
	wregex expr(L"[\\d]+");
	wchar_t *t = KeyValue.w_str();
	bool b = regex_match(t, expr);
	if (! b)
	{
		UnicodeString str = L"Значение должно быть целым числом";
		MessageDlg(str, mtError, mbOKCancel, 0);
		Params->Values[KeyName] = L"5";
	} else {
		if (ARow == 0) {
			InputDataStringGrid->ColCount = StrToInt(KeyValue);
		} else if (ARow == 1)
		{
			InputDataStringGrid->RowCount = StrToInt(KeyValue);
		}
	}

//	UnicodeString key = Params->Keys[ARow];
//	if (lastParam != Params->Values[key]) {
//		lastParam = L"88";
//		Params->Values[key] = L"88";
//	}
//
//	int *n = new int();
}
//---------------------------------------------------------------------------



