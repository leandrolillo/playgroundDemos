/*
 * PhysicsDemo.h
 *
 *  Created on: Mar 22, 2021
 *      Author: leandro
 */

#ifndef SRC_BASEDEMO_H_
#define SRC_BASEDEMO_H_

#include <iostream>
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGLRunner.h>
#include <AudioRunner.h>
#include<DefaultRenderer.h>
#include<Camera.h>

class BaseDemoRunner: public PlaygroundRunner {
protected:
  Logger *logger = LoggerFactory::getLogger(typeid(*this).name());
  VideoRunner &video;
  AudioRunner &audio;

  DefaultRenderer defaultRenderer;

  Camera camera;
  public:
  BaseDemoRunner(Playground &container) : PlaygroundRunner(container),
    video(*(VideoRunner *)this->getContainer().getRequiredRunner(VideoRunner::ID)),
    audio(*(AudioRunner *)this->getContainer().getRequiredRunner(AudioRunner::ID)),
    defaultRenderer(video)
  {
    logger->addAppender(LoggerFactory::getAppender("stdout"));
  }

  virtual unsigned char getId() const override {
    return 200;
  }

  virtual unsigned char getInterests() const override {
    return KEY_DOWN | KEY_UP | MOUSE_MOVE | MOUSE_WHEEL | MOUSE_BUTTON_DOWN | MOUSE_BUTTON_UP | RESIZE;
  }

  VideoRunner &getVideo() {
    return this->video;
  }

  AudioRunner getAudio() {
    return this->audio;
  }

  virtual void onResize(unsigned int width, unsigned int height) override {
    camera.setPerspectiveProjectionFov(45.0, (double) width / (double) height, 2, 300.0);
  }

  virtual bool initialize() override {
    constexpr unsigned int width = 800;
    constexpr unsigned int height = 600;
    this->video.resize(width, height);
    camera.setPerspectiveProjectionFov(45.0, (double) width / (double) height, 2, 300.0);
    
    camera.setViewMatrix(matriz_4x4::traslacion(vector(0.0f, -0.5f, -10.0f)));

    //logger->debug("Setting up video %d", video);
    video.setClearColor(0.0, 0.5, 0.0, 0.0);
    video.enable(VideoAttribute::DEPTH_TEST);
    video.enable(VideoAttribute::CULL_FACE, VideoAttribute::BACK);
    video.enable(VideoAttribute::BLEND, VideoAttribute::SRC_ALPHA, VideoAttribute::ONE_MINUS_SRC_ALPHA);

    logger->debug("Completed initialization");

    return true;
  }

  virtual void beforeLoop() override {
    defaultRenderer.clear();

  }

  virtual void afterLoop() override {
    defaultRenderer.render(camera);
  }

  virtual void onMouseWheel(int wheel) override {
    logger->info("Mouse wheel %d", wheel);
    camera.setPosition(camera.getPosition() - vector(0.0, 0.0, std::min(1.0, 0.1 * wheel)));
    logger->info("Camera position: %s", camera.getPosition().toString().c_str());
  }

  virtual void onKeyDown(unsigned int key, unsigned int keyModifier) override {
    switch (key) {
    case '+':
      camera.setPosition(camera.getPosition() - vector(0.0, 0.0, 0.1));
      break;
    case '-':
      camera.setPosition(camera.getPosition() + vector(0.0, 0.0, 0.1));
      break;
    case SDLK_LEFT:
      camera.setPosition(camera.getPosition() - vector(0.1, 0.0, 0.0));
      break;
    case SDLK_RIGHT:
      camera.setPosition(camera.getPosition() + vector(0.1, 0.0, 0.0));
      break;
    case SDLK_UP:
      camera.setPosition(camera.getPosition() + vector(0.0, 0.1, 0.0));
      break;
    case SDLK_DOWN:
      camera.setPosition(camera.getPosition() - vector(0.0, 0.1, 0.0));
      break;
//            case SDLK_BACKSPACE:
//                break;
//            case SDLK_SPACE:
//                break;
    }
  }

  virtual String toString() const {
    return logger->getBasename() + "(id:" + std::to_string(this->getId()) + ")";
  }

};
#endif
