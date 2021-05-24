//Movement component
#ifndef IQ_MOVEMENTCOMPONENT_H
#define IQ_MOVEMENTCOMPONENT_H

#include <component.h>
#include <math/vec2.h>

enum Direction {
	Up,
	Down,
	Left,
	Right
};

//Movement component requires a rigidbody on entity
class MovementComponent : public Component {
private:
	class Collider* collider; // collider instance

	Direction direction;

	/**
	* Moves in the direction and does a collision check
	*/
	void Move(Vec2 direction);
public:
	float movementSpeed = 200.f;
	float jumpForce = 400.f;

	MovementComponent();

	void BeginPlay();

	void Update() override;

	void StepLeft(float speed);
	void StepRight(float speed);
	void Jump(float force);

	Direction GetDirection();
};

#endif // !IQ_MOVEMENTCOMPONENT_H