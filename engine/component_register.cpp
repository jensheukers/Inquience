#include "component_register.h"
#include "debug.h"

Component_Register* Component_Register::instance;

Component_Register* Component_Register::GetInstance() {
	if (!instance) {
		instance = new Component_Register();
	}
	return instance;
}

Component* Component_Register::GetNewComponentInstance(std::string key) {
	if (!GetInstance()->components[key]) {
		Debug::Log(std::string("Component \'") + key + std::string("\' not found, prehaps register it?"));
		return nullptr;
	}

	return GetInstance()->components[key]->New();
}

std::vector<std::string> Component_Register::GetAvailableKeys()
{
	std::vector<std::string> r;
	for (std::map<std::string, Component*>::iterator it = GetInstance()->components.begin(); it != GetInstance()->components.end(); ++it) {
		r.push_back(it->first);
	}
	return r;
}
