#ifndef WB_EditorWindow_H
#define WB_EditorWindow_H

class WB_EditorWindow {
public:
	bool active; /***< If true the editor window will be displayed*/

	/**
	* Constructor
	*/
	WB_EditorWindow();

	/**
	* Handle gets called every frame when active
	*/
	virtual void Handle(class WB_Editor* editor) = 0;
};

#endif // !WB_EditorWindow