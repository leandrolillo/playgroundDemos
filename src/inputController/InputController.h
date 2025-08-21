/*
 * InputController.h
 *
 *  Created on: Jul 22, 2021
 *      Author: leandro
 */

#ifndef SRC_DEMO_INPUTCONTROLLER_INPUTCONTROLLER_H_
#define SRC_DEMO_INPUTCONTROLLER_INPUTCONTROLLER_H_

#include<Math3d.h>

class InputController {
protected:
    Camera &camera;
    matriz &playerTransform;

public:
    InputController(Camera &cameraReference, matriz &playerTransformReference) : camera(cameraReference), playerTransform(playerTransformReference) {
    }
    virtual ~InputController() {}
    virtual void update(real dt) {}
    virtual void reset() {}
    virtual void mouseButtonDown(unsigned char button, int x, int y) {}
    virtual void mouseButtonUp(unsigned char button, int x, int y) {}
    virtual void mouseMove(int x, int y, int dx, int dy) { }
    virtual void mouseWheel(int wheel) {}
    virtual void keyDown(unsigned int key, unsigned int keyModifier) {}
    virtual void keyUp(unsigned int key, unsigned int keyModifier) {}
};



#endif /* SRC_DEMO_INPUTCONTROLLER_INPUTCONTROLLER_H_ */
