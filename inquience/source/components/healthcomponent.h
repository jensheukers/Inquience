#ifndef IQ_HEALTHCOMPONENT_H
#define IQ_HEALTHCOMPONENT_H

#include <component.h>

class HealthComponent : public Component {
private:
	unsigned health; 
public:
	HealthComponent();

	virtual HealthComponent* New() { return new HealthComponent(); }
	virtual HealthComponent* Copy() const { return new HealthComponent(*this); }
};

#endif // !IQ_HEALTHCOMPONENT_H