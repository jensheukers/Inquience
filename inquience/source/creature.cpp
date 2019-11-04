#include "creature.h"

Creature::Creature() {
	healthComponent = this->AddComponent<HealthComponent>();

	//On death delete instance
	healthComponent->onHealthZero.AddLambda([=]() {
		delete GetParent()->RemoveChild(this);
	});
}

void Creature::TakeDamage(int amount) {
	healthComponent->TakeDamage(amount);
}

void Creature::Heal(int amount) {
	healthComponent->TakeHealth(amount);
}

int Creature::GetHealth() {
	return healthComponent->GetHealth();
}