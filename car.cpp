// Andreas Georgiou

#include <TL-Engine.h>
#include "car.h"
#include <iostream>

using namespace tle;

CCar::CCar(IMesh* carMesh, float x, float y, float z) {
	Initialize(carMesh, x, y, z);
}

void CCar::Initialize(IMesh* carMesh, float x, float y, float z) {
	carModel = carMesh->CreateModel(x, y, z);
	m_v = 0.0f;
	m_v_Reverse = 0.0f;
	m_steeringAngle = 0.0f;
	acceleration = 1.0f;
	top_Speed = 150;
}

void CCar::Update(I3DEngine* myEngine, float dt, bool collisionBool) {
	float dsForward = m_v * dt;
	float dsReverse = m_v_Reverse * dt;
	float stepAngle = 90.0f * dt;
	float topSpeedReverse = 30;

	// Slow down the car speed:
	if (!myEngine->KeyHeld(Key_W)) {
		carModel->MoveLocalZ(dsForward);
		if (m_v > 0) {
			m_v -= 120 * dt;
		}

		if (m_v < 0) {
			m_v += 120 * dt;
		}
	}

	// Move the car forward:
	if (myEngine->KeyHeld(Key_W)) {
		carModel->MoveLocalZ(dsForward);
		if (m_v < top_Speed) {
			m_v += (70 * acceleration) * dt;
		}
	}

	// Move the car backwards:
	if (myEngine->KeyHeld(Key_S)) {
		if (m_v > -topSpeedReverse) {
			m_v -= 200 * dt;
		}
		if (m_v < 1 && m_v > -1) {
			carModel->MoveLocalZ(-dsReverse);
		}
	}

	if (myEngine->KeyHeld(Key_A)) {
		m_steeringAngle -= stepAngle;
		carModel->RotateLocalY(-stepAngle);
	}

	if (myEngine->KeyHeld(Key_D)) {
		m_steeringAngle += stepAngle;
		carModel->RotateLocalY(+stepAngle);
	}
}
