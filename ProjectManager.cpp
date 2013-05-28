//---------------------------------------------------------------------------

#pragma hdrstop

#include "ProjectManager.h"
//---------------------------------------------------------------------------

ProjectManager* ProjectManager::instance = NULL;

ProjectManager& ProjectManager::getInstance()
{
	if (instance == NULL) {
        instance = new ProjectManager();
	}

	return (*instance);
}

ProjectManager::ProjectManager(): isSavedCurrentProject(false)
{

}

void ProjectManager::setIsCurrentProjectSaved(bool isSaved)
{
	isSavedCurrentProject = isSaved;
}

bool ProjectManager::isSavedCurrentPreject()
{
	return isSavedCurrentProject;
}
