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

		Estimates &criteriaEstimates = currentProject->getCriteriaEstimates();
		vector< vector<int> > &rates = criteriaEstimates.getRates();
		vector<double> &estimates = criteriaEstimates.getEstimates();

		TJSONObject *crteriaEstimatesJSON = new TJSONObject();
		TJSONArray *crteriaEstimatesArray = new TJSONArray();
		for (int i = 0; i < estimates.size(); ++i) {
			crteriaEstimatesArray->Add(FloatToStr(estimates[i]));
		}

		crteriaEstimatesJSON->AddPair(L"estimates", crteriaEstimatesArray);

		TJSONArray *crteriaRatesTable = new TJSONArray();
		for (int i = 0; i < rates.size(); ++i) {
			vector<int> &v = rates[i];
			TJSONArray *crteriaRatesRow = new TJSONArray();
			for (int j = 0; j < v.size(); ++j) {
				crteriaRatesRow->Add(IntToStr(v[j]));
			}

			crteriaRatesTable->Add(crteriaRatesRow);
		}

		crteriaEstimatesJSON->AddPair(L"rates", crteriaRatesTable);
		js->AddPair(L"criteriaEstimates", crteriaEstimatesJSON);

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

			alternativeEstimatesJSON->AddPair("estimates", rowData);

			vector< vector<int> > &rates = alternativeEstimates[i].getRates();
			TJSONArray *alternativeRatesTable = new TJSONArray();
			for (int j = 0; j < rates.size(); ++j) {
				vector<int> &v = rates[j];
				TJSONArray *alternativeRatesRow = new TJSONArray();
				for (int k = 0; k < v.size(); ++k) {
					alternativeRatesRow->Add(IntToStr(v[k]));
				}

			alternativeRatesTable->Add(alternativeRatesRow);
			}

			alternativeEstimatesJSON->AddPair(L"rates", alternativeRatesTable);
			tableData->AddElement(alternativeEstimatesJSON);
		}

		js->AddPair(L"alternativeEstimates", tableData);
		js->AddPair(L"projectName", getCurrentProject().getName());
		js->AddPair(L"method", IntToStr(getCurrentProject().getMethod()));
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
		TJSONObject *crteriaEstimatesJSON = (TJSONObject*) js->Get(L"criteriaEstimates")->JsonValue;
		TJSONArray *crteriaEstimatesArray = (TJSONArray*) crteriaEstimatesJSON->Get(L"estimates")->JsonValue;
		TJSONArray *crteriaRatesTable = (TJSONArray*) crteriaEstimatesJSON->Get(L"rates")->JsonValue;
		TJSONArray *alternativeEstimatesJSON = (TJSONArray*) js->Get(L"alternativeEstimates")->JsonValue;

		closeProject();
		currentProject = new Project();
		vector<UnicodeString> &criteriaNames = getCurrentProject().getCriteriaNames();
		for (int i = 0; i < colNamesJSON->Size(); ++i) {
			criteriaNames.push_back(colNamesJSON->Get(i)->Value());
		}

		Estimates &criteriaEstimates = currentProject->getCriteriaEstimates();
		vector<double> &estimates = criteriaEstimates.getEstimates();

		for (int i = 0; i < crteriaEstimatesArray->Size(); ++i) {
			estimates.push_back(StrToFloat(crteriaEstimatesArray->Get(i)->Value()));
		}

		vector< vector<int> > &rates = criteriaEstimates.getRates();
		for (int i = 0; i < crteriaRatesTable->Size(); ++i) {
			vector<int> v;
			TJSONArray *crteriaRatesRow = (TJSONArray*) crteriaRatesTable->Get(i);

			for (int j = 0; j < crteriaRatesRow->Size(); ++j) {
				v.push_back(StrToInt(crteriaRatesRow->Get(j)->Value()));
			}

			rates.push_back(v);
		}


		vector<Estimates> &alternativeEstimates = getCurrentProject().getAlternativeEstimates();
		for (int i = 0; i < alternativeEstimatesJSON->Size(); ++i) {
			TJSONObject *alternativeJSON = (TJSONObject*) alternativeEstimatesJSON->Get(i);
			UnicodeString name = alternativeJSON->Get(L"name")->JsonValue->Value();
			alternativeEstimates.push_back(Estimates(name));

			TJSONArray *estimatesJSON = (TJSONArray*) alternativeJSON->Get(L"estimates")->JsonValue;
			vector<double> &estimates = alternativeEstimates[i].getEstimates();
			for (int j = 0; j < estimatesJSON->Size(); ++j) {
				UnicodeString str = estimatesJSON->Get(j)->Value();
				estimates.push_back(str.Length() > 0 ? StrToFloat(str) : 0);
			}

			TJSONArray *alternativeRatesTable = (TJSONArray*) alternativeJSON->Get(L"rates")->JsonValue;
			vector< vector<int> > &rates = alternativeEstimates[i].getRates();
			for (int j = 0; j < alternativeRatesTable->Size(); ++j) {
				vector<int> v;
				TJSONArray *alternativeRatesRow = (TJSONArray*) alternativeRatesTable->Get(j);

				for (int k = 0; k < alternativeRatesRow->Size(); ++k) {
					v.push_back(StrToInt(alternativeRatesRow->Get(k)->Value()));
				}

				rates.push_back(v);
			}
		}

		getCurrentProject().setName(js->Get(L"projectName")->JsonValue->Value());
		getCurrentProject().setMethod(StrToInt(js->Get(L"method")->JsonValue->Value()));

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
