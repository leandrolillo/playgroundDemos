#include "../../base/BaseDemo.h"
#include "PhysicsRunner.h"
#include "GeometryRenderer.h"
#include<random>

#include "Entities.h"

constexpr real zMin = -1000;
constexpr real zMax = 1000;

static std::mt19937 mtRNE(std::chrono::system_clock::now().time_since_epoch().count()); //random number engine

class BreakoutRunner: public BaseDemoRunner {
  PhysicsRunner *physics = null;
  LightResource light;

  MeshResource *basketball = null;

  Background background;
  Particle *ball;

  GeometryRenderer geometryRenderer;

  //Level limits
  Particle *top = null;
  Particle *bottom = null;
  Particle *left = null;
  Particle *right = null;
public:
  BreakoutRunner() : background(0, 0), geometryRenderer(defaultRenderer),
  light(vector(0, 0, 0), vector(1, 1, 1), vector(1, 1, 1), vector(1, 1, 1), 1.0f) {

  }
  bool initialize() override {
    if (!BaseDemoRunner::initialize()) {
      return false;
    }

    TextureResource *backgroundTexture = (TextureResource *)this->getResourceManager().load("background.png", MimeTypes::TEXTURE);
    if(backgroundTexture != null) {
      background.setTexture(*backgroundTexture);
    }

    basketball = (MeshResource*) this->getResourceManager().load("geometry/basketball.json/basketball", MimeTypes::MESH);

    physics = (PhysicsRunner*) this->getContainer()->getRequiredRunner(PhysicsRunner::ID);
    ParticleManager &particleManager = physics->getParticleManager();

    /*Level limits*/
    top = &particleManager.addParticle(std::make_unique<Particle>(std::make_unique<AABB>(vector(0, 0, 0), vector(2, 1, 0.1))));
    top->setInverseMass(0);
    bottom = &particleManager.addParticle(std::make_unique<Particle>(std::make_unique<AABB>(vector(0, 0, 0), vector(2, 1, 0.1))));
    bottom->setInverseMass(0);
    left = &particleManager.addParticle(std::make_unique<Particle>(std::make_unique<AABB>(vector(0, 0, 0), vector(1, 2, 0.1))));
    left->setInverseMass(0);
    right = &particleManager.addParticle(std::make_unique<Particle>(std::make_unique<AABB>(vector(0, 0, 0), vector(1, 2, 0.1))));
    right->setInverseMass(0);

    /*Ball*/
    ball = &particleManager.addParticle(std::make_unique<Particle>(std::make_unique<Sphere>(vector(0, 0, 0), 10)));
    ball->setMass(5.0);

    reset();

    return true;
  }


  bool reset() {
    camera.setPosition(vector(0, -5, -10));

    std::uniform_int_distribution<int> distribution(0, 360);
    real angulo = distribution(mtRNE);
    real modulo = distribution(mtRNE) * 2;

    logger->info("Ball random values - angle [%.2f], module [%.2f]", angulo, modulo);
    ball->setPosition(vector(0, 0, 0));
    ball->setVelocity(vector(modulo * cos(radian(angulo)), modulo * sin(radian(angulo)), 0));

  }

  LoopResult doLoop() override {
    //logger->info("Ball: [%s] velocity [%s]", ball->getPosition().toString().c_str(), ball->getVelocity().toString().c_str());
    defaultRenderer.clear();
    //defaultRenderer.setLight(&light);
//    defaultRenderer.drawAxes(matriz_4x4::identidad);

    background.draw(defaultRenderer);

    defaultRenderer.drawObject(matriz_4x4::traslacion(ball->getBoundingVolume().getOrigin()) * matriz_4x4::zoom(0.1, 0.1, 0.1), basketball);

    geometryRenderer.render(ball->getBoundingVolume());
//    geometryRenderer.render(top->getBoundingVolume());
//    geometryRenderer.render(bottom->getBoundingVolume());
//    geometryRenderer.render(left->getBoundingVolume());
//    geometryRenderer.render(right->getBoundingVolume());

    defaultRenderer.render(camera);
    return LoopResult::CONTINUE;
  }

  virtual void onResize(unsigned int height, unsigned int width) override {
    //camera.setPerspectiveProjectionFov(45.0, (GLfloat) width / (GLfloat) height, 0.1, zMax);
    camera.setOrthographicProjection(height, width, zMin, zMax);
    background.resize(width, height);

    ((AABB &)top->getBoundingVolume()).setOrigin(vector(0, height * 0.5 - 10, 0));
    ((AABB &)top->getBoundingVolume()).setHalfSizes(vector(width * 0.5, 10, 10));

    ((AABB &)bottom->getBoundingVolume()).setOrigin(vector(0, height * -0.5 + 10, 0));
    ((AABB &)bottom->getBoundingVolume()).setHalfSizes(vector(width * 0.5, 10, 10));

    ((AABB &)left->getBoundingVolume()).setOrigin(vector(width * -0.5 + 10, 0, 0));
    ((AABB &)left->getBoundingVolume()).setHalfSizes(vector(10, height * 0.5, 10));

    ((AABB &)right->getBoundingVolume()).setOrigin(vector(width * 0.5 - 10, 0, 0));
    ((AABB &)right->getBoundingVolume()).setHalfSizes(vector(10, height *  0.5, 10));

  }

  virtual void onMouseWheel(int wheel) override {
    vector position = camera.getPosition() - vector(0.0, 0.0, std::min(1.0, 0.1 * wheel));
    position.z = std::max(zMin + 1, std::min(zMax - 1, position.z));
    camera.setPosition(position);
    logger->info("Camera [%s]", camera.toString().c_str());
  }

  virtual void onMouseMove(int x, int y, int dx, int dy, unsigned int buttons) override {
    if (dx != 0 || dy != 0) {
      camera.setPosition(camera.getPosition() + vector(dx, dy, 0));
    }
  }

  virtual void onKeyDown(unsigned int key, unsigned int keyModifier) override {
    switch (key) {
    case SDLK_SPACE:
      physics->setEnabled(!physics->getEnabled());
      break;

    case SDLK_BACKSPACE:
      reset();
      break;
    }
  }
};

class Breakout: public Playground {
public:
  Breakout(const String &resourcesBasePath) :
      Playground(resourcesBasePath) {
  }
  void initializePlayground() override {
    Playground::initializePlayground();
    this->addRunner(std::make_unique<OpenGLRunner>());
    this->addRunner(std::make_unique<AudioRunner>());
    this->addRunner(std::make_unique<PhysicsRunner>());
    this->addRunner(std::make_unique<BreakoutRunner>());
  }
};

int main(int argc, char **argv) {
  String repository = Paths::add(Paths::getDirname(argv[0]), "resources"); //assumes executable lies in playground/target folder
  Breakout playground(repository);
  playground.withName("Blockout");
  printf("\n\nRunning playground [%s]\n", playground.toString().c_str());
  playground.run();
  printf("done\n");
  return 0;
}
