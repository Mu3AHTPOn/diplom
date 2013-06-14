//---------------------------------------------------------------------------

#ifndef UIManagerH
#define UIManagerH
//---------------------------------------------------------------------------
#include <vcl.h>
#include <list>

using std::list;

class UIManager {
public:
	static UIManager &getInstance();
	~UIManager();
	void addForm(TForm *form);
	void removeForm(TForm *form);
	void closeApp(TForm *currentForm);
	bool &getAutoEval();
	bool &getIndicator();
	bool &getHint();
protected:
	UIManager();
	list<TForm*> *getForms();
private:
	list<TForm*> *forms;
	static UIManager *instance;
	bool autoEval, indicator, hint;
};
#endif
