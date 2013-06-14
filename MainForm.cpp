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

	//прячем таблицу ввода данных
	InputDataStringGrid->Visible = false;
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
		UIManager::getInstance().closeApp(this);
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

	UIManager::getInstance().closeApp(this);
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
	evalProject(true);
}
//---------------------------------------------------------------------------
//событие расчёта через панель быстрого доступа
void __fastcall TForm1::SpeedButton1Click(TObject *Sender)
{
	evalProject(true);
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
	} else if (InputDataStringGrid->Cells[ACol][ARow].IsEmpty()) {
		if (UIManager::getInstance().getIndicator()) {
			//закрашиваем пустые ячейки жёлтым цветом
			const TColor oldColor = InputDataStringGrid->Canvas->Brush->Color;
			InputDataStringGrid->Canvas->Brush->Color = emptyCellBG;
			InputDataStringGrid->Canvas->FillRect(InputDataStringGrid->CellRect(ACol, ARow));
			InputDataStringGrid->Canvas->Brush->Color = oldColor;
        }
	} else if (UIManager::getInstance().getIndicator() && StrToFloat(InputDataStringGrid->Cells[ACol][ARow]) == 0) {
		//закрашиваем красным ячейки с нулём
		const TColor oldColor = InputDataStringGrid->Canvas->Brush->Color;
		InputDataStringGrid->Canvas->Brush->Color = clRed;
		InputDataStringGrid->Canvas->FillRect(InputDataStringGrid->CellRect(ACol, ARow));
		InputDataStringGrid->Canvas->TextOut(Rect.Left+ 2, Rect.Top + 4, InputDataStringGrid->Cells[ACol][ARow]);
		InputDataStringGrid->Canvas->Brush->Color = oldColor;
	} else if (ARow == 1 && ! (InputDataStringGrid->Row == ARow && InputDataStringGrid->Col == ACol)) {
		//закрашиваем первую строку и пишем её значения
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
//открывает форму параметров проекта
void TForm1::editProject() {
	if (projectManager.isProjectOpen()) {
		NewProjectForm->ShowModal();
		showCurrentProject();

		projectManager.setIsCurrentProjectSaved(false);
	}
}
//производит расчёт проекта
void TForm1::evalProject(bool allowDialogs) {
	if (projectManager.isProjectOpen() && isDataValid(allowDialogs)) {
		if (projectManager.getCurrentProject().getMethod() == MathMethods::WS) {
		   evalWS();
		} else {
		   evalAHP();
		}
	}

	showHint(L"Расчёты произведены");
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

	projectManager.closeProject();
	showHint(L"Откройте либо создайте новый проект");
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
					showHint(L"Заполните оценку для критерия \"" +
							project.getCriteriaNames()[j - fixedCols] + L"\""
						);
				} else {
					showHint(L"Заполните оценку для альтернативы \"" +
							project.getAlternativeNames()[i - fixedRows - 1] +
							L"\" по критерию \"" +
							project.getCriteriaNames()[j - fixedCols] + L"\""
						);
                }
				if (allowDialogs) {
						Application->MessageBoxW(
							L"Введены неверные данные (вводить можно только целые числа)",
							L"Ошибка",
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
						L"Введены неверные данные (вводить можно только целые числа)",
						L"Ошибка",
						MB_OK| MB_ICONERROR
					);
				}

				InputDataStringGrid->Col = j;
				InputDataStringGrid->Row = i;
				return false;
			} else if (StrToFloat(val) == 0) {
				if (allowDialogs) {
					Application->MessageBoxW(
						L"Введено недопустимое значение",
						L"Ошибка",
						MB_OK| MB_ICONERROR
					);
				}

				Project &project = projectManager.getCurrentProject();
				showHint(L"Введено недопустимое значение для оценки альтернативы \"" +
							project.getAlternativeNames()[i - fixedRows - 1] +
							L"\" по критерию \"" +
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
//редактирование проекта
void __fastcall TForm1::MMEditProjectClick(TObject *Sender)
{
	editProject();
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

	if (isDataValid(false)) {
		if (UIManager::getInstance().getAutoEval()) {
			evalProject(false);
		} else {
			showHint(L"Нажмите расчёт!");
		}
	}
}
//---------------------------------------------------------------------------
//запоминаем нажатие на графике (необходимо для его перетаскивания и изменения размеров)
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
//после того, как клавиша мыши отпушена, проверяем не вышел ли график за
//пределы формы и возвращаем его обратно, если такое случилось
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
//возвращает true если мышь на границе графика (необходимо для замены курсора
//на курсор изменения размеров)
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
//перемещаем либо изменяем размер графика
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
			//перемещение графика
			control->Left -= changing.X;
			control->Top -= changing.Y;
		}

		lastChartMousePoint = Mouse->CursorPos;
	} else {
	   changeCursor(Sender, X, Y);
	}
}
//---------------------------------------------------------------------------
//меняем курсор объектов, которые могут менять размеры
void TForm1::changeCursor(TObject* Sender, int X, int Y)
{
	if (Sender == Chart1) {
	   changeChartCursor(Sender, X, Y);
	} else if (Sender == HintMemo) {
       changeHintCursor(Sender, X, Y);
    }
}
//---------------------------------------------------------------------------
//меняем курсор графика на курсов изменения раземра
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
//меняем курсор подсказки на курсов изменения раземра
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
	InputDataStringGrid->SetFocus();

	if (isDataValid(false)) {
		if (UIManager::getInstance().getAutoEval()) {
			evalProject(false);
		} else {
			showHint(L"Нажмите расчёт!");
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
//				L"Оценка не может быть равна 0",
//				L"Ошибка",
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
					showHint(L"Заполните оценку для критерия \""
							+ project.getCriteriaNames()[j] + L"\""
						);

					return;
				} else {
					showHint(L"Заполните оценку для альтернативы \""
							+ project.getAlternativeNames()[i - 1]
							+ L"\" по критерию \""
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

