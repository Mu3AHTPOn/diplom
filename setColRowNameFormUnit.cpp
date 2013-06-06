//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "setColRowNameFormUnit.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TsetCollRowNameForm *setCollRowNameForm;
//---------------------------------------------------------------------------
__fastcall TsetCollRowNameForm::TsetCollRowNameForm(TComponent* Owner)
	: TForm(Owner)
{

}

void TsetCollRowNameForm::setResultStr(UnicodeString *res)
{
	result = res;
}
//---------------------------------------------------------------------------
void __fastcall TsetCollRowNameForm::Button1Click(TObject *Sender)
{
	(*result) = Edit1->Text;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TsetCollRowNameForm::Button2Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TsetCollRowNameForm::FormShow(TObject *Sender)
{
	Edit1->Text = (*result);
	Edit1->SelectAll();
}
//---------------------------------------------------------------------------

