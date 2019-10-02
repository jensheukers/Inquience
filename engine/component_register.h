//All components should be registered in this file
#ifndef COMPONENT_REGISTER_H
#define COMPONENT_REGISTER_H
#include <map>
#include <string>

//Component includes
#include "components/animator.h"
#include "components/collider.h"
#include "components/rigidbody.h"
#include "components/sprite.h"

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
		components[typeid(BoxCollider).name()] = new BoxCollider();
		components[typeid(RigidBody).name()] = new RigidBody();
		components[typeid(Sprite).name()] = new Sprite();
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
};

#endif // !COMPONENT_REGISTER_H