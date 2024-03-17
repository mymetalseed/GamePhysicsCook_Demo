#ifndef _H_PARTICLE_
#define _H_PARTICLE_

#include "Rigidbody.h"
#define EULER_INTEGRATION

class Particle : public Rigidbody {
	vec3 position, oldPosition;
	vec3 forces;
	float mass, bounce;
	vec3 gravity;
	float friction;
#ifdef EULER_INTEGRATION
	vec3 velocity;
#endif

public:
	Particle();

	void Update(float deltaTime);
	void Render();
	void ApplyForces();
	void SolveConstraints(const std::vector<OBB>& constraints);
	void SetPosition(const vec3& pos);
	vec3 GetPosition();
	void SetBounce(float b);
	float GetBounce();

	void AddImpulse(const vec3& impulse);
	float InvMass();
	void SetMass(float m);
	vec3 GetVelocity();
	void SetFriction(float f);
};

#endif