//Creature class extends entity
#ifndef IQ_CREATURE_H
#define IQ_CREATURE_H
#include <entity.h>

#include <source/components/healthcomponent.h>
#include <components/sprite.h>

class Creature : public Entity {
private:
	HealthComponent* healthComponent;
	Sprite* sprite;
public:
	Creature();

	//Takes x amount of damage
	void TakeDamage(int amount);

	//Heals the  creature with x amount
	void Heal(int amount);
	int GetHealth();
};

#endif // !IQ_CREATURE_H