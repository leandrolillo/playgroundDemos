#include "../../base/BaseDemo.h"
#include "PhysicsRunner.h"
#include "GeometryRenderer.h"

#include "Entities.h"

class BreakoutRunner: public BaseDemoRunner {
  PhysicsRunner *physics = null;
  LightResource light;
  Background background;
  Particle *ball;

  GeometryRenderer geometryRenderer;
public:
  BreakoutRunner() : background(0, 0), geometryRenderer(defaultRenderer),
  light(vector(0, 0, 0), vector(0.6f, 0.6f, 0.6f), vector(0.2f, 0.2f, 0.2f), vector(0.1f, 0.1f, 0.1f), 1.0f) {

  }
  bool initialize() override {
    if (!BaseDemoRunner::initialize()) {
      return false;
    }

    TextureResource *backgroundTexture = (TextureResource *)this->getResourceManager().load("background.png", MimeTypes::TEXTURE);
    if(backgroundTexture != null) {
      background.setTexture(*backgroundTexture);
    }

    physics = (PhysicsRunner*) this->getContainer()->getRequiredRunner(PhysicsRunner::ID);
    ParticleManager &particleManager = physics->getParticleManager();

    ball = &particleManager.addParticle(std::make_unique<Particle>(std::make_unique<Sphere>(vector(0, 0, 0), 0.5)));

    return true;
  }

  LoopResult doLoop() override {
    defaultRenderer.clear();
    //defaultRenderer.setLight(&light);

    defaultRenderer.setTexture(null);
    defaultRenderer.drawAxes(matriz_4x4::identidad);

    background.draw(defaultRenderer);
    geometryRenderer.render(ball->getBoundingVolume());

    defaultRenderer.render(camera);
    return LoopResult::CONTINUE;
  }

  virtual void onResize(unsigned int height, unsigned int width) override {
    //camera.setPerspectiveProjectionFov(45.0, (GLfloat) width / (GLfloat) height, 0.1, 600.0);
    camera.setOrthographicProjection(height, width, -10, 10);
    background.resize(width, height);
  }

  virtual void onMouseWheel(int wheel) override {
    vector position = camera.getPosition() - vector(0.0, 0.0, std::min(1.0, 0.1 * wheel));
    position.z = std::max(-10.0f, std::min(10.0f, position.z));
    camera.setPosition(position);
    logger->info("Camera [%s]", camera.toString().c_str());
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
