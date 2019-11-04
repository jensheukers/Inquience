#ifndef IQ_HEALTHCOMPONENT_H
#define IQ_HEALTHCOMPONENT_H

#include <component.h>
#include <unique_types.h>

#define IQ_HEALTHCOMPONENT_MAX_HEALTH 100

class HealthComponent : public Component {
private:
	int health; 
public:
	Delegate onHealthZero; //Triggered whenever damage is taken and health is zero

	Delegate onTakeDamage;
	Delegate onTakeHealth;

	HealthComponent();

	void TakeDamage(const int damage);
	void TakeHealth(const int health);

	const int GetHealth();

	virtual HealthComponent* New() { return new HealthComponent(); }
	virtual HealthComponent* Copy() const { return new HealthComponent(*this); }
};

#endif // !IQ_HEALTHCOMPONENT_H