//---------------------------------------------------------------------------

#pragma hdrstop

#include "NewProjectFormUnit.h"
#include "UIManager.cpp"
//---------------------------------------------------------------------------

#pragma resource "*.dfm"
TCreateProjectForm *CreateProjectForm;
//---------------------------------------------------------------------------
__fastcall TCreateProjectForm::TCreateProjectForm(TComponent* Owner)
	: TForm(Owner)
{
	UIManager::getInstance()->addForm(CreateProjectForm);
}
//---------------------------------------------------------------------------
void __fastcall TCreateProjectForm::Panel1Click(TObject *Sender)
{

	TPanel &panel = *((TPanel*) Sender);
	panel.BevelOuter = TBevelCut::bvRaised;
 	Form1 = new TForm1(CreateProjectForm->Owner);
	Form1->Show();
	CreateProjectForm->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TCreateProjectForm::Panel1MouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	TPanel &panel = *((TPanel*) Sender);
	panel.Color = clBlack;
}
//---------------------------------------------------------------------------

void __fastcall TCreateProjectForm::Panel1MouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	TPanel &panel = *((TPanel*) Sender);
	panel.BevelOuter = TBevelCut::bvNone;
}
//---------------------------------------------------------------------------


void __fastcall TCreateProjectForm::Panel1MouseEnter(TObject *Sender)
{
	TPanel &panel = *((TPanel*) Sender);
	panel.BevelOuter = TBevelCut::bvLowered;
}
//---------------------------------------------------------------------------

void __fastcall TCreateProjectForm::Panel1MouseLeave(TObject *Sender)
{
	TPanel &panel = *((TPanel*) Sender);
	panel.Color = clBtnFace;
}
//---------------------------------------------------------------------------

void __fastcall TCreateProjectForm::Panel1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
	TPanel &panel = *((TPanel*) Sender);
	panel.Color = clAppWorkSpace;
}
//---------------------------------------------------------------------------



