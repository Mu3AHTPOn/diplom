//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NewProjectFormUnit.h"
#include "MainForm.cpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TNewProjectForm *NewProjectForm;
//---------------------------------------------------------------------------
__fastcall TNewProjectForm::TNewProjectForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TNewProjectForm::Panel1Click(TObject *Sender)
{
	TPanel &panel = *((TPanel*) Sender);
	panel.BevelOuter = TBevelCut::bvRaised;
	Form1 = new TForm1(0);
	Form1->Show();
	NewProjectForm->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TNewProjectForm::Panel1MouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	TPanel &panel = *((TPanel*) Sender);
	panel.Color = clBlack;
}
//---------------------------------------------------------------------------

void __fastcall TNewProjectForm::Panel1MouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	TPanel &panel = *((TPanel*) Sender);
	panel.BevelOuter = TBevelCut::bvNone;
}
//---------------------------------------------------------------------------


void __fastcall TNewProjectForm::Panel1MouseEnter(TObject *Sender)
{
	TPanel &panel = *((TPanel*) Sender);
	panel.BevelOuter = TBevelCut::bvLowered;
}
//---------------------------------------------------------------------------

void __fastcall TNewProjectForm::Panel1MouseLeave(TObject *Sender)
{
	TPanel &panel = *((TPanel*) Sender);
	panel.Color = clBtnFace;
}
//---------------------------------------------------------------------------

void __fastcall TNewProjectForm::Panel1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
	TPanel &panel = *((TPanel*) Sender);
	panel.Color = clAppWorkSpace;
}
//---------------------------------------------------------------------------


