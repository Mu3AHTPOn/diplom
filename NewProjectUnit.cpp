//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NewProjectUnit.h"
#include "EvalCriteriaWeights.cpp"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TNewProjectForm *NewProjectForm;
//---------------------------------------------------------------------------
__fastcall TNewProjectForm::TNewProjectForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TNewProjectForm::FormCreate(TObject *Sender)
{
	listEdit = new TEdit(this);
	listEdit->Parent = RowNamesListBox;
	listEdit->Visible = false;
	listEdit->Ctl3D = false;
	listEdit->BorderStyle = bsNone;
	listEdit->OnKeyPress = listEditKeyPress;
	listEdit->OnExit = ListEditExit;

	const int AHP = Canvas->TextWidth((*MethodComboBox->Items)[0]);
	const int WS = Canvas->TextWidth((*MethodComboBox->Items)[1]);

	MethodComboBox->Width = AHP > WS ? AHP + 30 : WS + 30;
	MethodComboBox->ItemIndex = 0;
}


//---------------------------------------------------------------------------
void __fastcall TNewProjectForm::RowsListBoxClick(TObject *Sender)
{
//	const int n = RowNamesListBox->Count
//	RowNamesListBox->ItemRect()
	showEditText(RowNamesListBox);
}
//---------------------------------------------------------------------------
void __fastcall TNewProjectForm::ColNamesListBoxDblClick(TObject *Sender)
{
	showEditText(ColNamesListBox);
}
//---------------------------------------------------------------------------
void __fastcall TNewProjectForm::listEditKeyPress(TObject *Sender, System::WideChar &Key)
{
	onListKeyPress(Key);
}
//---------------------------------------------------------------------------
void __fastcall TNewProjectForm::ListEditExit(TObject *Sender)
{
	onListExit();
}
//---------------------------------------------------------------------------
void __fastcall TNewProjectForm::RowNamesListBoxKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift)
{
	onListKeyDown(RowNamesListBox, Key, Shift);
}
//---------------------------------------------------------------------------
void TNewProjectForm::showEditText(TListBox *list)
{
	int i(list->ItemIndex);
	if (i == -1) {
		return;
	}

	listEdit->Parent = list;
	listEdit->Width = list->ClientWidth;

	TRect lRect(list->ItemRect(i));
	listEdit->Top = lRect.Top + 1;
	listEdit->Left= lRect.Left+ 1;
	listEdit->Height = (lRect.Bottom - lRect.Top) + 1;

	listEdit->Text = list->Items->Strings[i];

	listEdit->Visible = true;
	listEdit->SelectAll();
	listEdit->SetFocus();
}

//---------------------------------------------------------------------------

void TNewProjectForm::removeCurrentItem(TListBox *list)
{
	onListExit();
	int i = list->ItemIndex;
	if (i != -1) {
		list->Items->Delete(i);
		if (i > list->Count - 1) {
			list->ItemIndex = list->Count - 1;
		} else
		{
			list->ItemIndex = i;
		}
	}

	listEdit->Visible = false;
}
//---------------------------------------------------------------------------
void TNewProjectForm::addItem(TListBox *list, bool inEnd)
{
	onListExit();

	int index = inEnd ? -1 : list->ItemIndex;
	list->Items->Insert(index, "Введите название");
	if (index == -1) {
		list->ItemIndex = list->Count - 1;
	} else {
		list->ItemIndex = index;
	}

	showEditText(list);
}
//---------------------------------------------------------------------------
void TNewProjectForm::moveItemUp(TListBox *list)
{
	onListExit();
	int i = list->ItemIndex;
	if (i > 0) {
		list->Items->Move(i, i - 1);
		list->ItemIndex = i - 1;
	}
}

void TNewProjectForm::moveItemDown(TListBox *list)
{
	onListExit();
	int i = list->ItemIndex;
	if (i < list->Count - 1) {
		list->Items->Move(i, i + 1);
		list->ItemIndex = i + 1;
	}
}
//---------------------------------------------------------------------------
void TNewProjectForm::onListKeyPress(System::WideChar &Key)
{
	TListBox* list = (TListBox*) listEdit->Parent;
	if (Key == 13U) {      //onEnter
		listEdit->Visible = false;
		Key = 0U;
		list->SetFocus();
	} else if (Key == '\x1B')      //onEscape
	{
		listEdit->Visible = false;
		Key = 0U;
		list->SetFocus();
		listEdit->Text = L"";
	}

//	list->Items->Strings[list->ItemIndex] = listEdit->Text;
}
//---------------------------------------------------------------------------
void TNewProjectForm::onListKeyDown(TListBox* list, WORD &Key, TShiftState Shift)
{
	if (Key == 46U) {       //onDel
		removeCurrentItem(list);
	} else if (Key == 45U)                        //onInsert
	{
		addItem(list);
    }
}
//---------------------------------------------------------------------------
void TNewProjectForm::onListExit()
{
	TListBox *list = (TListBox*) listEdit->Parent;
	if (list->Count > 0 && listEdit->Text != L"") {
		list->Items->Strings[list->ItemIndex] = listEdit->Text;
		listEdit->Visible = false;
		listEdit->Text = L"";
	}
}
//---------------------------------------------------------------------------
void __fastcall TNewProjectForm::RowAddButtonClick(TObject *Sender)
{
	addItem(RowNamesListBox, true);
}

//---------------------------------------------------------------------------
void __fastcall TNewProjectForm::RowMoveUpButtonClick(TObject *Sender)
{
	moveItemUp(RowNamesListBox);
}
//---------------------------------------------------------------------------
void __fastcall TNewProjectForm::RowMoveDownButtonClick(TObject *Sender)
{
	moveItemDown(RowNamesListBox);
}
//---------------------------------------------------------------------------
void __fastcall TNewProjectForm::RowRemoveButtonClick(TObject *Sender)
{
	removeCurrentItem(RowNamesListBox);
}
//---------------------------------------------------------------------------
void __fastcall TNewProjectForm::CancelButtonClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------


void __fastcall TNewProjectForm::NextButtonClick(TObject *Sender)
{
	onListExit();
	int colCount = ColNamesListBox->Count;
	int rowCount = RowNamesListBox->Count;
	if (colCount == 0) {
		//TODO show error
		return;
	}

	if (rowCount == 0) {
		//TODO show error
		return;
	}

	ProjectManager &pm = ProjectManager::getInstance();
	Project &project = pm.newProject();

	vector<UnicodeString> &criteriaNames = project.getCriteriaNames();
	for (int i = 0; i < colCount; i++) {
		criteriaNames.push_back(ColNamesListBox->Items->Strings[i]);
	}

	vector<AlternativeEstimates> &alternativeEstimates = project.getAlternativeEstimates();
	for (int i = 0; i < rowCount; i++) {
		AlternativeEstimates a(RowNamesListBox->Items->Strings[i]);
		alternativeEstimates.push_back(a);
	}

	project.setName(ProjectName->Text);

	if (MethodComboBox->ItemIndex == 1)
	{
		Hide();
		bool isBack(false);
		EvalCriteriaWeightsForm->setBackPointer(&isBack);
		EvalCriteriaWeightsForm->ShowModal();
		if (isBack) {
			Show();
		} else {
			Close();
		}
	} else
	{
//			MessageDlg(L"Желаете ли вы рассчитать важность критериев с помощью метода анализа ииерархий?",
//						mtConfirmation,
//						mbYesNo,
//						0,
//						mbYes) == mrYes)
		if (Application->MessageBoxW(L"Желаете ли вы рассчитать важность критериев с помощью метода анализа ииерархий?",
									L"Подтвердить?",
									MB_YESNO| MB_ICONQUESTION
					)
				 == IDYES)

		{
			Hide();
			bool isBack(false);
			EvalCriteriaWeightsForm->setBackPointer(&isBack);
			EvalCriteriaWeightsForm->ShowModal();
			if (isBack) {
				Show();
			} else {
				Close();
			}
		} else {
			Close();
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall TNewProjectForm::ColAddButtonClick(TObject *Sender)
{
	addItem(ColNamesListBox, true);
}
//---------------------------------------------------------------------------

void __fastcall TNewProjectForm::ColRemoveButtonClick(TObject *Sender)
{
	removeCurrentItem(ColNamesListBox);
}
//---------------------------------------------------------------------------

void __fastcall TNewProjectForm::ColMoveUpButtonClick(TObject *Sender)
{
	moveItemUp(ColNamesListBox);
}
//---------------------------------------------------------------------------

void __fastcall TNewProjectForm::ColMoveDownButtonClick(TObject *Sender)
{
	moveItemDown(ColNamesListBox);
}
//---------------------------------------------------------------------------
void __fastcall TNewProjectForm::ColNamesListBoxKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift)
{
	onListKeyDown(ColNamesListBox, Key, Shift);
}
//---------------------------------------------------------------------------

void __fastcall TNewProjectForm::FormShow(TObject *Sender)
{
//	ProjectName->Text = (*projectName);
//
//	ColNamesListBox->Items->Clear();
//	vector<UnicodeString>::iterator iter;
//	for (iter = colNames->begin(); iter != colNames->end(); ++iter) {
//		ColNamesListBox->Items->Add(*iter);
//	}
//
//	RowNamesListBox->Items->Clear();
//	for (iter = rowNames->begin(); iter != rowNames->end(); ++iter) {
//		RowNamesListBox->Items->Add(*iter);
//	}
}
//---------------------------------------------------------------------------
void __fastcall TNewProjectForm::Button1Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------




