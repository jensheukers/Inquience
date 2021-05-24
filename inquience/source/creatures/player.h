//Player class extends creature
#include "../creature.h"

//Components
#include "../components/movementcomponent.h"

#include <components/collider.h>

class Player : public Creature {
private:
	MovementComponent* movementComponent;
	Collider* collider;
public:
	Player();

	void Update() override;
};