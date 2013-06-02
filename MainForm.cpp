//---------------------------------------------------------------------------

#include <vcl.h>
#include <Clipbrd.hpp>
#include <limits>


#pragma hdrstop

#include "MainForm.h"
#include "UIManager.cpp"
#include "setColRowNameFormUnit.cpp"
#include "NewProjectUnit.cpp"
#include <DBXJSON.hpp>
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
	colNames = new vector<UnicodeString>();
	rowNames = new vector<UnicodeString>();
	criteriaEstimates = new vector<double>();
	projectName = new UnicodeString(L"Новый проект");
	bool isClose(false), isOpen(false);
	NewProjectForm->setRowNamesArray(rowNames);
	NewProjectForm->setColNamesArray(colNames);
	NewProjectForm->setProjectName(projectName);

	EvalCriteriaWeightsForm = new TEvalCriteriaWeightsForm(this);
	EvalCriteriaWeightsForm->setColNamesArray(colNames);
	EvalCriteriaWeightsForm->setCriteriaEstimatesArray(criteriaEstimates);

	InputDataStringGrid->Visible = false;
	Form1->Caption = (*projectName);

	const int AHP = Canvas->TextWidth((*MethodComboBox->Items)[0]);
	const int WS = Canvas->TextWidth((*MethodComboBox->Items)[1]);

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
	InputDataStringGrid->ColCount = colNames->size() + fixedCols;
	InputDataStringGrid->RowCount = rowNames->size() + fixedRows + 1;
}

void __fastcall TForm1::FormCreate(TObject *Sender)
{

	MethodComboBox->ItemIndex = 0;

	fixedCols = InputDataStringGrid->FixedCols;
	fixedRows = InputDataStringGrid->FixedRows;

	initGrid();

//	InputDataStringGrid->Col = 1;
//	InputDataStringGrid->Row = 1;
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
	if (isDataValid()) {
		if (MethodComboBox->ItemIndex == 0) {
		   evalAHP();
		} else
		{
			evalWS();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SpeedButton1Click(TObject *Sender)
{
	if (isDataValid()) {
		if (MethodComboBox->ItemIndex == 0) {
		   evalAHP();
		} else
		{
			evalWS();
		}
    }
}

void TForm1::evalAHP()
{
//создаём множество объектов ahpsolver для оценок важности критериев и для
//каждого критерия в частности
try {
	const int criteriaCount = getCriteriaCount();
	const int objectCount = getObjectsCount();

	Matrix<double> criteriaEstimates(criteriaCount);

	Matrix<int> *objectEstimates = new Matrix<int>[criteriaCount];
	AHPSolver<int> *ahpObjects = new AHPSolver<int>[criteriaCount];

	for (int i = 0; i < criteriaCount; ++i) {
		criteriaEstimates[i][0] = InputDataStringGrid->Cells[i+fixedCols][fixedRows].ToDouble();
		objectEstimates[i] = *(new Matrix<int>(objectCount));
	}


	//fill objectEstimates
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

	Matrix<double> result = integrEstimate * criteriaEstimates;

	UnicodeString resultStr = L"(";

	for (int i = 0; i < result.getHeight() - 1; ++i) {
		resultStr += Format(L"%.3f, ", new TVarRec(result[i][0]), 1);
	}

	resultStr += Format(L"%.3f)", new TVarRec(result[result.getHeight() - 1][0]), 1);

	UnicodeString methodCaption = L"Метод анализа иерархий";
	UnicodeString resultCaption = L"Расчитанный рейтинг";
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
	const int objectCount = getObjectsCount();

	Matrix<double> criteriaEstimates(criteriaCount);
	Matrix<double> objectEstimates(objectCount, criteriaCount);

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

	Matrix<double> &result = (objectEstimates * criteriaEstimates).normalizeToOne();

	UnicodeString resultStr = L"(";

	for (int i = 0; i < result.getHeight() - 1; ++i) {
		resultStr += Format(L"%.3f, ", new TVarRec(result[i][0]), 1);
	}

	resultStr += Format(L"%.3f)", new TVarRec(result[result.getHeight() - 1][0]), 1);

	UnicodeString methodCaption = L"Метод взвешенной суммы мест";
	UnicodeString resultCaption = L"Расчитанный рейтинг";
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

	for (int i = 0; i < result.getHeight(); ++i)
	{
		Chart1->Series[0]->Add(result[i][0], rowNames->at(i));
	}

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
	for (int i = fixedCols; i < cols; i++) {
		TRect rect(InputDataStringGrid->CellRect(i, 0));
		if (rect.Contains(cursorPoint)) {
			UnicodeString *newName = &colNames->at(i - 1);
			TsetCollRowNameForm *form = new TsetCollRowNameForm(this);
			form->setResultStr(newName);
			try {
			  form->ShowModal();
			  if (newName != L"") {
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

	for (int i = fixedRows + 1; i < rows; i++) {                              // +1 - строка важности критериев
		TRect rect(InputDataStringGrid->CellRect(0, i));
		if (rect.Contains(cursorPoint)) {
			UnicodeString *newName = &rowNames->at(i - 2);
			TsetCollRowNameForm *form = new TsetCollRowNameForm(this);
			form->setResultStr(newName);
			try {
			  form->ShowModal();
			  if (newName != L"") {
				  setRowHeight(*newName);
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

	if (ARow == 1) {
		UnicodeString name = L"Важность критериев";
		setColWidth(name);
		InputDataStringGrid->Canvas->TextRect(Rect, name);
		return;
	}

	if (ACol == 0) {
		UnicodeString &name = rowNames->at(ARow - 2);
		setColWidth(name);
		InputDataStringGrid->Canvas->TextRect(Rect, name);
	}

	if (ARow == 0) {
		UnicodeString &name = colNames->at(ACol- 1);
        setRowHeight(name);

		InputDataStringGrid->Canvas->Font->Orientation = 90 * 10;
		InputDataStringGrid->Canvas->TextOut(Rect.Left, Rect.Bottom - 6, name);
	}
}
 //---------------------------------------------------------------------------

void TForm1::setRowHeight(UnicodeString &str)
{
	int rowHeight = InputDataStringGrid->RowHeights[0];
	int newRowHeight= InputDataStringGrid->Canvas->TextWidth(str) + 6 + 6;
	InputDataStringGrid->RowHeights[0] = rowHeight > newRowHeight ? rowHeight : newRowHeight;
}
 //---------------------------------------------------------------------------

void TForm1::setColWidth(UnicodeString &str, int col)
{
	int columnWidth = InputDataStringGrid->ColWidths[col];
	int newColumnWidth = InputDataStringGrid->Canvas->TextWidth(str) + 6 + 6 + InputDataStringGrid->GridLineWidth;
	InputDataStringGrid->ColWidths[col] = columnWidth > newColumnWidth ? columnWidth : newColumnWidth;
}
//---------------------------------------------------------------------------

void TForm1::saveProject()
{
    if (SaveDialog1->Execute(this->Handle))
	{
		UnicodeString fileName = SaveDialog1->FileName;
		TFileStream *fs = new TFileStream(fileName, fmCreate);
		try {
			TJSONObject *js = new TJSONObject();
			TJSONArray *colNamesJSON = new TJSONArray();
			TJSONArray *rowNamesJSON = new TJSONArray();
			vector<UnicodeString>::iterator iter;
			for (iter = colNames->begin(); iter != colNames->end(); ++iter) {
				colNamesJSON->Add(*iter);
			}

			js->AddPair(L"colNames", colNamesJSON);

			for (iter = rowNames->begin(); iter != rowNames->end(); ++iter) {
				rowNamesJSON->Add(*iter);
			}

			js->AddPair(L"rowNames", rowNamesJSON);

			TJSONArray *tableData = new TJSONArray();
			for (int i = fixedCols; i < InputDataStringGrid->ColCount; i++) {
				TJSONArray *rowData = new TJSONArray();
				for (int j = fixedRows; j < InputDataStringGrid->RowCount; j++) {
					rowData->Add(InputDataStringGrid->Cells[i][j]);
				}

				tableData->AddElement(rowData);
			}

			js->AddPair(L"tableData", tableData);
			js->AddPair(L"projectName", *projectName);
			UnicodeString projectJSON = js->ToString();

			fs->Write(projectJSON.BytesOf(), projectJSON.Length());
			js->Free();

			projectManager.setIsCurrentProjectSaved(true);

		} __finally {
			fs->Free();
		}
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
	UnicodeString fileName = OpenDialog1->FileName;
	TFileStream *fs = new TFileStream(fileName, fmOpenRead);
	try {
		int n = fs->Size;
		char *chars = new char[n+1];

		fs->Read(chars, n);
		chars[n] = '\0';
		UnicodeString str(chars);

		TJSONObject *js = (TJSONObject*) TJSONObject::ParseJSONValue(str);
		TJSONArray *colNamesJSON = (TJSONArray*) js->Get(L"colNames")->JsonValue;
		TJSONArray *rowNamesJSON = (TJSONArray*) js->Get(L"rowNames")->JsonValue;

		colNames->clear();
		for (int i = 0; i < colNamesJSON->Size(); ++i) {
			colNames->push_back(colNamesJSON->Get(i)->Value());
		}

		rowNames->clear();
		for (int i = 0; i < rowNamesJSON->Size(); ++i) {
			rowNames->push_back(rowNamesJSON->Get(i)->Value());
		}

		initGrid();
		InputDataStringGrid->Visible = true;
		InputDataStringGrid->FixedCols = 1;			//bug

		TJSONArray *tableData = (TJSONArray*) js->Get(L"tableData")->JsonValue;
		for (int i = 0; i < tableData->Size(); i++) {
			TJSONArray *rowData = (TJSONArray*) tableData->Get(i);
			for (int j = 0; j < rowData->Size(); j++) {
				UnicodeString str(rowData->Get(j)->Value());
				InputDataStringGrid->Cells[i + fixedCols][j + fixedRows] = str;
				setColWidth(str, i + fixedCols);
			}
		}

		(*projectName) = js->Get(L"projectName")->JsonValue->Value();
		Form1->Caption = (*projectName);

		delete [] chars;
		js->Free();
		InputDataStringGrid->Refresh();

        projectManager.setIsProjectOpen(true);
		projectManager.setIsCurrentProjectSaved(true);
		InputDataStringGrid->SetFocus();
	} __finally {
		fs->Free();
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

	NewProjectForm->ShowModal();
	if (colNames->size() > 0 && rowNames->size() > 0) {
	   projectManager.setIsProjectOpen(true);
		initGrid();
		InputDataStringGrid->Refresh();
		Form1->Caption = (*projectName);
		for (int i = 0; i < InputDataStringGrid->ColCount; i++) {
			for (int j = 0; j < InputDataStringGrid->RowCount; j++) {
					InputDataStringGrid->Cells[i][j] = L"";
			}
		}

		InputDataStringGrid->Visible = true;
		InputDataStringGrid->FixedCols = 1;			//bug
		InputDataStringGrid->SetFocus();
	}

	if (criteriaEstimates->size() > 0) {
		for (int i = 0; i < criteriaEstimates->size(); ++i)
		{
			UnicodeString str(Format(L"%.2f", new TVarRec(criteriaEstimates->at(i)), 1));
			InputDataStringGrid->Cells[i + 1][1] = str;
			setColWidth(str, i + 1);
        }
    }
}
//--------------------------------------------------------------------------
// return false if project doesn't close
bool TForm1::closeProject()
{
	if (! showSaveDialog())
	{
		return false;
	}

	colNames->clear();
	rowNames->clear();
	(*projectName) = L"Новый проект";
	InputDataStringGrid->Visible = false;
	projectManager.setIsProjectOpen(false);
	return true;
}
//--------------------------------------------------------------------------
//return false if user click on cancel
bool TForm1::showSaveDialog()
{
    if (projectManager.isProjectOpen() && ! projectManager.isSavedCurrentPreject()) {
       const int dialogResult = MessageDlg(L"Сохранить текущий проект?", mtConfirmation, mbYesNoCancel, 0);
		if (dialogResult == mrYes) {
		   saveProject();
		} else if (dialogResult == mrCancel) {
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
			const bool isValid = j == 1 ? regex_match(val.w_str(), floatGridRegex) : regex_match(val.w_str(), gridRegex);
			if (! isValid)
			{
				MessageDlg(L"Введены неверные данные (вводить можно только целые числа)", mtError, TMsgDlgButtons() << mbOK, 0);
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
	initGrid();
	InputDataStringGrid->Refresh();
	Form1->Caption = (*projectName);

	if (criteriaEstimates->size() > 0) {
		for (int i = 0; i < criteriaEstimates->size(); ++i)
		{
			UnicodeString str(Format(L"%.2f", new TVarRec(criteriaEstimates->at(i)), 1));
			InputDataStringGrid->Cells[i + 1][1] = str;
			setColWidth(str, i + 1);
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::InputDataStringGridSetEditText(TObject *Sender, int ACol,
		  int ARow, const UnicodeString Value)
{
	const bool isValid = ARow == 1 ? regex_match(Value.w_str(), floatGridRegex) : regex_match(Value.w_str(), gridRegex);
	if (! isValid)
	{
		UnicodeString str = InputDataStringGrid->Cells[ACol][ARow];
		InputDataStringGrid->Cells[ACol][ARow] = str.SubString(1, str.Length() - 1);
	} else if (ARow != 1 && StrToInt(Value) < 1)
	{
		MessageDlg(L"Оценки не должны быть меньше 1", mtError, TMsgDlgButtons() << mbOK, 0);
		InputDataStringGrid->Col = ACol;
		InputDataStringGrid->Row = ARow;
		InputDataStringGrid->Cells[ACol][ARow] = L"";
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
	isLeft = isRight = isTop = isBottom = false;
	isLeft= X < 3;
	isRight = X > Chart1->Width - 3;
	isTop = Y < 3;
	isBottom = Y > Chart1->Height - 3;
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


