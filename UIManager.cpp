//---------------------------------------------------------------------------

#pragma hdrstop
#pragma once
#include "UIManager.h"
//---------------------------------------------------------------------------

UIManager* UIManager::instance = NULL;

UIManager& UIManager::getInstance() {
	if (instance == NULL) {
		instance = new UIManager();
	}

	return *instance;
}

UIManager::UIManager() {
	forms = new list<TForm*>();
	autoEval = indicator = hint = true;
}

UIManager::~UIManager() {
	delete [] forms;
	delete [] instance;
}

list<TForm*>* UIManager::getForms(){
	return forms;
}

void UIManager::addForm(TForm *form)
{
	forms->push_front(form);
}

void UIManager::removeForm(TForm *form)
{
    forms->remove(form);
}

void UIManager::closeApp(TForm *currentForm)
{
	list<TForm*>::iterator i;
	forms->remove(currentForm);
	for (i = forms->begin(); i != forms->end(); ++i)
	{
		(*i)->Close();
	}

	forms->clear();
}

bool &UIManager::getAutoEval(){
	return autoEval;
}

bool &UIManager::getIndicator() {
	return indicator;
}

bool &UIManager::getHint() {
    return hint;
}
