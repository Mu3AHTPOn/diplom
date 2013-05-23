//---------------------------------------------------------------------------

#include <vcl.h>
#include <Clipbrd.hpp>
#include <limits>
#include "boost/regex.hpp"
#include "boost/format.hpp"

#pragma hdrstop

#include "MainForm.h"
#include "UIManager.cpp"
#include "setColRowNameFormUnit.cpp"
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

void TForm1::initGrid()
{
	InputDataStringGrid->ColCount = StrToInt(Params->Values[Params->Keys[0]]) + fixedCols;
	InputDataStringGrid->RowCount = StrToInt(Params->Values[Params->Keys[1]]) + fixedRows + 1;

	InputDataStringGrid->Cells[0][fixedRows] = L"Важность критериев";
	int textWidth, temp(0);
	textWidth = InputDataStringGrid->Canvas->TextWidth(InputDataStringGrid->Cells[0][fixedRows]);

	for (int i = fixedRows + 1; i < InputDataStringGrid->RowCount + 1; i++) {
		UnicodeString rowName = L"Объект " + IntToStr(i - fixedRows);
		InputDataStringGrid->Cells[0][i] = rowName;
		temp = InputDataStringGrid->Canvas->TextWidth(rowName);
		textWidth = temp > textWidth ? temp : textWidth;
	}

	InputDataStringGrid->ColWidths[0] = textWidth + 10;
}

void __fastcall TForm1::FormCreate(TObject *Sender)
{

	MethodComboBox->ItemIndex = 0;

	fixedCols = InputDataStringGrid->FixedCols;
	fixedRows = InputDataStringGrid->FixedRows;

	initGrid();

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
	if (! regex_match(KeyValue.w_str(), expr))
	{
		UnicodeString str = L"Значение должно быть целым числом";
		MessageDlg(str, mtError, mbOKCancel, 0);
		Params->Values[KeyName] = L"5";
	} else {
		initGrid();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N5Click(TObject *Sender)
{
	ResultRichEdit->Text = L"";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N8Click(TObject *Sender)
{
	if (MethodComboBox->ItemIndex == 0) {
	   evalAHP();
	} else
	{
		evalWS();
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SpeedButton1Click(TObject *Sender)
{
	if (MethodComboBox->ItemIndex == 0) {
	   evalAHP();
	} else
	{
		evalWS();
	}
}

void TForm1::evalAHP()
{
//создаём множество объектов ahpsolver для оценок важности критериев и для
//каждого критерия в частности
try {
	const int criteriaCount = getCriteriaCount();
	const int objectCount = getObjectsCount();

	Matrix<int> criteriaEstimates(criteriaCount);

	Matrix<int> *objectEstimates = new Matrix<int>[criteriaCount];
	AHPSolver<int> *ahpObjects = new AHPSolver<int>[criteriaCount];

	for (int i = 0; i < criteriaCount; ++i) {
		criteriaEstimates[i][0] = InputDataStringGrid->Cells[i+fixedCols][fixedRows].ToInt();
		objectEstimates[i] = *(new Matrix<int>(objectCount));
	}

	AHPSolver<int> ahpCriteriaEstimates(criteriaEstimates);

	//fill objectEstimates
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

	for (int i = 0; i < result.getHeight() - 1; ++i) {
		resultStr += Format(L"%.3f, ", new TVarRec(result[i][0]), 1);
	}

	resultStr += Format(L"%.3f)", new TVarRec(result[result.getHeight() - 1][0]), 1);

	UnicodeString resultCaption = L"Расчитанный рейтинг\n";
	ResultRichEdit->Text = resultCaption;
	UnicodeString resultVector = resultStr;
	ResultRichEdit->Text = ResultRichEdit->Text + resultVector;
	} catch (...)        //faken Embarcadero!
	{

    }
}

void TForm1::evalWS()
{
	const int criteriaCount = getCriteriaCount();
	const int objectCount = getObjectsCount();

	Matrix<int> criteriaEstimates(criteriaCount);
	Matrix<int> objectEstimates(objectCount, criteriaCount);

	for (int i = 0; i < criteriaCount; ++i) {
		criteriaEstimates[i][0] = InputDataStringGrid->Cells[i+fixedCols][fixedRows].ToInt();
	}

 	//fill objectEstimates
	TStringGrid *test = InputDataStringGrid;
	for (int i = 0; i < criteriaCount; ++i) {
		for (int j = 0; j < objectCount; ++j) {
			objectEstimates[j][i] = InputDataStringGrid->Cells[i + fixedCols][j + fixedRows + 1].ToInt();
		}
	}

	Matrix<int> &result = objectEstimates * criteriaEstimates;

	UnicodeString resultStr = L"(";

	for (int i = 0; i < result.getHeight() - 1; ++i) {
		resultStr += Format(L"%.d, ", new TVarRec(result[i][0]), 1);
	}

	resultStr += Format(L"%.d)", new TVarRec(result[result.getHeight() - 1][0]), 1);

	UnicodeString resultCaption = L"Расчитанный рейтинг\n";
	ResultRichEdit->Text = resultCaption;
	UnicodeString resultVector = resultStr;
	ResultRichEdit->Text = ResultRichEdit->Text + resultVector;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::InputDataStringGridDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State)
{
//	if (ARow == 0) {
//		UnicodeString fakenStr = L"sdfsdf";
////		InputDataStringGrid->Canvas->Brush->Color = clBlack;
//		InputDataStringGrid->Canvas->TextOutW(Rect.TopLeft().x, Rect.TopLeft().y, InputDataStringGrid->Cells[ACol][ARow]);
////		InputDataStringGrid->Canvas->Brush->Color = clBtnFace;
////		InputDataStringGrid->Canvas->FillRect(Rect);
//	}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::InputDataStringGridFixedCellClick(TObject *Sender, int ACol,
		  int ARow)
{
//	InputDataStringGrid->
}
//---------------------------------------------------------------------------


void __fastcall TForm1::InputDataStringGridDblClick(TObject *Sender)
{
//	const int top = InputDataStringGrid->BoundsRect.Top + Form1->Top;
//	const int left = InputDataStringGrid->Left + Form1->Left;
//	cursorPoint(Mouse->CursorPos.x - left, Mouse->CursorPos.y - top);
//	delete[] tagCursorPoint;
	const int cols = InputDataStringGrid->ColCount;
	const int rows = InputDataStringGrid->RowCount;
	TPoint cursorPoint = InputDataStringGrid->ScreenToClient(Mouse->CursorPos);
	for (int i = 0; i < cols; i++) {
		TRect rect = InputDataStringGrid->CellRect(i, 0);
		if (rect.Contains(cursorPoint)) {
			UnicodeString newName;
			TsetCollRowNameForm *form = new TsetCollRowNameForm(this);
			form->setResultStr(&newName);
			try {
			  form->ShowModal();
			  InputDataStringGrid->Cells[i][0] = newName;
			} __finally {
			  form->Free();
			}
			return;
		}
	}
	for (int j = 0; j < rows; j++) {
	}
}
//---------------------------------------------------------------------------

