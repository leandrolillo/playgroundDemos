/*
 * PhysicsDemo.h
 *
 *  Created on: Mar 22, 2021
 *      Author: leandro
 */

#include "../../base/BaseDemo.h"
#include <OpenALRunner.h>
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

  AudioSource *backgroundMusic = null;

public:
  using BaseDemoRunner::BaseDemoRunner; //inherit constructors

  bool initialize() override {
    if (!BaseDemoRunner::initialize()) {
      return false;
    }
    defaultRenderer.disableBlending();

    //video.enable(VideoAttribute::CULL_FACE, VideoAttribute::NONE);

    /*
     * There are the following scenarios:
     * obj -> mesh
     * obj -> vertex array
     * json -> mesh
     * json -> vertex array
     */

//        if((axesVertexArray = (VertexArrayResource *)this->getResourceManager().load("roadFighter/axes.obj", MimeTypes::VERTEXARRAY)) == null) {
//        	logger->error("Could not load axes model");
//        	return false;
//        }
//
//        if((carVertexArray = (VertexArrayResource *)this->getResourceManager().load("roadFighter/corvette.obj", MimeTypes::VERTEXARRAY)) == null) {
//        	logger->error("Could not load car model");
//        	return false;
//        }
//    	MeshCollection *meshCollection;
//
//        if((meshCollection = (MeshResource *)this->getResourceManager().load("roadFighter/axes.obj",
//        		MimeTypes::MESHCOLLECTION,
//				std::set<String> {},
//				std::map<String, String> {{"texture-filter", "nearest"}})) == null) {
//        	logger->error("Could not load mesh collection");
//        	return false;
//        }
    if ((carMesh = (MeshResource*) this->getResourceManager().load("corvette.obj/chevrolet_corvete", MimeTypes::MESH,
        std::set<String> { }, std::map<String, String> { { "texture-filter", "nearest" } })) == null) {
      logger->error("Could not load car mesh");
      return false;
    } else {
      logger->info("Loaded car Mesh");
    }

    if ((texturedBoxMesh = (MeshResource*) this->getResourceManager().load("texturedCube.obj/Cube", MimeTypes::MESH,
        std::set<String> { }, std::map<String, String> { { "texture-filter", "nearest" } })) == null) {
      logger->error("Could not load textured box mesh");
      return false;
    } else {
      logger->info("Loaded car Box Mesh");
    }

    if ((axesMesh = (MeshResource*) this->getResourceManager().load("axes.obj/Axes", MimeTypes::MESH,
        std::set<String> { }, std::map<String, String> { { "texture-filter", "nearest" } })) == null) {
      logger->error("Could not load axes model");
      return false;
    } else {
      logger->info("Loaded Axes");
    }

    backgroundMusic = this->audio.createSource("background.ogg", vector(0, 0, 0), vector(0, 0, 0), true);
    audio.playSource(*backgroundMusic);

    return true;
  }

  LoopResult doLoop() override {
//        defaultRenderer.drawAxes(matriz_4x4::identidad);

    defaultRenderer.drawObject(matrix_4x4::identidad, carVertexArray);
    defaultRenderer.drawObject(matrix_4x4::identidad, axesVertexArray);
    defaultRenderer.drawObject(matrix_4x4::identidad, axesMesh);
    defaultRenderer.drawObject(matrix_4x4::traslacion(0, 4, -3), texturedBoxMesh);
    defaultRenderer.drawObject(matrix_4x4::traslacion(0, 0, -3), carMesh);

    return LoopResult::CONTINUE;
  }

  void onMouseMove(int x, int y, int dx, int dy, unsigned int buttons)
      override {
    const bool *state = SDL_GetKeyboardState(NULL);

    if ((buttons & SDL_BUTTON_MMASK) && state[SDL_SCANCODE_LSHIFT]) {
      camera.setPosition(camera.getPosition() + vector(-dx, dy, 0.0) * 0.1f);
    } else if ((buttons & SDL_BUTTON_MMASK) && state[SDL_SCANCODE_LCTRL]) {
      camera.setPosition(camera.getPosition() + vector(0, 0, dy) * 0.1f);
    }

  }
  void onMouseButtonDown(unsigned char button, int x, int y) override {
  }
};

class RoadFighter: public Playground {
public:
  using Playground::Playground;

  void initializePlayground() override {
    Playground::initializePlayground();
    this->addRunner<OpenGLRunner>();
    this->addRunner<OpenALRunner>();
    this->addRunner<RoadFighterRunner>();
  }
};

int main(int argc, char **argv) {
  String repository = Paths::add(Paths::getDirname(argv[0]), "resources");
  RoadFighter playground(repository);
  playground.withName("Road Fighter");
  printf("\n\nRunning [%s]\n", playground.toString().c_str());
  playground.run();
  printf("done\n");
  return 0;
}
