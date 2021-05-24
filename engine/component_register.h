//All components should be registered in this file
#ifndef COMPONENT_REGISTER_H
#define COMPONENT_REGISTER_H
#include <map>
#include <string>

//Component includes
#include "components/animator.h"
#include "components/collider.h"
#include "components/sprite.h"
#include "components/ui.h"
#include "graphics/font.h"

class Component_Register {
private:
	static Component_Register* instance;
	std::map<std::string, Component*> components;
	/**
	* Constructor, adds all types to the map
	*/
	Component_Register() {
		components[typeid(Animator).name()] = new Animator();
		components[typeid(Collider).name()] = new Collider();
		components[typeid(Sprite).name()] = new Sprite();
		components[typeid(UIComponent).name()] = new UIComponent();
		components[typeid(Text).name()] = new Text(FontLoader::LoadFont("fonts/consola.ttf"));
	}

	/**
	* Returns the instance
	*/
	static Component_Register* GetInstance();

	/**
	* Destructor, cleans up all references
	*/
	~Component_Register() {
		for (std::map<std::string, Component*>::iterator it = components.begin(); it != components.end(); ++it)
			delete it->second;
	}

public:
	/**
	* Returns a newly created component instance of type where key matches
	*/
	static Component* GetNewComponentInstance(std::string key);

	/**
	* Returns all registered components keys
	*/
	static std::vector<std::string> GetAvailableKeys();

	/**
	* Registers a new component, creates 1 instance of type
	*/
	template<typename T>
	static void RegisterComponent() {
		GetInstance()->components[typeid(T).name()] = new T();
	};
};

#endif // !COMPONENT_REGISTER_H