/*
 * PlaygroundGeneralDemo.h
 *
 *  Created on: Mar 19, 2021
 *      Author: leandro
 */

#ifndef SRC_GENERALDEMO_H_
#define SRC_GENERALDEMO_H_

#include<Playground.h>
#include<OpenGLRunner.h>
#include<AudioRunner.h>

#include<vector>

#include "../base/BaseDemo.h"

class GeneralDemoRunner: public BaseDemoRunner {
private:
  //Graphical stuff
  vector viewPosition;
  vector lightPosition;
  vector *currentPosition;
  real rotation = 0;

  Source *lightAnnoyingSoundSource = null;

  TextureResource *pngTexture = null;
  TextureResource *pngTexture2 = null;
  TextureResource *jpgTexture = null;

  VertexArrayResource *triangleVertexArray = null;
  VertexArrayResource *sphereVertexArray = null;

  GeometryResource *sphereGeometry = null;
  GeometryResource *triangleGeometry = null;

  MaterialResource material;
  LightResource light;

  DefaultRenderer toonRenderer;
public:
  GeneralDemoRunner() :
      material(vector(1.0f, 0.5f, 0.31f), vector(1.0f, 0.5f, 0.31f),
          vector(0.5f, 0.5f, 0.5f), 32.0f), light(lightPosition,
          vector(0.2f, 0.2f, 0.2f), vector(0.5f, 0.5f, 0.5f),
          vector(1.0f, 1.0f, 1.0f), 1.0f) {

    currentPosition = &viewPosition;
  }

  void reset() {
    viewPosition = vector(0.0, 0.0f, 6.0);
    lightPosition = vector(0.0, 0.0, 0.0);
    camera.setPosition(viewPosition);
  }

  virtual bool init() override {
    BaseDemoRunner::init();

    // demo stuff
    lightAnnoyingSoundSource = audio->createSource("audio/voltage.wav");
    audio->playSource(lightAnnoyingSoundSource);

    pngTexture = (TextureResource*) getResourceManager()->load(
        "images/TEXTURA.PNG", MimeTypes::TEXTURE);
    pngTexture2 = (TextureResource*) getResourceManager()->load(
        "images/CEDFENCE.PNG", MimeTypes::TEXTURE);
    jpgTexture = (TextureResource*) getResourceManager()->load("images/irs.JPG",
        MimeTypes::TEXTURE);

    sphereVertexArray = (VertexArrayResource*) getResourceManager()->load(
        "geometry/sphere.json", MimeTypes::VERTEXARRAY);
    triangleVertexArray = (VertexArrayResource*) getResourceManager()->load(
        "geometry/triangle.json", MimeTypes::VERTEXARRAY);

    defaultRenderer.setLight(&light);
//        defaultRenderer.setMaterial(&material);
//        defaultRenderer.setTexture(pngTexture);

    toonRenderer.setShaderProgram(
        (ShaderProgramResource*) getResourceManager()->load(
            "shaders/toon.330.program.json", MimeTypes::SHADERPROGRAM));
    toonRenderer.setVideoRunner(video);
    toonRenderer.setLight(&light);
    toonRenderer.setMaterial(&material);
    toonRenderer.setTexture(pngTexture);

    video->enable(DEPTH_TEST, true);
    video->enable(CULL_FACE, CULL_FACE_BACK);

    reset();
    return true;
  }

  virtual LoopResult doLoop() override {

    /**
     * Update audio
     */
    if(lightAnnoyingSoundSource != null) {
      lightAnnoyingSoundSource->setPosition(
          vector3(lightPosition.x, lightPosition.y, -lightPosition.z));
      audio->updateSource(lightAnnoyingSoundSource);
      audio->updateListener(viewPosition);
    }

    /**
     * modulate light
     */
//        vector color(0.25 + 0.75 * sin(radian(rotation)), 0.25 + 0.75 * cos(radian(rotation)), 0.25 + 0.75 * sin(radian(rotation)));
//        light.setDiffuse(color);
//        light.setSpecular(color);
//        light.setAmbient(color);
    light.setPosition(lightPosition);
    camera.setPosition(viewPosition);

    /**
     * Render toon objects
     */
    toonRenderer.clear();
    toonRenderer.drawObject(
        matriz_4x4::base(
            matriz_3x3::matrizRotacion(0.0f, radian(rotation), 0.0f),
            vector3(2.0, 1.0, 0.0)), triangleVertexArray);
    toonRenderer.drawObject(
        matriz_4x4::base(
            matriz_3x3::matrizRotacion(0.0f, radian(rotation), 0.0f),
            vector3(-2.0, 1.0, 0.0)), sphereVertexArray);

    toonRenderer.render(camera);

    /**
     * Render default renderer objects
     */
    defaultRenderer.clear();
    defaultRenderer.setMaterial(&material);

    //draw axis for viewer reference
    defaultRenderer.drawAxes(matriz_4x4::identidad);

    //draw Light as a sphere
    defaultRenderer.drawSphere(matriz_4x4::traslacion(lightPosition), 0.1f);

    defaultRenderer.setTexture(pngTexture);
    defaultRenderer.drawObject(
        matriz_4x4::base(
            matriz_3x3::matrizRotacion(0.0f, radian(rotation), 0.0f),
            vector3(2.0, -1.0, 0.0)), triangleVertexArray);
    defaultRenderer.drawSphere(
        matriz_4x4::base(
            matriz_3x3::matrizRotacion(0.0f, radian(rotation), 0.0f),
            vector3(4.0, -1.0, 0.0)));
    defaultRenderer.setTexture(pngTexture2);
    defaultRenderer.drawBox(
        matriz_4x4::base(
            matriz_3x3::matrizRotacion(0.0f, radian(rotation), 0.0f),
            vector3(-2.0, -1.0, 0.0)));
    defaultRenderer.render(camera);

    rotation += 1;

    return LoopResult::CONTINUE;
  }
  void onMouseWheel(int wheel) override {
    *currentPosition += vector(0.0f, 0.0f, wheel);
    logger->verbose("%s", (*currentPosition).toString().c_str());
  }

  virtual void onMouseMove(int x, int y, int dx, int dy, unsigned int buttons)
      override {
    *currentPosition += vector(0.1f * dx, 0.1f * dy, 0);
    logger->verbose("%s", (*currentPosition).toString().c_str());
  }
  virtual void onKeyUp(unsigned int key, unsigned int keyModifier) override {
    switch (key) {
    case SDLK_RCTRL:
    case SDLK_LCTRL:
      currentPosition = &viewPosition;
      break;
    }
  }
  virtual void onKeyDown(unsigned int key, unsigned int keyModifier) override {
    switch (key) {
    case SDLK_RCTRL:
    case SDLK_LCTRL:
      currentPosition = &lightPosition;
      break;
    case SDLK_SPACE:
      reset();
      break;
    }
  }
};

class PlaygroundGeneralDemo: public Playground {
public:
  PlaygroundGeneralDemo(const String &resourcesBasePath) :
      Playground(resourcesBasePath) {
  }
  void init() override {
    Playground::init();
    this->addRunner(new GeneralDemoRunner());
    this->addRunner(new OpenGLRunner());
    this->addRunner(new AudioRunner());

  }
};

#endif /* SRC_GENERALDEMO_H_ */
