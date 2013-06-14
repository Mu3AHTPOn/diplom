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
	  maxConsistency(0.1), emptyCellBG(RGBA(255, 254, 198, 0))
{
	EvalCriteriaWeightsForm = this;
}
//---------------------------------------------------------------------------
//����� ����� ���������/����������� � ����������� ������, � ������ ������ ������
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

	if (CriteriaEstimates->Cells[ACol][ARow].IsEmpty() && ARow >= ACol) {
		//����������� ������ ������ ����� ������
		if (UIManager::getInstance().getIndicator()) {
			CriteriaEstimates->Canvas->Brush->Color = emptyCellBG;
			CriteriaEstimates->Canvas->FillRect(CriteriaEstimates->CellRect(ACol, ARow));
        }
	}
}

//---------------------------------------------------------------------------
//����� ����� �� ������� ������ � ������� ������ ���������
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
//������������� ������ �����
void TEvalCriteriaWeightsForm::setRowHeight(TObject *Sender, UnicodeString &str)
{
	TStringGrid *grid = (TStringGrid*) Sender;

	int rowHeight = grid->RowHeights[0];
	int newRowHeight= grid->Canvas->TextWidth(str) + 6 + 6;
	int oldHeight = grid->RowHeights[0];
	grid->RowHeights[0] = rowHeight > newRowHeight ? rowHeight : newRowHeight;
}

//---------------------------------------------------------------------------
//������������� ������ ��������
void TEvalCriteriaWeightsForm::setColWidth(TObject *Sender, UnicodeString &str, int col)
{
	TStringGrid *grid = (TStringGrid*) Sender;

	int columnWidth = grid->ColWidths[col];
	int newColumnWidth = grid->Canvas->TextWidth(str) + 6 + 6 + grid->GridLineWidth;
	grid->ColWidths[col] = columnWidth > newColumnWidth ? columnWidth : newColumnWidth;
}
//---------------------------------------------------------------------------
//��������� ������������ �����
void __fastcall TEvalCriteriaWeightsForm::CriteriaEstimatesSetEditText(TObject *Sender,
          int ACol, int ARow, const UnicodeString Value)
{
	if (! regex_match(Value.w_str(), gridRegex))
	{
		UnicodeString str = CriteriaEstimates->Cells[ACol][ARow];
		CriteriaEstimates->Cells[ACol][ARow] = str.SubString(1, str.Length() - 1);
	} else if (StrToInt(Value) < 1)
	{
		Application->MessageBoxW(L"������ �� ������ ���� ������ 1", L"������",  MB_OK| MB_ICONERROR);
		CriteriaEstimates->Col = ACol;
		CriteriaEstimates->Row = ARow;
		CriteriaEstimates->Cells[ACol][ARow] = L"";
	} else {
		vector< vector<int> > &rates = estimates->getRates();
		rates[ARow - 1][ACol - 1] = StrToInt(Value);
		//���� ���� ��������� ������, ����������� �� ������� ���������
		//�� ��������� ��� ����� ���� ���������, ��� ��������� �����������
		//��� ��� ������� ������
		if (ARow == ACol) {
			const int value = rates[ARow - 1][ACol - 1];
			for (int i = ACol; i < rates.size(); ++i) {
				rates[ARow - 1][i] = value;
				CriteriaEstimates->Cells[i + 1][ARow] = IntToStr(value);
			}
		}
	}

	//���� ��� ������� ��������� - ���������� ������
	if (isDataFilled()) {
		eval();
    }
}
//---------------------------------------------------------------------------
//�������� ����� � �������� ��������� ��� � ������
void __fastcall TEvalCriteriaWeightsForm::FormShow(TObject *Sender)
{
	currentProject = &ProjectManager::getInstance().getCurrentProject();

	step = 0;

	setData();
}
//---------------------------------------------------------------------------
//������� �� ������ �����
void __fastcall TEvalCriteriaWeightsForm::NextButtonClick(TObject *Sender)
{
	if (! isDataFilled()) {
		Application->MessageBoxW(
			L"������� ������ �� ���������! ��������� �, ��� �� ����������!",
			L"������!",
			MB_OK | MB_ICONERROR
		);
		return;
    }

	//���� ����������� ��������������� ������ ����� - ���������� ��������������
	if (consistency > maxConsistency) {
		UnicodeString str = L"����������� ��������������� ������ ���� ����� " +
							Format(L"%.2f%%", &TVarRec(maxConsistency * 100), 1) +
							L". ���������� ����������� ������";

		if (Application->MessageBoxW(
				str.w_str(),
				L"��������!",
				MB_YESNO | MB_ICONERROR) == IDYES)
	    {
			return;
        }
    }

	++step;

	//��������� ���������� �� �����
	const int method = currentProject->getMethod();
	const int size(method == Processor::WS ? 1 : currentProject->getCriteriaCount() + 1);
	if (step == size)
	{
		Close();
	} else {
		//��������� ����������� ����� �����, ���� ��� ����
        setData();
    }
}
//---------------------------------------------------------------------------
//������ �����
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
//�������� ������� �� �������������
bool TEvalCriteriaWeightsForm::isDataFilled() {
	vector< vector<int> > &rates = estimates->getRates();
	for (int i = 0; i < rates.size(); ++i)
	{
		vector<int> &v = rates[i];
		for (int j = 0; j < v.size(); ++j) {
			if (CriteriaEstimates->Cells[j + 1][i + 1].IsEmpty()) {
				return false;
			}
		}
	}

	return true;
}
//---------------------------------------------------------------------------
//������ ��������� �������� � �������������� ���
void TEvalCriteriaWeightsForm::eval() {
	const int size = estimates->getRates().size();
	Matrix<double> *m = processor.evalPairwiseMatrix(estimates->getRates());
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			UnicodeString result = Format(L"%.2f", &TVarRec((*m)[i][j]), 1);
			PairWiseGrid->Cells[j + 1][i + 1] = result;
			setColWidth(PairWiseGrid, result, j + 1);
		}
	}

	delete m;

	AHPSolver<int> *ahpEstimates = processor.evalAHP(*estimates);

	UnicodeString resultStr = L"(";

    vector<double> &priorities = estimates->getPriorities();
	for (int i = 0; i < size - 1; ++i) {
		resultStr += Format(L"%.3f; ", new TVarRec(priorities[i]), 1);
	}

	resultStr += Format(L"%.3f)", new TVarRec(priorities[size - 1]), 1);
	PrioritiesLabel->Caption = resultStr;

	ConsistencLabel->Font->Color = clBlack;
	consistency = ahpEstimates->evaluatePairwiseConsistency();
	ConsistencLabel->Caption = Format(L"%.2f%%", &TVarRec(consistency * 100), 1);
	if (consistency > 0.1) {
		ConsistencLabel->Font->Color = clRed;
	}

	delete ahpEstimates;
}
//---------------------------------------------------------------------------
//��� ������� ������ � ����������� ������ ��������� � �����������
void __fastcall TEvalCriteriaWeightsForm::CriteriaEstimatesGetEditText(TObject *Sender,
		  int ACol, int ARow, UnicodeString &Value)
{
	if (ACol > 1 && ARow < ACol) {
		CriteriaEstimates->Row = ACol;
	}
}
//---------------------------------------------------------------------------
//����������� ���������� �� �����
void TEvalCriteriaWeightsForm::setData() {
	int size;
	if (step == 0) {
		gridNames = &currentProject->getCriteriaNames();
		estimates= &currentProject->getCriteriaEstimates();
		size = currentProject->getCriteriaCount() + 1;
		ExplanationLabel->Text = L"������� ������������� �������� ��������� � ������� 1 (�� ����� �� 1 �� 9)";
		Label1->Caption = L"���������� ���������:";
	} else {
		vector<Estimates> &alternativeEstimates = currentProject->getAlternativeEstimates();
		estimates = &alternativeEstimates[step - 1];
		gridNames = &currentProject->getAlternativeNames();
		size = currentProject->getAlternativesCount() + 1;
		ExplanationLabel->Text = L"������� ������������� �������� ����������� �� �������� \"" +
									 currentProject->getCriteriaNames()[step - 1] + L"\" � ������� 1 (�� ����� �� 1 �� 9)";
		Label1->Caption = L"���������� �����������:";
	}

	vector< vector<int> > &rates = estimates->getRates();
	const int n = rates.size() - 1;
	rates[n][n] = 9;

	CriteriaEstimates->ColCount = size - 1;
	CriteriaEstimates->RowCount = size;

	CriteriaEstimates->Row = 1;
	CriteriaEstimates->Col = 1;
	CriteriaEstimates->Refresh();

	PairWiseGrid->ColCount = size;
	PairWiseGrid->RowCount = size;
	PairWiseGrid->RowHeights[0] = 10;      //���������� ������ ������ ������
	PairWiseGrid->Refresh();

	consistency = 0.0;
	ConsistencLabel->Font->Color = clBlack;
	ConsistencLabel->Caption = L"-";
	PrioritiesLabel->Caption = L"-";

	//���������� �������
	for (int i = 0 ; i < rates.size(); ++i) {
		vector<int> &v = rates[i];
		for (int j = 0; j < v.size(); ++j) {
			PairWiseGrid->Cells[j + 1][i + 1] = L"";
			const int val = v[j];
			if (val == 0) {
				CriteriaEstimates->Cells[j + 1][i + 1] = L"";
			} else {
				CriteriaEstimates->Cells[j + 1][i + 1] = IntToStr(val);
				if (i == j) {
					for (int k = j; k < v.size(); ++k) {
						v[k] = val;
						CriteriaEstimates->Cells[k + 1][i + 1] = IntToStr(val);
					}
				}
			}
		}
	}

	//������ ���� ��� ������ ���������
	if (isDataFilled()) {
		eval();
	}

	CriteriaEstimates->SetFocus();
}
//---------------------------------------------------------------------------
//����� ���������
void __fastcall TEvalCriteriaWeightsForm::CriteriaEstimatesSelectCell(TObject *Sender,
          int ACol, int ARow, bool &CanSelect)
{
	if (step == 0) {
		if (ACol == 1) {
			ExplanationLabel->Text = L"������� ������������� �������� ��������� � ������� 1 (�� ����� �� 1 �� 9";
		} else {
			ExplanationLabel->Text = L"������� ������������� �������� ��������� ��� ����� �������� \"" +
			gridNames->at(ACol - 2) + L"\" � ������� " + IntToStr(ACol);

			if (ACol > 2) {
				ExplanationLabel->Text = ExplanationLabel->Text + L" � ���������� (�� ����� �� 1 �� 9)";
			} else {
				ExplanationLabel->Text = ExplanationLabel->Text + L" (�� ����� �� 1 �� 9)";
            }
		}
	} else {
		if (ACol == 1) {
			UnicodeString middlePart = L"������� ������������� �������� ����������� �� �������� \"" + currentProject->getCriteriaNames()[step - 1];
//			UnicodeString str(middlePart + L"dfsd f");
			ExplanationLabel->Text = middlePart + L"\" � ������� 1 (�� ����� �� 1 �� 9";
		} else {
			ExplanationLabel->Text = L"������� ������������� �������� ����������� �� �������� \"" +
				currentProject->getCriteriaNames()[step - 1] + L"\" ��� ����� ������������ \"" +
				gridNames->at(ACol - 2) + L"\" � ������� " + IntToStr(ACol);

			if (ACol > 2) {
				ExplanationLabel->Text = ExplanationLabel->Text + L" � ����������";
			} else {
				ExplanationLabel->Text = ExplanationLabel->Text + L" (�� ����� �� 1 �� 9)";
            }
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TEvalCriteriaWeightsForm::FormHide(TObject *Sender)
{
	currentProject = NULL;
	estimates = NULL;
	gridNames = NULL;
}
//---------------------------------------------------------------------------

