#include "RigidbodyVolume.h"
#include "../Application/FixedFunctionPrimitives.h"

void RigidbodyVolume::ApplyForces() {
	forces = GRAVITY_CONST * mass;
}

void RigidbodyVolume::AddLinearImpulse(const vec3& impulse){
	velocity = velocity + impulse;
}

float RigidbodyVolume::InvMass() {
	if (mass == 0.0f) { return 0.0f; }
	return 1.0f / mass;
}

void RigidbodyVolume::SynchCollisionVolumes() {
	sphere.position = position;
	box.position = position;

	box.orientation = Rotation3x3(
		RAD2DEG(orientation.x),
		RAD2DEG(orientation.y),
		RAD2DEG(orientation.z)
	);
}

void RigidbodyVolume::Render() {
	SynchCollisionVolumes();
	if (type == RIGIDBODY_TYPE_SPHERE) {
		::Render(sphere);
	}
	else if (type == RIGIDBODY_TYPE_BOX) {
		::Render(box);
	}
}

void RigidbodyVolume::Update(float dt) {
	const float damping = 0.98f;
	vec3 acceleration = forces * InvMass();
	velocity = velocity + acceleration * dt;
	velocity = velocity * damping;
	if (type == RIGIDBODY_TYPE_BOX) {
		vec3 angAccel = MultiplyVector(torques, InvTensor());
		angVel = angVel + angAccel * dt;
		angVel = angVel * damping;
	}
	position = position + velocity * dt;
	if (type == RIGIDBODY_TYPE_BOX) {
		orientation = orientation + angVel * dt;
	}
	SynchCollisionVolumes();
}

CollisionManifold FindCollisionFeatures(
	RigidbodyVolume& ra, RigidbodyVolume& rb
) {
	CollisionManifold result;
	ResetCollisionManifold(&result);
	if (ra.type == RIGIDBODY_TYPE_SPHERE) {
		if (rb.type == RIGIDBODY_TYPE_SPHERE) {
			result = FindCollisionFeatures(ra.sphere, rb.sphere);
		}
		else if (rb.type == RIGIDBODY_TYPE_BOX) {
			result = FindCollisionFeatures(rb.box, ra.sphere);
			result.normal = result.normal * -1.0f;
		}
	}
	else if (ra.type == RIGIDBODY_TYPE_BOX) {
		if (rb.type == RIGIDBODY_TYPE_SPHERE) {
			result = FindCollisionFeatures(ra.box, rb.sphere);
		}
		else if (rb.type == RIGIDBODY_TYPE_BOX) {
			result = FindCollisionFeatures(rb.box, ra.box);
			result.normal = result.normal * -1.0f;
		}
	}
	return result;
}

//The first thing we do is find the relative velocity between the two rigidbodies.
//If therigidbodies are moving apart from each other,we stop the function as no collision can occur
void ApplyImpulse(RigidbodyVolume& A, RigidbodyVolume& B, const CollisionManifold& M, int c) {
	//Linear Velocity
	float invMass1 = A.InvMass();
	float invMass2 = B.InvMass();
	float invMassSum = invMass1 + invMass2;
	if (invMassSum == 0.0f) { return; }
	vec3 r1 = M.contacts[c] - A.position;
	vec3 r2 = M.contacts[c] - B.position;
	mat4 i1 = A.InvTensor();
	mat4 i2 = B.InvTensor();
	//Relative velocity
	vec3 relativeVel = (B.velocity + Cross(B.angVel,r2)) - (A.velocity + Cross(A.angVel,r1));
	//Relative collision normal
	vec3 relativeNorm = M.normal;
	Normalize(relativeNorm);
	//Moving away from each other?Do nothing!
	if (Dot(relativeVel, relativeNorm) > 0.0f) {
		return;
	}
	//Next we find the value of j.this is the magnitude of the impulse needed to
	//resolve the collision.As we are calculating j per contact,we divide the final j 
	//value by the number of contacts the intersection contains
	float e = fminf(A.cor, B.cor);
	float numerator = (-(1.0f + e) * Dot(relativeVel, relativeNorm));
	float d1 = invMassSum;
	vec3 d2 = Cross(MultiplyVector(Cross(r1, relativeNorm), i1), r1);
	vec3 d3 = Cross(MultiplyVector(Cross(r2, relativeNorm), i2), r2);
	float denominator = d1 + Dot(relativeNorm, d2 + d3);
	float j = (denominator == 0.0f) ? 0.0f : numerator / invMassSum;
	if (M.contacts.size() > 0.0f && j != 0.0f) {
		j /= (float)M.contacts.size();
	}

	//We multiply the collision normal by the magnitude of the impulse and apply the
	//resulting vector to the velocity of each of the colliding bodies.This is how we apply
	//Linear Impulse to the rigidbodies.We are modifying the welocity of each bodu directly
	//to make them push apart from each other
	vec3 impulse = relativeNorm * j;
	A.velocity = A.velocity - impulse * invMass1;
	B.velocity = B.velocity + impulse * invMass2;
	A.angVel = A.angVel - MultiplyVector(Cross(r1, impulse), i1);
	B.angVel = B.angVel + MultiplyVector(Cross(r2, impulse), i2);

	//Friction
	vec3 t = relativeVel - (relativeNorm * Dot(relativeVel, relativeNorm));
	if (CMP(MagnitudeSq(t), 0.0f)) {
		return;
	}
	Normalize(t);

	numerator = -Dot(relativeVel, t);
	d1 = invMassSum;
	d2 = Cross(MultiplyVector(Cross(r1, t), i1), r1);
	d3 = Cross(MultiplyVector(Cross(r2, t), i2), r2);
	denominator = d1 + Dot(t, d2 + d3);
	if (denominator == 0.0f) {
		return;
	}
	float jt = numerator / invMassSum;
	if (M.contacts.size() > 0.0f && jt != 0.0f) {
		jt /= (float)M.contacts.size();
	}
	if (CMP(jt, 0.0f)) {
		return;
	}
	float friction = sqrtf(A.friction * B.friction);
	if (jt > j * friction) {
		jt = j * friction;
	}
	else if (jt < -j * friction) {
		jt = -j * friction;
	}
	
	vec3 tangentImpuse = t * jt;

	A.velocity = A.velocity - tangentImpuse * invMass1;
	B.velocity = B.velocity + tangentImpuse * invMass2;
	A.angVel = A.angVel - MultiplyVector(Cross(r1, tangentImpuse), i1);
	B.angVel = B.angVel + MultiplyVector(Cross(r2, tangentImpuse), i2);
}

mat4 RigidbodyVolume::InvTensor() {
	float ix = 0.0f;
	float iy = 0.0f;
	float iz = 0.0f;
	float iw = 0.0f;

	if (mass != 0 && type == RIGIDBODY_TYPE_SPHERE) {
		float r2 = sphere.radius * sphere.radius;
		float fraction = (2.0f / 5.0f);
		ix = r2 * mass * fraction;
		iy = r2 * mass * fraction;
		iz = r2 * mass * fraction;
		iw = 1.0f;
	}
	else if(mass !=0 && type == RIGIDBODY_TYPE_BOX){
		vec3 size = box.size * 2.0f;
		float fraction = (1.0f / 12.0f);
		float x2 = size.x * size.x;
		float y2 = size.y * size.y;
		float z2 = size.z * size.z;
		ix = (y2 + z2) * mass * fraction;
		iy = (y2 + z2) * mass * fraction;
		iz = (x2 + y2) * mass * fraction;
		iw = 1.0f;
	}

	return Inverse(mat4(
		ix, 0, 0, 0,
		0, iy, 0, 0,
		0, 0, iz, 0,
		0, 0, 0, iw
	));
}

void RigidbodyVolume::AddRotationalImpulse(
	const vec3& point,const vec3& impulse
) {
	vec3 centerOfMass = position;
	vec3 torque = Cross(point - centerOfMass, impulse);
	vec3 angAccel = MultiplyVector(torque, InvTensor());
	angVel = angVel + angAccel;
}