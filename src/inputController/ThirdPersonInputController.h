/*
 * ThirdPersonInputController.h
 *
 *  Created on: Jul 22, 2021
 *      Author: leandro
 */

#ifndef SRC_DEMO_INPUTCONTROLLER_THIRDPERSONINPUTCONTROLLER_H_
#define SRC_DEMO_INPUTCONTROLLER_THIRDPERSONINPUTCONTROLLER_H_

#include "FPSInputController.h"

class ThirdPersonInputController : public FPSInputController {
    real distance = (real)4;
public:
    ThirdPersonInputController(Camera &cameraReference, matriz &playerTransformReference) : FPSInputController(cameraReference, playerTransformReference) {
        //reset();
    }

    virtual void reset() override {
        FPSInputController::reset();
        this->distance = (real)4;
    }

    real getDistance() const {
        return distance;
    }

    void setDistance(real distance) {
        this->distance = distance;
        refreshOrientation();
    }

    void mouseWheel(int wheel) override {
        setDistance(std::max((real)3, std::min((real)20, this->distance + (real)wheel)));
    }


protected:
    virtual void refreshPosition() override {
        vector cameradelta = camera.getOrientation() * vector(0, 0, distance);
        vector cameraPosition = position + vector(0, 1, 0) + cameradelta;
        camera.setPosition(this->constrainPosition(cameraPosition));

        playerTransform = matriz_4x4::traslacion(position);
    }
    void refreshOrientation() override {
        camera.setOrientation(matriz_3x3::matrizRotacion(radian(getPitch()), vector(1, 0, 0)) * matriz_3x3::matrizRotacion(radian(getYaw()), vector(0, 1, 0)));
    }
};



#endif /* SRC_DEMO_INPUTCONTROLLER_THIRDPERSONINPUTCONTROLLER_H_ */
