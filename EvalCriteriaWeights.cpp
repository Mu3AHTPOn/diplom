//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EvalCriteriaWeights.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TEvalCriteriaWeightsForm *EvalCriteriaWeightsForm;
//---------------------------------------------------------------------------
__fastcall TEvalCriteriaWeightsForm::TEvalCriteriaWeightsForm(TComponent* Owner)
	: TForm(Owner), gridRegex(L"[\\d]+"), step(0), consistency(0.0),
	  maxConsistency(0.1)
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
		Application->MessageBoxW(L"Оценки не должны быть меньше 1", L"Ошибка",  MB_OK| MB_ICONERROR);
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
	if (consistency > maxConsistency) {
		UnicodeString str = L"Коэффициент согласованности должен быть менее " +
							FloatToStr(maxConsistency) +
							L". Попробуйте согласовать оценки";

		if (Application->MessageBoxW(
				str.w_str(),
				L"Внимание!",
				MB_YESNO | MB_ICONERROR) == IDYES)
	    {
			return;
        }
    }

	++step;

	const int method = currentProject->getMethod();
	const int size(method == MathMethods::WS ? 1 : currentProject->getCriteriaCount() + 1);
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
			if (CriteriaEstimates->Cells[j + 1][i + 1].IsEmpty()) {
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

	priorities->clear();
	for (int i = 0; i < size; ++i)
	{
		priorities->push_back(eigen[i][0] / sum);
	}

	ConsistencLabel->Font->Color = clBlack;
	consistency = ahpEstimates.evaluatePairwiseConsistency();
	ConsistencLabel->Caption = Format(L"%.2f", &TVarRec(consistency), 1);
	if (consistency > 0.1) {
		ConsistencLabel->Font->Color = clRed;
    }
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
		priorities = &currentProject->getCriteriaEstimates().getPriorities();
		ExplanationLabel->Caption = L"Задайте относительную важность критериев";
	} else {
		vector<Estimates> &alternativeEstimates = currentProject->getAlternativeEstimates();
		Estimates &estiamtes = alternativeEstimates[step - 1];
		priorities = &estiamtes.getPriorities();
		gridNames = &currentProject->getAlternativeNames();
		rates = &estiamtes.getRates();
		size = currentProject->getAlternativesCount() + 1;
		ExplanationLabel->Caption = L"Задайте относительную важность альтернатив";
	}

	const int n = rates->size() - 1;
	rates->at(n)[n] = 9;

	CriteriaEstimates->ColCount = size - 1;
	CriteriaEstimates->RowCount = size;

	CriteriaEstimates->Row = 1;
	CriteriaEstimates->Col = 1;
	CriteriaEstimates->Refresh();

	PairWiseGrid->ColCount = size;
	PairWiseGrid->RowCount = size;
	PairWiseGrid->RowHeights[0] = 10;      //сбрасываем высоту первой строки
	PairWiseGrid->Refresh();

	ConsistencLabel->Font->Color = clBlack;

	for (int i = 0 ; i < rates->size(); ++i) {
		vector<int> &v = rates->at(i);
		for (int j = 0; j < v.size(); ++j) {
			PairWiseGrid->Cells[j + 1][i + 1] = L"";
			const int val = v[j];
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

	CriteriaEstimates->SetFocus();
}

void __fastcall TEvalCriteriaWeightsForm::CriteriaEstimatesSelectCell(TObject *Sender,
          int ACol, int ARow, bool &CanSelect)
{
	if (step == 0) {
		if (ACol == 1) {
			ExplanationLabel->Caption = L"Задайте относительную важность критериев";
		} else {
			ExplanationLabel->Caption = L"Задайте относительную важность критериев без учёта критерия \"" +
			gridNames->at(ACol - 1) + L"\"";

			if (ACol > 2) {
				ExplanationLabel->Caption = ExplanationLabel->Caption + L" и предидущих";
            }
		}
	} else {
        if (ACol == 1) {
			ExplanationLabel->Caption = L"Задайте относительную важность альтернатив";
		} else {
			ExplanationLabel->Caption = L"Задайте относительную важность альтернатив без учёта альтернативы \"" +
			gridNames->at(ACol - 1) + L"\"";

			if (ACol > 2) {
				ExplanationLabel->Caption = ExplanationLabel->Caption + L" и предидущих";
            }
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TEvalCriteriaWeightsForm::FormHide(TObject *Sender)
{
	currentProject = NULL;
	priorities = NULL;
	rates = NULL;
	gridNames = NULL;
}
//---------------------------------------------------------------------------

