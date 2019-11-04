#include "healthcomponent.h"

HealthComponent::HealthComponent() {
	this->health = IQ_HEALTHCOMPONENT_MAX_HEALTH;
}

void HealthComponent::TakeDamage(const int damage) {
	this->health -= damage;

	if (this->health < 0) {
		this->health = 0;
	}

	onTakeDamage.Execute();

	//Health is zero, execute healthZero lambda's
	if (this->health == 0) onHealthZero.Execute();
}

void HealthComponent::TakeHealth(const int health) {
	this->health += health;

	if (this->health > IQ_HEALTHCOMPONENT_MAX_HEALTH) {
		this->health = IQ_HEALTHCOMPONENT_MAX_HEALTH;
	}

	onTakeHealth.Execute();
}

const int HealthComponent::GetHealth() {
	return this->health;
}