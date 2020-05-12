#include "creature.h"

Creature::Creature() {
	healthComponent = this->AddComponent<HealthComponent>();

	//On death delete instance
	healthComponent->onHealthZero.AddLambda([=]() {
		delete GetParent()->RemoveChild(this);
	});

	//Add component and set default texture to placeholder
	sprite = this->AddComponent<Sprite>();
	sprite->SetTexture(TextureLoader::LoadTarga("res/textures/placeholder.tga"));

	//Set scale to 32x32
	localScale = Vec2(32, 32);
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