#ifndef MANAGED_H
#define MANAGED_H

/**
* Managed is the base class for all entities and component
*/

class Managed {
public:
	/**
	* @brief tries to cast to type given.
	*/
	template<class T>
	T* Cast() {
		return static_cast<T*>(this);
	}
};

#endif // MANAGED_H