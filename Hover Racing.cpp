// Andreas Georgiou
// Hover Racing.cpp: A program using the TL-Engine
// GitHub Repository

#include <iostream>
#include <TL-Engine.h>	// TL-Engine include file and namespace
#include "car.h"
#include <math.h>

using namespace tle;

// CAMERA STATES
#define CAMERA_FIRST_PERSON   2100
#define CAMERA_FREE_MOVING    2200
#define CAMERA_THIRD_PERSON   2300
#define CAMERA_SURVEILLANCE	  2400
#define CAMERA_TOP_VIEW		  2500

// GAME STATES
#define DEFAULT_GAME_STATE		100
#define COUNTDOWN_GAME_STATE    200
#define STAGE_0_COMPLETE		300
#define STAGE_1_COMPLETE		400
#define STAGE_2_COMPLETE		500
#define STAGE_3_COMPLETE		600
#define STAGE_4_COMPLETE		700
#define STAGE_5_COMPLETE		800
#define STAGE_6_COMPLETE		900
#define STAGE_7_COMPLETE		1000
#define STAGE_8_COMPLETE		1100
#define STAGE_9_COMPLETE		1200
#define STAGE_10_COMPLETE		1300
#define RACE_COMPLETE			1400

#define NORMAL_GAME				3000
#define RESTART_GAME			4000

// CAR STATES
#define FORWARD					5000
#define LEFT					5100
#define BACKWARD				5200
#define RIGHT					5300

//======================================================================UPDATES & STATES================================================================================================//
void UpdateGame(I3DEngine* myEngine, float& dt, int& gameState, int& state) {
	// Initialize timer for each frame
	dt = myEngine->Timer();

	if (myEngine->KeyHit(Key_Escape))
		myEngine->Stop();

	if (myEngine->KeyHit(Key_Space) && gameState == DEFAULT_GAME_STATE)
		gameState = COUNTDOWN_GAME_STATE;
}

void countdown(IFont* myFont, float& seconds, int& gameState, float& dt) {
	if (seconds > 1 && seconds < 2) {
		myFont->Draw("3", 50, 475);
	}

	if (seconds > 2 && seconds < 3) {
		myFont->Draw("2", 50, 475);
	}

	if (seconds > 3 && seconds < 4) {
		myFont->Draw("1", 50, 475);
	}

	if (seconds > 4 && seconds < 5) {
		myFont->Draw("GO!", 50, 475);
	}

	if (seconds > 4.3) {
		gameState = STAGE_0_COMPLETE;
	}

}

void updateCar(I3DEngine* myEngine, float& dt, CCar arrayOfCars[], const int carNum, bool collisionBool) {
	// Update Car:
	arrayOfCars[0].Update(myEngine, dt, collisionBool);
}

void updateOpponentCar(I3DEngine* myEngine, float& dt, CCar arrayOfCars[], const int carNum, bool collisionBool, int& gameState,
	float& rotation, float& opponentCarSpeed, float& steeringAngleOfOC, bool& collisionWithCars, IModel* dummyModel[], int& dummyCounter) {

	float stepAngle = 200.0f * dt;
	// Update NC-Player Car:
	if (gameState > COUNTDOWN_GAME_STATE && collisionWithCars == false) {

		opponentCarSpeed = 90.0f;
		arrayOfCars[1].getCarModel()->MoveLocalZ(opponentCarSpeed * dt);

		for (int i = 0; i < 21; i++) {
			if (dummyCounter == i) {
				arrayOfCars[1].getCarModel()->LookAt(dummyModel[i]);

				// Opponent car follows dummies straight line
				if (dummyModel[dummyCounter]->GetZ() - 5 < arrayOfCars[1].getCarModel()->GetZ() && dummyCounter < 5) {
					dummyCounter++;
				}

				// Opponent car follows dummies left line
				if (dummyModel[dummyCounter]->GetX() + 5 > arrayOfCars[1].getCarModel()->GetX() && dummyCounter >= 5 && dummyCounter < 11) {
					dummyCounter++;
				}

				// Opponent car follows dummies back line					
				if (dummyModel[dummyCounter]->GetZ() + 5 > arrayOfCars[1].getCarModel()->GetZ() && dummyCounter >= 11 && dummyCounter < 15) {
					dummyCounter++;
				}

				// Opponent car follows dummies right line
				if (dummyModel[dummyCounter]->GetX() - 5 < arrayOfCars[1].getCarModel()->GetX() && dummyCounter >= 15 && dummyCounter < 21) {
					dummyCounter++;
				}

				if (dummyCounter == 20) {
					dummyCounter = 0;
				}
			}
		}
	}
}

void gameStates(int& gameState, IFont* myFont, float seconds, I3DEngine* myEngine, float& dt, CCar arrayOfCars[], const int carNum, float& carModelRadiusX, float& carModelRadiusZ,
	bool collisionBool, float rotation, float& opponentCarSpeed, float& steeringAngleOfOC, bool& collisionWithCars, IModel* dummyModel[],
	int& dummyCounter, int& damagePoints, int& lap, int& lapOpponent, int& currentCheckpoint, int& currentCheckpointOpponent) {

	// Output speed on the board
	int carSpeed = arrayOfCars[0].getCarSpeed();
	string strSpeed = to_string(carSpeed);
	myFont->Draw("Speed: " + strSpeed, 50, 550);

	// Output position on the board
	if (lap > lapOpponent) {
		myFont->Draw("Position: 1/2 ", 50, 525);
	}

	if (lap < lapOpponent) {
		myFont->Draw("Position: 2/2 ", 50, 525);
	}

	if (lap == lapOpponent) {
		if (currentCheckpoint > currentCheckpointOpponent) {
			myFont->Draw("Position: 1/2 ", 50, 525);
		}

		if (currentCheckpoint < currentCheckpointOpponent) {
			myFont->Draw("Position: 2/2 ", 50, 525);
		}
	}

	if (lap == lapOpponent && currentCheckpoint == currentCheckpointOpponent) {
		myFont->Draw("Position: -/2 ", 50, 525);
	}

	// Output damage points on the board
	string strDamage = to_string(damagePoints);
	myFont->Draw("Damage: " + strDamage, 50, 575);

	// Output current lap on the board
	string strLap = to_string(lap);
	myFont->Draw("Lap: " + strLap + "/3", 50, 600);

	if (damagePoints <= 0) {
		gameState = RACE_COMPLETE;
		myFont->Draw("BROKEN!", 50, 475);
		myFont->Draw("GAME OVER!", 553, 100);
		myFont->Draw("Hit R to Restart", 535, 140);
	}

	if (gameState == DEFAULT_GAME_STATE) {
		myFont->Draw("Hit space to start", 553, 100);
		myFont->Draw("READY?", 50, 450);

	}

	if (gameState == COUNTDOWN_GAME_STATE) {
		myFont->Draw("READY?", 50, 450);
		countdown(myFont, seconds, gameState, dt);
	}

	if (gameState == STAGE_0_COMPLETE) {
		myFont->Draw("Checkpoint 1/10", 50, 450);
		updateCar(myEngine, dt, arrayOfCars, carNum, collisionBool);
		updateOpponentCar(myEngine, dt, arrayOfCars, carNum, collisionBool, gameState, rotation, opponentCarSpeed, steeringAngleOfOC,
			collisionWithCars, dummyModel, dummyCounter);
		arrayOfCars[0].setCarRadius(carModelRadiusX, carModelRadiusZ, arrayOfCars, carNum, gameState);
	}

	if (gameState == STAGE_1_COMPLETE) {
		myFont->Draw("Checkpoint 2/10", 50, 450);
		updateCar(myEngine, dt, arrayOfCars, carNum, collisionBool);
		updateOpponentCar(myEngine, dt, arrayOfCars, carNum, collisionBool, gameState, rotation, opponentCarSpeed, steeringAngleOfOC,
			collisionWithCars, dummyModel, dummyCounter);
		arrayOfCars[0].setCarRadius(carModelRadiusX, carModelRadiusZ, arrayOfCars, carNum, gameState);
	}

	if (gameState == STAGE_2_COMPLETE) {
		myFont->Draw("Checkpoint 3/10", 50, 450);
		updateCar(myEngine, dt, arrayOfCars, carNum, collisionBool);
		updateOpponentCar(myEngine, dt, arrayOfCars, carNum, collisionBool, gameState, rotation, opponentCarSpeed, steeringAngleOfOC,
			collisionWithCars, dummyModel, dummyCounter);
		arrayOfCars[0].setCarRadius(carModelRadiusX, carModelRadiusZ, arrayOfCars, carNum, gameState);
	}

	if (gameState == STAGE_3_COMPLETE) {
		myFont->Draw("Checkpoint 4/10", 50, 450);
		updateCar(myEngine, dt, arrayOfCars, carNum, collisionBool);
		updateOpponentCar(myEngine, dt, arrayOfCars, carNum, collisionBool, gameState, rotation, opponentCarSpeed, steeringAngleOfOC,
			collisionWithCars, dummyModel, dummyCounter);
		arrayOfCars[0].setCarRadius(carModelRadiusX, carModelRadiusZ, arrayOfCars, carNum, gameState);
	}

	if (gameState == STAGE_4_COMPLETE) {
		myFont->Draw("Checkpoint 5/10", 50, 450);
		updateCar(myEngine, dt, arrayOfCars, carNum, collisionBool);
		updateOpponentCar(myEngine, dt, arrayOfCars, carNum, collisionBool, gameState, rotation, opponentCarSpeed, steeringAngleOfOC,
			collisionWithCars, dummyModel, dummyCounter);
		arrayOfCars[0].setCarRadius(carModelRadiusX, carModelRadiusZ, arrayOfCars, carNum, gameState);
	}

	if (gameState == STAGE_5_COMPLETE) {
		myFont->Draw("Checkpoint 6/10", 50, 450);
		updateCar(myEngine, dt, arrayOfCars, carNum, collisionBool);
		updateOpponentCar(myEngine, dt, arrayOfCars, carNum, collisionBool, gameState, rotation, opponentCarSpeed, steeringAngleOfOC,
			collisionWithCars, dummyModel, dummyCounter);
		arrayOfCars[0].setCarRadius(carModelRadiusX, carModelRadiusZ, arrayOfCars, carNum, gameState);
	}

	if (gameState == STAGE_6_COMPLETE) {
		myFont->Draw("Checkpoint 7/10", 50, 450);
		updateCar(myEngine, dt, arrayOfCars, carNum, collisionBool);
		updateOpponentCar(myEngine, dt, arrayOfCars, carNum, collisionBool, gameState, rotation, opponentCarSpeed, steeringAngleOfOC,
			collisionWithCars, dummyModel, dummyCounter);
		arrayOfCars[0].setCarRadius(carModelRadiusX, carModelRadiusZ, arrayOfCars, carNum, gameState);
	}

	if (gameState == STAGE_7_COMPLETE) {
		myFont->Draw("Checkpoint 8/10", 50, 450);
		updateCar(myEngine, dt, arrayOfCars, carNum, collisionBool);
		updateOpponentCar(myEngine, dt, arrayOfCars, carNum, collisionBool, gameState, rotation, opponentCarSpeed, steeringAngleOfOC,
			collisionWithCars, dummyModel, dummyCounter);
		arrayOfCars[0].setCarRadius(carModelRadiusX, carModelRadiusZ, arrayOfCars, carNum, gameState);
	}

	if (gameState == STAGE_8_COMPLETE) {
		myFont->Draw("Checkpoint 9/10", 50, 450);
		updateCar(myEngine, dt, arrayOfCars, carNum, collisionBool);
		updateOpponentCar(myEngine, dt, arrayOfCars, carNum, collisionBool, gameState, rotation, opponentCarSpeed, steeringAngleOfOC,
			collisionWithCars, dummyModel, dummyCounter);
		arrayOfCars[0].setCarRadius(carModelRadiusX, carModelRadiusZ, arrayOfCars, carNum, gameState);
	}

	if (gameState == STAGE_9_COMPLETE) {
		myFont->Draw("Last Checkpoint!", 50, 450);
		updateCar(myEngine, dt, arrayOfCars, carNum, collisionBool);
		updateOpponentCar(myEngine, dt, arrayOfCars, carNum, collisionBool, gameState, rotation, opponentCarSpeed, steeringAngleOfOC,
			collisionWithCars, dummyModel, dummyCounter);
		arrayOfCars[0].setCarRadius(carModelRadiusX, carModelRadiusZ, arrayOfCars, carNum, gameState);
	}


	if (gameState == RACE_COMPLETE) {
		myFont->Draw("RACE COMPLETE", 50, 450);

		if (lapOpponent < 3 && damagePoints > 0) {
			myFont->Draw("You finished 1st!", 553, 100);
		}

		if (lapOpponent == 3 && damagePoints > 0) {
			myFont->Draw("You finished 2nd!", 553, 100);
		}
	}
}

//======================================================================CAMERA MOVEMENTS================================================================================================//
void cameraMove(I3DEngine* myEngine, ICamera* myCamera, float& movement, float& movementLR, float dt) {
	if (myEngine->KeyHeld(Key_Up)) {
		movement += 200 * dt;
	}

	if (myEngine->KeyHeld(Key_Down)) {
		movement -= 200 * dt;
	}

	if (myEngine->KeyHeld(Key_Left)) {
		movementLR -= 200 * dt;
	}

	if (myEngine->KeyHeld(Key_Right)) {
		movementLR += 200 * dt;
	}

	if (myEngine->KeyHit(Key_C)) {
		movement = 0;
		movementLR = 0;
		myCamera->ResetOrientation();
	}
}

void rotateCamera(I3DEngine* myEngine, ICamera* myCamera, float angleCameraUD, float angleCameraLR, float mouseSensitivity, float dt) {
	// Rotate Camera (Mouse)
	angleCameraUD = mouseSensitivity * dt * myEngine->GetMouseMovementY();
	myCamera->RotateLocalX(angleCameraUD);

	angleCameraLR = mouseSensitivity * dt * myEngine->GetMouseMovementX();
	myCamera->RotateY(angleCameraLR);
}

int playerView(I3DEngine* myEngine, ICamera* myCamera, float rotateAngle,
	float initialAngleCamera, float dt, bool& topView, int& cameraState, float cameraX, float cameraY, float cameraZ, float& movement, float& movementLR) {

	if (cameraState == CAMERA_FREE_MOVING) {
		myCamera->DetachFromParent();
		cameraX = 0, cameraY = 60, cameraZ = -120;
		myCamera->SetPosition(cameraX + movementLR, cameraY, cameraZ + movement);
		cameraMove(myEngine, myCamera, movement, movementLR, dt);
	}

	if (myEngine->KeyHit(Key_1)) {
		movement = 0;
		movementLR = 0;
		myCamera->ResetOrientation();
		cameraState = CAMERA_FIRST_PERSON;
	}

	if (myEngine->KeyHit(Key_3)) {
		movement = 0;
		movementLR = 0;
		myCamera->ResetOrientation();
		cameraState = CAMERA_THIRD_PERSON;
	}

	if (myEngine->KeyHit(Key_4)) {
		movement = 0;
		movementLR = 0;
		myCamera->ResetOrientation();
		topView = true;
		cameraState = CAMERA_SURVEILLANCE;
	}

	if (myEngine->KeyHit(Key_7)) {
		movement = 0;
		movementLR = 0;
		myCamera->ResetOrientation();
		topView = true;
		cameraState = CAMERA_TOP_VIEW;
	}

	return cameraState;
}

int GoFirstPerson(I3DEngine* myEngine, ICamera* myCamera, int& cameraState, CCar arrayOfCars[], float initialAngleCamera, float rotateAngle, float cameraX, float cameraY,
	float cameraZ, float& movement, float& movementLR, float dt, bool& topView) {

	if (cameraState == CAMERA_FIRST_PERSON) {
		myCamera->DetachFromParent();
		cameraX = 0, cameraY = 5, cameraZ = 0;
		myCamera->SetPosition(cameraX + movementLR, cameraY, cameraZ + movement);
		myCamera->AttachToParent(arrayOfCars[0].getCarModel());
		cameraMove(myEngine, myCamera, movement, movementLR, dt);

	}

	if (myEngine->KeyHit(Key_2)) {
		movement = 0;
		movementLR = 0;
		myCamera->ResetOrientation();
		cameraState = CAMERA_FREE_MOVING;
	}

	if (myEngine->KeyHit(Key_3)) {
		movement = 0;
		movementLR = 0;
		myCamera->ResetOrientation();
		cameraState = CAMERA_THIRD_PERSON;
	}

	if (myEngine->KeyHit(Key_4)) {
		movement = 0;
		movementLR = 0;
		myCamera->ResetOrientation();
		topView = true;
		cameraState = CAMERA_SURVEILLANCE;
	}

	if (myEngine->KeyHit(Key_7)) {
		movement = 0;
		movementLR = 0;
		myCamera->ResetOrientation();
		topView = true;
		cameraState = CAMERA_TOP_VIEW;
	}

	return cameraState;
}

int GoThirdPerson(I3DEngine* myEngine, ICamera* myCamera, int& cameraState, CCar arrayOfCars[], float initialAngleCamera, float rotateAngle, float cameraX, float cameraY,
	float cameraZ, float& movement, float& movementLR, float dt, bool& topView) {

	if (cameraState == CAMERA_THIRD_PERSON) {
		myCamera->DetachFromParent();
		cameraX = 0, cameraY = 20, cameraZ = -50;
		myCamera->SetPosition(cameraX + movementLR, cameraY, cameraZ + movement);
		myCamera->AttachToParent(arrayOfCars[0].getCarModel());
		cameraMove(myEngine, myCamera, movement, movementLR, dt);
	}

	if (myEngine->KeyHit(Key_1)) {
		movement = 0;
		movementLR = 0;
		myCamera->ResetOrientation();
		cameraState = CAMERA_FIRST_PERSON;
	}

	if (myEngine->KeyHit(Key_2)) {
		movement = 0;
		movementLR = 0;
		myCamera->ResetOrientation();
		cameraState = CAMERA_FREE_MOVING;
	}

	if (myEngine->KeyHit(Key_4)) {
		movement = 0;
		movementLR = 0;
		myCamera->ResetOrientation();
		topView = true;
		cameraState = CAMERA_SURVEILLANCE;
	}

	if (myEngine->KeyHit(Key_7)) {
		movement = 0;
		movementLR = 0;
		myCamera->ResetOrientation();
		topView = true;
		cameraState = CAMERA_TOP_VIEW;
	}

	return cameraState;
}

int GoSurveillance(I3DEngine* myEngine, ICamera* myCamera, int& cameraState, CCar arrayOfCars[], float initialAngleCamera, float rotateAngle, float cameraX, float cameraY,
	float cameraZ, float& movement, float& movementLR, float dt, bool& topView) {

	if (cameraState == CAMERA_SURVEILLANCE) {
		myCamera->DetachFromParent();
		cameraX = 0, cameraY = 60, cameraZ = -120;
		myCamera->SetPosition(cameraX + movementLR, cameraY, cameraZ + movement);
		myCamera->LookAt(arrayOfCars[0].getCarModel());
		cameraMove(myEngine, myCamera, movement, movementLR, dt);
	}

	if (myEngine->KeyHit(Key_1)) {
		movement = 0;
		movementLR = 0;
		myCamera->ResetOrientation();
		topView = false;
		cameraState = CAMERA_FIRST_PERSON;
	}

	if (myEngine->KeyHit(Key_2)) {
		movement = 0;
		movementLR = 0;
		myCamera->ResetOrientation();
		topView = false;
		cameraState = CAMERA_FREE_MOVING;
	}

	if (myEngine->KeyHit(Key_3)) {
		movement = 0;
		movementLR = 0;
		myCamera->ResetOrientation();
		topView = false;
		cameraState = CAMERA_THIRD_PERSON;
	}

	if (myEngine->KeyHit(Key_7)) {
		movement = 0;
		movementLR = 0;
		myCamera->ResetOrientation();
		topView = true;
		cameraState = CAMERA_TOP_VIEW;
	}

	return cameraState;
}

int GoTopView(I3DEngine* myEngine, ICamera* myCamera, float rotateAngle,
	float initialAngleCamera, float dt, bool& topView, int& cameraState, CCar arrayOfCars[], float cameraX, float cameraY, float cameraZ, float& movement, float& movementLR) {

	if (cameraState == CAMERA_TOP_VIEW) {
		myCamera->DetachFromParent();
		cameraX = 0, cameraY = 700, cameraZ = -1.79;
		myCamera->SetPosition(cameraX + movementLR, cameraY, cameraZ + movement);
		myCamera->ResetOrientation();
		myCamera->RotateX(rotateAngle);
		myCamera->AttachToParent(arrayOfCars[0].getCarModel());
		cameraMove(myEngine, myCamera, movement, movementLR, dt);
	}

	if (myEngine->KeyHit(Key_1)) {
		movement = 0;
		movementLR = 0;
		myCamera->ResetOrientation();
		topView = false;
		cameraState = CAMERA_FIRST_PERSON;
	}

	if (myEngine->KeyHit(Key_2)) {
		movement = 0;
		movementLR = 0;
		myCamera->ResetOrientation();
		topView = false;
		cameraState = CAMERA_FREE_MOVING;
	}

	if (myEngine->KeyHit(Key_3)) {
		movement = 0;
		movementLR = 0;
		myCamera->ResetOrientation();
		topView = false;
		cameraState = CAMERA_THIRD_PERSON;
	}

	if (myEngine->KeyHit(Key_4)) {
		movement = 0;
		movementLR = 0;
		myCamera->ResetOrientation();
		topView = true;
		cameraState = CAMERA_SURVEILLANCE;
	}
	return cameraState;
}

int UpdateCamera(I3DEngine* myEngine, ICamera* myCamera, float rotateAngle,
	float initialAngleCamera, float dt, bool& topView, int cameraState, CCar arrayOfCars[], float cameraX, float cameraY, float cameraZ, float& movement, float& movementLR) {

	switch (cameraState) {
	case CAMERA_FREE_MOVING:
		cameraState = playerView(myEngine, myCamera, rotateAngle, initialAngleCamera, dt, topView, cameraState, cameraX, cameraY, cameraZ, movement, movementLR);
		break;

	case CAMERA_FIRST_PERSON:
		cameraState = GoFirstPerson(myEngine, myCamera, cameraState, arrayOfCars, initialAngleCamera, rotateAngle, cameraX, cameraY, cameraZ, movement, movementLR, dt, topView);
		break;

	case CAMERA_THIRD_PERSON:
		cameraState = GoThirdPerson(myEngine, myCamera, cameraState, arrayOfCars, initialAngleCamera, rotateAngle, cameraX, cameraY, cameraZ, movement, movementLR, dt, topView);
		break;

	case CAMERA_SURVEILLANCE:
		cameraState = GoSurveillance(myEngine, myCamera, cameraState, arrayOfCars, initialAngleCamera, rotateAngle, cameraX, cameraY, cameraZ, movement, movementLR, dt, topView);
		break;

	case CAMERA_TOP_VIEW:
		cameraState = GoTopView(myEngine, myCamera, rotateAngle, initialAngleCamera, dt, topView, cameraState, arrayOfCars, cameraX, cameraY, cameraZ, movement, movementLR);
		break;
	}
	return cameraState;
}

//=====================================================================CHECK COLLISIONS=================================================================================================//
void wallsCollision(CCar arrayOfCars[], IModel* wallsModel[], float& TxMin, float& TxMax, float& TzMin, float& TzMax, float wallsModelRadiusX, float wallsModelRadiusZ,
	const int carNum, const int wallsNum, float carModelRadiusX, float carModelRadiusZ, float& collisionSeconds, bool& collisionBool,
	float carX, float carZ, int carSpeed, int& damagePoints) {

	// Collision with walls
	for (int i = 0; i < wallsNum; i++) {
		TxMin = wallsModel[i]->GetX() - wallsModelRadiusX - carModelRadiusX;
		TxMax = wallsModel[i]->GetX() + wallsModelRadiusX + carModelRadiusX;
		TzMin = wallsModel[i]->GetZ() - wallsModelRadiusZ - carModelRadiusZ;
		TzMax = wallsModel[i]->GetZ() + wallsModelRadiusZ + carModelRadiusZ;

		// Car Position
		carX = arrayOfCars[0].getCarModel()->GetX();
		carZ = arrayOfCars[0].getCarModel()->GetZ();

		// If car collides with inner wall 
		if (carX > TxMin && carX < TxMax && carZ > TzMin && carZ < TzMax) {

			// Collision effects
			arrayOfCars[0].setCarSpeedOnCollision();

			// Car collides with inner right walls
			if (carX > 0 && carX <= 40 && carZ < 290) {
				arrayOfCars[0].getCarModel()->SetLocalX(carX - (1));
				// When car collides player loses 5 points
				damagePoints -= 5;
			}

			// Car collides with outer right walls
			if (carX > 40 && carZ < 290) {
				arrayOfCars[0].getCarModel()->SetLocalX(carX + (1));
				// When car collides player loses 5 points
				damagePoints -= 5;
			}

			// Car collides with inner left walls
			if (carX < 0 && carX >= -40 && carZ < 290) {
				arrayOfCars[0].getCarModel()->SetLocalX(carX + (1));
				// When car collides player loses 5 points
				damagePoints -= 5;
			}

			// Car collides with outer left walls
			if (carX < -40 && carZ < 290) {
				arrayOfCars[0].getCarModel()->SetLocalX(carX - (1));
				// When car collides player loses 5 points
				damagePoints -= 5;
			}

			// Car collides with inner up walls
			if (carZ > 300 && carZ <= 350 && carX < -40) {
				arrayOfCars[0].getCarModel()->SetLocalZ(carZ - (1));
				// When car collides player loses 5 points
				damagePoints -= 5;
			}

			// Car collides with outer up walls
			if (carZ > 350 && carX <= -40) {
				arrayOfCars[0].getCarModel()->SetLocalZ(carZ + (1));
				// When car collides player loses 5 points
				damagePoints -= 5;
			}

			// Car collides with inner down walls
			if (carZ < 300 && carZ >= 270 && carX < -40) {
				arrayOfCars[0].getCarModel()->SetLocalZ(carZ + (1));
				// When car collides player loses 5 points
				damagePoints -= 5;
			}

			// Car collides with outer down walls
			if (carZ < 270 && carX < -40) {
				arrayOfCars[0].getCarModel()->SetLocalZ(carZ - (1));
				// When car collides player loses 5 points
				damagePoints -= 5;
			}

			if (carSpeed < 0) {
				arrayOfCars[0].getCarModel()->SetLocalZ(carZ - (1));
			}
		}
	}
}

void wallsCollisionOpponent(CCar arrayOfCars[], IModel* wallsModel[], float& TxMin, float& TxMax, float& TzMin, float& TzMax, float wallsModelRadiusX, float wallsModelRadiusZ,
	const int carNum, const int wallsNum, float carModelRadiusX, float carModelRadiusZ, float& collisionSeconds, bool& collisionBool,
	float carX, float carZ, int carSpeed, int opponentCarSpeed) {

	// Collision with walls
	for (int i = 0; i < wallsNum; i++) {
		TxMin = wallsModel[i]->GetX() - wallsModelRadiusX - carModelRadiusX;
		TxMax = wallsModel[i]->GetX() + wallsModelRadiusX + carModelRadiusX;
		TzMin = wallsModel[i]->GetZ() - wallsModelRadiusZ - carModelRadiusZ;
		TzMax = wallsModel[i]->GetZ() + wallsModelRadiusZ + carModelRadiusZ;

		// Car Position
		carX = arrayOfCars[1].getCarModel()->GetX();
		carZ = arrayOfCars[1].getCarModel()->GetZ();

		// If car collides with inner wall 
		if (carX > TxMin && carX < TxMax && carZ > TzMin && carZ < TzMax) {

			// Car collides with inner right walls
			if (carX > 0 && carX <= 40 && carZ < 290) {
				arrayOfCars[1].getCarModel()->SetLocalX(carX - (1));
			}

			// Car collides with outer right walls
			if (carX > 40 && carZ < 290) {
				arrayOfCars[1].getCarModel()->SetLocalX(carX + (1));
			}

			// Car collides with inner left walls
			if (carX < 0 && carX >= -40 && carZ < 290) {
				arrayOfCars[1].getCarModel()->SetLocalX(carX + (1));
			}

			// Car collides with outer left walls
			if (carX < -40 && carZ < 290) {
				arrayOfCars[1].getCarModel()->SetLocalX(carX - (1));
			}

			// Car collides with inner up walls
			if (carZ > 300 && carZ <= 350 && carX < -40) {
				arrayOfCars[1].getCarModel()->SetLocalZ(carZ - (1));
			}

			// Car collides with outer up walls
			if (carZ > 350 && carX <= -40) {
				arrayOfCars[1].getCarModel()->SetLocalZ(carZ + (1));
			}

			// Car collides with inner down walls
			if (carZ < 300 && carZ >= 270 && carX < -40) {
				arrayOfCars[1].getCarModel()->SetLocalZ(carZ + (1));
			}

			// Car collides with outer down walls
			if (carZ < 270 && carX < -40) {
				arrayOfCars[1].getCarModel()->SetLocalZ(carZ - (1));
			}
		}
	}
}

void checkpointsCollision(CCar arrayOfCars[], IModel* checkpointsModel[], float& Dx, float& Dz, float modelRadius, const int carNum, const int modelNum, float carModelRadiusX,
	float carModelRadiusZ, int& gameState, int& passed, int& checkpointsPassed, int& currentCheckpoint, int& lap, I3DEngine* myEngine, int& state, IModel* cross, int& checkRotation) {

	// Coordinates for cross
	float x;
	float y;
	float z;

	if (currentCheckpoint >= 3 && currentCheckpoint <= 5 && checkRotation == 0) {
		cross->RotateLocalY(90);
		checkRotation = 1;
	}

	if (currentCheckpoint == 5 && checkRotation == 1) {
		cross->RotateLocalY(-45);
		checkRotation = 2;
	}

	if (currentCheckpoint > 5 && currentCheckpoint <= 7 && checkRotation == 2) {
		cross->RotateLocalY(-45);
		checkRotation = 3;
	}

	if (currentCheckpoint >= 8 && currentCheckpoint <= 10 && checkRotation == 3) {
		cross->RotateLocalY(90);
		checkRotation = 4;
	}

	// Collision with checkpoint
	if (checkpointsPassed < modelNum) {
		Dx = arrayOfCars[0].getCarModel()->GetX() - checkpointsModel[currentCheckpoint]->GetX();
		Dz = arrayOfCars[0].getCarModel()->GetZ() - checkpointsModel[currentCheckpoint]->GetZ();

		x = checkpointsModel[currentCheckpoint]->GetX();
		y = checkpointsModel[currentCheckpoint]->GetY();
		z = checkpointsModel[currentCheckpoint]->GetZ();

		if (passed == checkpointsPassed) {

			float collisionDistance = sqrt(Dx * Dx + Dz * Dz);

			cross->SetLocalPosition(x, y, z);

			if (collisionDistance < (carModelRadiusZ / 2) + modelRadius) {
				collisionDistance = 0;
				collisionDistance = sqrt(Dx * Dx + Dz * Dz);
				checkpointsPassed++;
				passed = checkpointsPassed;
				currentCheckpoint++;
				gameState += 100;
			}

			if (passed == modelNum) {
				checkpointsPassed = 0;
				passed = 0;
				currentCheckpoint = 0;
				lap++;
				gameState = STAGE_0_COMPLETE;
				cross->ResetOrientation();
				checkRotation = 0;
			}
			if (lap == 3) {
				gameState = RACE_COMPLETE;
			}
		}
	}
}

void checkpointsCollisionOpponent(CCar arrayOfCars[], IModel* checkpointsModel[], float& Sx, float& Sz, float modelRadius, const int carNum, const int modelNum, float carModelRadiusX,
	float carModelRadiusZ, int& gameState, int& passedOpponent, int& checkpointsPassedOpponent, int& currentCheckpointOpponent, int& lapOpponent, I3DEngine* myEngine, int& state) {

	// Collision with checkpoint
	if (checkpointsPassedOpponent < modelNum) {
		Sx = arrayOfCars[1].getCarModel()->GetX() - checkpointsModel[currentCheckpointOpponent]->GetX();
		Sz = arrayOfCars[1].getCarModel()->GetZ() - checkpointsModel[currentCheckpointOpponent]->GetZ();

		if (passedOpponent == checkpointsPassedOpponent) {

			float collisionDistance = sqrt(Sx * Sx + Sz * Sz);

			if (collisionDistance < (carModelRadiusZ / 2) + modelRadius) {
				collisionDistance = 0;
				collisionDistance = sqrt(Sx * Sx + Sz * Sz);
				checkpointsPassedOpponent++;
				passedOpponent = checkpointsPassedOpponent;
				currentCheckpointOpponent++;
			}

			if (passedOpponent == modelNum) {
				checkpointsPassedOpponent = 0;
				passedOpponent = 0;
				currentCheckpointOpponent = 0;
				lapOpponent++;
			}
		}
	}
}

void objectCollision(CCar arrayOfCars[], IModel* model[], float& Gx, float& Gz, float modelRadius, const int carNum, const int modelNum, float carModelRadiusX, float carModelRadiusZ,
	bool& collisionBool, float carX, float carZ, float collisionSeconds, int carSpeed, int& damagePoints, int& movingState, I3DEngine* myEngine) {

	// Collision with model
	for (int i = 0; i < modelNum; i++) {

		Gx = arrayOfCars[0].getCarModel()->GetX() - model[i]->GetX();
		Gz = arrayOfCars[0].getCarModel()->GetZ() - model[i]->GetZ();

		float collisionDistance = sqrt(Gx * Gx + Gz * Gz);

		if (collisionDistance < ((carModelRadiusZ + carModelRadiusX) / 2 + modelRadius)) {
			// Collision effects

			carSpeed = arrayOfCars[0].getCarSpeed();


			// Straight Line
			if (movingState == FORWARD && carSpeed > 0) {
				arrayOfCars[0].getCarModel()->SetLocalZ(arrayOfCars[0].getCarModel()->GetZ() - 0.2);
				// When car collides player loses 5 points
				damagePoints -= 5;
				arrayOfCars[0].setCarSpeedOnCollision();
			}

			// Left Line
			if (movingState == LEFT && carSpeed > 0) {
				arrayOfCars[0].getCarModel()->SetLocalX(arrayOfCars[0].getCarModel()->GetX() + 0.2);
				// When car collides player loses 5 points
				damagePoints -= 5;
				arrayOfCars[0].setCarSpeedOnCollision();
			}

			// Back Line
			if (movingState == BACKWARD && carSpeed > 0) {
				arrayOfCars[0].getCarModel()->SetLocalZ(arrayOfCars[0].getCarModel()->GetZ() + 0.2);
				// When car collides player loses 5 points
				damagePoints -= 5;
				arrayOfCars[0].setCarSpeedOnCollision();
			}

			// Right Line
			if (movingState == RIGHT && carSpeed > 0) {
				arrayOfCars[0].getCarModel()->SetLocalX(arrayOfCars[0].getCarModel()->GetX() - 0.2);
				// When car collides player loses 5 points
				damagePoints -= 5;
				arrayOfCars[0].setCarSpeedOnCollision();
			}

			// If player moves revers
			if (myEngine->KeyHeld(Key_S) && carSpeed < 0) {
				// Straight Line
				if (movingState == FORWARD) {
					arrayOfCars[0].getCarModel()->SetLocalZ(arrayOfCars[0].getCarModel()->GetZ() + 0.2);
					// When car collides player loses 5 points
					damagePoints -= 1;
					arrayOfCars[0].setCarSpeedOnCollision();
				}


				// Left Line
				if (movingState == LEFT) {
					arrayOfCars[0].getCarModel()->SetLocalX(arrayOfCars[0].getCarModel()->GetX() - 0.2);
					// When car collides player loses 5 points
					damagePoints -= 1;
					arrayOfCars[0].setCarSpeedOnCollision();
				}

				// Back Line
				if (movingState == BACKWARD) {
					arrayOfCars[0].getCarModel()->SetLocalZ(arrayOfCars[0].getCarModel()->GetZ() - 0.2);
					// When car collides player loses 5 points
					damagePoints -= 1;
					arrayOfCars[0].setCarSpeedOnCollision();
				}

				// Right Line
				if (movingState == RIGHT) {
					arrayOfCars[0].getCarModel()->SetLocalX(arrayOfCars[0].getCarModel()->GetX() + 0.2);
					// When car collides player loses 5 points
					damagePoints -= 1;
					arrayOfCars[0].setCarSpeedOnCollision();
				}
			}
		}
	}
}

void objectCollisionOpponent(CCar arrayOfCars[], IModel* model[], float& Gx, float& Gz, float modelRadius, const int carNum, const int modelNum, float carModelRadiusX, float carModelRadiusZ,
	bool& collisionBool, float carX, float carZ, float collisionSeconds, int carSpeed, int opponentCarSpeed) {

	// Collision with model
	for (int i = 0; i < modelNum; i++) {

		Gx = arrayOfCars[1].getCarModel()->GetX() - model[i]->GetX();
		Gz = arrayOfCars[1].getCarModel()->GetZ() - model[i]->GetZ();

		float collisionDistance = sqrt(Gx * Gx + Gz * Gz);


		if (collisionDistance < ((carModelRadiusZ + carModelRadiusX) / 2 + modelRadius)) {
			// Collision effects

			carSpeed = opponentCarSpeed;

			// Straight Line
			if (arrayOfCars[1].getCarModel()->GetZ() < 350 && arrayOfCars[1].getCarModel()->GetZ() > -360 && arrayOfCars[1].getCarModel()->GetX() > -40 && carSpeed > 0) {
				arrayOfCars[1].getCarModel()->SetLocalZ(arrayOfCars[1].getCarModel()->GetZ() - 0.2);
				arrayOfCars[1].getCarModel()->SetLocalX(arrayOfCars[1].getCarModel()->GetX() + 0.2);
			}

			// Left Line
			if (arrayOfCars[1].getCarModel()->GetZ() >= 270 && arrayOfCars[1].getCarModel()->GetX() <= -40 && carSpeed > 0) {
				arrayOfCars[1].getCarModel()->SetLocalZ(arrayOfCars[1].getCarModel()->GetZ() + 0.2);
				arrayOfCars[1].getCarModel()->SetLocalX(arrayOfCars[1].getCarModel()->GetX() + 0.2);
			}

			// Back Line
			if (arrayOfCars[1].getCarModel()->GetZ() <= 270 && arrayOfCars[1].getCarModel()->GetX() <= -440 && carSpeed > 0) {
				arrayOfCars[1].getCarModel()->SetLocalX(arrayOfCars[1].getCarModel()->GetX() - 0.2);
				arrayOfCars[1].getCarModel()->SetLocalZ(arrayOfCars[1].getCarModel()->GetZ() + 0.2);
			}
			// Right Line
			if (arrayOfCars[1].getCarModel()->GetZ() <= -240 && arrayOfCars[1].getCarModel()->GetX() >= -460 && carSpeed > 0) {
				arrayOfCars[1].getCarModel()->SetLocalX(arrayOfCars[1].getCarModel()->GetX() - 0.2);
				arrayOfCars[1].getCarModel()->SetLocalZ(arrayOfCars[1].getCarModel()->GetZ() + 0.2);
			}
		}
	}
}

void carsCollision(CCar arrayOfCars[], float& HxMin, float& HxMax, float& IzMin, float& IzMax, const int carNum, float carModelRadiusX, float carModelRadiusZ,
	float carX, float carZ, float carSpeed, float& opponentCarSpeed, bool& collisionWithCars, float& JxMin, float& JxMax, float& KzMin,
	float& KzMax, float carXopponent, float carZopponent, int& damagePoints, int& movingState) {

	// Car Position
	carX = arrayOfCars[0].getCarModel()->GetX();
	carZ = arrayOfCars[0].getCarModel()->GetZ();

	carXopponent = arrayOfCars[1].getCarModel()->GetX();
	carZopponent = arrayOfCars[1].getCarModel()->GetZ();

	float collisionDistance = sqrt((carX - carXopponent) * (carX - carXopponent) + ((carZ - carZopponent) * (carZ - carZopponent)));

	if (collisionDistance < ((carModelRadiusZ + carModelRadiusX) / 2 + (carModelRadiusZ + carModelRadiusX) / 2)) {
		// Collision effects

		carSpeed = arrayOfCars[0].getCarSpeed();


		// If car collides with other car when behind
		if (movingState == FORWARD && carZ < carZopponent) {
			arrayOfCars[0].setCarSpeedOnCollisionWithOpponent();
		}

		if (movingState == LEFT && carX > carXopponent) {
			arrayOfCars[0].setCarSpeedOnCollisionWithOpponent();
		}

		if (movingState == BACKWARD && carZ > carZopponent) {
			arrayOfCars[0].setCarSpeedOnCollisionWithOpponent();
		}

		if (movingState == RIGHT && carX < carXopponent) {
			arrayOfCars[0].setCarSpeedOnCollisionWithOpponent();
		}

		// If car collides with other car when in front
		if (movingState == FORWARD && carZ > carZopponent) {
			arrayOfCars[1].getCarModel()->SetLocalZ(carZopponent - 0.5);
		}

		if (movingState == LEFT && carX < carXopponent) {
			arrayOfCars[1].getCarModel()->SetLocalX(carXopponent + 0.5);
		}

		if (movingState == BACKWARD && carZ < carZopponent) {
			arrayOfCars[1].getCarModel()->SetLocalZ(carZopponent + 0.5);
		}

		if (movingState == RIGHT && carX > carXopponent) {
			arrayOfCars[1].getCarModel()->SetLocalX(carXopponent - 0.5);
		}
	}
}

//=====================================================================OTHER FEATURES=================================================================================================//
void liftCarWhenMove(CCar arrayOfCars[], float dt, int carSpeed, float opponentCarSpeed) {

	carSpeed = arrayOfCars[0].getCarSpeed();

	int hoverHeight = 5;

	if (carSpeed > 1 && arrayOfCars[0].getCarModel()->GetLocalY() < hoverHeight) {
		arrayOfCars[0].getCarModel()->MoveLocalY(5 * dt);
	}

	if (carSpeed < -15 && arrayOfCars[0].getCarModel()->GetLocalY() > 0) {
		arrayOfCars[0].getCarModel()->MoveLocalY(-10 * dt);
	}

	if (carSpeed < 1 && carSpeed > -1 && arrayOfCars[0].getCarModel()->GetLocalY() > 0) {
		arrayOfCars[0].getCarModel()->MoveLocalY(-5 * dt);
	}

	if (opponentCarSpeed > 1 && arrayOfCars[1].getCarModel()->GetLocalY() < hoverHeight) {
		arrayOfCars[1].getCarModel()->MoveLocalY(5 * dt);
	}

	if (opponentCarSpeed < -15 && arrayOfCars[1].getCarModel()->GetLocalY() < hoverHeight) {
		arrayOfCars[1].getCarModel()->MoveLocalY(5 * dt);
	}

	if (opponentCarSpeed < 1 && carSpeed > -1 && arrayOfCars[1].getCarModel()->GetLocalY() > 0) {
		arrayOfCars[1].getCarModel()->MoveLocalY(-5 * dt);
	}
}

void boostCar(CCar arrayOfCars[], I3DEngine* myEngine, IFont* myFont, float& dt, int carSpeed, float& boostSeconds, int gameState, bool& booster, int& overHeatCounter,
	bool& overHeated) {
	carSpeed = arrayOfCars[0].getCarSpeed();

	if (myEngine->KeyHeld(Key_Space) && gameState != DEFAULT_GAME_STATE && boostSeconds < 3 && overHeatCounter < 3 && overHeated == false) {
		arrayOfCars[0].setCarSpeedOnBoost();
		booster = true;
	}

	// OVERHEAT MESSAGE
	if (boostSeconds > 0.2 && boostSeconds < 2 && myEngine->KeyHeld(Key_Space) && overHeated == false) {
		overHeatCounter = 1;
		string str = to_string(overHeatCounter);
		myFont->Draw("Overheat: " + str, 50, 500);
	}

	if (boostSeconds > 2 && boostSeconds < 3.5 && myEngine->KeyHeld(Key_Space) && overHeated == false) {
		overHeatCounter = 2;
		string str = to_string(overHeatCounter);
		myFont->Draw("Release the boost!: " + str, 50, 500);
	}

	if (boostSeconds > 3.5 && boostSeconds < 6 && myEngine->KeyHeld(Key_Space) && overHeated == false) {
		overHeatCounter = 3;
		string str = to_string(overHeatCounter);
	}

	// If car is overheated
	if (overHeatCounter == 3) {
		myFont->Draw("Overheated!! ", 50, 500);
		arrayOfCars[0].setCarSpeedOnOverheat();
		overHeated = true;
	}

	if (!myEngine->KeyHeld(Key_Space) && gameState > COUNTDOWN_GAME_STATE) {
		arrayOfCars[0].setCarSpeedWithoutBoost();
		overHeatCounter = 0;

	}


	if (!myEngine->KeyHeld(Key_Space) && overHeated == false && gameState > COUNTDOWN_GAME_STATE) {
		myFont->Draw("BOOST", 50, 500);
	}

	if (boostSeconds == 0) {
		arrayOfCars[0].setCarSpeedWithoutBoost();
	}

	// Boost timing (If user holds spaces booster is enabled (boosterSeconds : 4)
	if (booster == true) {
		boostSeconds += dt;
		if (boostSeconds > 4.1) {
			booster = false;
			boostSeconds = 0;
		}
	}

	// Boost timing (If user holds spaces booster for more than 3 seconds overHeated is enabled (overheatedTiming : 5)
	if (overHeated == true && !myEngine->KeyHeld(Key_Space)) {
		boostSeconds += dt;
		if (boostSeconds > 5 && boostSeconds < 6) {
			myFont->Draw("BOOST", 50, 500);
		}

		if (boostSeconds > 6) {
			overHeated = false;
			boostSeconds = 0;
		}
	}
}

int getCarPosition(CCar arrayOfCars[], float carZposition, float carXposition, int& movingState) {
	carZposition = arrayOfCars[0].getCarModel()->GetZ();
	carXposition = arrayOfCars[0].getCarModel()->GetX();

	if (carZposition > -370 && carXposition >= -40) {
		movingState = FORWARD;
	}

	if (carZposition >= 270 && carXposition < -40) {
		movingState = LEFT;
	}

	if (carZposition < 270 && carXposition <= -460) {
		movingState = BACKWARD;
	}

	if (carZposition <= -250 && carXposition < -40) {
		movingState = RIGHT;
	}
	return movingState;
}

void restartGame(I3DEngine* myEngine, CCar arrayOfCars[], float& carX, float& carY, float& carZ, int& gameState, int& cameraState, float& seconds, float& collisionSeconds, bool& collisionBool,
	float& distance, int& passed, int& passedOpponent, int& checkpointsPassed, int& checkpointsPassedOpponent, float& boostSeconds, bool& booster, bool& overHeated, int& overHeatCounter,
	float& opponentCarSpeed, float& rotation, float& steeringAngleOfOC, bool& collisionWithCars, int& damagePoints, int& dummyCounter, int& lap, int& lapOpponent, int& movingState, int& currentCheckpoint,
	int& currentCheckpointOpponent, int& checkRotation, IModel* cross) {
	// Restart
	if (gameState == RACE_COMPLETE && myEngine->KeyHit(Key_R)) {
		float carX = 3;
		float carY = 0;
		float carZ = -15;

		arrayOfCars[0].getCarModel()->ResetOrientation();
		arrayOfCars[0].getCarModel()->SetPosition(carX, carY, carZ);

		carX -= 6;
		carZ += 5;
		carY += 0;
		arrayOfCars[1].getCarModel()->ResetOrientation();
		arrayOfCars[1].getCarModel()->SetPosition(carX, carY, carZ);
		gameState = DEFAULT_GAME_STATE;
		cameraState = CAMERA_FREE_MOVING;

		// Game variables
		seconds = 0;
		collisionSeconds = 0;
		collisionBool = false;
		distance = 0;
		passed = 0;
		passedOpponent = 0;

		// Initialize Checkpoints:
		checkpointsPassed = 0;
		checkpointsPassedOpponent = 0;
		currentCheckpoint = 0;
		currentCheckpointOpponent = 0;

		// Car variables 
		boostSeconds = 0;
		booster = false;
		overHeated = false;
		overHeatCounter = 0;
		opponentCarSpeed = 0;
		rotation = 0;
		steeringAngleOfOC = 0;
		collisionWithCars = false;
		damagePoints = 100;
		dummyCounter = 0;

		lap = 0;
		lapOpponent = 0;
		movingState = FORWARD;
		checkRotation = 0;
		cross->ResetOrientation();
	}
}

//===========================================================================MAIN=====================================================================================================//
void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("./media");

	// Initialize timer to avoid first frame problems

	float dt = myEngine->Timer();

	// Game variables
	float seconds = 0;
	float collisionSeconds = 0;
	bool collisionBool = false;
	float distance = 0;
	int passed = 0;
	int passedOpponent = 0;

	int gameState = DEFAULT_GAME_STATE;

	// Camera Variables
	int cameraState = CAMERA_FREE_MOVING;
	float rotateAngle = 75.0;
	float initialAngleCamera = 10;
	bool topView = false;
	float cameraX = 0, cameraY = 0, cameraZ = 0;
	float movement = 0, movementLR = 0;
	float mouseSensitivity = 50.0;
	float angleCameraUD = mouseSensitivity * myEngine->GetMouseMovementY();
	float angleCameraLR = mouseSensitivity * myEngine->GetMouseMovementX();

	// Camera
	ICamera* myCamera = myEngine->CreateCamera(kManual, 0, 60, -120);
	myCamera->RotateX(initialAngleCamera);

	/**** Set up your scene here ****/
	// Meshes
	IMesh* carMesh = myEngine->LoadMesh("race2.x");
	IMesh* skyboxMesh = myEngine->LoadMesh("Skybox 07.x");
	IMesh* floorMesh = myEngine->LoadMesh("ground.x");
	IMesh* checkpointMesh = myEngine->LoadMesh("heckpoint.x");
	IMesh* wallMesh = myEngine->LoadMesh("Wall.x");
	IMesh* isleMesh = myEngine->LoadMesh("IsleStraight.x");
	IMesh* tankSmallMesh = myEngine->LoadMesh("TankSmall1.x");
	IMesh* tankSmallMesh2 = myEngine->LoadMesh("TankSmall2.x");
	IMesh* dummyMesh = myEngine->LoadMesh("Dummy.x");
	IMesh* crossMesh = myEngine->LoadMesh("Cross.x");

	ISprite* backdrop = myEngine->CreateSprite("weave200.png", 30, 440);
	IFont* myFont = myEngine->LoadFont("Comic Sans MS", 25);

	// Models
	IModel* skyboxModel = skyboxMesh->CreateModel(0, -960, 0);
	IModel* floorModel = floorMesh->CreateModel(0, 0.2, 0);
	IModel* cross = crossMesh->CreateModel(0, 8, 5);

	// Positions of walls
	float AxMin, AxMax, AzMin, AzMax, BxMin, BxMax, BzMin, BzMax,
		CxMin, CxMax, CzMin, CzMax, DxMin, DxMax, DzMin, DzMax;

	float MxMin, MxMax, MzMin, MzMax, NxMin, NxMax, NzMin, NzMax,
		OxMin, OxMax, OzMin, OzMax, PxMin, PxMax, PzMin, PzMax;

	// Positions of checkpoints
	float Ex, Ez;
	float Sx, Sz;

	// Positions of isles
	float Fx, Fz, Gx, Gz;

	// Positions of tanks
	float Hx, Hz, Tx, Tz;

	// Positions of cars
	float  HxMin, HxMax, IzMin, IzMax, JxMin, JxMax, KzMin, KzMax, Kx, Kz, Lx, Lz;

	//================================================ARRAYS OF MODELS===================================================//

	// Initialize Walls:
	float wallsModelLRRadiusX = 1.0;
	float wallsModelLRRadiusZ = 5.0;
	float wallsModelUDRadiusX = 5.0;
	float wallsModelUDRadiusZ = 1.0;

	const int wallsLeftNum = 30;
	const int wallsRightNum = 30;
	const int wallsUpNum = 30;
	const int wallsDownNum = 30;

	IModel* wallModelLeft[wallsLeftNum];
	IModel* wallModelRight[wallsRightNum];
	IModel* wallModelUp[wallsUpNum];
	IModel* wallModelDown[wallsDownNum];

	int wallsLRZ = -25;
	int wallsLRY = 0;
	float wallsLRX = 40.0;
	int wallsUPZ = 270;
	float wallsUPX = -44.0;

	for (int i = 0; i < wallsLeftNum; i++) {
		// Create a corridor
		if (wallsLRZ >= 130 && wallsLRZ <= 160) {
			wallsLRX = 10;
			wallsLRY = -2;
		}
		else {
			wallsLRX = 40;
			wallsLRY = 0;
		}
		wallModelLeft[i] = wallMesh->CreateModel(-wallsLRX, wallsLRY, wallsLRZ);
		wallModelRight[i] = wallMesh->CreateModel(wallsLRX, wallsLRY, wallsLRZ);

		// Create walls after the left turn
		wallModelUp[i] = wallMesh->CreateModel(wallsUPX, 0, wallsUPZ + 80);
		wallModelDown[i] = wallMesh->CreateModel(wallsUPX, 0, wallsUPZ);
		wallModelUp[i]->RotateY(-90);
		wallModelDown[i]->RotateY(-90);
		wallsLRZ += 10;
		wallsUPX -= 10;
	}

	// Initialize Checkpoints:
	float checkpointsModelRadius = 3.0;
	int checkpointsPassed = 0;
	int checkpointsPassedOpponent = 0;
	const int checkPointsNum = 10;



	IModel* checkpointsModel[checkPointsNum];
	int checkpointZ = 0;
	int checkpointY = 5;
	float checkpointX = 0;
	int currentCheckpoint = 0;
	int currentCheckpointOpponent = 0;

	for (int i = 0; i < checkPointsNum; i++) {
		if (i < 2) {
			checkpointsModel[i] = checkpointMesh->CreateModel(checkpointX, checkpointY, checkpointZ);
			checkpointZ += 100;
		}

		if (i == 2) {
			checkpointsModel[i] = checkpointMesh->CreateModel(checkpointX, checkpointY, 240);
			checkpointZ += 100;
		}

		if (i == 3) {
			checkpointZ = 310;
			checkpointX = -110;
			checkpointsModel[i] = checkpointMesh->CreateModel(checkpointX, checkpointY, checkpointZ);
			checkpointsModel[i]->RotateY(90);
		}

		if (i > 3 && i < 5) {
			checkpointX -= 200;
			checkpointsModel[i] = checkpointMesh->CreateModel(checkpointX, checkpointY, checkpointZ);
			checkpointsModel[i]->RotateY(90);
		}

		if (i == 5) {
			checkpointX = -500;
			checkpointZ = 280;
			checkpointsModel[i] = checkpointMesh->CreateModel(checkpointX, checkpointY, checkpointZ);
			checkpointsModel[i]->RotateY(30);
			checkpointZ -= 200;
		}

		if (i == 6) {
			checkpointsModel[i] = checkpointMesh->CreateModel(checkpointX, checkpointY, checkpointZ);
			checkpointZ -= 200;
		}

		if (i == 7) {
			checkpointsModel[i] = checkpointMesh->CreateModel(checkpointX, checkpointY, checkpointZ);
			checkpointZ -= 200;
			checkpointX += 200;
		}

		if (i > 7) {
			checkpointsModel[i] = checkpointMesh->CreateModel(checkpointX, checkpointY, checkpointZ);
			checkpointsModel[i]->RotateY(90);
			checkpointX += 200;
		}
	}

	// Initialize Isles:
	float islesModelRadius = 1.5;

	const int islesNum = 10;
	IModel* isleModelLeft[islesNum];
	IModel* isleModelRight[islesNum];

	int islesZ = 0;
	int islesX = 9;

	for (int i = 0; i < islesNum; i++) {
		if (i < 2) {
			isleModelLeft[i] = isleMesh->CreateModel(-islesX, 0, islesZ);
			isleModelRight[i] = isleMesh->CreateModel(islesX, 0, islesZ);
			islesZ += 100;
		}

		if (i == 2) {
			isleModelLeft[i] = isleMesh->CreateModel(-islesX, 0, 240);
			isleModelRight[i] = isleMesh->CreateModel(islesX, 0, 240);
			islesZ += 100;
		}

		if (i == 3) {
			islesZ = 301;
			islesX = -110;
			isleModelLeft[i] = isleMesh->CreateModel(islesX, 0, islesZ);
			isleModelRight[i] = isleMesh->CreateModel(islesX, 0, islesZ + 18);
			isleModelLeft[i]->RotateY(90);
			isleModelRight[i]->RotateY(90);
		}

		if (i > 3 && i < 5) {
			islesX -= 200;
			isleModelLeft[i] = isleMesh->CreateModel(islesX, 0, islesZ);
			isleModelRight[i] = isleMesh->CreateModel(islesX, 0, islesZ + 18);
			isleModelLeft[i]->RotateY(90);
			isleModelRight[i]->RotateY(90);
		}

		if (i == 5) {
			islesX = -500;
			islesZ = 280;
			isleModelLeft[i] = isleMesh->CreateModel(islesX + 8, 0, islesZ - 4);
			isleModelRight[i] = isleMesh->CreateModel(islesX - 8, 0, islesZ + 4);
			isleModelLeft[i]->RotateY(30);
			isleModelRight[i]->RotateY(30);
			islesZ -= 200;
		}

		if (i == 6) {
			isleModelLeft[i] = isleMesh->CreateModel(islesX - 9, 0, islesZ);
			isleModelRight[i] = isleMesh->CreateModel(islesX + 9, 0, islesZ);
			islesZ -= 200;
		}

		if (i == 7) {
			isleModelLeft[i] = isleMesh->CreateModel(islesX + 9, 0, islesZ);
			isleModelRight[i] = isleMesh->CreateModel(islesX - 9, 0, islesZ);
			islesZ -= 200;
			islesX += 200;

		}

		if (i > 7) {
			isleModelLeft[i] = isleMesh->CreateModel(islesX, 0, islesZ + 9);
			isleModelRight[i] = isleMesh->CreateModel(islesX, 0, islesZ - 9);
			isleModelLeft[i]->RotateY(90);
			isleModelRight[i]->RotateY(90);
			islesX += 200;
		}
	}

	// Initialize Tanks:
	float tanksModelRadius = 1.75;
	const int tanksNum = 75;

	IModel* tanksModel[tanksNum];

	int tanksX = 10;
	int tanksZ = 120;

	float tanksZCorridor = 120;

	// Add tanks on the corridor corners
	for (int i = 0; i < tanksNum; i++) {
		if (i < 2) {
			tanksModel[i] = tankSmallMesh->CreateModel(tanksX - 2, 0, tanksZCorridor);
			// Rotate
			tanksZCorridor += 50;
		}

		if (i == 2) {
			// Buried tank
			tanksModel[i] = tankSmallMesh->CreateModel(-90, -8, 300);
			tanksZCorridor = 120;
		}

		if (i > 2 && i < 5) {
			tanksModel[i] = tankSmallMesh->CreateModel(-tanksX + 2, 0, tanksZCorridor);
			// Rotate
			tanksModel[i]->RotateY(180);
			tanksZCorridor += 50;
		}

		if (i == 5) {
			tanksX = 50;
			tanksZ = 270;
			tanksModel[i] = tankSmallMesh->CreateModel(tanksX, 0, tanksZ);
		}

		if (i > 5 && i < 18) {
			tanksX -= 8;
			tanksZ += 8;
			tanksModel[i] = tankSmallMesh->CreateModel(tanksX, 0, tanksZ);
		}

		if (i == 18) {
			tanksZ = 350;
			tanksX = -440;
			tanksModel[i] = tankSmallMesh->CreateModel(tanksX, 0, tanksZ);
		}

		if (i > 18 && i < 24) {
			tanksZ -= 10;
			tanksX -= 20;
			tanksModel[i] = tankSmallMesh->CreateModel(tanksX, 0, tanksZ);
		}

		if (i >= 24 && i < 38) {
			tanksX = -540;
			tanksZ -= 30;
			tanksModel[i] = tankSmallMesh->CreateModel(tanksX, 0, tanksZ);
		}

		if (i >= 38 && i < 50) {
			tanksZ -= 20;
			tanksModel[i] = tankSmallMesh->CreateModel(tanksX, 0, tanksZ);
		}

		if (i >= 50 && i < 65) {
			tanksX += 40;
			tanksModel[i] = tankSmallMesh->CreateModel(tanksX, 0, tanksZ);
		}

		if (i >= 65) {
			tanksZ += 40;
			tanksModel[i] = tankSmallMesh->CreateModel(tanksX, 0, tanksZ);
		}
	}

	// Initialize Tanks 2:
	float tanksModelRadius2 = 1.75;
	const int tanksNum2 = 52;

	IModel* tanksModel2[tanksNum2];

	int tanksX2 = -460;
	int tanksZ2 = 270;


	// Add tanks on the corridor corners
	for (int i = 0; i < tanksNum2; i++) {
		if (i <= 25) {
			tanksModel2[i] = tankSmallMesh2->CreateModel(tanksX2, 0, tanksZ2);
			tanksZ2 -= 20;
		}

		if (i > 25 && i < 41) {
			tanksX2 += 20;
			tanksModel2[i] = tankSmallMesh2->CreateModel(tanksX2, 0, tanksZ2);
		}

		if (i >= 41) {
			tanksX2 += 10;
			tanksZ2 += 20;
			tanksModel2[i] = tankSmallMesh2->CreateModel(tanksX2, 0, tanksZ2);
		}

	}

	// Add Dummies 
	IModel* dummyModel[20];
	int dummyX = -5;
	int dummyY = 5;
	int dummyZ = 0;

	for (int i = 0; i < 20; i++) {
		if (i == 0) {
			dummyModel[i] = dummyMesh->CreateModel(-3, dummyY, 0);
		}

		if (i == 1) {
			dummyModel[i] = dummyMesh->CreateModel(0, dummyY, 100);
		}

		if (i == 2) {
			dummyModel[i] = dummyMesh->CreateModel(+2, dummyY, 200);
		}

		if (i == 3) {
			dummyModel[i] = dummyMesh->CreateModel(0, dummyY, 280);
		}

		if (i == 4) {
			dummyModel[i] = dummyMesh->CreateModel(-3, dummyY, 285);
		}

		if (i == 5) {
			dummyModel[i] = dummyMesh->CreateModel(-6, dummyY, 290);
		}

		if (i == 6) {
			dummyModel[i] = dummyMesh->CreateModel(-110, dummyY, 310);
		}

		if (i == 7) {
			dummyModel[i] = dummyMesh->CreateModel(-210, dummyY, 310);
		}

		if (i == 8) {
			dummyModel[i] = dummyMesh->CreateModel(-310, dummyY, 310);
		}

		if (i == 9) {
			dummyModel[i] = dummyMesh->CreateModel(-410, dummyY, 310);
		}

		if (i == 10) {
			dummyModel[i] = dummyMesh->CreateModel(-505, dummyY, 280);
		}

		if (i == 11) {
			dummyModel[i] = dummyMesh->CreateModel(-500, dummyY, 180);
		}

		if (i == 12) {
			dummyModel[i] = dummyMesh->CreateModel(-500, dummyY, 80);
		}

		if (i == 13) {
			dummyModel[i] = dummyMesh->CreateModel(-500, dummyY, -80);
		}

		if (i == 14) {
			dummyModel[i] = dummyMesh->CreateModel(-500, dummyY, -180);
		}

		if (i == 15) {
			dummyModel[i] = dummyMesh->CreateModel(-450, dummyY, -280);
		}

		if (i == 16) {
			dummyModel[i] = dummyMesh->CreateModel(-400, dummyY, -330);
		}

		if (i == 17) {
			dummyModel[i] = dummyMesh->CreateModel(-300, dummyY, -320);
		}

		if (i == 18) {
			dummyModel[i] = dummyMesh->CreateModel(-200, dummyY, -320);
		}

		if (i == 19) {
			dummyModel[i] = dummyMesh->CreateModel(-80, dummyY, -320);
		}
	}

	// Initialize Car:
	float carModelRadiusX = 2.0;
	float carModelRadiusZ = 6.0;
	float carX = 3;
	float carY = 0;
	float carZ = -15;
	float carXopponent = 0;
	float carZopponent = 0;

	const int carNum = 2;
	CCar arrayOfCars[carNum];

	for (int i = 0; i < carNum; i++) {
		arrayOfCars[i].Initialize(carMesh, carX, carY, carZ);
		carX = arrayOfCars[0].getCarModel()->GetX();
		carY = arrayOfCars[0].getCarModel()->GetY();
		carZ = arrayOfCars[0].getCarModel()->GetZ();
		carX -= 6;
		carZ += 5;
		carY += 0;
	}

	carXopponent = arrayOfCars[1].getCarModel()->GetX();
	carZopponent = arrayOfCars[1].getCarModel()->GetZ();

	arrayOfCars[1].getCarModel()->SetSkin("RedGlow.jpg");

	// Car variables 
	int carSpeed = arrayOfCars[0].getCarSpeed();
	float boostSeconds = 0;
	bool booster = false;
	bool overHeated = false;
	int overHeatCounter = 0;
	float opponentCarSpeed = 0;
	float rotation = 0;
	float steeringAngleOfOC = 0;
	bool collisionWithCars = false;
	int damagePoints = 100;
	int dummyCounter = 0;

	int lap = 0;
	int lapOpponent = 0;
	int state = NORMAL_GAME;
	int movingState = FORWARD;
	int checkRotation = 0;

	//-----------------------------------------------END OF ARRAYS OF MODELS-------------------------------------------------------//

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();
		UpdateGame(myEngine, dt, gameState, state);

		/**** Update your scene each frame here ****/
		cameraState = UpdateCamera(myEngine, myCamera, rotateAngle, initialAngleCamera, dt, topView, cameraState,
			arrayOfCars, cameraX, cameraY, cameraZ, movement, movementLR);

		if (topView == false) {
			rotateCamera(myEngine, myCamera, angleCameraUD, angleCameraLR, mouseSensitivity, dt);
		}

		if (gameState == COUNTDOWN_GAME_STATE) {
			seconds += dt;
		}

		gameStates(gameState, myFont, seconds, myEngine, dt, arrayOfCars, carNum, carModelRadiusX, carModelRadiusZ, collisionBool,
			rotation, opponentCarSpeed, steeringAngleOfOC, collisionWithCars, dummyModel, dummyCounter, damagePoints, lap, lapOpponent, currentCheckpoint,
			currentCheckpointOpponent);

		// COLLISIONS

		// Check Left walls collision
		wallsCollision(arrayOfCars, wallModelLeft, AxMin, AxMax, AzMin, AzMax, wallsModelLRRadiusX, wallsModelLRRadiusZ, carNum, wallsLeftNum, carModelRadiusX,
			carModelRadiusZ, collisionSeconds, collisionBool, carX, carZ, carSpeed, damagePoints);
		// Check Right walls collision
		wallsCollision(arrayOfCars, wallModelRight, BxMin, BxMax, BzMin, BzMax, wallsModelLRRadiusX, wallsModelLRRadiusZ, carNum, wallsRightNum, carModelRadiusX,
			carModelRadiusZ, collisionSeconds, collisionBool, carX, carZ, carSpeed, damagePoints);
		// Check Up walls collision
		wallsCollision(arrayOfCars, wallModelUp, CxMin, CxMax, CzMin, CzMax, wallsModelUDRadiusX, wallsModelUDRadiusZ, carNum, wallsUpNum, carModelRadiusX,
			carModelRadiusZ, collisionSeconds, collisionBool, carX, carZ, carSpeed, damagePoints);
		// Check Down walls collision
		wallsCollision(arrayOfCars, wallModelDown, DxMin, DxMax, DzMin, DzMax, wallsModelUDRadiusX, wallsModelUDRadiusZ, carNum, wallsDownNum, carModelRadiusX,
			carModelRadiusZ, collisionSeconds, collisionBool, carX, carZ, carSpeed, damagePoints);

		// Check checkpoints collision
		checkpointsCollision(arrayOfCars, checkpointsModel, Ex, Ez, checkpointsModelRadius, carNum, checkPointsNum, carModelRadiusX,
			carModelRadiusZ, gameState, passed, checkpointsPassed, currentCheckpoint, lap, myEngine, state, cross, checkRotation);

		// Check checkpoints collision
		checkpointsCollisionOpponent(arrayOfCars, checkpointsModel, Sx, Sz, checkpointsModelRadius, carNum, checkPointsNum, carModelRadiusX,
			carModelRadiusZ, gameState, passedOpponent, checkpointsPassedOpponent, currentCheckpointOpponent, lapOpponent, myEngine, state);


		// Check isles collision
		objectCollision(arrayOfCars, isleModelRight, Fx, Fz, islesModelRadius, carNum, islesNum, carModelRadiusX, carModelRadiusZ,
			collisionBool, carX, carZ, collisionSeconds, carSpeed, damagePoints, movingState, myEngine);
		objectCollision(arrayOfCars, isleModelLeft, Gx, Gz, islesModelRadius, carNum, islesNum, carModelRadiusX, carModelRadiusZ,
			collisionBool, carX, carZ, collisionSeconds, carSpeed, damagePoints, movingState, myEngine);

		// Check tanks collision
		objectCollision(arrayOfCars, tanksModel, Hx, Hz, tanksModelRadius, carNum, tanksNum, carModelRadiusX, carModelRadiusZ,
			collisionBool, carX, carZ, collisionSeconds, carSpeed, damagePoints, movingState, myEngine);

		// Check orange tanks collision
		objectCollision(arrayOfCars, tanksModel2, Tx, Tz, tanksModelRadius2, carNum, tanksNum2, carModelRadiusX, carModelRadiusZ,
			collisionBool, carX, carZ, collisionSeconds, carSpeed, damagePoints, movingState, myEngine);

		// Lift hover car on move
		liftCarWhenMove(arrayOfCars, dt, carSpeed, opponentCarSpeed);

		//Boost hover car
		boostCar(arrayOfCars, myEngine, myFont, dt, carSpeed, boostSeconds, gameState, booster, overHeatCounter, overHeated);

		// Cars Collision
		carsCollision(arrayOfCars, HxMin, HxMax, IzMin, IzMax, carNum, carModelRadiusX, carModelRadiusZ, carX, carZ, carSpeed, opponentCarSpeed,
			collisionWithCars, JxMin, JxMax, KzMin, KzMax, carXopponent, carZopponent, damagePoints, movingState);

		// Enemy car collisions
		objectCollisionOpponent(arrayOfCars, isleModelLeft, Kx, Kz, islesModelRadius, carNum, islesNum, carModelRadiusX, carModelRadiusZ, collisionBool,
			carX, carZ, collisionSeconds, carSpeed, opponentCarSpeed);

		// Enemy car collisions
		objectCollisionOpponent(arrayOfCars, isleModelRight, Lx, Lz, islesModelRadius, carNum, islesNum, carModelRadiusX, carModelRadiusZ, collisionBool,
			carX, carZ, collisionSeconds, carSpeed, opponentCarSpeed);

		// Check Left walls collision
		wallsCollisionOpponent(arrayOfCars, wallModelLeft, MxMin, MxMax, MzMin, MzMax, wallsModelLRRadiusX, wallsModelLRRadiusZ, carNum, wallsLeftNum, carModelRadiusX,
			carModelRadiusZ, collisionSeconds, collisionBool, carX, carZ, carSpeed, opponentCarSpeed);
		// Check Right walls collision
		wallsCollisionOpponent(arrayOfCars, wallModelRight, NxMin, NxMax, NzMin, NzMax, wallsModelLRRadiusX, wallsModelLRRadiusZ, carNum, wallsRightNum, carModelRadiusX,
			carModelRadiusZ, collisionSeconds, collisionBool, carX, carZ, carSpeed, opponentCarSpeed);
		// Check Up walls collision
		wallsCollisionOpponent(arrayOfCars, wallModelUp, OxMin, OxMax, OzMin, OzMax, wallsModelUDRadiusX, wallsModelUDRadiusZ, carNum, wallsUpNum, carModelRadiusX,
			carModelRadiusZ, collisionSeconds, collisionBool, carX, carZ, carSpeed, opponentCarSpeed);
		// Check Down walls collision
		wallsCollisionOpponent(arrayOfCars, wallModelDown, PxMin, PxMax, PzMin, PzMax, wallsModelUDRadiusX, wallsModelUDRadiusZ, carNum, wallsDownNum, carModelRadiusX,
			carModelRadiusZ, collisionSeconds, collisionBool, carX, carZ, carSpeed, opponentCarSpeed);

		//Get car position
		movingState = getCarPosition(arrayOfCars, carZ, carX, movingState);

		// Restart game
		restartGame(myEngine, arrayOfCars, carX, carY, carZ, gameState, cameraState, seconds, collisionSeconds, collisionBool, distance, passed, passedOpponent, checkpointsPassed,
			checkpointsPassedOpponent, boostSeconds, booster, overHeated, overHeatCounter, opponentCarSpeed, rotation, steeringAngleOfOC, collisionWithCars, damagePoints, dummyCounter,
			lap, lapOpponent, movingState, currentCheckpoint, currentCheckpointOpponent, checkRotation, cross);

	}
	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
