/*
 * PhysicsDemo.h
 *
 *  Created on: Mar 22, 2021
 *      Author: leandro
 */

#ifndef SRC_OBJDEMO_H_
#define SRC_OBJDEMO_H_

#include <iostream>
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGLRunner.h>
#include<renderers/GridRenderer.h>

#include "base/BaseDemo.h"

class ObjDemoRunner: public BaseDemoRunner {
	LightResource light;
	MaterialResource material;

	GridRenderer gridRenderer;

	VertexArrayResource *obj;
	TextureResource *texture;

	real rotacion = 0.0f;

	vector posicion;

public:
	ObjDemoRunner() : material(vector(0.2f, 0.2f, 0.2f), vector(0.5f, 0.5f, 0.5f), vector(0.5f, 0.5f, 0.5f), 32.0f),
            light(vector(0.0f, 0.0f, 1.0f), vector(0.3f, 0.3f, 0.3f), vector(0.5f, 0.5f, 0.5f), vector(1.0f, 1.0f, 1.0f), 1.0f){
	    reset();
	}

	void reset() {
	    posicion = vector(0, 0, -5);
	    light.setPosition(vector(0, 0, -5));
        camera.setPosition(vector(0.0f, 0.0f, 5.0f));
	}

	bool init() override {
		BaseDemoRunner::init();

		gridRenderer.setVideoRunner(video);
	    defaultRenderer.setLight(&light);

//	    texture = (TextureResource *)this->getContainer()->getResourceManager()->load("images/fern.png", MimeTypes::TEXTURE);
//        obj = (VertexArrayResource *)this->getContainer()->getResourceManager()->load("geometry/fern.obj", MimeTypes::VERTEXARRAY);
        texture = (TextureResource *)this->getContainer()->getResourceManager()->load("images/lowPolyTree.png", MimeTypes::TEXTURE);
        obj = (VertexArrayResource *)this->getContainer()->getResourceManager()->load("geometry/lowPolyTree.obj/lowPolyTree.obj", MimeTypes::VERTEXARRAY);

//	    texture = (TextureResource *)this->getContainer()->getResourceManager()->load("images/lowPolyTree.png", MimeTypes::TEXTURE);
        //obj = (VertexArrayResource *)this->getContainer()->getResourceManager()->load("geometry/bunny.obj", MimeTypes::VERTEXARRAY);
//        obj = (VertexArrayResource *)this->getContainer()->getResourceManager()->load("geometry/untitled.obj", MimeTypes::VERTEXARRAY);


		reset();

		return true;
	}

	LoopResult doLoop() override {
	    gridRenderer.render(camera);

		defaultRenderer.clear();
        defaultRenderer.drawAxes(matriz_4x4::identidad, 1.0f);
        defaultRenderer.setTexture(texture);
		defaultRenderer.drawSphere(matriz_4x4::traslacion(posicion), 0.1f);
		defaultRenderer.drawObject(matriz_4x4::rotacion(0.0, radian(rotacion), 0.0), obj);
		defaultRenderer.render(camera);

		rotacion+=0.1;

		return LoopResult::CONTINUE;
	}

	void onMouseWheel(int wheel) override {
        camera.setPosition(camera.getPosition() - vector(0.0f, 0.0f, wheel));
        posicion = posicion + vector(0.0f, 0.0f, wheel);

        light.setPosition(posicion);
        logger->info("viewMatrix:\n%s\nlight:%s\nposition:%s\n",
                camera.getViewMatrix().toString("%.2f").c_str(),
                light.getPosition().toString("%.2f").c_str(),
                posicion.toString("%.2f").c_str());
	}

	virtual void onMouseMove(int x, int y, int dx, int dy, unsigned int buttons) override {
        camera.setPosition(camera.getPosition() - vector(0.1f * dx, 0.1f * dy, 0));
        light.setPosition(camera.getPosition());
        posicion = posicion + vector(0.1f * dx, 0.1f * dy, 0);

        logger->info("viewMatrix:\n%s\nlight:%s\nposition:%s\n",
                camera.getViewMatrix().toString("%.2f").c_str(),
                light.getPosition().toString("%.2f").c_str(),
                posicion.toString("%.2f").c_str());
	}

	void onMouseButtonDown(unsigned char button, int x, int y) override {
	}

    virtual void onKeyDown(unsigned int key, unsigned int keyModifier) override {
        switch (key) {
            case SDLK_SPACE:
                reset();
                break;
        }
    }
};

class ObjDemoPlayground: public Playground {
public:
    ObjDemoPlayground(const String &resourcesBasePath) :
			Playground(resourcesBasePath) {
	}
	void init() override {
		Playground::init();
		this->addRunner(new OpenGLRunner());
		this->addRunner(new AudioRunner());
		this->addRunner(new ObjDemoRunner());
	}
};



#endif /* SRC_OBJDEMO_H_ */
