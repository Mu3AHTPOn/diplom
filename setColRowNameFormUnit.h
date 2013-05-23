//---------------------------------------------------------------------------

#ifndef setColRowNameFormUnitH
#define setColRowNameFormUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TsetCollRowNameForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *Edit1;
	TButton *Button1;
	TButton *Button2;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
	UnicodeString* result;
public:		// User declarations
	__fastcall TsetCollRowNameForm(TComponent* Owner);
	void setResultStr(UnicodeString *res);
};
//---------------------------------------------------------------------------
extern PACKAGE TsetCollRowNameForm *setCollRowNameForm;
//---------------------------------------------------------------------------
#endif
