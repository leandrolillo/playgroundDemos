/*
 * PhysicsDemo.h
 *
 *  Created on: Mar 22, 2021
 *      Author: leandro
 */
#include <iostream>
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGLRunner.h>
#include <OpenALRunner.h>
#include<renderers/GridRenderer.h>

#include "../../base/BaseDemo.h"

class ObjDemoRunner: public BaseDemoRunner {
  LightResource light {vector(0.0f, 0.0f, 1.0f), vector(0.3f, 0.3f, 0.3f), vector(0.5f, 0.5f, 0.5f), vector(1.0f, 1.0f, 1.0f), 1.0f};
  MaterialResource material {vector(0.2f, 0.2f, 0.2f), vector(0.5f, 0.5f, 0.5f), vector(0.5f, 0.5f, 0.5f), 32.0f};

  GridRenderer gridRenderer { video };

  VertexArrayResource *obj;
  TextureResource *texture;

  real rotacion = 0.0f;

  vector posicion;

public:
  ObjDemoRunner(Playground &container) : BaseDemoRunner(container) {
    defaultRenderer.setLight(&light);

    texture = (TextureResource*) this->getContainer().getResourceManager().load("images/lowPolyTree.png", MimeTypes::TEXTURE);
    obj = (VertexArrayResource*) this->getContainer().getResourceManager().load("geometry/lowPolyTree.obj/lowPolyTree.obj",
        MimeTypes::VERTEXARRAY);

    reset();
  }

  void reset() {
    posicion = vector(0, 0, -5);
    light.setPosition(vector(0, 0, -5));
    camera.setPosition(vector(0.0f, 0.0f, 5.0f));
  }

  LoopResult doLoop() override {
    gridRenderer.render(camera);

    defaultRenderer.clear();
    defaultRenderer.drawAxes(matriz_4x4::identidad, 1.0f);
    defaultRenderer.setTexture(texture);
    defaultRenderer.drawSphere(matriz_4x4::traslacion(posicion), 0.1f);
    defaultRenderer.drawObject(matriz_4x4::rotacion(0.0, radian(rotacion), 0.0), obj);
    defaultRenderer.render(camera);

    rotacion += 0.1;

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
    default:
      BaseDemoRunner::onKeyDown(key, keyModifier);
      break;

    }
  }
};

class ObjDemoPlayground: public Playground {
public:
  ObjDemoPlayground(const String &resourcesBasePath) :
      Playground(resourcesBasePath) {
    this->addRunner<OpenGLRunner>();
    this->addRunner<OpenALRunner>();
    this->addRunner<ObjDemoRunner>();
  }
};

int main(int argc, char **argv) {
  String repository = Paths::add(Paths::getDirname(argv[0]), "resources"); //assumes executable lies in playground/target folder
  ObjDemoPlayground playground(repository);
  playground.withName("ObjDemoPlayground");
  printf("\n\nRunning [%s]\n", playground.toString().c_str());
  playground.run();
  printf("done\n");
  return 0;
}
