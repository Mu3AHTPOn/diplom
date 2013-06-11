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

// [\d]*(.|,)[\d]+
TForm1 *Form1;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner), gridRegex(L"[\\d]+"),floatGridRegex(L"[\\d]*(.|,){0,1}[\\d]*"), projectManager(ProjectManager::getInstance()),
	isOnChartButtonPresssed(false)
{
	Form1 = this;
	UIManager::getInstance()->addForm(Form1);
	Form1->Hide();
	NewProjectForm = new TNewProjectForm(this);
	bool isClose(false), isOpen(false);
//	Project & currentProject = projectManager.getCurrentProject();

	EvalCriteriaWeightsForm = new TEvalCriteriaWeightsForm(this);

	InputDataStringGrid->Visible = false;

	MethodComboBox->Canvas->Font->Size = MethodComboBox->Font->Size;
	const int AHP = MethodComboBox->Canvas->TextWidth((*MethodComboBox->Items)[0]);
	const int WS = MethodComboBox->Canvas->TextWidth((*MethodComboBox->Items)[1]);

	MethodComboBox->Width = AHP > WS ? AHP + 30 : WS + 30;
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
		int i = 0, j = InputDataStringGrid->Col, k = InputDataStringGrid->Row;
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
				j = InputDataStringGrid->Col;
				++k;
			}

			pasteString += str[i];
			++i;
		}

		InputDataStringGrid->Cells[j][k] = pasteString;

	} else if (Shift.Contains(ssCtrl) && Key == 67U) { //copy event

	} else if (Key == 46U) {
		InputDataStringGrid->Cells[InputDataStringGrid->Col][InputDataStringGrid->Row] = "";
	}
}
//---------------------------------------------------------------------------

void TForm1::initGrid()
{
	InputDataStringGrid->ColCount = getCriteriaCount() + fixedCols;
	InputDataStringGrid->RowCount = getAlternativesCount() + fixedRows + 1;
}

void __fastcall TForm1::FormCreate(TObject *Sender)
{

	MethodComboBox->ItemIndex = 0;

	fixedCols = InputDataStringGrid->FixedCols;
	fixedRows = InputDataStringGrid->FixedRows;

//	InputDataStringGrid->Col = 1;
//	InputDataStringGrid->Row = 1;
}
//---------------------------------------------------------------------------



int TForm1::getCriteriaCount()
{
	return projectManager.getCurrentProject().getCriteriaCount();

}
//--------------------------------------------------------------------------
int TForm1::getAlternativesCount()
{
	return projectManager.getCurrentProject().getAlternativesCount();

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

void __fastcall TForm1::MMCloseAppClick(TObject *Sender)
{
	if (closeProject())
	{
		UIManager::getInstance()->closeApp(this);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (! closeProject())
	{
		Action = caNone;
		return;
	}

	UIManager::getInstance()->closeApp(this);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::N5Click(TObject *Sender)
{
	ResultRichEdit->Text = L"";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N8Click(TObject *Sender)
{
	if (projectManager.isProjectOpen() && isDataValid()) {
		if (MethodComboBox->ItemIndex == MathMethods::WS) {
		   evalWS();
		} else {
		   evalAHP();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SpeedButton1Click(TObject *Sender)
{
	if (projectManager.isProjectOpen() && isDataValid()) {
		if (MethodComboBox->ItemIndex == MathMethods::WS) {
		    evalWS();
		} else
		{
			evalAHP();
		}
    }
}

void TForm1::evalAHP()
{
//создаём множество объектов ahpsolver для оценок важности критериев и для
//каждого критерия в частности
try {
	const int criteriaCount = getCriteriaCount();
	const int alternativesCount = getAlternativesCount();
	Project &currentProject = projectManager.getCurrentProject();

	Matrix<double> criteriaEstimates(criteriaCount);

	Matrix<double> *objectEstimates = new Matrix<double>[criteriaCount];
	AHPSolver<double> *ahpObjects = new AHPSolver<double>[criteriaCount];

	vector<double> &priorities = currentProject.getCriteriaEstimates().getPriorities();
	vector<Estimates> &alternativeEstimates = currentProject.getAlternativeEstimates();
	for (int i = 0; i < criteriaCount; ++i) {
		criteriaEstimates[i][0] = priorities[i];
		objectEstimates[i] = *(new Matrix<double>(alternativesCount));
		for (int j = 0; j < alternativesCount; ++j) {
			(objectEstimates[i])[j][0] = alternativeEstimates[i].getPriorities()[j];
		}

		ahpObjects[i] = *(new AHPSolver<double>(objectEstimates[i]));
	}


	//fill objectEstimates
//	vector<double> &priorities = currentProject.getCriteriaEstimates().getPriorities();
//	for (int i = 0; i < criteriaCount; ++i) {
//
//		for (int j = 0; j < alternativesCount; ++j) {
//			curr[j][0] = priorities[j];
//		}
//
//		ahpObjects[i] = *(new AHPSolver<int>(curr));
//	}

	Matrix<double> integrEstimate(ahpObjects[0].getMaxEigenVectors());
	for(int i = 1; i < criteriaCount; ++i)
	{
		integrEstimate.append(ahpObjects[i].getMaxEigenVectors());
	}

	Matrix<double> result = integrEstimate * criteriaEstimates;

	UnicodeString resultStr = L"(";

	for (int i = 0; i < result.getHeight() - 1; ++i) {
		resultStr += Format(L"%.3f; ", &TVarRec(result[i][0]), 1);
	}

	resultStr += Format(L"%.3f)", &TVarRec(result[result.getHeight() - 1][0]), 1);

	UnicodeString methodCaption = L"Метод анализа иерархий";
	UnicodeString resultCaption = L"Приоритеты альтернатив";
	ResultRichEdit->Lines->Add(methodCaption);
	ResultRichEdit->Lines->Add(resultCaption);
//	ResultRichEdit->SelStart= methodCaption.Length();
//	ResultRichEdit->SelAttributes->Color = clRed;
//	ResultRichEdit->SelAttributes->Style = ResultRichEdit->SelAttributes->Style << fsBold;
//	ResultRichEdit->Lines->Add(methodCaption);
	ResultRichEdit->Lines->Add(resultStr);
	ResultRichEdit->Lines->Add("");
//	int t = ResultRichEdit->Perform(EM_LINEINDEX, ResultRichEdit->Lines->Count - 1, 0);;
//	ResultRichEdit->SelStart = t;
	for (int i = 0; i < ResultRichEdit->Lines->Count; ++i)
	{
		ResultRichEdit->Perform(EM_SCROLL, SB_LINEDOWN, 0);
	}


	showResultAtChart(result);
	} catch (...)        //faken Embarcadero!
	{

    }
}

void TForm1::evalWS()
{
	const int criteriaCount = getCriteriaCount();
	const int alternativesCount = getAlternativesCount();

	Matrix<double> criteriaEstimates(criteriaCount);
	Matrix<double> objectEstimates(alternativesCount, criteriaCount);

	for (int i = 0; i < criteriaCount; ++i) {
		criteriaEstimates[i][0] = InputDataStringGrid->Cells[i+fixedCols][fixedRows].ToDouble();
	}

 	//fill objectEstimates
	TStringGrid *test = InputDataStringGrid;
	for (int i = 0; i < criteriaCount; ++i) {
		for (int j = 0; j < alternativesCount; ++j) {
			objectEstimates[j][i] = InputDataStringGrid->Cells[i + fixedCols][j + fixedRows + 1].ToInt();
		}
	}

	Matrix<double> &result = (objectEstimates * criteriaEstimates).normalizeToOne();

	UnicodeString resultStr = L"(";

	for (int i = 0; i < result.getHeight() - 1; ++i) {
		resultStr += Format(L"%.3f; ", new TVarRec(result[i][0]), 1);
	}

	resultStr += Format(L"%.3f)", new TVarRec(result[result.getHeight() - 1][0]), 1);

	UnicodeString methodCaption = L"Метод взвешенной суммы мест";
	UnicodeString resultCaption = L"Приоритеты альтернатив";
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

void TForm1::showResultAtChart(Matrix<double> &result)
{
	Chart1->Series[0]->Clear();

	double max(0);
	vector<UnicodeString> alternativeNames = projectManager.getCurrentProject().getAlternativeNames();
	for (int i = 0; i < result.getHeight(); ++i)
	{
		max = result[i][0] > max ? result[i][0] : max;
		Chart1->Series[0]->Add(result[i][0], alternativeNames[i]);
	}

	Chart1->Axes->Left->Maximum = max * 1.2;

}
//---------------------------------------------------------------------------


void __fastcall TForm1::InputDataStringGridDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State)
{
	if (ACol == 0 || ARow == 0) {
		drawFixedColNames(ACol, ARow, Rect);
	}

	if (ARow == 1 && ! (InputDataStringGrid->Row == ARow && InputDataStringGrid->Col == ACol))
	{
		InputDataStringGrid->Canvas->Brush->Color = cl3DLight;
		InputDataStringGrid->Canvas->FillRect(InputDataStringGrid->CellRect(ACol, ARow));
		UnicodeString str = ACol == 0 ? UnicodeString(L"Важность критериев") : InputDataStringGrid->Cells[ACol][ARow];
		InputDataStringGrid->Canvas->TextOut(Rect.Left + 2, Rect.Top + 4, str);
	}
}
//---------------------------------------------------------------------------

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
				  delete newName;
			  }
			} __finally {
			  form->Free();
			}
			return;
		}
	}

	for (int i = fixedRows + 1; i < rows; i++) {                              // +1 - строка важности критериев
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
				  delete newName;
				  InputDataStringGrid->Refresh();
			  }
			} __finally {
			  form->Free();
			}
			return;
		}
	}
}
//---------------------------------------------------------------------------
void TForm1::drawFixedColNames(int ACol, int ARow, TRect &Rect)
{
	if (ACol == 0 && ARow == 0) {
		return;
	}

	vector<UnicodeString> &criteriaNames = projectManager.getCurrentProject().getCriteriaNames();
	vector<UnicodeString> &alternativeNames = projectManager.getCurrentProject().getAlternativeNames();

	if (ARow == 1) {
		UnicodeString name = L"Важность критериев";
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

		InputDataStringGrid->Canvas->Font->Orientation = 90 * 10;
		InputDataStringGrid->Canvas->TextOut(Rect.Left, Rect.Bottom - 6, name);
	}
}
 //---------------------------------------------------------------------------

void TForm1::setRowHeight(UnicodeString &str)
{
	int rowHeight = InputDataStringGrid->RowHeights[0];
	InputDataStringGrid->Canvas->Font->Size = InputDataStringGrid->Font->Size;
	int newRowHeight= InputDataStringGrid->Canvas->TextWidth(str) + 6 + 6;
	InputDataStringGrid->RowHeights[0] = rowHeight > newRowHeight ? rowHeight : newRowHeight;
}
 //---------------------------------------------------------------------------

void TForm1::setColWidth(UnicodeString &str, int col)
{
	int columnWidth = InputDataStringGrid->ColWidths[col];
	InputDataStringGrid->Canvas->Font->Size = InputDataStringGrid->Font->Size;
	int newColumnWidth = InputDataStringGrid->Canvas->TextWidth(str) + 6 + 6 + InputDataStringGrid->GridLineWidth;
	InputDataStringGrid->ColWidths[col] = columnWidth > newColumnWidth ? columnWidth : newColumnWidth;
}
//---------------------------------------------------------------------------

void TForm1::saveProject()
{
    if (projectManager.isProjectOpen() && SaveDialog1->Execute(this->Handle))
	{
		projectManager.saveProject(SaveDialog1->FileName);
	}
}
//--------------------------------------------------------------------------
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
			Application->MessageBoxW(
				L"Файл проекта повреждён!",
				L"Ошибка!",
				MB_OK| MB_ICONERROR
		   );
		}
	}
}
//--------------------------------------------------------------------------
void TForm1::newProject()
{
	if (! closeProject())
	{
        return;
	}

   	projectManager.newProject();
	NewProjectForm->ShowModal();
	showCurrentProject();
//	if (getCriteriaCount() > 0 && getAlternativesCount() > 0) {
//	   projectManager.setIsProjectOpen(true);
//		initGrid();
//
//		InputDataStringGrid->Refresh();
//		Form1->Caption = projectManager.getCurrentProject().getName();
//		MethodComboBox->ItemIndex = projectManager.getCurrentProject().getMethod();
//		for (int i = 0; i < InputDataStringGrid->ColCount; i++) {
//			for (int j = 0; j < InputDataStringGrid->RowCount; j++) {
//					InputDataStringGrid->Cells[i][j] = L"";
//			}
//		}
//
//		InputDataStringGrid->Visible = true;
//		InputDataStringGrid->FixedCols = 1;			//bug
//		InputDataStringGrid->SetFocus();
//	}
//
//	if (criteriaEstimates->size() > 0) {
//		for (int i = 0; i < criteriaEstimates->size(); ++i)
//		{
//			UnicodeString str(Format(L"%.2f", &TVarRec(criteriaEstimates->at(i)), 1));
//			InputDataStringGrid->Cells[i + 1][1] = str;
//			setColWidth(str, i + 1);
//        }
//	}
}
//--------------------------------------------------------------------------
// return false if project doesn't close
bool TForm1::closeProject()
{
	if (! showSaveDialog())
	{
		return false;
	}

	if (! projectManager.isProjectOpen())
	{
        return true;
    }

	for (int i = 0; i < getCriteriaCount() + 1; ++i)              //очистка табицы
	{
		for (int j = 0; j < getAlternativesCount() + 1; ++j)
		{
            InputDataStringGrid->Cells[i][j] = L"";
        }
    }

	InputDataStringGrid->Visible = false;
	Chart1->Series[0]->Clear();
	ResultRichEdit->Clear();
	MethodComboBox->ItemIndex = 0;

	projectManager.closeProject();
	return true;
}
//--------------------------------------------------------------------------
//return false if user click on cancel
bool TForm1::showSaveDialog()
{
	if (projectManager.isProjectOpen() && ! projectManager.isSavedCurrentPreject()) {
	   const int dialogResult = Application->MessageBoxW(
									L"Сохранить текущий проект?",
									L"Сохранить проект?",
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
bool TForm1::isDataValid()
{
	const int cols = InputDataStringGrid->ColCount;
	const int rows = InputDataStringGrid->RowCount;
	for (int i = fixedCols; i < cols; ++i)
	{
		for (int j = fixedRows ; j < rows; ++j)
		{
			const UnicodeString &val = InputDataStringGrid->Cells[i][j];
			if (val.IsEmpty()) {
                return false;
			}

			const bool allowFloat = projectManager.getCurrentProject().getMethod() == MathMethods::AHP;
			const bool isValid = allowFloat || j == 1 ?
						regex_match(val.w_str(), floatGridRegex) :
						regex_match(val.w_str(), gridRegex);

			if (! isValid)
			{
				Application->MessageBoxW(
					L"Введены неверные данные (вводить можно только целые числа)",
					L"Ошибка",
					MB_OK| MB_ICONERROR
				);

				InputDataStringGrid->Col = i;
				InputDataStringGrid->Row = j;
				return false;
			} else if (StrToFloat(val) < 0.000000001) {
				Application->MessageBoxW(
					L"Введены неверные данные (введите оценки больше 0)",
					L"Ошибка",
					MB_OK| MB_ICONERROR
				);

				InputDataStringGrid->Col = i;
				InputDataStringGrid->Row = j;
				return false;
            }
		}

//		try {
//			double t = InputDataStringGrid->Cells[i+fixedCols][fixedRows].ToDouble();
//            UnicodeString
//		} catch (EConvertError &e) {
//			for (int k = 0;  k < cols; ++k) {
//				InputDataStringGrid->Cells[+fixedCols][fixedRows].
//			}
//		}
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

void __fastcall TForm1::MMEditProjectClick(TObject *Sender)
{
	NewProjectForm->ShowModal();
	showCurrentProject();
//	initGrid();
//	MethodComboBox->ItemIndex = projectManager.getCurrentProject().getMethod();
//
//	for (int i = 0; i < getAlternativesCount(); ++i) {
//        vector<double> &estimates = projectManager.getCurrentProject().getAlternativeEstimates()[i].getPriorities();
//		for (int j = 0; j < getCriteriaCount(); ++j) {
//			InputDataStringGrid->Cells[j + fixedCols][i + fixedRows + 1] = FloatToStr(estimates[j]);
//        }
//    }
//
//	InputDataStringGrid->Refresh();
//	Form1->Caption = projectManager.getCurrentProject().getName();
//
//	if (criteriaEstimates->size() > 0) {
//		for (int i = 0; i < criteriaEstimates->size(); ++i)
//		{
//			UnicodeString str(Format(L"%.2f", &TVarRec(criteriaEstimates->at(i)), 1));
//			InputDataStringGrid->Cells[i + 1][1] = str;
//			setColWidth(str, i + 1);
//        }
//	}

	projectManager.setIsCurrentProjectSaved(false);
}
//---------------------------------------------------------------------------

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
						L"Оценки не должны быть меньше 1",
						L"Ошибка",
						MB_OK| MB_ICONERROR
					);

					InputDataStringGrid->Col = ACol;
					InputDataStringGrid->Row = ARow;
					InputDataStringGrid->Cells[ACol][ARow] = L"";
				} else {
					currentProject.getAlternativeEstimates()[ARow - fixedRows - 1].getPriorities()[ACol - fixedCols] = StrToFloat(Value);
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
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Chart1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
	if (Button == mbLeft) {
		isOnChartButtonPresssed = true;
		lastChartMousePoint = Mouse->CursorPos;
		isOnChartBorder(X, Y);
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Chart1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{
	isOnChartButtonPresssed = false;
	if (Chart1->Left < 0) {
		Chart1->Left = 0;
	}

	if (Chart1->Top < 0) {
        Chart1->Top = 0;
	}

	if (Chart1->Top > Form1->Height - Chart1->Height)
	{
		Chart1->Top = Form1->Height - Chart1->Height;
	}

	if (Chart1->Left > Form1->Width - Chart1->Width)
	{
		Chart1->Left = Form1->Width - Chart1->Width;
    }
}
//---------------------------------------------------------------------------
void TForm1::isOnChartBorder(int X, int Y)
{
	const int offset = 5;
	isLeft = isRight = isTop = isBottom = false;
	isLeft= X < offset;
	isRight = X > Chart1->Width - offset;
	isTop = Y < offset;
	isBottom = Y > Chart1->Height - offset;
}

void __fastcall TForm1::Chart1MouseMove(TObject *Sender, TShiftState Shift, int X,
		  int Y)
{
	if (isOnChartButtonPresssed) {
		const TPoint changing(lastChartMousePoint.X - Mouse->CursorPos.X, lastChartMousePoint.Y - Mouse->CursorPos.Y);
		if (isLeft)
		{
			Chart1->Width += changing.X;
			Chart1->Left -= changing.X;

			if (isTop)
			{
				Chart1->Height += changing.Y;
				Chart1->Top -= changing.Y;
			} else if (isBottom) {
				Chart1->Height -= changing.Y;
			}
		} else if (isRight) {
			Chart1->Width -= changing.X;

			if (isTop)
			{
				Chart1->Height += changing.Y;
				Chart1->Top -= changing.Y;
			} else if (isBottom) {
				Chart1->Height -= changing.Y;
			}
		} else if (isBottom) {
           Chart1->Height -= changing.Y;
		} else if (isTop) {
			Chart1->Height += changing.Y;
			Chart1->Top -= changing.Y;
		} else {
			Chart1->Left -= changing.X;
			Chart1->Top -= changing.Y;
		}

		lastChartMousePoint = Mouse->CursorPos;
	} else {
       changeCursor(X, Y);
	}
}
//---------------------------------------------------------------------------
void TForm1::changeCursor(int X, int Y)
{
	isOnChartBorder(X, Y);
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
				UnicodeString str(Format(L"%.2f", &TVarRec(priorities[j]), 1));
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
			UnicodeString str(Format(L"%.2f", &TVarRec(criteriaPriorities[i]), 1));
			InputDataStringGrid->Cells[i + fixedCols][fixedRows ] = str;
			setColWidth(str, i + fixedCols);
		} else {
			InputDataStringGrid->Cells[i + fixedCols][fixedRows ] = L"";
        }
    }

	Form1->Caption = projectManager.getCurrentProject().getName();
	MethodComboBox->ItemIndex = projectManager.getCurrentProject().getMethod();
	InputDataStringGrid->SetFocus();
}
void __fastcall TForm1::MethodComboBoxSelect(TObject *Sender)
{
	projectManager.getCurrentProject().setMethod(MethodComboBox->ItemIndex);
}
//---------------------------------------------------------------------------

