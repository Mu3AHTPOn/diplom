//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EvalCriteriaWeights.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TEvalCriteriaWeightsForm *EvalCriteriaWeightsForm;
//---------------------------------------------------------------------------
__fastcall TEvalCriteriaWeightsForm::TEvalCriteriaWeightsForm(TComponent* Owner)
	: TForm(Owner), gridRegex(L"[\\d]+"), step(0)
{
	EvalCriteriaWeightsForm = this;
}
//---------------------------------------------------------------------------
void __fastcall TEvalCriteriaWeightsForm::CriteriaEstimatesDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State)
{
	if (ACol == 0 || ARow == 0) {
		drawFixedColNames(ACol, ARow, Rect);
		return;
	}

	if (ARow == 1 && ACol > 0 && ACol <= step)
	{
		CriteriaEstimates->Canvas->Brush->Color = cl3DLight;
		CriteriaEstimates->Canvas->FillRect(CriteriaEstimates->CellRect(ACol, ARow));
	}

//	if (ARow == 1 && ! (CriteriaEstimates->Row == ARow && CriteriaEstimates->Col == ACol))
//	{
//		CriteriaEstimates->Canvas->Brush->Color = cl3DLight;
//		CriteriaEstimates->Canvas->FillRect(CriteriaEstimates->CellRect(ACol, ARow));
//		UnicodeString str = ACol == 0 ? UnicodeString(L"Важность критериев") : CriteriaEstimates->Cells[ACol][ARow];
//		CriteriaEstimates->Canvas->TextOut(Rect.Left + 2, Rect.Top + 4, str);
//    }
}

void TEvalCriteriaWeightsForm::setColNamesArray(vector<UnicodeString> *inVector)
{
	this->colNames = inVector;
	userEstimates = new int * [inVector->size()];
}
//---------------------------------------------------------------------------
void TEvalCriteriaWeightsForm::drawFixedColNames(int ACol, int ARow, TRect &Rect)
{
	if (ACol == 0 && ARow == 0) {
		return;
	}

	if (ARow == 1) {
		UnicodeString name = L"Важность критериев";
		setColWidth(name);
		CriteriaEstimates->Canvas->TextRect(Rect, name);
		return;
	}

	if (ARow == 0) {
		UnicodeString &name = colNames->at(ACol- 1);
        setRowHeight(name);

		CriteriaEstimates->Canvas->Font->Orientation = 90 * 10;
		CriteriaEstimates->Canvas->TextOut(Rect.Left, Rect.Bottom - 6, name);
	}
}
//---------------------------------------------------------------------------
void TEvalCriteriaWeightsForm::setRowHeight(UnicodeString &str)
{
	int rowHeight = CriteriaEstimates->RowHeights[0];
	int newRowHeight= CriteriaEstimates->Canvas->TextWidth(str) + 6 + 6;
	int oldHeight = CriteriaEstimates->RowHeights[0];
	CriteriaEstimates->RowHeights[0] = rowHeight > newRowHeight ? rowHeight : newRowHeight;

	int offset = CriteriaEstimates->RowHeights[0] - oldHeight;
	CriteriaEstimates->Height += offset;
	this->Height += offset;
	NextButton->Top += offset;
	BackButton->Top += offset;
}
//---------------------------------------------------------------------------
void TEvalCriteriaWeightsForm::setColWidth(UnicodeString &str)
{
	int columnWidth = CriteriaEstimates->ColWidths[0];
	int newColumnWidth = CriteriaEstimates->Canvas->TextWidth(str) + 6 + 6 + CriteriaEstimates->GridLineWidth;
	CriteriaEstimates->ColWidths[0] = columnWidth > newColumnWidth ? columnWidth : newColumnWidth;
}
//---------------------------------------------------------------------------
void __fastcall TEvalCriteriaWeightsForm::CriteriaEstimatesSetEditText(TObject *Sender,
          int ACol, int ARow, const UnicodeString Value)
{
	if (! regex_match(Value.w_str(), gridRegex))
		{
			UnicodeString str = CriteriaEstimates->Cells[ACol][ARow];
			CriteriaEstimates->Cells[ACol][ARow] = str.SubString(1, str.Length() - 1);
		} else if (StrToInt(Value) < 1)
		{
			MessageDlg(L"Оценки не должны быть меньше 1", mtError, TMsgDlgButtons() << mbOK, 0);
			CriteriaEstimates->Col = ACol;
			CriteriaEstimates->Row = ARow;
			CriteriaEstimates->Cells[ACol][ARow] = L"";
		}
}
//---------------------------------------------------------------------------
void TEvalCriteriaWeightsForm::setCriteriaEstimatesArray(vector<double> *inVector)
{
	criteriaEstimates = inVector;
}
//---------------------------------------------------------------------------
void __fastcall TEvalCriteriaWeightsForm::FormShow(TObject *Sender)
{
	CriteriaEstimates->ColCount = colNames->size() + 1;
	UnicodeString str = UnicodeString(L"Важность критериев");
	setColWidth(str);
	step = 0;

//		CriteriaEstimates->Cells[1][1] = L"3";
//		CriteriaEstimates->Cells[2][1] = L"9";
//		CriteriaEstimates->Cells[3][1] = L"5";
//		CriteriaEstimates->Cells[4][1] = L"7";
//		CriteriaEstimates->Cells[5][1] = L"5";
//		CriteriaEstimates->Cells[6][1] = L"7";
}
//---------------------------------------------------------------------------

void __fastcall TEvalCriteriaWeightsForm::NextButtonClick(TObject *Sender)
{
	const int size = colNames->size();
	userEstimates[step] = new int[size];
	for (int i = 0; i < step; ++i)
	{
		userEstimates[step][i] = userEstimates[step - 1][i];
	}

	for (int i = step; i < colNames->size(); ++i)
	{
		userEstimates[step][i] = StrToInt(CriteriaEstimates->Cells[i + 1][1]);
		CriteriaEstimates->Cells[i + 1][1] = L"";
	}

	++step;
	CriteriaEstimates->Col = step + 1;
	CriteriaEstimates->FixedCols = step + 1;
	CriteriaEstimates->SetFocus();

//	if (step == 1)
//	{
//		CriteriaEstimates->Cells[2][1] = L"9";
//		CriteriaEstimates->Cells[3][1] = L"7";
//		CriteriaEstimates->Cells[4][1] = L"6";
//		CriteriaEstimates->Cells[5][1] = L"5";
//		CriteriaEstimates->Cells[6][1] = L"7";
//	} else if (step == 2)
//	{
//		CriteriaEstimates->Cells[3][1] = L"9";
//		CriteriaEstimates->Cells[4][1] = L"7";
//		CriteriaEstimates->Cells[5][1] = L"7";
//		CriteriaEstimates->Cells[6][1] = L"7";
//	} else if (step == 3) {
//		CriteriaEstimates->Cells[4][1] = L"7";
//		CriteriaEstimates->Cells[5][1] = L"9";
//		CriteriaEstimates->Cells[6][1] = L"8";
//	} else if (step == 4) {
//		CriteriaEstimates->Cells[5][1] = L"9";
//		CriteriaEstimates->Cells[6][1] = L"7";
//    }


	if (step == size - 1)
	{
		userEstimates[step] = new int[size];
        for (int i = 0; i < step; ++i)
		{
			userEstimates[step][i] = userEstimates[step - 1][i];
		}

		userEstimates[step][step] = 9;           // no matter

		Matrix<double> m(size, size);
		for (int i = 0; i < size; ++i)
		{
			for (int j = 0; j < size; ++j)
			{
				m[i][j] = userEstimates[j][i] / userEstimates[i][j];
            }
        }

		AHPSolver<int> *ahpCriteriaEstimates = new AHPSolver<int>();
		ahpCriteriaEstimates->setPairwiseComparationMatrix(m);
		Matrix<double> &eigen = ahpCriteriaEstimates->getMaxEigenVectors();
		double sum(0);
		for (int i = 0; i < eigen.getHeight(); ++i)
		{
			sum += eigen[i][0];
		}

		criteriaEstimates->clear();
		for (int i = 0; i < size; ++i)
		{
			criteriaEstimates->push_back(eigen[i][0] / sum);
		}

//		delete [] ahpCriteriaEstimates;           //error
		Close();
	}
}
//---------------------------------------------------------------------------


void __fastcall TEvalCriteriaWeightsForm::BackButtonClick(TObject *Sender)
{
	if (step > 0) {
		--step;
		CriteriaEstimates->FixedCols = step + 1;
		for (int i = step; i < colNames->size(); ++i)
		{
			CriteriaEstimates->Cells[i + 1][1] = userEstimates[step][i];
		}
	} else {
		(*isBack) = true;
        Close();
    }
}
//---------------------------------------------------------------------------
void TEvalCriteriaWeightsForm::setBackPointer(bool *back)
{
    isBack = back;
}

