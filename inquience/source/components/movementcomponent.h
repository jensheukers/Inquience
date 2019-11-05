//Movement component
#ifndef IQ_MOVEMENTCOMPONENT_H
#define IQ_MOVEMENTCOMPONENT_H

#include <component.h>
#include <math/vec2.h>

//Movement component requires a rigidbody on entity
class MovementComponent : public Component {
private:
	class RigidBody* rigidBody; // Rigidbody instance

	struct RaycastData* groundRaycast;
	struct RaycastData* leftRaycast;
	struct RaycastData* rightRaycast;

	bool grounded;
	bool canStepRight;
	bool canStepLeft;

public:
	MovementComponent();

	void BeginPlay();

	void ResetHorizontalVelocity();
	void StepLeft(float speed);
	void StepRight(float speed);
	void Jump(float force);

	//Raycast uses a size to determine how far it should check, size should be from center of object to outer bounds
	void AddRaycasts(const float size);
	void RemoveRaycasts();

	const bool IsGrounded();

	~MovementComponent();
};

#endif // !IQ_MOVEMENTCOMPONENT_H