//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NewProjectUnit.h"
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
}


//---------------------------------------------------------------------------
void __fastcall TNewProjectForm::RowsListBoxClick(TObject *Sender)
{
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
void TNewProjectForm::setColNamesArray(vector<UnicodeString> *inVector)
{
	this->colNames = inVector;
}

void TNewProjectForm::setRowNamesArray(vector<UnicodeString> *inVector)
{
	this->rowNames = inVector;
}
//---------------------------------------------------------------------------
void TNewProjectForm::setProjectName(UnicodeString *projectName)
{
	this->projectName = projectName;
}
//---------------------------------------------------------------------------
void TNewProjectForm::addItem(TListBox *list, bool inEnd)
{
	onListExit();

	int index = inEnd ? -1 : list->ItemIndex;
	list->Items->Insert(index, "¬ведите название");
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

	colNames->clear();
	for (int i = 0; i < colCount; i++) {
		colNames->push_back(ColNamesListBox->Items->Strings[i]);
	}

	rowNames->clear();
	for (int i = 0; i < rowCount; i++) {
		rowNames->push_back(RowNamesListBox->Items->Strings[i]);
	}

	(*projectName) = ProjectName->Text;

	Close();
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
	ProjectName->Text = (*projectName);

	ColNamesListBox->Items->Clear();
	vector<UnicodeString>::iterator iter;
	for (iter = colNames->begin(); iter != colNames->end(); ++iter) {
		ColNamesListBox->Items->Add(*iter);
	}

	RowNamesListBox->Items->Clear();
	for (iter = rowNames->begin(); iter != rowNames->end(); ++iter) {
		RowNamesListBox->Items->Add(*iter);
	}
}
//---------------------------------------------------------------------------
void __fastcall TNewProjectForm::Button1Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


