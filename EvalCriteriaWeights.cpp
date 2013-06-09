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
		drawFixedColNames(Sender, ACol, ARow, Rect);
		return;
	}

	if (Sender == CriteriaEstimates && ACol > 1) {
		if (ARow < ACol) {
			CriteriaEstimates->Canvas->Brush->Color = cl3DLight;
			CriteriaEstimates->Canvas->FillRect(CriteriaEstimates->CellRect(ACol, ARow));
        }
	}
}

//---------------------------------------------------------------------------
void TEvalCriteriaWeightsForm::drawFixedColNames(TObject *Sender, int ACol, int ARow, TRect &Rect)
{
	if (ACol == 0 && ARow == 0) {
		return;
	}

	TStringGrid *grid = (TStringGrid*) Sender;

	if (ACol == 0) {
		UnicodeString &name = gridNames->at(ARow- 1);
		setColWidth(Sender, name);
		grid->Canvas->TextRect(Rect, name);
		return;
	}

	if (ARow == 0) {
		UnicodeString name = grid == CriteriaEstimates ? IntToStr(ACol) : gridNames->at(ACol- 1);

		setRowHeight(Sender, name);

		if (grid == PairWiseGrid) {
			grid->Canvas->Font->Orientation = 90 * 10;
			grid->Canvas->TextOut(Rect.Left, Rect.Bottom - 6, name);
		} else {
			grid->Canvas->TextOut(Rect.Left, Rect.Top + 6, name);
        }


	}
}
//---------------------------------------------------------------------------
void TEvalCriteriaWeightsForm::setRowHeight(TObject *Sender, UnicodeString &str)
{
	TStringGrid *grid = (TStringGrid*) Sender;

	int rowHeight = grid->RowHeights[0];
	int newRowHeight= grid->Canvas->TextWidth(str) + 6 + 6;
	int oldHeight = grid->RowHeights[0];
	grid->RowHeights[0] = rowHeight > newRowHeight ? rowHeight : newRowHeight;

//	int offset = grid->RowHeights[0] - oldHeight;
//	grid->Height += offset;
//	this->Height += offset;
//	NextButton->Top += offset;
//	BackButton->Top += offset;
}

//---------------------------------------------------------------------------
void TEvalCriteriaWeightsForm::setColWidth(TObject *Sender, UnicodeString &str, int col)
{
	TStringGrid *grid = (TStringGrid*) Sender;

	int columnWidth = grid->ColWidths[col];
	int newColumnWidth = grid->Canvas->TextWidth(str) + 6 + 6 + grid->GridLineWidth;
	grid->ColWidths[col] = columnWidth > newColumnWidth ? columnWidth : newColumnWidth;
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
		Application->MessageBoxW(L"ќценки не должны быть меньше 1", L"ќшибка",  MB_OK| MB_ICONERROR);
		CriteriaEstimates->Col = ACol;
		CriteriaEstimates->Row = ARow;
		CriteriaEstimates->Cells[ACol][ARow] = L"";
	} else {
		rates->at(ARow - 1)[ACol - 1] = StrToInt(Value);
		if (ARow == ACol) {
			const int value = rates->at(ARow - 1)[ACol - 1];
			for (int i = ACol; i < rates->size(); ++i) {
				rates->at(ARow - 1)[i] = value;
				CriteriaEstimates->Cells[i + 1][ARow] = IntToStr(value);
			}
		}
	}

	if (isDataFilled()) {
		eval();
    }
}
//---------------------------------------------------------------------------
void __fastcall TEvalCriteriaWeightsForm::FormShow(TObject *Sender)
{
	currentProject = &ProjectManager::getInstance().getCurrentProject();

	step = 0;

	setData();
}
//---------------------------------------------------------------------------

void __fastcall TEvalCriteriaWeightsForm::NextButtonClick(TObject *Sender)
{
	++step;

	const int size(currentProject->getMethod() == 0 ? 1 : currentProject->getCriteriaCount() + 1);
	if (step == size)
	{
		Close();
	} else {
        setData();
    }
}
//---------------------------------------------------------------------------


void __fastcall TEvalCriteriaWeightsForm::BackButtonClick(TObject *Sender)
{
	if (step > 0) {
		--step;
		setData();
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
//---------------------------------------------------------------------------
bool TEvalCriteriaWeightsForm::isDataFilled() {
	for (int i = 0; i < rates->size(); ++i)
	{
		vector<int> &v = rates->at(i);
		for (int j = 0; j < v.size(); ++j) {
			if (CriteriaEstimates->Cells[j + 1][i + 1] == L"") {
				return false;
			}
		}
	}

	return true;
}
//---------------------------------------------------------------------------
void TEvalCriteriaWeightsForm::eval() {
	const int size = rates->size();
	Matrix<double> m(size, size);
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			m[i][j] = rates->at(i)[j] / (double) rates->at(j)[i];
			UnicodeString result = Format(L"%.2f", &TVarRec(m[i][j]), 1);
			PairWiseGrid->Cells[j + 1][i + 1] = result;
			setColWidth(PairWiseGrid, result, j + 1);
		}
	}

	AHPSolver<int> ahpEstimates;
	ahpEstimates.setPairwiseComparationMatrix(m);
	Matrix<double> &eigen = ahpEstimates.getMaxEigenVectors();
	double sum(0);
	for (int i = 0; i < eigen.getHeight(); ++i)
	{
		sum += eigen[i][0];
	}

	vector<double> &estimates = currentProject->getCriteriaEstimates().getEstimates();
	estimates.clear();
	for (int i = 0; i < size; ++i)
	{
		estimates.push_back(eigen[i][0] / sum);
	}

	ConsistencLabel->Caption = Format(L"%.2f", &TVarRec(ahpEstimates.evaluatePairwiseConsistency()), 1);
}
//---------------------------------------------------------------------------
void __fastcall TEvalCriteriaWeightsForm::CriteriaEstimatesGetEditText(TObject *Sender,
		  int ACol, int ARow, UnicodeString &Value)
{
	if (ACol > 1 && ARow < ACol) {
		CriteriaEstimates->Row = ACol;
	}
}
//---------------------------------------------------------------------------
void TEvalCriteriaWeightsForm::setData() {
	int size;
	if (step == 0) {
		gridNames = &currentProject->getCriteriaNames();
		rates = &currentProject->getCriteriaEstimates().getRates();
		size = currentProject->getCriteriaCount() + 1;
	} else {
		vector<Estimates> &alternativeEstimates = currentProject->getAlternativeEstimates();
		Estimates &estiamtes = alternativeEstimates[step - 1];
		alternativeNames = vector<UnicodeString>();
		for (int i = 0; i < alternativeEstimates.size(); ++i) {
			alternativeNames.push_back(alternativeEstimates[i].getName());
		}

		gridNames = &alternativeNames;
		rates = &estiamtes.getRates();
        size = currentProject->getAlternativesCount() + 1;
	}

	const int n = rates->size() - 1;
	rates->at(n)[n] = 9;

	CriteriaEstimates->ColCount = size - 1;
	CriteriaEstimates->RowCount = size;

	PairWiseGrid->ColCount = size;
	PairWiseGrid->RowCount = size;

	for (int i = 0 ; i < rates->size(); ++i) {
		vector<int> &v = rates->at(i);
		for (int j = 0; j < v.size(); ++j) {
			PairWiseGrid->Cells[j + 1][i + 1] = L"";
			if (v[j] == 0) {
				CriteriaEstimates->Cells[j + 1][i + 1] = L"";
			} else {
				CriteriaEstimates->Cells[j + 1][i + 1] = IntToStr(v[j]);
				if (i == j) {
					for (int k = j; k < v.size(); ++k) {
						CriteriaEstimates->Cells[k + 1][i + 1] = IntToStr(v[j]);
					}
				}
			}
        }
	}

	if (isDataFilled()) {
        eval();
    }
}
