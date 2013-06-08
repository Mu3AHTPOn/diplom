//---------------------------------------------------------------------------

#ifndef NewProjectUnitH
#define NewProjectUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>

#include <vector>
#include "ProjectManager.cpp"

using std::vector;
//---------------------------------------------------------------------------
class TNewProjectForm : public TForm
{
__published:	// IDE-managed Components
	TListBox *RowNamesListBox;
	TSpeedButton *RowAddButton;
	TSpeedButton *RowRemoveButton;
	TSpeedButton *RowMoveUpButton;
	TSpeedButton *RowMoveDownButton;
	TLabel *Label1;
	TListBox *ColNamesListBox;
	TSpeedButton *ColAddButton;
	TSpeedButton *ColRemoveButton;
	TSpeedButton *ColMoveUpButton;
	TSpeedButton *ColMoveDownButton;
	TLabel *Label2;
	TButton *CancelButton;
	TButton *NextButton;
	TLabel *Label3;
	TEdit *ProjectName;
	TComboBox *MethodComboBox;
	TLabel *Label4;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall RowsListBoxClick(TObject *Sender);
	void __fastcall listEditKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall ListEditExit(TObject *Sender);
	void __fastcall RowNamesListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RowMoveUpButtonClick(TObject *Sender);
	void __fastcall RowMoveDownButtonClick(TObject *Sender);
	void __fastcall RowRemoveButtonClick(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);
	void __fastcall NextButtonClick(TObject *Sender);
	void __fastcall RowAddButtonClick(TObject *Sender);
	void __fastcall ColAddButtonClick(TObject *Sender);
	void __fastcall ColRemoveButtonClick(TObject *Sender);
	void __fastcall ColMoveUpButtonClick(TObject *Sender);
	void __fastcall ColMoveDownButtonClick(TObject *Sender);
	void __fastcall ColNamesListBoxDblClick(TObject *Sender);
	void __fastcall ColNamesListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);

private:	// User declarations
	TEdit *listEdit;
	void showEditText(TListBox *list);
	void removeCurrentItem(TListBox *list);
	void addItem(TListBox* list, bool inEnd = false);
	void moveItemUp(TListBox* list);
	void moveItemDown(TListBox* list);
	void onListKeyDown(TListBox* list, WORD &Key, TShiftState Shift);
	void onListKeyPress(System::WideChar &Key);
	void onListExit();

	Project *currentProject;

public:		// User declarations
	__fastcall TNewProjectForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TNewProjectForm *NewProjectForm;
//---------------------------------------------------------------------------
#endif
