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
	: TForm(Owner), isNewProject(false)
{
}
//---------------------------------------------------------------------------
void __fastcall TNewProjectForm::FormCreate(TObject *Sender)
{
	comboBoxNamesListEdit = new TEdit(this);
	comboBoxNamesListEdit->Parent = RowNamesListBox;
	comboBoxNamesListEdit->Visible = false;
	comboBoxNamesListEdit->Ctl3D = false;
	comboBoxNamesListEdit->BorderStyle = bsNone;
	comboBoxNamesListEdit->OnKeyPress = listEditKeyPress;
	comboBoxNamesListEdit->OnExit = ListEditExit;

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

	comboBoxNamesListEdit->Parent = list;
	comboBoxNamesListEdit->Width = list->ClientWidth;

	TRect lRect(list->ItemRect(i));
	comboBoxNamesListEdit->Top = lRect.Top + 1;
	comboBoxNamesListEdit->Left= lRect.Left+ 1;
	comboBoxNamesListEdit->Height = (lRect.Bottom - lRect.Top) + 1;

	comboBoxNamesListEdit->Text = list->Items->Strings[i];

	comboBoxNamesListEdit->Visible = true;
	comboBoxNamesListEdit->SelectAll();
	comboBoxNamesListEdit->SetFocus();
}

//---------------------------------------------------------------------------

void TNewProjectForm::removeCurrentItem(TListBox *list)
{
	onListExit();
	int i = list->ItemIndex;
	if (i != -1) {
		list->Items->Delete(i);

		if (list == RowNamesListBox) {
			currentProject->removeEstimate(i, Project::ALTERNATIVE);
		} else {
			currentProject->removeEstimate(i, Project::CRITERIA);
		}

		if (i > list->Count - 1) {
			list->ItemIndex = list->Count - 1;
		} else
		{
			list->ItemIndex = i;
		}
	}

	comboBoxNamesListEdit->Visible = false;
}
//---------------------------------------------------------------------------
void TNewProjectForm::addItem(TListBox *list, bool inEnd)
{
	onListExit();

	int index = inEnd ? -1 : list->ItemIndex;
	UnicodeString defVal("Введите название");
	list->Items->Insert(index, defVal);

	if (list == RowNamesListBox) {
		currentProject->addEstimate(index, defVal, Project::ALTERNATIVE);
	} else {
		currentProject->addEstimate(index, defVal, Project::CRITERIA);
	}

	//-1 значит добавить в конец
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
	TListBox* list = (TListBox*) comboBoxNamesListEdit->Parent;
	if (Key == 13U) {      //onEnter
		comboBoxNamesListEdit->Visible = false;
		Key = 0U;
		list->SetFocus();
	} else if (Key == '\x1B')      //onEscape
	{
		comboBoxNamesListEdit->Visible = false;
		Key = 0U;
		list->SetFocus();
		comboBoxNamesListEdit->Text = L"";
	}
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
	TListBox *list = (TListBox*) comboBoxNamesListEdit->Parent;
	if (list->Count > 0 && comboBoxNamesListEdit->Text != L"") {
		list->Items->Strings[list->ItemIndex] = comboBoxNamesListEdit->Text;

        if (list == RowNamesListBox) {
			currentProject->getAlternativeNames()[list->ItemIndex] = comboBoxNamesListEdit->Text;
		} else {
			currentProject->getCriteriaNames()[list->ItemIndex] = comboBoxNamesListEdit->Text;
		}

		comboBoxNamesListEdit->Visible = false;
		comboBoxNamesListEdit->Text = L"";
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
	if (isNewProject) {
       ProjectManager::getInstance().closeProject();
    }
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

	currentProject->setName(ProjectName->Text);

	if (currentProject->getMethod() == MathMethods::AHP)
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
	currentProject = &ProjectManager::getInstance().getCurrentProject();
	if (currentProject != NULL) {
		ProjectName->Text = currentProject->getName();
        MethodComboBox->ItemIndex = currentProject->getMethod();

		ColNamesListBox->Items->Clear();
		RowNamesListBox->Items->Clear();

		{
			vector<UnicodeString>::const_iterator iter = currentProject->getCriteriaNames().begin();
			vector<UnicodeString>::const_iterator endIter = currentProject->getCriteriaNames().end();
			while (iter != endIter) {
				ColNamesListBox->Items->Add(*iter);
				++iter;
			}
		}

		{
			vector<UnicodeString>::const_iterator iter = currentProject->getAlternativeNames().begin();
			vector<UnicodeString>::const_iterator endIter = currentProject->getAlternativeNames().end();

			while (iter != endIter) {
				RowNamesListBox->Items->Add(*iter);
				++iter;
			}
		}
	}
}
//---------------------------------------------------------------------------
void TNewProjectForm::setIsNewProject(bool isNew) {
	 isNewProject = isNew;
}
//---------------------------------------------------------------------------
void __fastcall TNewProjectForm::MethodComboBoxSelect(TObject *Sender)
{
	currentProject->setMethod(MethodComboBox->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TNewProjectForm::FormHide(TObject *Sender)
{
	currentProject = NULL;
	isNewProject = false;
}
//---------------------------------------------------------------------------

