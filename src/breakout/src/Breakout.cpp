#include "../../base/BaseDemo.h"
#include "PhysicsRunner.h"
#include "GeometryRenderer.h"
#include<random>

#include "Entities.h"
#include "BreakoutLevelAdapter.h"

constexpr real zMin = -1000;
constexpr real zMax = 1000;

static std::mt19937 mtRNE(std::chrono::system_clock::now().time_since_epoch().count()); //random number engine

constexpr real DEPTH = 10;
constexpr real PADDLE_VELOCITY = 200.0;
constexpr real PADDLE_WIDTH = 60;
constexpr real PADDLE_HEIGHT = 10;

class BreakoutRunner: public BaseDemoRunner {
  PhysicsRunner *physics = null;
  LightResource light;

  MeshResource *basketball = null;

  Background background;
  Particle *ball = null;
  Particle *paddle = null;

  BreakoutLevel *level = null;
  std::vector<AABB *>bricks;

  GeometryRenderer geometryRenderer;

  //Level limits
  AABB *top = null;
  AABB *bottom = null;
  AABB *left = null;
  AABB *right = null;
public:
  BreakoutRunner() :  background(0, 0),
                      geometryRenderer(defaultRenderer),
                      light(vector(0, 0, 0), vector(1, 1, 1), vector(1, 1, 1), vector(1, 1, 1), 1.0f) {
  }

  bool initialize() override {
    if (!BaseDemoRunner::initialize()) {
      return false;
    }

    this->getResourceManager().addAdapter(std::make_unique<BreakoutLevelAdapter>());

    TextureResource *backgroundTexture = (TextureResource *)this->getResourceManager().load("background.png", MimeTypes::TEXTURE);
    if(backgroundTexture != null) {
      background.setTexture(*backgroundTexture);
    }

    basketball = (MeshResource*) this->getResourceManager().load("geometry/basketball.json/basketball", MimeTypes::MESH);

    physics = (PhysicsRunner*) this->getContainer()->getRequiredRunner(PhysicsRunner::ID);
    ParticleManager &particleManager = physics->getParticleManager();
    particleManager.getCollisionDetector().setRestitution(1.0);

    /*Level limits*/
    top = (AABB *)&particleManager.addScenery(std::make_unique<AABB>(vector(0, 0, 0), vector(2, 1, 0.1)));
    bottom = (AABB *)&particleManager.addScenery(std::make_unique<AABB>(vector(0, 0, 0), vector(2, 1, 0.1)));
    left = (AABB *)&particleManager.addScenery(std::make_unique<AABB>(vector(0, 0, 0), vector(1, 2, 0.1)));
    right = (AABB *)&particleManager.addScenery(std::make_unique<AABB>(vector(0, 0, 0), vector(1, 2, 0.1)));

    /*Ball*/
    ball = &particleManager.addParticle(std::make_unique<Particle>(std::make_unique<Sphere>(vector(0, 0, 0), 10)));

    paddle = &particleManager.addParticle(std::make_unique<Particle>(std::make_unique<AABB>(vector(0, 0, 0), vector(PADDLE_WIDTH, PADDLE_HEIGHT, DEPTH * 0.5))));
    //ball->setMass(1.0);
    ball->setDamping(1.0);



    level = (BreakoutLevel *)getResourceManager().load("level-0.json", BreakoutLevel::MimeType);

    if(level) {
      for(unsigned int i = 0; i < level->getRows(); i++) {
        for(unsigned int j = 0; j < level->getColumns(); j++) {
          this->bricks.push_back((AABB *)&particleManager.addScenery(std::make_unique<AABB>(vector(0, 0, 0), vector(1, 1, 1))));
          this->bricks.back()->setOnCollisionHandler([brick=bricks.back()](GeometryContact &contact) { brick->setStatus(false); });

          if(level->getBrickAt(i, j) == 0) {
            this->bricks.back()->setStatus(false);
          }
        }
      }
    }



    reset();

    return true;
  }

  virtual void onResize(unsigned int height, unsigned int width) override {
    camera.setOrthographicProjection(width, height, zMin, zMax);
    background.resize(width, height);

    real halfDepth = DEPTH * 0.5;

    top->setOrigin(vector(0, height * 0.51 + halfDepth, 0));
    top->setHalfSizes(vector(width * 0.5, halfDepth, halfDepth));

    bottom->setOrigin(vector(0, height * -0.51 - halfDepth, 0));
    bottom->setHalfSizes(vector(width * 0.5, halfDepth, halfDepth));

    left->setOrigin(vector(width * -0.51 - halfDepth, 0, 0));
    left->setHalfSizes(vector(halfDepth, height * 0.5, halfDepth));

    right->setOrigin(vector(width * 0.51 + + halfDepth, 0, 0));
    right->setHalfSizes(vector(halfDepth, height *  0.5, halfDepth));

    paddle->setPosition(vector(
        paddle->getPosition().x,
        height * -0.5 + 2.0 * ((AABB &)paddle->getBoundingVolume()).getHalfSizes().y,
        paddle->getPosition().z));


    if(level) {
      int margin = 1;

      int brick_height = height * 0.75 / level->getRows() - margin;
      int brick_width = width / level->getColumns() - margin;

      int left = -((int)width >> 1) + (width - (brick_width + margin) * level->getColumns()) / 2; //centered
      int bottom = ((int)height >> 1) - (int)level->getRows() * ((int)brick_height + margin);

      for(unsigned int i = 0; i < level->getRows(); i++) {
        for(unsigned int j = 0; j < level->getColumns(); j++) {
          this->bricks[i * level->getColumns() + j]->setHalfSizes(vector((int)brick_width >> 1, (int)brick_height >> 1, halfDepth));
          this->bricks[i * level->getColumns() + j]->setPosition(vector(left + (int)j * (margin + (int)brick_width), bottom + (int)i * (margin + (int)brick_height), -halfDepth));
        }
      }
    }
  }

  bool reset() {
    camera.setPosition(vector(0, 0, -10));

    std::uniform_int_distribution<int> distribution(10, 170);
    real angulo = distribution(mtRNE);
    real modulo = distribution(mtRNE) * 10;

    //logger->info("Ball random values - angle [%.2f], module [%.2f]", angulo, modulo);
    real ballY = paddle->getPosition().y + ((AABB &)paddle->getBoundingVolume()).getHalfSizes().y + ((Sphere &)ball->getBoundingVolume()).getRadius();
    ball->setPosition(vector(0, ballY, 0));
    ball->setVelocity(vector(modulo * cos(radian(angulo)), modulo * sin(radian(angulo)), 0));

    paddle->setPosition(vector(0, paddle->getPosition().y, 0));
    paddle->setVelocity(vector(0, 0, 0));
    paddle->setInverseMass(0.0);
  }



  LoopResult doLoop() override {
    background.draw(defaultRenderer);

    defaultRenderer.drawObject(matriz_4x4::traslacion(ball->getBoundingVolume().getOrigin()) * matriz_4x4::zoom(0.1, 0.1, 0.1), basketball);

    geometryRenderer.render(ball->getBoundingVolume());
    geometryRenderer.render(paddle->getBoundingVolume());

    for(auto &brick : bricks) {
      if(brick->getStatus()) {
        geometryRenderer.render(*brick);
      }
    }

    geometryRenderer.render(*top);
    geometryRenderer.render(*bottom);
    geometryRenderer.render(*left);
    geometryRenderer.render(*right);

    return LoopResult::CONTINUE;
  }

  virtual void onMouseWheel(int wheel) override {
    vector position = camera.getPosition() - vector(0.0, 0.0, std::min(1.0, 0.1 * wheel));
    position.z = std::max(zMin + 1, std::min(zMax - 1, position.z));
    camera.setPosition(position);
    logger->info("[%s]", camera.toString().c_str());
  }

  virtual void onMouseMove(int x, int y, int dx, int dy, unsigned int buttons) override {
    if (dx != 0 || dy != 0) {
      camera.setPosition(camera.getPosition() + vector(dx, dy, 0));
    }
  }

  virtual void onKeyUp(unsigned int key, unsigned int keyModifier) override {
    switch (key) {
      case SDLK_LEFT:
      case SDLK_A:
      case SDLK_RIGHT:
      case SDLK_D:
        paddle->setVelocity(vector(0, 0, 0));
        break;
    }
  }

  virtual void onKeyDown(unsigned int key, unsigned int keyModifier) override {
    switch (key) {
      case SDLK_LEFT:
      case SDLK_A:
        paddle->setVelocity(vector(-PADDLE_VELOCITY, 0, 0));
        break;
      case SDLK_RIGHT:
      case SDLK_D:
        paddle->setVelocity(vector(PADDLE_VELOCITY, 0, 0));
        break;

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
