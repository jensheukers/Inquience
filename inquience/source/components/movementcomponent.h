//Movement component
#ifndef IQ_MOVEMENTCOMPONENT_H
#define IQ_MOVEMENTCOMPONENT_H

#include <component.h>
#include <math/vec2.h>

//Movement component requires a rigidbody on entity
class MovementComponent : public Component {
private:
	class RigidBody* rigidBody; // Rigidbody instance

	float stepSpeed; // Amount of speed for horizontal movement
	float jumpForce; // Amount of jumpforce
	bool grounded; // If true the entity is grounded
public:
	MovementComponent();

	void BeginPlay();

	void Step(int side); // Move horizontally

	void Jump();

	void SetJumpForce(float force);
	const float GetJumpForce();

	void SetStepSpeed(float speed);
	const float GetStepSpeed();

	const bool IsGrounded();
};

#endif // !IQ_MOVEMENTCOMPONENT_H