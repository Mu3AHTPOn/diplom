//---------------------------------------------------------------------------

#ifndef ProjectManagerH
#define ProjectManagerH
//---------------------------------------------------------------------------

#include <vcl.h>
#include <map>

using std::map;

class ProjectManager {

public:
	inline bool isSavedCurrentPreject();
	inline void setIsCurrentProjectSaved(bool isSaved);
    inline bool isProjectOpen();
	inline void setIsProjectOpen(bool isSaved);
	static ProjectManager& getInstance();
protected:
	ProjectManager();
private:
	map<UnicodeString, UnicodeString> recentProjects;
	bool isSavedCurrentProject, isOpenProject;
	static ProjectManager* instance;
};
#endif
