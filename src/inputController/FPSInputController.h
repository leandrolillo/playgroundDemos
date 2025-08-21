/*
 * FPSInputController.h
 *
 *  Created on: Jul 22, 2021
 *      Author: leandro
 */

#ifndef SRC_DEMO_INPUTCONTROLLER_FPSINPUTCONTROLLER_H_
#define SRC_DEMO_INPUTCONTROLLER_FPSINPUTCONTROLLER_H_

#include "InputController.h"
#include <Camera.h>

class FPSInputController : public InputController {
protected:
    real pitch;
    real yaw;
    vector position;
    vector velocity;

public:
    FPSInputController(Camera &cameraReference, matriz &playerTransformReference) : InputController(cameraReference, playerTransformReference) {
        pitch = 0;
        yaw = 0;
        reset();
    }

    void setPitch(real pitch) {
        this->pitch = pitch;
        refreshOrientation();
    }

    real getPitch() const {
        return this->pitch;
    }

    void setYaw(real yaw) {
        this->yaw = yaw;
        refreshOrientation();
    }

    real getYaw() const {
        return this->yaw;
    }

    void setPosition(const vector &viewPosition) {
        this->position = viewPosition;
        this->refreshPosition();
    }

    void setVelocity(const vector &viewVelocity) {
        this->velocity = viewVelocity;
    }

    virtual void reset() override {
        this->position = vector(0, 0, 0);
        this->velocity = vector(0, 0, 0);
        this->setPitch(0);
        this->setYaw(0);
    }

    void mouseMove(int x, int y, int dx, int dy) override {
        this->setPitch(this->getPitch() + dy);
        this->setYaw(this->getYaw() + dx);
    }

    void keyUp(unsigned int key, unsigned int keyModifier) override {
        switch (key) {
            case 'w':
            case 'W':
            case 's':
            case 'S':
                velocity.z = 0;
                break;
            case 'a':
            case 'A':
            case 'd':
            case 'D':
                velocity.x = 0;
                break;
        }
    }

    void keyDown(unsigned int key, unsigned int keyModifier) override {
        switch (key) {
            case 'w':
            case 'W':
                velocity.z = -3;
                break;
            case 's':
            case 'S':
                velocity.z = 3;
                break;
            case 'a':
            case 'A':
                velocity.x = -3;
                break;
            case 'd':
            case 'D':
                velocity.x = 3;
                break;
        }
    }

    virtual void update(real dt) override {
        this->setPosition(this->constrainPosition(this->position + camera.getOrientation() * velocity * dt + vector(0.0, -1.0, 0.0)));
    }


protected:
    virtual vector constrainPosition(const vector &position) {
        return position;
    }

    virtual void refreshPosition() {
        camera.setPosition(this->position + vector(0.0, 1.0, 0.0));
        playerTransform = matriz_4x4::traslacion(this->position) * (matriz_4x4)camera.getOrientation();

    }
    virtual void refreshOrientation() {
        camera.setOrientation(matriz_3x3::matrizRotacion(radian(pitch), vector(1, 0, 0)) *
                matriz_3x3::matrizRotacion(radian(yaw), vector(0, 1, 0)));
    }


};


#endif /* SRC_DEMO_INPUTCONTROLLER_FPSINPUTCONTROLLER_H_ */
