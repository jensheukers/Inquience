//Player class extends creature
#include "../creature.h"

//Components
#include "../components/movementcomponent.h"
#include <components/rigidbody.h>
#include <components/collider.h>

class Player : public Creature {
private:
	BoxCollider* collider;
	RigidBody* rigidBody;
	MovementComponent* movementComponent;
public:
	Player();

	void Update() override;
};