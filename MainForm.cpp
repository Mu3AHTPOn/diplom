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
//конструктор формы
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner),
	gridRegex(L"[\\d]+"),
	floatGridRegex(L"[\\d]*(.|,){0,1}[\\d]*")
	, projectManager(ProjectManager::getInstance()),
	isOnChartButtonPresssed(false) {

	Form1 = this;
	UIManager::getInstance()->addForm(Form1);
	NewProjectForm = new TNewProjectForm(this);

	EvalCriteriaWeightsForm = new TEvalCriteriaWeightsForm(this);

	//прячем таблицу ввода данных
	InputDataStringGrid->Visible = false;

	//определение ширины MethodComboBox
	MethodComboBox->Canvas->Font->Size = MethodComboBox->Font->Size;
	const int AHP = MethodComboBox->Canvas->TextWidth((*MethodComboBox->Items)[0]);
	const int WS = MethodComboBox->Canvas->TextWidth((*MethodComboBox->Items)[1]);

	MethodComboBox->Width = AHP > WS ? AHP + 30 : WS + 30;
}
//---------------------------------------------------------------------------
//событие нажатия на кнопку таблицы исходных данных
void __fastcall TForm1::InputDataStringGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Shift.Contains(ssCtrl)&& Key == 86U) { //вставка из Excel
		//получаем длину строки буфера обмена
		TClipboard *cb = Clipboard();
		wchar_t *str = new wchar_t[0];
		int len = cb->GetTextBuf(str, 0);
		//получаем строку из буфера обмена
		str = new wchar_t[len];
		cb->GetTextBuf(str, len);
		UnicodeString uStr(str, len);
		int i = 0, j = InputDataStringGrid->Col, k = InputDataStringGrid->Row;
		//заполняем таблицу данными
		UnicodeString pasteString;
		while(str[i])
		{
			//при символе "Tab" переход вправо
			if (str[i] == '\t') {
				InputDataStringGrid->Cells[j][k] = pasteString;
				pasteString = "";

				++i;
				++j;
			}
			//при символе новой строки  переход вниз
			if (str[i] == '\n') {
				InputDataStringGrid->Cells[j][k] = pasteString;
				pasteString = "";

				++i;
				j = InputDataStringGrid->Col;
				++k;
			}

			//накопление строки если ни один из символов выше не
			//встретился
			pasteString += str[i];
			++i;
		}

		//заполнение ячейки
		InputDataStringGrid->Cells[j][k] = pasteString;

	} else if (Shift.Contains(ssCtrl) && Key == 67U) { //copy event

	} else if (Key == 46U) {	//on del button
		InputDataStringGrid->Cells[InputDataStringGrid->Col][InputDataStringGrid->Row] = L"";
	}
}
//---------------------------------------------------------------------------
//инициализация размеров таблицы
void TForm1::initGrid()
{
	InputDataStringGrid->ColCount = getCriteriaCount() + fixedCols;
	InputDataStringGrid->RowCount = getAlternativesCount() + fixedRows + 1;
}
//---------------------------------------------------------------------------
//событие создания форты
void __fastcall TForm1::FormCreate(TObject *Sender)
{

	MethodComboBox->ItemIndex = MathMethods::WS;

	fixedCols = InputDataStringGrid->FixedCols;
	fixedRows = InputDataStringGrid->FixedRows;
}
//---------------------------------------------------------------------------
//возвращает количество критериев если проект открыт
int TForm1::getCriteriaCount()
{
	Project &project = projectManager.getCurrentProject();
	return &project == NULL ? 0 : project.getCriteriaCount();

}
//--------------------------------------------------------------------------
//возвращает количество альтернатив если проект открыт
int TForm1::getAlternativesCount()
{
	Project &project = projectManager.getCurrentProject();
	return &project == NULL ? 0 : project.getAlternativesCount();

}
//---------------------------------------------------------------------------
//прячет коретку из поля результатов вычислений
void __fastcall TForm1::Memo1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	HideCaret(ResultRichEdit->Handle);
}
//---------------------------------------------------------------------------
//прячет коретку из поля результатов вычислений
void __fastcall TForm1::Memo1MouseEnter(TObject *Sender)
{
	HideCaret(ResultRichEdit->Handle);
}
//---------------------------------------------------------------------------
//событие закртия приложения через гланое меню
void __fastcall TForm1::MMCloseAppClick(TObject *Sender)
{
	//если проект не удалось закрыть - форма не закрывается
	if (closeProject())
	{
		UIManager::getInstance()->closeApp(this);
		Close();
	}
}
//---------------------------------------------------------------------------
//событие на закрытии формы
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
//событие контекстного меню резуьтатов вычислений "Очистить"
void __fastcall TForm1::N5Click(TObject *Sender)
{
	ResultRichEdit->Text = L"";
	Chart1->Series[0]->Clear();
}
//---------------------------------------------------------------------------
//событие расчёта через главное меню
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
//событие расчёта через панель быстрого доступа
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
//---------------------------------------------------------------------------
//расчёт методом анализа иерархий
void TForm1::evalAHP()
{
	Matrix<double> &result = processor.evalWS();
	showResult(result, L"Метод анализа иерархий");
	delete &result;
}
//расчёт методом взвешенной суммы мест
void TForm1::evalWS()
{
	Matrix<double> &result = processor.evalWS();
	showResult(result, L"Метод взвешенной суммы мест");
	delete &result;
}
//отображание результатов на графику
void TForm1::showResultAtChart(Matrix<double> &result)
{
	Chart1->Series[0]->Clear();

	double max(0);
	vector<UnicodeString> alternativeNames = projectManager.getCurrentProject().getAlternativeNames();
	//расчёт максимального значения для шкалы по Y
	for (int i = 0; i < result.getHeight(); ++i)
	{
		max = result[i][0] > max ? result[i][0] : max;
		Chart1->Series[0]->Add(result[i][0], alternativeNames[i]);
	}

	Chart1->Axes->Left->Maximum = max * 1.2;

}
//---------------------------------------------------------------------------
//отображение результотов
void TForm1::showResult(Matrix<double> &result, UnicodeString method) {
	UnicodeString resultStr = L"(";

	for (int i = 0; i < result.getHeight() - 1; ++i) {
		resultStr += Format(L"%.3f; ", new TVarRec(result[i][0]), 1);
	}

	resultStr += Format(L"%.3f)", new TVarRec(result[result.getHeight() - 1][0]), 1);

	UnicodeString methodCaption = method;
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
//---------------------------------------------------------------------------
//событие рисования ячейки таблицы
void __fastcall TForm1::InputDataStringGridDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State)
{
	//пишем название в фикированных столбцах
	if (ACol == 0 || ARow == 0) {
		drawFixedColNames(ACol, ARow, Rect);
	}

	//закрашиваем первую строку и пишем её значения
	if (ARow == 1 && ! (InputDataStringGrid->Row == ARow && InputDataStringGrid->Col == ACol))
	{
		InputDataStringGrid->Canvas->Brush->Color = cl3DLight;
		InputDataStringGrid->Canvas->FillRect(InputDataStringGrid->CellRect(ACol, ARow));
		UnicodeString str = ACol == 0 ? UnicodeString(L"Важность критериев") : InputDataStringGrid->Cells[ACol][ARow];
		InputDataStringGrid->Canvas->TextOut(Rect.Left + 2, Rect.Top + 4, str);
	}
}
//---------------------------------------------------------------------------
//событие двойного нажатия на ячейки, показывает форму изменения имени
//критерия либо альтернативы и устанавливает введённое в форме значение
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
//занимается отрисовкой имён альтернатив и критериев
//необходима для динамической установки ширины/высоты ячеек и
//рисования вертикального текста
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

		//устанавливаем ориентацию текста как 90 градусуов и небодьшое смещение
		InputDataStringGrid->Canvas->Font->Orientation = 90 * 10;
		InputDataStringGrid->Canvas->TextOut(Rect.Left, Rect.Bottom - 6, name);
	}
}
 //---------------------------------------------------------------------------
//вычисляет и устанавливет высоту строки
void TForm1::setRowHeight(UnicodeString &str)
{
	int rowHeight = InputDataStringGrid->RowHeights[0];
	InputDataStringGrid->Canvas->Font->Size = InputDataStringGrid->Font->Size;
	int newRowHeight= InputDataStringGrid->Canvas->TextWidth(str) + 6 + 6;
	InputDataStringGrid->RowHeights[0] = rowHeight > newRowHeight ? rowHeight : newRowHeight;
}
 //---------------------------------------------------------------------------
//вычисляет и устанавливает ширину столбца
void TForm1::setColWidth(UnicodeString &str, int col)
{
	int columnWidth = InputDataStringGrid->ColWidths[col];
	InputDataStringGrid->Canvas->Font->Size = InputDataStringGrid->Font->Size;
	int newColumnWidth = InputDataStringGrid->Canvas->TextWidth(str) + 6 + 6 + InputDataStringGrid->GridLineWidth;
	InputDataStringGrid->ColWidths[col] = columnWidth > newColumnWidth ? columnWidth : newColumnWidth;
}
//---------------------------------------------------------------------------
//сохраняет текущий проект
void TForm1::saveProject()
{
    if (projectManager.isProjectOpen() && SaveDialog1->Execute(this->Handle))
	{
		projectManager.saveProject(SaveDialog1->FileName);
	}
}
//--------------------------------------------------------------------------
//загружает проект
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
			//сообщение об ошибке в случае невозможности открытия
			Application->MessageBoxW(
				L"Файл проекта повреждён!",
				L"Ошибка!",
				MB_OK| MB_ICONERROR
		   );
		}
	}
}
//--------------------------------------------------------------------------
//создаёт новый проект
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
		//если проект был создан - заполняем таблицу
		showCurrentProject();
    }
}
//--------------------------------------------------------------------------
// закрываем проект
bool TForm1::closeProject()
{
    //покаызваем диалог о сохранении текущего проекта
	if (! showSaveDialog())
	{
		return false;
	}

	if (! projectManager.isProjectOpen())
	{
        return true;
    }

	//очистка табицы
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
	MethodComboBox->ItemIndex = 0;

	projectManager.closeProject();
	return true;
}
//--------------------------------------------------------------------------
//предлагаем сохранить текущий проект
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
//проверяем нет ли непривальных данных в таблице
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
//редактирование проекта
void __fastcall TForm1::MMEditProjectClick(TObject *Sender)
{
	if (projectManager.isProjectOpen()) {
		NewProjectForm->ShowModal();
		showCurrentProject();

		projectManager.setIsCurrentProjectSaved(false);
    }
}
//---------------------------------------------------------------------------
//провряем, что ввёл пользователь и выводим диалог ошибки если необходимо
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
}
//---------------------------------------------------------------------------
//запоминаем нажатие на графике (необходимо для его перетаскивания и изменения размеров)
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
//после того, как клавиша мыши отпушена, проверяем не вышел ли график за
//пределы формы и возвращаем его обратно, если такое случилось
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
//возвращает true если мышь на границе графика (необходимо для замены курсора
//на курсор изменения размеров)
void TForm1::isOnChartBorder(int X, int Y)
{
	const int offset = 5;
	isLeft = isRight = isTop = isBottom = false;
	isLeft= X < offset;
	isRight = X > Chart1->Width - offset;
	isTop = Y < offset;
	isBottom = Y > Chart1->Height - offset;
}
//перемещаем либо изменяем размер графика
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
			//перемещение графика
			Chart1->Left -= changing.X;
			Chart1->Top -= changing.Y;
		}

		lastChartMousePoint = Mouse->CursorPos;
	} else {
       changeCursor(X, Y);
	}
}
//---------------------------------------------------------------------------
//меняем курсов на курсов изменения раземра
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
//отображаем текущий проект на форме
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
	MethodComboBox->ItemIndex = projectManager.getCurrentProject().getMethod();
	InputDataStringGrid->SetFocus();
}
void __fastcall TForm1::MethodComboBoxSelect(TObject *Sender)
{
	if (projectManager.isProjectOpen()) {
		projectManager.getCurrentProject().setMethod(MethodComboBox->ItemIndex);
    }
}
//---------------------------------------------------------------------------

