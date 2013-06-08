//---------------------------------------------------------------------------

#pragma hdrstop

#include "ProjectManager.h"

#pragma once
//---------------------------------------------------------------------------

ProjectManager* ProjectManager::instance = NULL;

ProjectManager& ProjectManager::getInstance()
{
	if (instance == NULL) {
        instance = new ProjectManager();
	}

	return (*instance);
}

ProjectManager::ProjectManager(): isSavedCurrentProject(false), isOpenProject(false)
{
	currentProject = NULL;
}

ProjectManager::~ProjectManager()
{
	delete [] instance;
	delete [] currentProject;
}

void ProjectManager::setIsCurrentProjectSaved(bool isSaved)
{
	isSavedCurrentProject = isSaved;
}

bool ProjectManager::isSavedCurrentPreject()
{
	return isSavedCurrentProject;
}

void ProjectManager::setIsProjectOpen(bool isOpen)
{
	isOpenProject = isOpen;
}

bool ProjectManager::isProjectOpen()
{
	return isOpenProject;
}

Project & ProjectManager::getCurrentProject()
{
    return *currentProject;
}

void ProjectManager::saveProject(UnicodeString fileName)
{
    TFileStream *fs = new TFileStream(fileName, fmCreate);
	try {
		TJSONObject *js = new TJSONObject();
		TJSONArray *criteriaNamesJSON = new TJSONArray();
		const vector<UnicodeString> &criteriaNames = getCurrentProject().getCriteriaNames();
		vector<UnicodeString>::const_iterator iter;
		for (iter = criteriaNames.begin(); iter != criteriaNames.end(); ++iter) {
			criteriaNamesJSON->Add(*iter);
		}

		js->AddPair(L"criteriaNames", criteriaNamesJSON);

		vector<Estimates> alternativeEstimates = getCurrentProject().getAlternativeEstimates();
		TJSONArray *tableData = new TJSONArray();
		for (int i = 0; i < alternativeEstimates.size(); i++) {
			TJSONObject *alternativeEstimatesJSON = new TJSONObject();
			alternativeEstimatesJSON->AddPair("name", alternativeEstimates[i].getName());
			TJSONArray *rowData = new TJSONArray();

			const vector<double> &estimates = alternativeEstimates[i].getEstimates();
			for (int j = 0; j < estimates.size(); j++) {
				rowData->Add(FloatToStr(estimates[j]));
			}

			alternativeEstimatesJSON->AddPair("data", rowData);
			tableData->AddElement(alternativeEstimatesJSON);
		}

		js->AddPair(L"alternativeEstimates", tableData);
		js->AddPair(L"projectName", getCurrentProject().getName());
		UnicodeString projectJSON = js->ToString();

		fs->Write(projectJSON.BytesOf(), projectJSON.Length());
		js->Free();

		setIsCurrentProjectSaved(true);

	} __finally {
		fs->Free();
	}
}

Project & ProjectManager::loadProject(UnicodeString fileName)
{
    TFileStream *fs = new TFileStream(fileName, fmOpenRead);
	try {
		int n = fs->Size;
		char *chars = new char[n+1];

		fs->Read(chars, n);
		chars[n] = '\0';
		UnicodeString str(chars);

		TJSONObject *js = (TJSONObject*) TJSONObject::ParseJSONValue(str);
		TJSONArray *colNamesJSON = (TJSONArray*) js->Get(L"criteriaNames")->JsonValue;
		TJSONArray *alternativeEstimatesJSON = (TJSONArray*) js->Get(L"alternativeEstimates")->JsonValue;

		closeProject();
		currentProject = new Project();
		vector<UnicodeString> &criteriaNames = getCurrentProject().getCriteriaNames();
		for (int i = 0; i < colNamesJSON->Size(); ++i) {
			criteriaNames.push_back(colNamesJSON->Get(i)->Value());
		}

		vector<Estimates> &alternativeEstimates = getCurrentProject().getAlternativeEstimates();
		for (int i = 0; i < alternativeEstimatesJSON->Size(); ++i) {
			TJSONObject *alternativeJSON = (TJSONObject*) alternativeEstimatesJSON->Get(i);
			UnicodeString name = alternativeJSON->Get(L"name")->JsonValue->Value();
			alternativeEstimates.push_back(Estimates(name));

			TJSONArray *estimatesJSON = (TJSONArray*) alternativeJSON->Get(L"data")->JsonValue;
			vector<double> &estimates = alternativeEstimates[i].getEstimates();
			for (int j = 0; j < estimatesJSON->Size(); ++j) {
				UnicodeString str = estimatesJSON->Get(j)->Value();
				estimates.push_back(str.Length() > 0 ? StrToFloat(str) : 0);
            }
		}

		getCurrentProject().setName(js->Get(L"projectName")->JsonValue->Value());

		delete [] chars;
		js->Free();

		setIsProjectOpen(true);
		setIsCurrentProjectSaved(true);
		return getCurrentProject();
	} __finally {
		fs->Free();
	}
}

void ProjectManager::closeProject() {
	delete currentProject;
	currentProject = NULL;
	setIsProjectOpen(false);
	setIsCurrentProjectSaved(false);
}

Project & ProjectManager::newProject()
{
	closeProject();
	currentProject = new Project();
	return *currentProject;
}
