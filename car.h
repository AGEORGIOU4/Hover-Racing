// Andreas Georgiou

#pragma once
#include <TL-Engine.h>

using namespace tle;

#define DEFAULT_GAME_STATE		100
#define COUNTDOWN_GAME_STATE    200
#define STAGE_0_COMPLETE		300
#define STAGE_1_COMPLETE		400
#define STAGE_2_COMPLETE		500
#define STAGE_3_COMPLETE		600
#define STAGE_4_COMPLETE		700
#define STAGE_5_COMPLETE		800
#define RACE_COMPLETE			900

class CCar
{
public:
	// Member functions (constructors, getters, setters)
	// Constructors:
	CCar() { };
	CCar(IMesh* carMesh, float x, float y, float z);

	void Update(I3DEngine* myEngine, float dt, bool collisionBool);
	void Initialize(IMesh* carMesh, float x, float y, float z);

	// Getters, Setters
	IModel* getCarModel() { return carModel; }

	float getSteeringAngle() {
		if (m_steeringAngle > 180 || m_steeringAngle < -180) {
			m_steeringAngle = 0;
		}
		return m_steeringAngle;
	}

	int getCarSpeed() { return m_v; }

	// Set different collision points for every hover angle
	void setCarRadius(float& carModelRadiusX, float& carModelRadiusZ, CCar arrayOfCars[], const int carNum, int gameState) {
		float steeringAngle;

		for (int i = 0; i < carNum; i++) {
			steeringAngle = arrayOfCars[0].getSteeringAngle();
		}

		// When hover goes straight
		if (steeringAngle > -15 && steeringAngle < 15) {
			carModelRadiusX = 2.0;
			carModelRadiusZ = 6.5;
		}

		if (steeringAngle > -30 && steeringAngle < -15 || steeringAngle < 30 && steeringAngle > 15) {
			carModelRadiusX = 2.5;
			carModelRadiusZ = 6.0;
		}

		if (steeringAngle > -45 && steeringAngle < -30 || steeringAngle < 45 && steeringAngle > 30) {
			carModelRadiusX = 4.0;
			carModelRadiusZ = 4.5;
		}
		if (steeringAngle > -60 && steeringAngle < -45 || steeringAngle < 60 && steeringAngle > 45) {
			carModelRadiusX = 5.5;
			carModelRadiusZ = 3.0;
		}

		if (steeringAngle > -75 && steeringAngle < -60 || steeringAngle < 75 && steeringAngle > 60) {
			carModelRadiusX = 6.0;
			carModelRadiusZ = 2.5;
		}


		if (steeringAngle > -90 && steeringAngle < -75 || steeringAngle < 75 && steeringAngle > 60) {
			carModelRadiusX = 6.5;
			carModelRadiusZ = 2.0;
		}

		if (steeringAngle > -105 && steeringAngle < -90 || steeringAngle < 105 && steeringAngle > 90) {
			carModelRadiusX = 6.5;
			carModelRadiusZ = 2.0;
		}

		if (steeringAngle > -120 && steeringAngle < -105 || steeringAngle < 120 && steeringAngle > 105) {
			carModelRadiusX = 6.0;
			carModelRadiusZ = 2.5;
		}

		if (steeringAngle > -135 && steeringAngle < -120 || steeringAngle < 135 && steeringAngle > 120) {
			carModelRadiusX = 5.5;
			carModelRadiusZ = 3.0;
		}
		if (steeringAngle > -150 && steeringAngle < -135 || steeringAngle < 150 && steeringAngle > 135) {
			carModelRadiusX = 4.5;
			carModelRadiusZ = 4.0;
		}

		if (steeringAngle > -165 && steeringAngle < -150 || steeringAngle < 165 && steeringAngle > 150) {
			carModelRadiusX = 2.5;
			carModelRadiusZ = 6.0;
		}

		if (steeringAngle > -180 && steeringAngle < -165 || steeringAngle < 180 && steeringAngle > 165) {
			carModelRadiusX = 2.0;
			carModelRadiusZ = 6.5;
		}
	}

	void setCarSpeedOnCollision() { m_v = (m_v / 2.5) * -1; }
	void setCarSpeedOnCollisionWithOpponent() { m_v = (m_v / 7.5) * -1; }
	void setCarSpeed() { m_v = 90.0f; 	m_v_Reverse = 20.0f; }
	void setCarSpeedOnBoost() { acceleration = 5.0f; top_Speed = 180; }
	void setCarSpeedWithoutBoost() { acceleration = 1.0f; top_Speed = 150; }
	void setCarSpeedOnOverheat() { m_v = 60; }

private:
	// Member variables
	IModel* carModel;
	float m_v;
	float m_v_Reverse;
	float m_steeringAngle;
	bool collisionBool;
	float acceleration;
	int top_Speed;
	int gamestate;
};


