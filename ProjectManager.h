//---------------------------------------------------------------------------

#ifndef ProjectManagerH
#define ProjectManagerH
//---------------------------------------------------------------------------

#include <vcl.h>
#include <DBXJSON.hpp>
#include <map>
#include "Project.cpp"

using std::map;

class ProjectManager {

public:
	inline bool isSavedCurrentPreject();
	inline void setIsCurrentProjectSaved(bool isSaved);
	inline bool isProjectOpen();
	inline void setIsProjectOpen(bool isSaved);
	static ProjectManager& getInstance();
	inline Project & getCurrentProject();
	Project & newProject();
	Project & loadProject(UnicodeString fileName);
	void saveProject(UnicodeString fileName);
	void closeProject();
protected:
	ProjectManager();
	~ProjectManager();
private:
	map<UnicodeString, UnicodeString> recentProjects;
	bool isSavedCurrentProject, isOpenProject;
	Project *currentProject;
	static ProjectManager* instance;
};
#endif
