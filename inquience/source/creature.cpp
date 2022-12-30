#include "creature.h"

Creature::Creature() {
	healthComponent = this->AddComponent<HealthComponent>();

	//On death delete instance
	healthComponent->onHealthZero.AddLambda([=]() {
		delete GetParent()->RemoveChild(this);
	});

	//Add component and set default texture to placeholder
	sprite = this->AddComponent<Sprite>();
	sprite->SetTexture(TextureLoader::LoadTarga("res/textures/character_spritesheet.tga"));

	animator = this->AddComponent<Animator>();
	animator->AddAnimation("res/animations/character/run_left.json");
	animator->AddAnimation("res/animations/character/run_right.json");

	//Set scale to 32x32
	scale = Vec2(64, 64);
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

void Creature::PlayAnimation(std::string name) {
	animator->SetActiveAnimation(name);
}