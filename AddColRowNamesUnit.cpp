//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AddColRowNamesUnit.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TAddColRowNamesForm *AddColRowNamesForm;
//---------------------------------------------------------------------------
__fastcall TAddColRowNamesForm::TAddColRowNamesForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAddColRowNamesForm::FormCreate(TObject *Sender)
{
	listEdit = new TEdit(this);
	listEdit->Visible = false;
	listEdit->Ctl3D = false;
	listEdit->BorderStyle = bsNone;
	listEdit->OnKeyPress = listEditKeyPress;
	listEdit->OnExit = RowsListBoxExit;
}


//---------------------------------------------------------------------------
void __fastcall TAddColRowNamesForm::RowsListBoxClick(TObject *Sender)
{
	showEditText(RowNamesListBox);
}
//---------------------------------------------------------------------------
void __fastcall TAddColRowNamesForm::ColNamesListBoxDblClick(TObject *Sender)
{
	showEditText(ColNamesListBox);
}
//---------------------------------------------------------------------------
void __fastcall TAddColRowNamesForm::listEditKeyPress(TObject *Sender, System::WideChar &Key)
{
	onListKeyPress(Key);
}
//---------------------------------------------------------------------------
void __fastcall TAddColRowNamesForm::RowsListBoxExit(TObject *Sender)
{
	onListExit(RowNamesListBox);
}
//---------------------------------------------------------------------------
void __fastcall TAddColRowNamesForm::RowNamesListBoxKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift)
{
	onListKeyDown(RowNamesListBox, Key, Shift);
}
//---------------------------------------------------------------------------

void TAddColRowNamesForm::showEditText(TListBox *list)
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

void TAddColRowNamesForm::removeCurrentItem(TListBox *list)
{
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
	listEdit->Text = L"";
}
//---------------------------------------------------------------------------
void TAddColRowNamesForm::setColNamesArray(vector<UnicodeString> *vector)
{
	this->colNames = vector;
}

void TAddColRowNamesForm::setRowNamesArray(vector<UnicodeString> *vector)
{
	this->rowNames = vector;
}
//---------------------------------------------------------------------------
void TAddColRowNamesForm::addItem(TListBox *list, bool inEnd)
{
	int index = inEnd ? -1 : list->ItemIndex;
	list->Items->Insert(index, "������� ��������");
	if (index == -1) {
		list->ItemIndex = list->Count - 1;
	} else {
		list->ItemIndex = index;
	}

	showEditText(list);
}
//---------------------------------------------------------------------------
void TAddColRowNamesForm::moveItemUp(TListBox *list)
{
	int i = list->ItemIndex;
	if (i > 0) {
		list->Items->Move(i, i - 1);
		list->ItemIndex = i - 1;
	}
}

void TAddColRowNamesForm::moveItemDown(TListBox *list)
{
	int i = list->ItemIndex;
	if (i < list->Count - 1) {
		list->Items->Move(i, i + 1);
		list->ItemIndex = i + 1;
	}
}
//---------------------------------------------------------------------------
void TAddColRowNamesForm::onListKeyPress(System::WideChar &Key)
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
	} else
	{
		list->Items->Strings[list->ItemIndex] = listEdit->Text;
	}
}
//---------------------------------------------------------------------------
void TAddColRowNamesForm::onListKeyDown(TListBox* list, WORD &Key, TShiftState Shift)
{
	if (Key == 46U) {       //onDel
		removeCurrentItem(list);
	} else if (Key == 45U)                        //onInsert
	{
		addItem(list);
    }
}
//---------------------------------------------------------------------------
void TAddColRowNamesForm::onListExit(TListBox* list)
{
//	->Items->Strings[RowsListBox->ItemIndex] = listEdit->Text;
	listEdit->Visible = false;
	listEdit->Text = L"";
}
//---------------------------------------------------------------------------
void __fastcall TAddColRowNamesForm::RowAddButtonClick(TObject *Sender)
{
	addItem(RowNamesListBox, true);
}

//---------------------------------------------------------------------------
void __fastcall TAddColRowNamesForm::RowMoveUpButtonClick(TObject *Sender)
{
	moveItemUp(RowNamesListBox);
}
//---------------------------------------------------------------------------
void __fastcall TAddColRowNamesForm::RowMoveDownButtonClick(TObject *Sender)
{
	moveItemDown(RowNamesListBox);
}
//---------------------------------------------------------------------------
void __fastcall TAddColRowNamesForm::RowRemoveButtonClick(TObject *Sender)
{
	removeCurrentItem(RowNamesListBox);
}
//---------------------------------------------------------------------------
void __fastcall TAddColRowNamesForm::CancelButtonClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------


void __fastcall TAddColRowNamesForm::NextButtonClick(TObject *Sender)
{
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

	for (int i = 0; i < colCount; i++) {
		colNames->push_back(ColNamesListBox->Items->Strings[i]);
	}

	for (int i = 0; i < rowCount; i++) {
		rowNames->push_back(i);
	}

	Close();
}

//---------------------------------------------------------------------------

void __fastcall TAddColRowNamesForm::ColAddButtonClick(TObject *Sender)
{
	addItem(ColNamesListBox, true);
}
//---------------------------------------------------------------------------

void __fastcall TAddColRowNamesForm::ColRemoveButtonClick(TObject *Sender)
{
	removeCurrentItem(ColNamesListBox);
}
//---------------------------------------------------------------------------

void __fastcall TAddColRowNamesForm::ColMoveUpButtonClick(TObject *Sender)
{
	moveItemUp(ColNamesListBox);
}
//---------------------------------------------------------------------------

void __fastcall TAddColRowNamesForm::ColMoveDownButtonClick(TObject *Sender)
{
	moveItemDown(ColNamesListBox);
}
//---------------------------------------------------------------------------
void __fastcall TAddColRowNamesForm::ColNamesListBoxKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift)
{
	onListKeyDown(ColNamesListBox, Key, Shift);
}
//---------------------------------------------------------------------------
