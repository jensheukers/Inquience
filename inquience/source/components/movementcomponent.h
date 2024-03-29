//Movement component
#ifndef IQ_MOVEMENTCOMPONENT_H
#define IQ_MOVEMENTCOMPONENT_H

#include <component.h>
#include <math/vec2.h>

//Movement component requires a rigidbody on entity
class MovementComponent : public Component {
private:
	class RigidBody* rigidBody; // Rigidbody instance
public:
	float movementSpeed = 200.f;
	float jumpForce = 400.f;

	void BeginPlay();

	void Update() override;

	void StepLeft(float speed);
	void StepRight(float speed);
	void Jump(float force);
};

#endif // !IQ_MOVEMENTCOMPONENT_H