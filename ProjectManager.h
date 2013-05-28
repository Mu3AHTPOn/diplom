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
	static ProjectManager& getInstance();
protected:
	ProjectManager();
private:
	map<UnicodeString, UnicodeString> recentProjects;
	bool isSavedCurrentProject;
	static ProjectManager* instance;
};
#endif
