//---------------------------------------------------------------------------

#include <vcl.h>
#include <Clipbrd.hpp>
#include <limits>

#pragma hdrstop

#include "MainForm.h"
#include "UIManager.cpp"
#include "setColRowNameFormUnit.cpp"
#include "NewProjectUnit.cpp"
//---------------------------------------------------------------------------

#pragma resource "*.dfm"

TForm1 *Form1;

//---------------------------------------------------------------------------
//����������� �����
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner),
	gridRegex(L"[\\d]+"),
	floatGridRegex(L"[\\d]*(\\.|,){0,1}?[\\d]*")
	, projectManager(ProjectManager::getInstance()),
	isOnChartButtonPresssed(false),
	emptyCellBG(RGBA(255, 254, 198, 0)) {

	Form1 = this;
	UIManager::getInstance().addForm(Form1);
	NewProjectForm = new TNewProjectForm(this);

//	HintLabel->Handle;
//	SetWindowLongW(h, GWL_STYLE, GetWindowLongW(h, GWL_STYLE) | BS_MULTILINE);
	EvalCriteriaWeightsForm = new TEvalCriteriaWeightsForm(this);

	//������ ������� ����� ������
	InputDataStringGrid->Visible = false;
}
//---------------------------------------------------------------------------
//������� ������� �� ������ ������� �������� ������
void __fastcall TForm1::InputDataStringGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Shift.Contains(ssCtrl)&& Key == 86U) { //������� �� Excel
		//�������� ����� ������ ������ ������
		TClipboard *cb = Clipboard();
		wchar_t *str = new wchar_t[0];
		int len = cb->GetTextBuf(str, 0);
		//�������� ������ �� ������ ������
		str = new wchar_t[len];
		cb->GetTextBuf(str, len);
		UnicodeString uStr(str, len);
		int i = 0, j = InputDataStringGrid->Col, k = InputDataStringGrid->Row;
		//��������� ������� �������
		UnicodeString pasteString;
		while(str[i])
		{
			//��� ������� "Tab" ������� ������
			if (str[i] == '\t') {
				InputDataStringGrid->Cells[j][k] = pasteString;
				pasteString = "";

				++i;
				++j;
			}
			//��� ������� ����� ������  ������� ����
			if (str[i] == '\n') {
				InputDataStringGrid->Cells[j][k] = pasteString;
				pasteString = "";

				++i;
				j = InputDataStringGrid->Col;
				++k;
			}

			//���������� ������ ���� �� ���� �� �������� ���� ��
			//����������
			pasteString += str[i];
			++i;
		}

		//���������� ������
		InputDataStringGrid->Cells[j][k] = pasteString;

	} else if (Shift.Contains(ssCtrl) && Key == 67U) { //copy event

	} else if (Key == 46U) {	//on del button
		InputDataStringGrid->Cells[InputDataStringGrid->Col][InputDataStringGrid->Row] = L"";
	}
}
//---------------------------------------------------------------------------
//������������� �������� �������
void TForm1::initGrid()
{
	InputDataStringGrid->ColCount = getCriteriaCount() + fixedCols;
	InputDataStringGrid->RowCount = getAlternativesCount() + fixedRows + 1;
}
//---------------------------------------------------------------------------
//������� �������� �����
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	fixedCols = InputDataStringGrid->FixedCols;
	fixedRows = InputDataStringGrid->FixedRows;
}
//---------------------------------------------------------------------------
//���������� ���������� ��������� ���� ������ ������
int TForm1::getCriteriaCount()
{
	Project &project = projectManager.getCurrentProject();
	return &project == NULL ? 0 : project.getCriteriaCount();

}
//--------------------------------------------------------------------------
//���������� ���������� ����������� ���� ������ ������
int TForm1::getAlternativesCount()
{
	Project &project = projectManager.getCurrentProject();
	return &project == NULL ? 0 : project.getAlternativesCount();

}
//---------------------------------------------------------------------------
//������ ������� �� ���� ����������� ����������
void __fastcall TForm1::Memo1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	HideCaret(ResultRichEdit->Handle);
}
//---------------------------------------------------------------------------
//������ ������� �� ���� ����������� ����������
void __fastcall TForm1::Memo1MouseEnter(TObject *Sender)
{
	HideCaret(ResultRichEdit->Handle);
}
//---------------------------------------------------------------------------
//������� ������� ���������� ����� ������ ����
void __fastcall TForm1::MMCloseAppClick(TObject *Sender)
{
	//���� ������ �� ������� ������� - ����� �� �����������
	if (closeProject())
	{
		UIManager::getInstance().closeApp(this);
		Close();
	}
}
//---------------------------------------------------------------------------
//������� �� �������� �����
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (! closeProject())
	{
		Action = caNone;
		return;
	}

	UIManager::getInstance().closeApp(this);
}
//---------------------------------------------------------------------------
//������� ������������ ���� ���������� ���������� "��������"
void __fastcall TForm1::N5Click(TObject *Sender)
{
	ResultRichEdit->Text = L"";
	Chart1->Series[0]->Clear();
}
//---------------------------------------------------------------------------
//������� ������� ����� ������� ����
void __fastcall TForm1::N8Click(TObject *Sender)
{
	evalProject(true);
}
//---------------------------------------------------------------------------
//������� ������� ����� ������ �������� �������
void __fastcall TForm1::SpeedButton1Click(TObject *Sender)
{
	evalProject(true);
}
//---------------------------------------------------------------------------
//������ ������� ������� ��������
void TForm1::evalAHP()
{
	Matrix<double> &result = processor.evalWS();
	showResult(result, L"����� ������� ��������");
	delete &result;
}
//������ ������� ���������� ����� ����
void TForm1::evalWS()
{
	Matrix<double> &result = processor.evalWS();
	showResult(result, L"����� ���������� ����� ����");
	delete &result;
}
//����������� ����������� �� �������
void TForm1::showResultAtChart(Matrix<double> &result)
{
	Chart1->Series[0]->Clear();

	double max(0);
	vector<UnicodeString> alternativeNames = projectManager.getCurrentProject().getAlternativeNames();
	//������ ������������� �������� ��� ����� �� Y
	for (int i = 0; i < result.getHeight(); ++i)
	{
		max = result[i][0] > max ? result[i][0] : max;
		Chart1->Series[0]->Add(result[i][0], alternativeNames[i]);
	}

	Chart1->Axes->Left->Maximum = max * 1.2;

}
//---------------------------------------------------------------------------
//����������� �����������
void TForm1::showResult(Matrix<double> &result, UnicodeString method) {
	UnicodeString resultStr = L"(";

	for (int i = 0; i < result.getHeight() - 1; ++i) {
		resultStr += Format(L"%.3f; ", new TVarRec(result[i][0]), 1);
	}

	resultStr += Format(L"%.3f)", new TVarRec(result[result.getHeight() - 1][0]), 1);

	UnicodeString methodCaption = method;
	UnicodeString resultCaption = L"���������� �����������";
	ResultRichEdit->Lines->Add(methodCaption);
	ResultRichEdit->Lines->Add(resultCaption);
	ResultRichEdit->Lines->Add(resultStr);
	ResultRichEdit->Lines->Add(L"");
    for (int i = 0; i < ResultRichEdit->Lines->Count; ++i)
	{
		ResultRichEdit->Perform(EM_SCROLL, SB_LINEDOWN, 0);
	}

	showResultAtChart(result);
}
//---------------------------------------------------------------------------
//������� ��������� ������ �������
void __fastcall TForm1::InputDataStringGridDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State)
{
	//����� �������� � ������������ ��������
	if (ACol == 0 || ARow == 0) {
		drawFixedColNames(ACol, ARow, Rect);
	} else if (InputDataStringGrid->Cells[ACol][ARow].IsEmpty()) {
		if (UIManager::getInstance().getIndicator()) {
			//����������� ������ ������ ����� ������
			const TColor oldColor = InputDataStringGrid->Canvas->Brush->Color;
			InputDataStringGrid->Canvas->Brush->Color = emptyCellBG;
			InputDataStringGrid->Canvas->FillRect(InputDataStringGrid->CellRect(ACol, ARow));
			InputDataStringGrid->Canvas->Brush->Color = oldColor;
        }
	} else if (UIManager::getInstance().getIndicator() && StrToFloat(InputDataStringGrid->Cells[ACol][ARow]) == 0) {
		//����������� ������� ������ � ����
		const TColor oldColor = InputDataStringGrid->Canvas->Brush->Color;
		InputDataStringGrid->Canvas->Brush->Color = clRed;
		InputDataStringGrid->Canvas->FillRect(InputDataStringGrid->CellRect(ACol, ARow));
		InputDataStringGrid->Canvas->TextOut(Rect.Left+ 2, Rect.Top + 4, InputDataStringGrid->Cells[ACol][ARow]);
		InputDataStringGrid->Canvas->Brush->Color = oldColor;
	} else if (ARow == 1 && ! (InputDataStringGrid->Row == ARow && InputDataStringGrid->Col == ACol)) {
		//����������� ������ ������ � ����� � ��������
		InputDataStringGrid->Canvas->Brush->Color = cl3DLight;
		InputDataStringGrid->Canvas->FillRect(InputDataStringGrid->CellRect(ACol, ARow));
		UnicodeString str = ACol == 0 ? UnicodeString(L"�������� ���������") : InputDataStringGrid->Cells[ACol][ARow];
		InputDataStringGrid->Canvas->TextOut(Rect.Left + 2, Rect.Top + 4, str);
	}
}
//---------------------------------------------------------------------------
//������� �������� ������� �� ������, ���������� ����� ��������� �����
//�������� ���� ������������ � ������������� �������� � ����� ��������
void __fastcall TForm1::InputDataStringGridDblClick(TObject *Sender)
{
	const int cols = InputDataStringGrid->ColCount;
	const int rows = InputDataStringGrid->RowCount;
	TPoint cursorPoint = InputDataStringGrid->ScreenToClient(Mouse->CursorPos);
	vector<UnicodeString> &criteriaNames =  projectManager.getCurrentProject().getCriteriaNames();
	for (int i = fixedCols; i < cols; i++) {
		TRect rect(InputDataStringGrid->CellRect(i, 0));
		if (rect.Contains(cursorPoint)) {
			UnicodeString *newName = &criteriaNames[i - 1];
			TsetCollRowNameForm *form = new TsetCollRowNameForm(this);
			form->setResultStr(newName);
			try {
			  form->ShowModal();
			  if (! newName->IsEmpty()) {
				  setColWidth(*newName);
				  InputDataStringGrid->Refresh();
				  projectManager.setIsCurrentProjectSaved(false);
			  }
			} __finally {
			  form->Free();
			}
			return;
		}
	}

	for (int i = fixedRows + 1; i < rows; i++) {                              // +1 - ������ �������� ���������
		TRect rect(InputDataStringGrid->CellRect(0, i));
		if (rect.Contains(cursorPoint)) {
			UnicodeString *newName = new UnicodeString();
			TsetCollRowNameForm *form = new TsetCollRowNameForm(this);
			form->setResultStr(newName);
			try {
			  form->ShowModal();
			  if (! newName->IsEmpty()) {
				  setRowHeight(*newName);
				  projectManager.getCurrentProject().getAlternativeNames()[i - 2] = *newName;
				  InputDataStringGrid->Refresh();
				  projectManager.setIsCurrentProjectSaved(false);
			  }
			} __finally {
			  form->Free();
			}
			return;
		}
	}
}
//---------------------------------------------------------------------------
//���������� ���������� ��� ����������� � ���������
//���������� ��� ������������ ��������� ������/������ ����� �
//��������� ������������� ������
void TForm1::drawFixedColNames(int ACol, int ARow, TRect &Rect)
{
	if (ACol == 0 && ARow == 0) {
		return;
	}

	vector<UnicodeString> &criteriaNames = projectManager.getCurrentProject().getCriteriaNames();
	vector<UnicodeString> &alternativeNames = projectManager.getCurrentProject().getAlternativeNames();

	if (ARow == 1) {
		UnicodeString name = L"�������� ���������";
		setColWidth(name);
		InputDataStringGrid->Canvas->TextRect(Rect, name);
		return;
	}

	if (ACol == 0) {
		UnicodeString name = alternativeNames[ARow - 2];
		setColWidth(name);
		InputDataStringGrid->Canvas->TextRect(Rect, name);
	}

	if (ARow == 0) {
		UnicodeString &name = criteriaNames[ACol- 1];
        setRowHeight(name);

		//������������� ���������� ������ ��� 90 ��������� � ��������� ��������
		InputDataStringGrid->Canvas->Font->Orientation = 90 * 10;
		InputDataStringGrid->Canvas->TextOut(Rect.Left, Rect.Bottom - 6, name);
	}
}
 //---------------------------------------------------------------------------
//��������� � ������������ ������ ������
void TForm1::setRowHeight(UnicodeString &str)
{
	int rowHeight = InputDataStringGrid->RowHeights[0];
	InputDataStringGrid->Canvas->Font->Size = InputDataStringGrid->Font->Size;
	int newRowHeight= InputDataStringGrid->Canvas->TextWidth(str) + 6 + 6;
	InputDataStringGrid->RowHeights[0] = rowHeight > newRowHeight ? rowHeight : newRowHeight;
}
 //---------------------------------------------------------------------------
//��������� � ������������� ������ �������
void TForm1::setColWidth(UnicodeString &str, int col)
{
	int columnWidth = InputDataStringGrid->ColWidths[col];
	InputDataStringGrid->Canvas->Font->Size = InputDataStringGrid->Font->Size;
	int newColumnWidth = InputDataStringGrid->Canvas->TextWidth(str) + 6 + 6 + InputDataStringGrid->GridLineWidth;
	InputDataStringGrid->ColWidths[col] = columnWidth > newColumnWidth ? columnWidth : newColumnWidth;
}
//---------------------------------------------------------------------------
//��������� ������� ������
void TForm1::saveProject()
{
	if (projectManager.isProjectOpen() && SaveDialog1->Execute(this->Handle))
	{
		projectManager.saveProject(SaveDialog1->FileName);
	}
}
//--------------------------------------------------------------------------
//��������� ����� ���������� �������
void TForm1::editProject() {
	if (projectManager.isProjectOpen()) {
		NewProjectForm->ShowModal();
		showCurrentProject();

		projectManager.setIsCurrentProjectSaved(false);
	}
}
//���������� ������ �������
void TForm1::evalProject(bool allowDialogs) {
	if (projectManager.isProjectOpen() && isDataValid(allowDialogs)) {
		if (projectManager.getCurrentProject().getMethod() == MathMethods::WS) {
		   evalWS();
		} else {
		   evalAHP();
		}
	}

	showHint(L"������� �����������");
}
//--------------------------------------------------------------------------
//��������� ������
void TForm1::loadProject()
{
	if (! closeProject())
	{
        return;
    }

	if (OpenDialog1->Execute(this->Handle)) {
		try {
			projectManager.loadProject(OpenDialog1->FileName);
			showCurrentProject();
		} catch (...) {
			//��������� �� ������ � ������ ������������� ��������
			Application->MessageBoxW(
				L"���� ������� ��������!",
				L"������!",
				MB_OK| MB_ICONERROR
		   );
		}
	}
}
//--------------------------------------------------------------------------
//������ ����� ������
void TForm1::newProject()
{
	if (! closeProject())
	{
        return;
	}

	projectManager.newProject();
	NewProjectForm->setIsNewProject(true);
	NewProjectForm->ShowModal();
	if (projectManager.isProjectOpen()) {
		//���� ������ ��� ������ - ��������� �������
		showCurrentProject();
    }
}
//--------------------------------------------------------------------------
// ��������� ������
bool TForm1::closeProject()
{
    //���������� ������ � ���������� �������� �������
	if (! showSaveDialog())
	{
		return false;
	}

	if (! projectManager.isProjectOpen())
	{
        return true;
    }

	//������� ������
	for (int i = 0; i < getCriteriaCount() + 1; ++i)
	{
		for (int j = 0; j < getAlternativesCount() + 1; ++j)
		{
            InputDataStringGrid->Cells[i][j] = L"";
        }
    }

	InputDataStringGrid->Visible = false;
	Chart1->Series[0]->Clear();
	ResultRichEdit->Clear();

	projectManager.closeProject();
	showHint(L"�������� ���� �������� ����� ������");
	return true;
}
//--------------------------------------------------------------------------
//���������� ��������� ������� ������
bool TForm1::showSaveDialog()
{
	if (projectManager.isProjectOpen() && ! projectManager.isSavedCurrentPreject()) {
	   const int dialogResult = Application->MessageBoxW(
									L"��������� ������� ������?",
									L"��������� ������?",
									MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON3
							   );
		if (dialogResult == IDYES) {
		   saveProject();
		} else if (dialogResult == IDCANCEL) {
			return false;
		}
	}

	return true;
}
//--------------------------------------------------------------------------
//��������� ��� �� ������������ ������ � �������
bool TForm1::isDataValid(bool allowDialogs)
{
	const int cols = InputDataStringGrid->ColCount;
	const int rows = InputDataStringGrid->RowCount;
	for (int i = fixedRows; i < rows; ++i)
	{
		for (int j = fixedCols; j < cols; ++j)
		{
			const UnicodeString &val = InputDataStringGrid->Cells[j][i];
			if (val.IsEmpty()) {
				Project &project = projectManager.getCurrentProject();
				if (i == fixedRows) {
					showHint(L"��������� ������ ��� �������� \"" +
							project.getCriteriaNames()[j - fixedCols] + L"\""
						);
				} else {
					showHint(L"��������� ������ ��� ������������ \"" +
							project.getAlternativeNames()[i - fixedRows - 1] +
							L"\" �� �������� \"" +
							project.getCriteriaNames()[j - fixedCols] + L"\""
						);
                }
				if (allowDialogs) {
						Application->MessageBoxW(
							L"������� �������� ������ (������� ����� ������ ����� �����)",
							L"������",
							MB_OK| MB_ICONERROR
						);
					}
				return false;
			}

			const bool allowFloat = projectManager.getCurrentProject().getMethod() == MathMethods::AHP;
			const bool isValid = allowFloat || i == 1 ?
						regex_match(val.w_str(), floatGridRegex) :
						regex_match(val.w_str(), gridRegex);

			if (! isValid)
			{
				if (allowDialogs) {
					Application->MessageBoxW(
						L"������� �������� ������ (������� ����� ������ ����� �����)",
						L"������",
						MB_OK| MB_ICONERROR
					);
				}

				InputDataStringGrid->Col = j;
				InputDataStringGrid->Row = i;
				return false;
			} else if (StrToFloat(val) == 0) {
				if (allowDialogs) {
					Application->MessageBoxW(
						L"������� ������������ ��������",
						L"������",
						MB_OK| MB_ICONERROR
					);
				}

				Project &project = projectManager.getCurrentProject();
				showHint(L"������� ������������ �������� ��� ������ ������������ \"" +
							project.getAlternativeNames()[i - fixedRows - 1] +
							L"\" �� �������� \"" +
							project.getCriteriaNames()[j - fixedCols] + L"\""
						);

				InputDataStringGrid->Col = j;
				InputDataStringGrid->Row = i;
				return false;
            }
		}
	}

	return true;
}
//--------------------------------------------------------------------------
void __fastcall TForm1::MMNewProjectClick(TObject *Sender)
{
	newProject();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton2Click(TObject *Sender)
{
	saveProject();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton3Click(TObject *Sender)
{
	loadProject();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NewProjectButtonClick(TObject *Sender)
{
	newProject();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton5Click(TObject *Sender)
{
	closeProject();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MMCloseProjectClick(TObject *Sender)
{
	closeProject();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MMOPentProjectClick(TObject *Sender)
{
	loadProject();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MMSaveProjectClick(TObject *Sender)
{
	saveProject();
}
//---------------------------------------------------------------------------
//�������������� �������
void __fastcall TForm1::MMEditProjectClick(TObject *Sender)
{
	editProject();
}
//---------------------------------------------------------------------------
//��������, ��� ��� ������������ � ������� ������ ������ ���� ����������
void __fastcall TForm1::InputDataStringGridSetEditText(TObject *Sender, int ACol,
		  int ARow, const UnicodeString Value)
{
	Project &currentProject = projectManager.getCurrentProject();
	const bool isAHP = currentProject.getMethod() == MathMethods::AHP;
	const bool isValid = isAHP || ARow == 1 ? regex_match(Value.w_str(), floatGridRegex) : regex_match(Value.w_str(), gridRegex);
	if (! isValid)
	{
		UnicodeString str = InputDataStringGrid->Cells[ACol][ARow];
		InputDataStringGrid->Cells[ACol][ARow] = str.SubString(1, str.Length() - 1);
	} else {
		if (! Value.IsEmpty()) {
			if (ARow != 1)
			{
				if (! isAHP && StrToInt(Value) < 1) {
					Application->MessageBoxW(
						L"������ �� ������ ���� ������ 1",
						L"������",
						MB_OK| MB_ICONERROR
					);

					InputDataStringGrid->Col = ACol;
					InputDataStringGrid->Row = ARow;
					InputDataStringGrid->Cells[ACol][ARow] = L"";
				} else {
					currentProject.getAlternativeEstimates()[ACol - fixedCols].getPriorities()[ARow- fixedRows - 1] = StrToFloat(Value);
				}
			} else {
				currentProject.getCriteriaEstimates().getPriorities()[ACol - fixedCols] = StrToFloat(Value);
			}
		}
	}

	if (ARow != 0) {
		UnicodeString str(Value);
		setColWidth(str, ACol);
	}

	projectManager.setIsCurrentProjectSaved(false);

	if (isDataValid(false)) {
		if (UIManager::getInstance().getAutoEval()) {
			evalProject(false);
		} else {
			showHint(L"������� ������!");
		}
	}
}
//---------------------------------------------------------------------------
//���������� ������� �� ������� (���������� ��� ��� �������������� � ��������� ��������)
void __fastcall TForm1::Chart1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
	if (Button == mbLeft) {
		isOnChartButtonPresssed = true;
		lastChartMousePoint = Mouse->CursorPos;
		isOnControlBorder(Sender, X, Y);
    }
}
//---------------------------------------------------------------------------
//����� ����, ��� ������� ���� ��������, ��������� �� ����� �� ������ ��
//������� ����� � ���������� ��� �������, ���� ����� ���������
void __fastcall TForm1::Chart1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{
	TControl *control = (TControl*) Sender;
	isOnChartButtonPresssed = false;
	if (control->Left < 0) {
		control->Left = 0;
	}

	if (control->Top < 0) {
		control->Top = 0;
	}

	if (control->Top > Form1->Height - control->Height)
	{
		control->Top = Form1->Height - control->Height;
	}

	if (control->Left > Form1->Width - control->Width)
	{
		control->Left = Form1->Width - control->Width;
	}
}
//---------------------------------------------------------------------------
//���������� true ���� ���� �� ������� ������� (���������� ��� ������ �������
//�� ������ ��������� ��������)
void TForm1::isOnControlBorder(TObject *Sender, int X, int Y)
{
	TControl *control = (TControl*) Sender;
	const int offset = 5;
	isLeft = isRight = isTop = isBottom = false;
	isLeft = X < offset;
	const int rightOffset = Sender == Chart1 ? offset : offset + 5;  //Memo has right padding;
	isRight = X > control->Width - rightOffset;
	isTop = Y < offset;
	const int bottomOffset = Sender == Chart1 ? offset : offset + 5;  //Memo has bottom padding
	isBottom = Y > control->Height - bottomOffset;
}
//���������� ���� �������� ������ �������
void __fastcall TForm1::Chart1MouseMove(TObject *Sender, TShiftState Shift, int X,
		  int Y)
{
	TControl *control = (TControl*) Sender;
	if (isOnChartButtonPresssed) {
		const TPoint changing(lastChartMousePoint.X - Mouse->CursorPos.X, lastChartMousePoint.Y - Mouse->CursorPos.Y);
		if (isLeft)
		{
			control->Width += changing.X;
			control->Left -= changing.X;

			if (isTop)
			{
				control->Height += changing.Y;
				control->Top -= changing.Y;
			} else if (isBottom) {
				control->Height -= changing.Y;
			}
		} else if (isRight) {
			control->Width -= changing.X;

			if (isTop)
			{
				control->Height += changing.Y;
				control->Top -= changing.Y;
			} else if (isBottom) {
				control->Height -= changing.Y;
			}
		} else if (isBottom) {
		   control->Height -= changing.Y;
		} else if (isTop) {
			control->Height += changing.Y;
			control->Top -= changing.Y;
		} else {
			//����������� �������
			control->Left -= changing.X;
			control->Top -= changing.Y;
		}

		lastChartMousePoint = Mouse->CursorPos;
	} else {
	   changeCursor(Sender, X, Y);
	}
}
//---------------------------------------------------------------------------
//������ ������ ��������, ������� ����� ������ �������
void TForm1::changeCursor(TObject* Sender, int X, int Y)
{
	if (Sender == Chart1) {
	   changeChartCursor(Sender, X, Y);
	} else if (Sender == HintMemo) {
       changeHintCursor(Sender, X, Y);
    }
}
//---------------------------------------------------------------------------
//������ ������ ������� �� ������ ��������� �������
void TForm1::changeChartCursor(TObject* c, int X, int Y)
{
	isOnControlBorder(c, X, Y);
	if (isLeft)
	{
		if (isTop)
		{
			Chart1->OriginalCursor = crSizeNWSE;
		} else if (isBottom) {
			Chart1->OriginalCursor = crSizeNESW;
		} else {
			Chart1->OriginalCursor = crSizeWE;
		}
	} else if (isRight) {
		if (isTop)
		{
			Chart1->OriginalCursor = crSizeNESW;
		} else if (isBottom) {
			Chart1->OriginalCursor = crSizeNWSE;;
		} else {
			Chart1->OriginalCursor = crSizeWE;
		}
	} else if (isBottom || isTop) {
		Chart1->OriginalCursor = crSizeNS;
	} else {
		Chart1->OriginalCursor = crDefault;
	}
}
//---------------------------------------------------------------------------
//������ ������ ��������� �� ������ ��������� �������
void TForm1::changeHintCursor(TObject* c, int X, int Y)
{
	isOnControlBorder(c, X, Y);
	if (isLeft)
	{
		if (isTop)
		{
			HintMemo->Cursor = crSizeNWSE;
		} else if (isBottom) {
			HintMemo->Cursor = crSizeNESW;
		} else {
			HintMemo->Cursor = crSizeWE;
		}
	} else if (isRight) {
		if (isTop)
		{
			HintMemo->Cursor = crSizeNESW;
		} else if (isBottom) {
			HintMemo->Cursor = crSizeNWSE;;
		} else {
			HintMemo->Cursor = crSizeWE;
		}
	} else if (isBottom || isTop) {
		HintMemo->Cursor = crSizeNS;
	} else {
		HintMemo->Cursor = crDefault;
	}
}
//---------------------------------------------------------------------------
//���������� ������� ������ �� �����
void TForm1::showCurrentProject()
{
 	initGrid();
	InputDataStringGrid->Visible = true;
	InputDataStringGrid->FixedCols = 1;			//bug

	//fill grid

	Project &currentProject = projectManager.getCurrentProject();
	vector<Estimates> &alternativeEstimates = currentProject.getAlternativeEstimates();
	for (int i = 0; i < alternativeEstimates.size(); ++i)
	{
		vector<double> &priorities = alternativeEstimates[i].getPriorities();
		for (int j = 0; j < priorities.size() ; ++j)
		{
			if (priorities[j] != 0) {
				UnicodeString str(Format(L"%.2g", &TVarRec(priorities[j]), 1));
				InputDataStringGrid->Cells[i + fixedCols][j + fixedRows + 1] = str;
				setColWidth(str, i + fixedCols);
			} else {
				InputDataStringGrid->Cells[i + fixedCols][j + fixedRows + 1] = L"";
            }
		}
	}

	vector<double> &criteriaPriorities = currentProject.getCriteriaEstimates().getPriorities();
	for (int i = 0; i < criteriaPriorities.size(); ++i)
	{
		if (criteriaPriorities[i] != 0) {
			UnicodeString str(Format(L"%.2g", &TVarRec(criteriaPriorities[i]), 1));
			InputDataStringGrid->Cells[i + fixedCols][fixedRows ] = str;
			setColWidth(str, i + fixedCols);
		} else {
			InputDataStringGrid->Cells[i + fixedCols][fixedRows ] = L"";
        }
    }

	Form1->Caption = projectManager.getCurrentProject().getName();
	InputDataStringGrid->SetFocus();

	if (isDataValid(false)) {
		if (UIManager::getInstance().getAutoEval()) {
			evalProject(false);
		} else {
			showHint(L"������� ������!");
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::EditProjectSpeedButtonClick(TObject *Sender)
{
	editProject();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::InputDataStringGridSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect)
{
	const int col = InputDataStringGrid->Col;
	const int row = InputDataStringGrid->Row;
	if (row == fixedRows) {
		UnicodeString str = InputDataStringGrid->Cells[col][row];
//		if (! str.IsEmpty() && StrToFloat(str) == 0) {
//			Application->MessageBoxW(
//				L"������ �� ����� ���� ����� 0",
//				L"������",
//				MB_OK | MB_ICONERROR
//			);
//			InputDataStringGrid->Cells[col][row] = NULL;
//			InputDataStringGrid->Col = ACol = col;
//			InputDataStringGrid->Row = ARow = row;
//        }
	}

	for (int i = 0; i < getAlternativesCount() + 1; ++i) {
		for (int j = 0; j < getCriteriaCount(); ++j) {
			if (InputDataStringGrid->Cells[j + fixedCols][i + fixedRows].IsEmpty()) {
				Project &project = projectManager.getCurrentProject();
				if (i == 0) {
					showHint(L"��������� ������ ��� �������� \""
							+ project.getCriteriaNames()[j] + L"\""
						);

					return;
				} else {
					showHint(L"��������� ������ ��� ������������ \""
							+ project.getAlternativeNames()[i - 1]
							+ L"\" �� �������� \""
							+ project.getCriteriaNames()[j] + L"\""
						);

					return;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MMHintClick(TObject *Sender)
{
	MMHint->Checked = ! MMHint->Checked;
	UIManager::getInstance().getHint() = MMHint->Checked;
	HintMemo->Visible = MMHint->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MMIndicatorClick(TObject *Sender)
{
	MMIndicator->Checked = ! MMIndicator->Checked;
	UIManager::getInstance().getIndicator() = MMIndicator->Checked;
	InputDataStringGrid->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MMAutoEvalClick(TObject *Sender)
{
	MMAutoEval->Checked = ! MMAutoEval->Checked;
	UIManager::getInstance().getAutoEval() = MMAutoEval->Checked;
	if (MMAutoEval->Checked) {
		evalProject(false);
    }
}
//---------------------------------------------------------------------------
void TForm1::showHint(UnicodeString text) {
	HintMemo->Text = text;
}
//---------------------------------------------------------------------------

