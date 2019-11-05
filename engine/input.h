// Header file for Input class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, November 2019
#ifndef INPUT_H
#define INPUT_H

//Define Button macros

#define		BUTTONCODE_LEFT		0
#define		BUTTONCODE_RIGHT	1
#define		BUTTONCODE_MIDDLE	2

//Define Key macros
#define		KEYCODE_EMPTY_KEY 0
#define 	KEYCODE_UNKNOWN   -1
#define 	KEYCODE_SPACE   32
#define 	KEYCODE_APOSTROPHE   39 /* ' */
#define 	KEYCODE_COMMA   44 /* , */
#define 	KEYCODE_MINUS   45 /* - */
#define 	KEYCODE_PERIOD   46 /* . */
#define 	KEYCODE_SLASH   47 /* / */
#define 	KEYCODE_0   48
#define 	KEYCODE_1   49
#define 	KEYCODE_2   50
#define 	KEYCODE_3   51
#define 	KEYCODE_4   52
#define 	KEYCODE_5   53
#define 	KEYCODE_6   54
#define 	KEYCODE_7   55
#define 	KEYCODE_8   56
#define 	KEYCODE_9   57
#define 	KEYCODE_SEMICOLON   59 /* ; */
#define 	KEYCODE_EQUAL   61 /* = */
#define 	KEYCODE_A   65
#define 	KEYCODE_B   66
#define 	KEYCODE_C   67
#define 	KEYCODE_D   68
#define 	KEYCODE_E   69
#define 	KEYCODE_F   70
#define 	KEYCODE_G   71
#define 	KEYCODE_H   72
#define 	KEYCODE_I   73
#define 	KEYCODE_J   74
#define 	KEYCODE_K   75
#define 	KEYCODE_L   76
#define 	KEYCODE_M   77
#define 	KEYCODE_N   78
#define 	KEYCODE_O   79
#define 	KEYCODE_P   80
#define 	KEYCODE_Q   81
#define 	KEYCODE_R   82
#define 	KEYCODE_S   83
#define 	KEYCODE_T   84
#define 	KEYCODE_U   85
#define 	KEYCODE_V   86
#define 	KEYCODE_W   87
#define 	KEYCODE_X   88
#define 	KEYCODE_Y   89
#define 	KEYCODE_Z   90
#define 	KEYCODE_LEFT_BRACKET   91 /* [ */
#define 	KEYCODE_BACKSLASH   92 /* \ */
#define 	KEYCODE_RIGHT_BRACKET   93 /* ] */
#define 	KEYCODE_GRAVE_ACCENT   96 /* ` */
#define 	KEYCODE_WORLD_1   161 /* non-US #1 */
#define 	KEYCODE_WORLD_2   162 /* non-US #2 */
#define 	KEYCODE_ESCAPE   256
#define 	KEYCODE_ENTER   257
#define 	KEYCODE_TAB   258
#define 	KEYCODE_BACKSPACE   259
#define 	KEYCODE_INSERT   260
#define 	KEYCODE_DELETE   261
#define 	KEYCODE_RIGHT   262
#define 	KEYCODE_LEFT   263
#define 	KEYCODE_DOWN   264
#define 	KEYCODE_UP   265
#define 	KEYCODE_PAGE_UP   266
#define 	KEYCODE_PAGE_DOWN   267
#define 	KEYCODE_HOME   268
#define 	KEYCODE_END   269
#define 	KEYCODE_CAPS_LOCK   280
#define 	KEYCODE_SCROLL_LOCK   281
#define 	KEYCODE_NUM_LOCK   282
#define 	KEYCODE_PRINT_SCREEN   283
#define 	KEYCODE_PAUSE   284
#define 	KEYCODE_F1   290
#define 	KEYCODE_F2   291
#define 	KEYCODE_F3   292
#define 	KEYCODE_F4   293
#define 	KEYCODE_F5   294
#define 	KEYCODE_F6   295
#define 	KEYCODE_F7   296
#define 	KEYCODE_F8   297
#define 	KEYCODE_F9   298
#define 	KEYCODE_F10   299
#define 	KEYCODE_F11   300
#define 	KEYCODE_F12   301
#define 	KEYCODE_F13   302
#define 	KEYCODE_F14   303
#define 	KEYCODE_F15   304
#define 	KEYCODE_F16   305
#define 	KEYCODE_F17   306
#define 	KEYCODE_F18   307
#define 	KEYCODE_F19   308
#define 	KEYCODE_F20   309
#define 	KEYCODE_F21   310
#define 	KEYCODE_F22   311
#define 	KEYCODE_F23   312
#define 	KEYCODE_F24   313
#define 	KEYCODE_F25   314
#define 	KEYCODE_KP_0   320
#define 	KEYCODE_KP_1   321
#define 	KEYCODE_KP_2   322
#define 	KEYCODE_KP_3   323
#define 	KEYCODE_KP_4   324
#define 	KEYCODE_KP_5   325
#define 	KEYCODE_KP_6   326
#define 	KEYCODE_KP_7   327
#define 	KEYCODE_KP_8   328
#define 	KEYCODE_KP_9   329
#define 	KEYCODE_KP_DECIMAL   330
#define 	KEYCODE_KP_DIVIDE   331
#define 	KEYCODE_KP_MULTIPLY   332
#define 	KEYCODE_KP_SUBTRACT   333
#define 	KEYCODE_KP_ADD   334
#define 	KEYCODE_KP_ENTER   335
#define 	KEYCODE_KP_EQUAL   336
#define 	KEYCODE_LEFT_SHIFT   340
#define 	KEYCODE_LEFT_CONTROL   341
#define 	KEYCODE_LEFT_ALT   342
#define 	KEYCODE_LEFT_SUPER   343
#define 	KEYCODE_RIGHT_SHIFT   344
#define 	KEYCODE_RIGHT_CONTROL   345
#define 	KEYCODE_RIGHT_ALT   346
#define 	KEYCODE_RIGHT_SUPER   347
#define 	KEYCODE_MENU   348
#define 	KEYCODE_LAST   KEYCODE_MENU

#include <map>
#include <iostream>

#include "math/vec2.h"
#include "unique_types.h"

enum KeyEvent_Type {
	Get,
	GetDown,
	GetUp
};

struct KeyEvent {
	int keyCode;
	KeyEvent_Type type;

	/**
	* Constructor, takes the keycode and the type
	*/
	KeyEvent(int keyCode, KeyEvent_Type type);

	/**
	* Checks if key is pressed
	* @return bool, true if pressed
	*/
	bool Check();
};

typedef std::vector<KeyEvent> KeyCombo;

struct KeyComboEvent {
private:
	KeyCombo combo; /***< The combo list*/
public:
	Delegate onActivate; /***< Executed when the combo is pressed*/

	/**
	* Constructor
	* @param KeyCombo
	*/
	KeyComboEvent(KeyCombo combo = {});

	/**
	* Checks if keycombo is pressed, also calls the onActivate delegate
	* @return bool, true if all keys in combo are pressed
	*/
	bool Check();
};

struct Axis {
private:
	int positiveKey; /***< The positive key of the axis (1)*/
	int negativeKey; /***< The positive key of the axis (-1)*/
public:
	/**
	* Constructor
	* @param int	Positive key
	* @param int	Negative key
	*/
	Axis(int positiveKey, int negativeKey);

	/**
	* Returns the current value of the axis
	* @return float
	*/
	float GetValue();
};

class Input {
private:
	//global members
	static Input* _instance; /***< The Input singleton Instance */
	
	//local members
	std::map<int, bool> _keys; /***< Map containing  all keys pressed this frame */
	std::map<int, bool> _keysLast; /***< @brief Map containing all keys pressed last frame */

	std::map<int, bool> _buttons; /***< @brief Map containing all buttons pressed this frame */
	std::map<int, bool> _buttonsLast; /***< @brief Map containing all buttons pressed last frame */

	std::map<std::string, Axis*> _axises; /***< Map that contains axises*/

	int lastKey; /***< @brief The last key pressed */

	Vec2 _mousePos; /***< @brief Point2 of floats containing Mouse Position */
	Vec2 _scrollOffset; /***< The mouse scroll offset*/

	/**
	* Returns the instance if found, else creates a new instance and returns.
	*/
	static Input* GetInstance();
public:

	/**
	* Initialize the Input class
	*/
	static void Init(struct GLFWwindow* window);

	/**
	* Gets called every frame by Core to handle keys and lastkey
	*/
	static void HandleUpdates();

	/**
	* Returns true if Key is down this frame, but not down last frame.
	*/
	static bool GetKeyDown(int keyCode);

	/**
	* Returns true if Key is down this frame and last frame.
	*/
	static bool GetKey(int keyCode);

	/**
	* Returns true if Key is down last frame, but not down this frame
	*/
	static bool GetKeyUp(int keyCode);

	/**
	* Sets a key
	*/
	static void SetKey(int keyCode, bool state);

	/**
	* Returns true if Key is down this frame, but not down last frame.
	*/
	static bool GetButtonDown(int buttonCode);

	/**
	* Returns true if Key is down this frame and last frame.
	*/
	static bool GetButton(int buttonCode);

	/**
	* Returns true if Key is down last frame, but not down this frame
	*/
	static bool GetButtonUp(int buttonCode);

	/**
	* Sets a key
	*/
	static void SetButton(int buttonCode, bool state);

	/**
	* Sets the mouse position
	*/
	static void SetMousePos(Vec2 point);

	/**
	* Returns the mouse position as a Point2f
	*/
	static Vec2 GetMousePosition();

	/**
	* Sets the scroll offset
	*/
	static void SetScrollOffset(const Vec2 offset);

	/**
	* Returns the scroll offset
	*/
	static const Vec2 GetScrollOffset();

	/**
	* Returns the last key pressed this frame, if there is no key pressed this frame it returns KEYCODE_EMPTY_KEY
	*/
	static int GetLastKey();

	/**
	* Checks a combo then returns result
	*/
	static bool CheckCombo(KeyComboEvent comboEvent);

	/**
	* Adds a axis to the axises vector
	*/
	static void AddAxis(std::string name, Axis* axis);

	/**
	* Returns the value of the axis
	*/
	static const float GetAxis(std::string name);

	/**
	* Terminates the input class
	*/
	static void Terminate();
};

#endif // !INPUT_H
