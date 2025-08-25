/*
 * PhysicsDemo.h
 *
 *  Created on: Mar 22, 2021
 *      Author: leandro
 */

#ifndef SRC_BATTLEROAD_H_
#define SRC_BATTLEROAD_H_

#include "../base/BaseDemo.h"
#include <MeshResource.h>

class RoadFighterRunner: public BaseDemoRunner {
//    ParticleManager particleManager;
//    const CollisionTester &intersectionTester = *(particleManager.getCollisionDetector().getIntersectionTester());

	MeshResource *carMesh = null;
	MeshResource *texturedBoxMesh = null;
	MeshResource *axesMesh = null;
	VertexArrayResource *carVertexArray = null;
	VertexArrayResource *axesVertexArray = null;
//	TextureResource *texture = null;

	Source *backgroundMusic = null;

public:
    bool init() override {
    	if(!BaseDemoRunner::init()) {
    		return false;
    	}

    	//video->enable(CULL_FACE, GL_NONE);

    	/*
    	 * There are the following scenarios:
    	 * obj -> mesh
    	 * obj -> vertex array
    	 * json -> mesh
    	 * json -> vertex array
    	 */

//        if((axesVertexArray = (VertexArrayResource *)this->getResourceManager()->load("roadFighter/axes.obj", MimeTypes::VERTEXARRAY)) == null) {
//        	logger->error("Could not load axes model");
//        	return false;
//        }
//
//        if((carVertexArray = (VertexArrayResource *)this->getResourceManager()->load("roadFighter/corvette.obj", MimeTypes::VERTEXARRAY)) == null) {
//        	logger->error("Could not load car model");
//        	return false;
//        }

//    	MeshCollection *meshCollection;
//
//        if((meshCollection = (MeshResource *)this->getResourceManager()->load("roadFighter/axes.obj",
//        		MimeTypes::MESHCOLLECTION,
//				std::set<String> {},
//				std::map<String, String> {{"texture-filter", "nearest"}})) == null) {
//        	logger->error("Could not load mesh collection");
//        	return false;
//        }

        if((carMesh = (MeshResource *)this->getResourceManager()->load("roadFighter/corvette.obj/chevrolet_corvete",
        		MimeTypes::MESH,
				std::set<String> {},
				std::map<String, String> {{"texture-filter", "nearest"}})) == null) {
        	logger->error("Could not load car mesh");
        	return false;
        }

        if((texturedBoxMesh = (MeshResource *)this->getResourceManager()->load("roadFighter/texturedCube.obj/Cube",
        		MimeTypes::MESH,
				std::set<String> {},
				std::map<String, String> {{"texture-filter", "nearest"}})) == null) {
        	logger->error("Could not load textured box mesh");
        	return false;
        }

        if((axesMesh = (MeshResource *)this->getResourceManager()->load("roadFighter/axes.obj/Axes", MimeTypes::MESH,
        		std::set<String> {},
				std::map<String, String> {{"texture-filter", "nearest"}})) == null) {
        	logger->error("Could not load axes model");
        	return false;
        }

        backgroundMusic = this->audio->createSource("roadFighter/background.ogg", vector(0, 0, 0), vector(0, 0, 0), true);
        audio->playSource(backgroundMusic);

        return true;
    }


    LoopResult doLoop() override {
        defaultRenderer.clear();
//        defaultRenderer.drawAxes(matriz_4x4::identidad);

        defaultRenderer.drawObject(matrix_4x4::identidad, carVertexArray);
        defaultRenderer.drawObject(matrix_4x4::identidad, axesVertexArray);
        defaultRenderer.drawObject(matrix_4x4::identidad, axesMesh);
        defaultRenderer.drawObject(matrix_4x4::traslacion(0, 4, -3), texturedBoxMesh);
        defaultRenderer.drawObject(matrix_4x4::traslacion(0, 0, -3), carMesh);


        defaultRenderer.render(camera);

        return LoopResult::CONTINUE;
    }

    void onMouseMove(int x, int y, int dx, int dy, unsigned int buttons) override {
    	const bool *state = SDL_GetKeyboardState(NULL);

    	if((buttons & SDL_BUTTON_MMASK) && state[SDL_SCANCODE_LSHIFT]) {
    		camera.setPosition(camera.getPosition() + vector(-dx, dy, 0.0) * 0.1f);
    	}
    	else if((buttons & SDL_BUTTON_MMASK) && state[SDL_SCANCODE_LCTRL]) {
    		camera.setPosition(camera.getPosition() + vector(0, 0, dy) * 0.1f);
    	}

    }
//
//    void onMouseButtonUp(unsigned char button, int x, int y) override {
//    }
//
//	void onMouseWheel(int wheel) override {
//		logger->info("Mouse wheel %d", wheel);
//	    camera.setPosition(camera.getPosition() - vector(0.0, 0.0, std::min(1.0, 0.1 * wheel)));
//	    logger->info("Camera position: %s", camera.getPosition().toString().c_str());
//	}
//
//
    void onMouseButtonDown(unsigned char button, int x, int y) override {
    }
//
//    void onKeyDown(unsigned int key, unsigned int keyModifier) override {
//        switch (key) {
//			case '+':
//				camera.setPosition(camera.getPosition() - vector(0.0, 0.0, 0.1));
//			break;
//			case '-':
//				camera.setPosition(camera.getPosition() + vector(0.0, 0.0, 0.1));
//			break;
//			case SDLK_LEFT:
//				camera.setPosition(camera.getPosition() - vector(0.1, 0.0, 0.0));
//			break;
//			case SDLK_RIGHT:
//				camera.setPosition(camera.getPosition() + vector(0.1, 0.0, 0.0));
//			break;
//        	case SDLK_UP:
//        		camera.setPosition(camera.getPosition() + vector(0.0, 0.1, 0.0));
//        	break;
//        	case SDLK_DOWN:
//        		camera.setPosition(camera.getPosition() - vector(0.0, 0.1, 0.0));
//        		break;
//            case SDLK_BACKSPACE:
//                reset();
//                break;
//            case SDLK_SPACE:
//                break;
//        }
//    }
};

class RoadFighter: public Playground {
public:
	RoadFighter(const String &resourcesBasePath) : Playground(resourcesBasePath) {
    }
    void init() override {
        Playground::init();
        this->addRunner(new OpenGLRunner());
        this->addRunner(new AudioRunner());
        this->addRunner(new RoadFighterRunner());
    }
};

#endif
