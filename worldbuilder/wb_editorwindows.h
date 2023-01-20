#ifndef WB_EDITORWINDOWS_H
#define WB_EDITORWINDOWS_H

//Editor Window Definitions
#define WB_PAINTERWINDOW_IMGUI_TILESIZE 64

#include <components/sprite.h>

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

class WB_PainterWindow : WB_EditorWindow {
private:
	Sprite* sprite;
	UV* curUvData;

	int pixelsPerTile;
public:
	WB_PainterWindow() { this->sprite = nullptr; this->pixelsPerTile = 32; this->curUvData = nullptr; };
	void Handle(WB_Editor* editor) override;

	UV* GetCurrentUvData() { return curUvData; }
};

class WB_EditorGridSettingsWindow : public WB_EditorWindow {
public:
	void Handle(WB_Editor* editor) override;
};


#endif // !WB_EditorWindows