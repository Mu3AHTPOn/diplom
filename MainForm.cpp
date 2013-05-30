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
	: TForm(Owner), gridRegex(L"[\\d]*"), projectManager(ProjectManager::getInstance())
{
	Form1 = this;
	UIManager::getInstance()->addForm(Form1);
	Form1->Hide();
	NewProjectForm = new TNewProjectForm(this);
	colNames = new vector<UnicodeString>();
	rowNames = new vector<UnicodeString>();
	projectName = new UnicodeString(L"Новый проект");
	bool isClose(false), isOpen(false);
	NewProjectForm->setRowNamesArray(rowNames);
	NewProjectForm->setColNamesArray(colNames);
	NewProjectForm->setProjectName(projectName);
//	NewProjectForm->setIsClose(&isClose);
//	NewProjectForm->setIsOpen(&isOpen);
//	NewProjectForm->ShowModal();
//	
//	if (isClose) {
//		this->Close();
//		return;
//	}
//
//	if (isOpen) {
//		fixedCols = InputDataStringGrid->FixedCols;
//		fixedRows = InputDataStringGrid->FixedRows;
//        loadProject();
//	}

	InputDataStringGrid->Visible = false;
	Form1->Caption = (*projectName);

	const int AHP = Form1->Canvas->TextWidth(L"Метод анализа иерархий");
	const int WS = Form1->Canvas->TextWidth(L"Метод взвешенной суммы мест");

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
	InputDataStringGrid->ColCount = colNames->size() + fixedCols;
	InputDataStringGrid->RowCount = rowNames->size() + fixedRows + 1;
}

void __fastcall TForm1::FormCreate(TObject *Sender)
{

	MethodComboBox->ItemIndex = 0;

	fixedCols = InputDataStringGrid->FixedCols;
	fixedRows = InputDataStringGrid->FixedRows;

	initGrid();

	activeCell.X = 1;
	activeCell.Y = 1;
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

void __fastcall TForm1::ParamsValidate(TObject *Sender, int ACol, int ARow, const UnicodeString KeyName,
		  const UnicodeString KeyValue)
{
//	wregex expr(L"[\\d]+");
//	if (! regex_match(KeyValue.w_str(), expr))
//	{
//		UnicodeString str = L"Значение должно быть целым числом";
//		MessageDlg(str, mtError, mbOKCancel, 0);
//		Params->Values[KeyName] = L"5";
//	} else {
//		initGrid();
//	}
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

	Matrix<double> &result = objectEstimates.normalize() * criteriaEstimates.normalize();

	UnicodeString resultStr = L"(";

	for (int i = 0; i < result.getHeight() - 1; ++i) {
		resultStr += Format(L"%.3f, ", new TVarRec(result[i][0]), 1);
	}

	resultStr += Format(L"%.3f)", new TVarRec(result[result.getHeight() - 1][0]), 1);

	UnicodeString resultCaption = L"Расчитанный рейтинг\n";
	ResultRichEdit->Text = resultCaption;
	UnicodeString resultVector = resultStr;
	ResultRichEdit->Text = ResultRichEdit->Text + resultVector;

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

void TForm1::setColWidth(UnicodeString &str)
{
	int columnWidth = InputDataStringGrid->ColWidths[0];
	int newColumnWidth = InputDataStringGrid->Canvas->TextWidth(str) + 6 + 6 + InputDataStringGrid->GridLineWidth;
	InputDataStringGrid->ColWidths[0] = columnWidth > newColumnWidth ? columnWidth : newColumnWidth;
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

		TJSONArray *tableData = (TJSONArray*) js->Get(L"tableData")->JsonValue;
		for (int i = 0; i < tableData->Size() - fixedCols; i++) {
			TJSONArray *rowData = (TJSONArray*) tableData->Get(i);
			for (int j = 0; j < rowData->Size(); j++) {
				InputDataStringGrid->Cells[i + fixedCols][j + fixedRows] = rowData->Get(j)->Value();
			}
		}

		(*projectName) = js->Get(L"projectName")->JsonValue->Value();
		Form1->Caption = (*projectName);

		delete [] chars;
		js->Free();
		initGrid();
		InputDataStringGrid->Visible = true;
		InputDataStringGrid->FixedCols = 1;			//bug
		InputDataStringGrid->Refresh();

        projectManager.setIsProjectOpen(true);
        projectManager.setIsCurrentProjectSaved(true);
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
	for (int i = 0; i < cols; ++i)
	{
		for (int j = 0; j < rows; ++j)
		{
			if (! regex_match(InputDataStringGrid->Cells[i][j].w_str(), gridRegex))
			{
				MessageDlg(L"Ошибка! Введены неверные данные (вводить можно только целые числа)", mtError, TMsgDlgButtons() << mbOK, 0);
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

void __fastcall TForm1::MMEditProjectClick(TObject *Sender)
{
    NewProjectForm->ShowModal();
	initGrid();
	InputDataStringGrid->Refresh();
	Form1->Caption = (*projectName);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::InputDataStringGridSetEditText(TObject *Sender, int ACol,
		  int ARow, const UnicodeString Value)
{
	if (! regex_match(Value.w_str(), gridRegex))
	{
		UnicodeString str = InputDataStringGrid->Cells[ACol][ARow];
		InputDataStringGrid->Cells[ACol][ARow] = str.SubString(1, str.Length() - 1);
	}

	projectManager.setIsCurrentProjectSaved(false);
}
//---------------------------------------------------------------------------

