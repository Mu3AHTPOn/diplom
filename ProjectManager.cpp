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
		TJSONArray *colNamesJSON = new TJSONArray();
		TJSONArray *rowNamesJSON = new TJSONArray();
		const vector<UnicodeString> &criteriaNames = getCurrentProject().getCriteriaNames();
		vector<UnicodeString>::const_iterator iter;
		for (iter = criteriaNames.begin(); iter != criteriaNames.end(); ++iter) {
			colNamesJSON->Add(*iter);
		}

		js->AddPair(L"colNames", colNamesJSON);


		const vector<UnicodeString> &alternativeNames = getCurrentProject().getAlternativeNames();
		for (iter = alternativeNames.begin(); iter != alternativeNames.end(); ++iter) {
			rowNamesJSON->Add(*iter);
		}

		js->AddPair(L"rowNames", rowNamesJSON);

		vector<AlternativeEstimates> alternativeEstimates = getCurrentProject().getAlternativeEstimates();
		TJSONArray *tableData = new TJSONArray();
		for (int i = 0; i < alternativeEstimates.size(); i++) {
			TJSONArray *rowData = new TJSONArray();
			const vector<double> &estimates = alternativeEstimates[i].getEstimates();
			for (int j = 0; j < estimates.size(); j++) {
				rowData->Add(FloatToStr(estimates[j]));
			}

			tableData->AddElement(rowData);
		}

		js->AddPair(L"tableData", tableData);
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
		TJSONArray *colNamesJSON = (TJSONArray*) js->Get(L"colNames")->JsonValue;
		TJSONArray *rowNamesJSON = (TJSONArray*) js->Get(L"rowNames")->JsonValue;

		closeProject();
		currentProject = new Project();
		vector<UnicodeString> &criteriaNames = getCurrentProject().getCriteriaNames();
		for (int i = 0; i < colNamesJSON->Size(); ++i) {
			criteriaNames.push_back(colNamesJSON->Get(i)->Value());
		}

		vector<AlternativeEstimates> &alternativeEstimates = getCurrentProject().getAlternativeEstimates();
		for (int i = 0; i < rowNamesJSON->Size(); ++i) {
			alternativeEstimates.push_back(AlternativeEstimates(rowNamesJSON->Get(i)->Value()));
		}

		TJSONArray *tableData = (TJSONArray*) js->Get(L"tableData")->JsonValue;
		for (int i = 0; i < tableData->Size(); i++) {
			TJSONArray *rowData = (TJSONArray*) tableData->Get(i);
			vector<double> &estimates = alternativeEstimates[i].getEstimates();
			for (int j = 0; j < rowData->Size(); j++) {
				estimates.push_back(StrToFloat(rowData->Get(j)->Value()));
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
	delete [] currentProject;
	setIsProjectOpen(false);
	setIsCurrentProjectSaved(false);
}

Project & ProjectManager::newProject()
{
	closeProject();
	currentProject = new Project();
	return *currentProject;
}
