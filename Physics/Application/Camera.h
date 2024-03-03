#ifndef _H_CAMERA_
#define _H_CAMERA_

#include "../Math/matrices.h"
#include "Geometry3D.h"

class Camera {
protected:
	float m_nFov;
	float m_nAspect;
	float m_nNear;
	float m_nFar;
	float m_nWidth;
	float m_nHeight;

	mat4 m_matWorld;//World Transform
	//View Transform = Inverse(World Transform)
	mat4 m_matProj;
	int m_nProjectionMode;
	// ^0 - Perspective,1 - Ortho,2-User
public:
	Camera();
	inline virtual ~Camera(){}
	
	mat4 GetWorldMatrix();
	mat4 GetViewMatrix();//Inverse of World
	mat4 GetProjectionMatrix();
	void SetProjection(const mat4& projection);
	void SetWorld(const mat4& view);

	float GetAspect();
	bool IsOrthograhic();
	bool IsPerspective();
	bool IsOrthoNormal();
	void OrthoNormalize();

	void Resize(int width, int height);
	void Perspective(float fov, float aspect, float zNear, float zFar);
	void Orthographic(float width, float height, float zNear, float zFar);
	Frustum GetFrustum();
};

class OrbitCamera : public Camera {
protected:
	vec3 target;
	vec2 panSpeed;

	float zoomDistance;
	vec2 zoomDistanceLimit;
	float zoomSpeed;

	vec2 rotationSpeed;
	vec2 yRotationLimit;
	vec2 currenrtRotation;

	float ClampAngle(float angle, float min, float max);
public:
	OrbitCamera();
	inline virtual ~OrbitCamera(){}

	void Rotate(const vec2& deltaRot, float deltaTime);
	void Zoom(float deltaZoom, float deltaTime);
	void Pan(const vec2& deltaPan,float deltaTime);

	void Update(float dt);


	Frustum GetFrustum();
};


#endif // !1
