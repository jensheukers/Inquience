#ifndef WB_EDITORWINDOWS_H
#define WB_EDITORWINDOWS_H

//Editor Window Definitions
#define WB_PAINTERWINDOW_IMGUI_TILESIZE 32
#define WB_EditorWindowToolBarSpace 0.025f //Offset from top bar

#include <components/sprite.h>

#include <unique_types.h>

class WB_Editor;

class WB_EditorWindow {
private:
	bool isFocused;
protected:
	/**
	* Handle gets called every frame when active
	*/
	virtual void Handle(WB_Editor* editor) = 0;
public:
	Vec2 sizeDiv; /**Size vector division amount, we defide by screen width and height*/
	Vec2 posDiv; /**Pos vector division amount, we defide by screen width and height*/
	bool allowMove; /** If true we can freely move around the window*/
	bool active; /***< If true the editor window will be displayed*/
	const char* title; /***< The title of the input window*/

	/**
	* Constructor
	*/
	WB_EditorWindow(const char* title = "InputWindow", bool allowMove = false, Vec2 sizeDiv = Vec2(1, 1), Vec2 posDiv = Vec2(0,0));

	/**
	* @brief Update calls Handle and handles some transformations
 	*/
	void Update(WB_Editor* editor);

	/**
	* @brief Returns isFocused value
	* @return bool
	*/
	bool IsFocused() { return isFocused; }
};

#define WB_EDITORINPUTWINDOW_SIZEDIVX 3
#define WB_EDITORINPUTWINDOW_SIZEDIVY 8
class WB_EditorInputWindow : public WB_EditorWindow {
protected:

	char buffer[256] = ""; /***< Buffer holds string*/
public:
	Delegate onApply; /***< Gets executed when apply is clicked*/

	WB_EditorInputWindow(const char* title) : WB_EditorWindow(title, true, Vec2(WB_EDITORINPUTWINDOW_SIZEDIVX, WB_EDITORINPUTWINDOW_SIZEDIVY)) {}

	virtual void Handle(WB_Editor* editor) override;

	/**
	* Returns the buffer
	*/
	const char* GetBuffer() { return this->buffer; }
};

class WB_EditorTwoInputWindow : public WB_EditorInputWindow {
protected:
	char buffer2[256] = ""; /***< Buffer holds string*/
public:
	virtual void Handle(WB_Editor* editor) override;

	/**
	* Returns the second buffer
	*/
	const char* GetSecondBuffer() { return this->buffer2; }
};

#define WB_PAINTERWINDOW_SIZEDIVX 5
#define WB_PAINTERWINDOW_SIZEDIVY 1
class WB_PainterWindow : public WB_EditorWindow {
private:
	Sprite* sprite;
	UV* curUvData;

	int pixelsPerTile;
public:
	WB_PainterWindow() : WB_EditorWindow("Painter", false, Vec2(WB_PAINTERWINDOW_SIZEDIVX, WB_PAINTERWINDOW_SIZEDIVY), Vec2(0.8f, 0)) { 
		this->sprite = nullptr; this->pixelsPerTile = 32; this->curUvData = nullptr; 
	};

	void Handle(WB_Editor* editor) override;

	UV* GetCurrentUvData() { return curUvData; };
};

#define WB_EDITORGRIDSETTINGSWINDOW_SIZEDIVX 3
#define WB_EDITORGRIDSETTINGSWINDOW_SIZEDIVY 6
class WB_EditorGridSettingsWindow : public WB_EditorWindow {
public:
	WB_EditorGridSettingsWindow() : WB_EditorWindow("Grid Settings", true, Vec2(WB_EDITORGRIDSETTINGSWINDOW_SIZEDIVX, WB_EDITORGRIDSETTINGSWINDOW_SIZEDIVY)) {}

	void Handle(WB_Editor* editor) override;
};


#define WB_EDITORHIERARCHY_SIZEDIVX 5
#define WB_EDITORHIERARCHY_SIZEDIVY 1
class WB_EditorHierarchy : public WB_EditorWindow {
private:
	/**
	* Construct a treenode of entities
	*/
	void ConstructTreenode(WB_Editor* editor, class Entity* entity);
public:
	WB_EditorHierarchy() : WB_EditorWindow("Hierarchy", true, Vec2(WB_EDITORHIERARCHY_SIZEDIVX, WB_EDITORHIERARCHY_SIZEDIVY)) {}

	void Handle(WB_Editor* editor) override;
};

class WB_EditorViewport : public WB_EditorWindow {
public:
	WB_EditorViewport() : WB_EditorWindow("Viewport", true) {}

	void Handle(WB_Editor* editor) override;
};

#define WB_EDITORINSPECTOR_SIZEDIVX 5
#define WB_EDITORINSPECTOR_SIZEDIVY 1
class WB_EditorInspector : public WB_EditorWindow {
private:
	bool addComponentActive;
public:
	WB_EditorInspector() :WB_EditorWindow("Inspector", true, Vec2(WB_EDITORINSPECTOR_SIZEDIVX, WB_EDITORINSPECTOR_SIZEDIVY)) {
		addComponentActive = false;
	}

	void Handle(WB_Editor* editor) override;
};

#define WB_EDITORRESOURCEBROWSER_SIZEDIVX 1
#define WB_EDITORRESOURCEBROWSER_SIZEDIVY 5
class WB_EditorResourceBrowser : public WB_EditorWindow {
public:
	WB_EditorResourceBrowser() : WB_EditorWindow("Resource Browser", false, Vec2(WB_EDITORRESOURCEBROWSER_SIZEDIVX, WB_EDITORRESOURCEBROWSER_SIZEDIVY)) {}

	void Handle(WB_Editor* editor) override;
};

#endif // !WB_EditorWindows