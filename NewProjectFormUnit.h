//---------------------------------------------------------------------------

#ifndef NewProjectFormUnitH
#define NewProjectFormUnitH
//---------------------------------------------------------------------------
#include <vcl.h>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include "MainForm.cpp"

//---------------------------------------------------------------------------
class TCreateProjectForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel5;
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel3;
	TPanel *Panel4;
	void __fastcall Panel1Click(TObject *Sender);
	void __fastcall Panel1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall Panel1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall Panel1MouseEnter(TObject *Sender);
	void __fastcall Panel1MouseLeave(TObject *Sender);
	void __fastcall Panel1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);

private:	// User declarations
public:		// User declarations
	__fastcall TCreateProjectForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCreateProjectForm *CreateProjectForm;
//---------------------------------------------------------------------------
#endif
