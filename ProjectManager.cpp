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

ProjectManager::ProjectManager(): isSavedCurrentProject(false) ,isOpenProject(false)
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

void ProjectManager::setIsProjectOpen(bool isOpen)
{
	isOpenProject = isOpen;
}

bool ProjectManager::isProjectOpen()
{
	return isOpenProject;
}
