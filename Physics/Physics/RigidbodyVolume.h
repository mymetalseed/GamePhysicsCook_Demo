#ifndef _H_MASS_RIGIDBODY_
#define _H_MASS_RIGIDBODY_

#include "Rigidbody.h"
#define GRAVITY_CONST vec3(0.0f,-9.82f,0.0f)

class RigidbodyVolume : public Rigidbody {
public:
	vec3 position;
	vec3 velocity;
	vec3 forces;	//Sum of all forces
	float mass;
	float cor;	//Coeffcient of restitution
	float friction;

	OBB box;
	Sphere sphere;

	vec3 orientation;
	vec3 angVel;
	vec3 torques; // Sum torques

	
public:
	inline RigidbodyVolume() :
		cor(0.5f), mass(1.0f), friction(0.6f) {
		type = RIGIDBODY_TYPE_BASE;
	}
	inline RigidbodyVolume(int bodyType) :
		cor(0.5f), mass(1.0f), friction(0.6f)
	{
		type = bodyType;
	}

	~RigidbodyVolume(){}

	void Render();
	void Update(float dt);	//Update position
	void ApplyForces();

	void SynchCollisionVolumes();
	float InvMass();
	void AddLinearImpulse(const vec3& impulse);

	mat4 InvTensor();
	virtual void AddRotationalImpulse(const vec3& point, const vec3& impulse);
};

CollisionManifold FindCollisionFeatures(
	RigidbodyVolume& ra, RigidbodyVolume& rb
);
void ApplyImpulse(RigidbodyVolume& A, RigidbodyVolume& B, const CollisionManifold& M, int c);


#endif