/*
 * PhysicsDemo.h
 *
 *  Created on: Mar 22, 2021
 *      Author: leandro
 */
#include <iostream>
#include <stdio.h>
#include <random>

#define GL_SILENCE_DEPRECATION
#include <OpenGLRunner.h>
#include <OpenALRunner.h>
#include<PhysicsRunner.h>

#include<SkyboxRenderer.h>
#include<GridRenderer.h>
#include "GeometryRenderer.h"
#include<Gravity.h>

#include<Geometry.h>

#include "../../base/BaseDemo.h"

constexpr unsigned int numberOfParticles = 60;

class PhysicsDemoRunner;

static std::mt19937 mtRNE(std::chrono::system_clock::now().time_since_epoch().count()); //random number engine

class BulletParticle: public Particle
{
private:
  PhysicsDemoRunner *runner = null;

public:
  BulletParticle() : Particle(std::make_unique<Sphere> (vector(0, 0, 0), 0.1)) {

  }
  void setRunner(PhysicsDemoRunner *runner);
  void afterIntegrate(real dt);
  void onCollisionResolved(const ParticleContact &contact);
};

class PhysicsDemoRunner: public BaseDemoRunner {
  PhysicsRunner *physics {(PhysicsRunner*) this->getContainer().getRequiredRunner(PhysicsRunner::ID)};

  /**
   * This demo stuff
   */
  std::unique_ptr<AudioSource> gunshotSource;
  std::unique_ptr<AudioSource> bounceSource;

  //Plane ground = Plane(vector(0, 0, 0), vector(0, 1, 0));
  Particle *spherePlatform = null;
  Particle *aabbPlatform = null;

  unsigned long to = 0;
  real invPerformanceFreq = 1.0f;

  /**
   * Renderers - defaultRenderer inherited from base demo
   */
  GeometryRenderer geometryRenderer { defaultRenderer };
  SkyboxRenderer skyboxRenderer { video };
  GridRenderer gridRenderer { video };

  TextureResource *textureResource = null;
  MaterialResource materials[3] = { MaterialResource(vector(1, 0.5, 0.5), vector(1, 0.5, 0.5), vector(1, 1, 1), 32),
      MaterialResource(vector(0.5, 1, 0.5), vector(0.5, 1, 0.5), vector(1, 1, 1), 32),
      MaterialResource(vector(0.5, 0.5, 1), vector(0.5, 0.5, 1), vector(1, 1, 1), 32) };

  LightResource light = LightResource(vector(0, 0, 3), vector(0.4f, 0.4f, 0.4f), vector(0.5f, 0.5f, 0.5f), vector(1, 1, 1), 1.0);
  MaterialResource material = MaterialResource(vector(0.5, 0.5, 0.5), vector(0.7, 0.7, 0.7), vector(1, 1, 1), 32);

  MeshResource *basketball = null;

  std::vector<BulletParticle *>bullets;


  //Blockout
  Particle *top = null;
  Particle *bottom = null;
  Particle *left = null;
  Particle *right = null;

  Particle * ball = null;

  vector breakoutPosition {0, 4, 0};

  public:

  PhysicsDemoRunner(Playground &container) : BaseDemoRunner(container) {
    gunshotSource = audio.createSource("audio/handgunfire.wav", vector(0, 0, 0), vector(0, 0, 0), false);
    bounceSource = audio.createSource("audio/twang3.wav", vector(0, 0, 0), vector(0, 0, 0), false);

    textureResource = (TextureResource*) this->getResourceManager().load("images/basketball.png", MimeTypes::TEXTURE);
    basketball = (MeshResource*) this->getResourceManager().load("geometry/basketball.json/basketball", MimeTypes::MESH);

    skyboxRenderer.setSize(200);

    defaultRenderer.setLight(&light);

    ParticleManager &particleManager = physics->getParticleManager();

    spherePlatform = &particleManager.addParticle(std::make_unique<Particle>(std::make_unique<Sphere>(vector(0.0, 0.0, 0.0), 0.1)));
    spherePlatform->setInverseMass(0.0);

    aabbPlatform = &particleManager.addParticle(std::make_unique<Particle>(std::make_unique<AABB>(vector(0.0, 1.0, 0.0), vector(0.5, 0.05, 0.05))));
    aabbPlatform->setInverseMass(0.0);

    particleManager.addParticle(std::make_unique<Particle>(std::make_unique<Plane>(vector(0, 0, 0), vector(0, 1, 0)))).setInverseMass(0.0);

    particleManager.addForce(std::make_unique<Gravity>(vector(0.0, -9.8, 0.0)));

    real height = 9;
    real width = 9;
    real wall_depth = 1;
    real wall_half_depth = wall_depth * 0.5;
    top = &particleManager.addParticle(std::make_unique<Particle>(std::make_unique<AABB>(breakoutPosition + vector(0, height * 0.5, 0), vector(width * 0.5 - wall_half_depth, wall_half_depth, wall_half_depth)))).setInverseMass(0);
    bottom = &particleManager.addParticle(std::make_unique<Particle>(std::make_unique<AABB>(breakoutPosition + vector(0, height * -0.5, 0), vector(width * 0.5 - wall_half_depth, wall_half_depth, wall_half_depth)))).setInverseMass(0);
    left = &particleManager.addParticle(std::make_unique<Particle>(std::make_unique<AABB>(breakoutPosition + vector(width * -0.5, 0, 0), vector(wall_half_depth, height * 0.5 - wall_half_depth, wall_half_depth)))).setInverseMass(0);
    right = &particleManager.addParticle(std::make_unique<Particle>(std::make_unique<AABB>(breakoutPosition + vector(width * 0.5, 0, 0), vector(wall_half_depth, height *  0.5 - wall_half_depth, wall_half_depth)))).setInverseMass(0);

    for (int index = 0; index < numberOfParticles; index++) {
      BulletParticle &bullet = (BulletParticle &)particleManager.addParticle(std::make_unique<BulletParticle>());
      bullet.setStatus(false);
      bullet.setRunner(this);
      bullets.push_back(&bullet);
    }

    ball = &particleManager.addParticle(std::make_unique<Particle>(std::make_unique<Sphere>(breakoutPosition, 0.1)));

    reset();
  }

  void reset() {
    for(auto &bullet : bullets) {
      bullet->setStatus(false);
    }

    std::uniform_int_distribution<int> distribution(0, two_pi);
    real angulo = distribution(mtRNE);
    real modulo = distribution(mtRNE) * 0.1;

    if(ball != null) {
        logger->info("Ball random values - angle [%.2f], module [%.2f]", angulo, modulo);
        ball->setPosition(breakoutPosition);
        ball->setVelocity(vector(modulo * cos(angulo), modulo * sin(angulo), 0));
        ball->setAcceleration(vector(0, 0, 0));
        ball->setMass(0.1);
        ball->setDamping(0.99f);
    }

    video.setMousePosition(video.getScreenWidth() >> 1, video.getScreenHeight() >> 1);

    camera.setPosition(breakoutPosition + vector(0.0f, 0.0f, 10.0f));
    spherePlatform->setPosition(vector(0, 0.5, 0));
    aabbPlatform->setPosition(vector(0, 1.0, 0.0));
  }

  //real elapsedTime = 0;
  LoopResult doLoop() override {
    //real dt = getStopWatch().getElapsedTime();
    //elapsedTime += dt;
    //logger->info("Elapsed Time: %.3f=%.3f, dt=%.6f", getStopWatch().getTotalTime(), elapsedTime, dt);

    defaultRenderer.drawAxes(matriz_4x4::identidad);
//    defaultRenderer.drawLine(matriz_4x4::identidad, vector(-1, 0, 0), vector(1, 0, 0));
//    defaultRenderer.drawLine(matriz_4x4::identidad, vector(0, -1, 0), vector(0, 1, 0));
//    defaultRenderer.drawLine(matriz_4x4::identidad, vector(0, 0, -1), vector(0, 0, 1));

    /**
     * Render "platforms"
     */
    defaultRenderer.setTexture(textureResource);
    defaultRenderer.drawBox(matriz_4x4::traslacion(aabbPlatform->getPosition()),
        2.0 * ((AABB &) aabbPlatform->getBoundingVolume()).getHalfSizes().x,
        2.0 * ((AABB &) aabbPlatform->getBoundingVolume()).getHalfSizes().y,
        2.0 * ((AABB &) aabbPlatform->getBoundingVolume()).getHalfSizes().z);

    defaultRenderer.drawObject(matriz_4x4::traslacion(spherePlatform->getPosition()) * matriz_4x4::zoom(0.1, 0.1, 0.1), basketball);

    /**
     * Render basketballs
     */
    for (auto &particle : bullets)
    {
      if (particle->getStatus() == true) {
        defaultRenderer.setTexture(textureResource);
        defaultRenderer.setMaterial(&material);
        defaultRenderer.drawObject(matriz_4x4::traslacion(particle->getPosition()) * matriz_4x4::zoom(0.1, 0.1, 0.1), basketball);
      }
    }

    geometryRenderer.render(top->getBoundingVolume());
    geometryRenderer.render(bottom->getBoundingVolume());
    geometryRenderer.render(left->getBoundingVolume());
    geometryRenderer.render(right->getBoundingVolume());
    if(ball) {
      defaultRenderer.drawObject(matriz_4x4::traslacion(ball->getPosition()) * matriz_4x4::zoom(0.1, 0.1, 0.1), basketball);
    }


    skyboxRenderer.render(camera);
    gridRenderer.render(camera);

    spherePlatform->setVelocity(vector(sin(this->getStopWatch().getTotalTime() + M_PI_2), 0, 0));
    aabbPlatform->setVelocity(vector(-sin(this->getStopWatch().getTotalTime() + M_PI_2), 0, 0));

    return LoopResult::CONTINUE;
  }

  void onCollision(BulletParticle *bulletParticle, const ParticleContact &contact) {
    //logger->info("Collision relative speed: %f - sound: %s", contact.getRelativeSpeed(), contact.getRelativeSpeed() < -0.1 ? "on" : "off");

    if (contact.getRelativeSpeed() < -0.5) {
      //logger->info("Collision relative speed: %f - sound on", contact.getRelativeSpeed());
      bounceSource->setPosition(bulletParticle->getPosition());
      bounceSource->play();
    }
  }

  void afterIntegrate(BulletParticle *bulletParticle) {
    if (bulletParticle->getPosition().modulo() > 100) {
      bulletParticle->setStatus(false);
    }
  }

  void fire(const vector &position, bool isStatic = false) {
    BulletParticle *bullet = null;

    logger->debug("Iterating particles");
    for (auto particle : bullets)
    {
      if (!particle->getStatus()) {
        bullet = particle;
        break;
      }
    }

    if (bullet != null) {
      bullet->setPosition(position);
      if (isStatic) {
        bullet->setVelocity(vector(0, 0, 0));
        bullet->setDamping(0.99f);
      } else {
        bullet->setVelocity(camera.getOrientation().columna(2).normalizado() * -35);
        bullet->setDamping(0.99f);
      }
      bullet->setAcceleration(vector(0, 0, 0));
      bullet->setMass(0.1);

      bullet->setStatus(true);

      gunshotSource->setPosition(position);
      gunshotSource->play();

      //logger->info("bullet at position: %s", bullet->getPosition().toString("%.2f").c_str());

    } else {
      logger->debug("no particle found");
    }
  }

  void onMouseWheel(int wheel) override {
    camera.setPosition(camera.getPosition() - vector(0.0f, 0.0f, wheel));
    audio.updateListener(camera.getPosition());
    //logger->debug("camera: %s", camera.getPosition().toString("%.2f").c_str());
  }

  void onMouseMove(int x, int y, int dx, int dy, unsigned int buttons) override {
    camera.setPosition(camera.getPosition() - vector(0.1f * dx, 0.1f * dy, 0));
    audio.updateListener(camera.getPosition());
    //logger->debug("camera: %s", camera.getPosition().toString("%.2f").c_str());
  }

  void onMouseButtonDown(unsigned char button, int x, int y) override {
    fire(camera.getPosition());

    float randomDx = ((real) rand() / (real) RAND_MAX * 0.1 - 0.05);
    //logger->info("RandomDx %f", randomDx);
    fire(vector(randomDx, camera.getPosition().y - 0.5, 0.0), true);
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

class PhysicsPlayground: public Playground {
public:
  PhysicsPlayground(const String &resourcesBasePath) : Playground(resourcesBasePath) {
    this->addRunner<OpenGLRunner>();
    this->addRunner<OpenALRunner>();
    this->addRunner<PhysicsRunner>();
    this->addRunner<PhysicsDemoRunner>();
  }
};

void BulletParticle::setRunner(PhysicsDemoRunner *runner) {
  this->runner = runner;
}

void BulletParticle::afterIntegrate(real dt) {
  if (runner != null) {
    runner->afterIntegrate(this);
  }
}

void BulletParticle::onCollisionResolved(const ParticleContact &contact) {
  if (runner != null) {
    runner->onCollision(this, contact);
  }

}

int main(int argc, char **argv) {
  String repository = Paths::add(Paths::getDirname(argv[0]), "resources"); //assumes executable lies in playground/target folder
  PhysicsPlayground playground(repository);
  playground.withName("PhysicsDemo");
  printf("\n\nRunning [%s]\n", playground.toString().c_str());
  playground.run();
  printf("done\n");
  return 0;
}
