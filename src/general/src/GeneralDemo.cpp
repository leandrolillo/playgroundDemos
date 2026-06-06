/*
 * PlaygroundGeneralDemo.h
 *
 *  Created on: Mar 19, 2021
 *      Author: leandro
 */
#include<Playground.h>
#include<OpenGLRunner.h>
#include<OpenALRunner.h>

#include<vector>

#include "../../base/BaseDemo.h"

class GeneralDemoRunner: public BaseDemoRunner {
private:
  //Graphical stuff
  vector viewPosition;
  vector lightPosition;
  vector *currentPosition = &viewPosition;
  real rotation = 0;

  std::unique_ptr<AudioSource> lightAnnoyingSoundSource;

  TextureResource *pngTexture = null;
  TextureResource *pngTexture2 = null;
  TextureResource *jpgTexture = null;

  VertexArrayResource *triangleVertexArray = null;
  VertexArrayResource *sphereVertexArray = null;

  GeometryResource *sphereGeometry = null;
  GeometryResource *triangleGeometry = null;

  MaterialResource material {vector(1.0f, 0.5f, 0.31f), vector(1.0f, 0.5f, 0.31f), vector(0.5f, 0.5f, 0.5f), 32.0f};
  LightResource light {lightPosition, vector(0.2f, 0.2f, 0.2f), vector(0.5f, 0.5f, 0.5f), vector(1.0f, 1.0f, 1.0f), 1.0f};

  DefaultRenderer toonRenderer { video };
  public:

  using BaseDemoRunner::BaseDemoRunner; //inherit constructors

  void reset() {
    viewPosition = vector(0.0, 0.0f, 6.0);
    lightPosition = vector(0.0, 0.0, 0.0);
    camera.setPosition(viewPosition);
  }

  virtual bool initialize() override {
    BaseDemoRunner::initialize();

    // demo stuff
    lightAnnoyingSoundSource = audio.createSource("audio/voltage.wav");
    lightAnnoyingSoundSource->play();

    pngTexture = (TextureResource*) getResourceManager().load("images/TEXTURA.PNG", MimeTypes::TEXTURE);
    pngTexture2 = (TextureResource*) getResourceManager().load("images/CEDFENCE.PNG", MimeTypes::TEXTURE);
    jpgTexture = (TextureResource*) getResourceManager().load("images/irs.JPG", MimeTypes::TEXTURE);

    sphereVertexArray = (VertexArrayResource*) getResourceManager().load("geometry/sphere.json", MimeTypes::VERTEXARRAY);
    triangleVertexArray = (VertexArrayResource*) getResourceManager().load("geometry/triangle.json", MimeTypes::VERTEXARRAY);

    defaultRenderer.setLight(&light);
//        defaultRenderer.setMaterial(&material);
//        defaultRenderer.setTexture(pngTexture);

    toonRenderer.setShaderProgram((ShaderProgramResource*) getResourceManager().load("shaders/toon.330.program.json", MimeTypes::SHADERPROGRAM));
    toonRenderer.setLight(&light);
    toonRenderer.setMaterial(&material);
    toonRenderer.setTexture(pngTexture);

    reset();
    return true;
  }

  virtual LoopResult doLoop() override {
    /**
     * modulate light
     */
//        vector color(0.25 + 0.75 * sin(radian(rotation)), 0.25 + 0.75 * cos(radian(rotation)), 0.25 + 0.75 * sin(radian(rotation)));
//        light.setDiffuse(color);
//        light.setSpecular(color);
//        light.setAmbient(color);


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
    //defaultRenderer.clear();
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
    //defaultRenderer.render(camera);

    rotation += 0.1;

    return LoopResult::CONTINUE;
  }
  void onMouseWheel(int wheel) override {
    *currentPosition += vector(0.0f, 0.0f, wheel);
    logger->verbose("%s", (*currentPosition).toString().c_str());

    /**
     * Update positions
     */
    if (lightAnnoyingSoundSource) {
      lightAnnoyingSoundSource->setPosition(vector3(lightPosition.x, lightPosition.y, -lightPosition.z));
      audio.updateListener(viewPosition);
    }

    light.setPosition(lightPosition);
    camera.setPosition(viewPosition);

  }

  virtual void onMouseMove(int x, int y, int dx, int dy, unsigned int buttons)
      override {
    *currentPosition += vector(0.1f * dx, 0.1f * dy, 0);
    logger->verbose("%s", (*currentPosition).toString().c_str());

    /**
     * Update positions
     */
    if (lightAnnoyingSoundSource) {
      lightAnnoyingSoundSource->setPosition(vector3(lightPosition.x, lightPosition.y, -lightPosition.z));
      audio.updateListener(viewPosition);
    }

    light.setPosition(lightPosition);
    camera.setPosition(viewPosition);

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
  void initializePlayground() override {
    Playground::initializePlayground();
    this->addRunner<OpenGLRunner>();
    this->addRunner<OpenALRunner>();
    this->addRunner<GeneralDemoRunner>();
  }
};

int main(int argc, char **argv) {
  String repository = Paths::add(Paths::getDirname(argv[0]), "resources"); //assumes executable lies in playground/target folder
  PlaygroundGeneralDemo playground(repository);
  playground.withName("GeneralDemo");
  printf("\n\nRunning [%s]\n", playground.toString().c_str());
  playground.run();
  printf("done\n");
  return 0;
}

