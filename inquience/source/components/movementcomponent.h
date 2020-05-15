//Movement component
#ifndef IQ_MOVEMENTCOMPONENT_H
#define IQ_MOVEMENTCOMPONENT_H

#include <component.h>
#include <math/vec2.h>

//Movement component requires a rigidbody on entity
class MovementComponent : public Component {
private:
	class RigidBody* rigidBody; // Rigidbody instance

	bool grounded;
	bool canStepRight;
	bool canStepLeft;

public:
	MovementComponent();

	void BeginPlay();

	void StepLeft(float speed);
	void StepRight(float speed);
	void Jump(float force);

	const bool IsGrounded();
};

#endif // !IQ_MOVEMENTCOMPONENT_H