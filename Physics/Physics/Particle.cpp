#include "Particle.h"
#include "../Application/Geometry3D.h"
#include "../Application/FixedFunctionPrimitives.h"

void Particle::SetPosition(const vec3& pos) {
	position = oldPosition = pos;
}

vec3 Particle::GetPosition() {
	return position;
}

void Particle::SetBounce(float b) {
	bounce = b;
}

float Particle::GetBounce() {
	return bounce;
}

Particle::Particle() {
	type = RIGIDBODY_TYPE_PARTICLE;
	friction = 0.95f;
	gravity = vec3(0.0f, -9.82f, 0.0f);
	mass = 1.0f;
	bounce = 0.7f;
}

void Particle::Render() {
	Sphere visual(position, 0.1f);
	::Render(visual);
}

void Particle::ApplyForces() {
	forces = gravity;
}

void Particle::Update(float deltaTime) {
#ifdef EULER_INTEGRATION
	oldPosition = position;
	vec3 acceleration = forces * InvMass();
#ifdef ACCURATE_EULER_INTEGRATION
	vec3 oldVelocity = velocity;
	velocity = velocity * friction + acceleration * deltaTime;
	position = position + (oldVelocity + velocity) * 0.5f * deltaTime;
#else
	velocity = velocity * friction + acceleration * deltaTime;
	position = position + velocity * deltaTime;
#endif
#else
	vec3 velocity = position - oldPosition;
	oldPosition = position;
	float deltaSquare = deltaTime * deltaTime;
	position = position + (velocity * friction + forces * deltaSquare);
#endif
}

void Particle::SolveConstraints(const std::vector<OBB>& constraints) {
	int size = constraints.size();
	for (int i = 0; i < size; ++i) {
		Line traveled(oldPosition, position);
		if (Linetest(constraints[i], traveled)) {
			vec3 velocity = position - oldPosition;
			vec3 direction = Normalized(velocity);
			Ray ray(oldPosition, direction);
			RaycastResult result;
			if (Raycast(constraints[i], ray, &result)) {
				position = result.point + result.normal * 0.002f;
				vec3 vn = result.normal * Dot(result.normal, velocity);
				vec3 vt = velocity - vn;
				oldPosition = position - (vt - vn * bounce);
				break;
			}
		}
	}
}

void Particle::AddImpulse(const vec3& impulse) {
#ifdef EULER_INTEGRATION
	velocity = velocity + impulse;
#else
	vec3 velocity = position - oldPosition;
	velocity = velocity + impulse;
	oldPosition = position - velocity;
#endif
}

float Particle::InvMass() {
	if (mass == 0.0f) {
		return 0.0f;
	}
	return 1.0f / mass;
}

void Particle::SetMass(float m) {
	if (m < 0) {
		m = 0;
	}
	mass = m;
}

void Particle::SetFriction(float f) {
	if (f < 0) {
		f = 0;
	}
	friction = f;
}

vec3 Particle::GetVelocity() {
#ifdef EULER_INTEGRATION
	return velocity;
#else
	return position - oldPosition;
#endif
}

