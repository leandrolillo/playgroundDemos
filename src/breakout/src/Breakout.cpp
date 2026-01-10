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

//TODO: move this to levels
constexpr real BALL_VELOCITY = 500; //pixels per second
constexpr real PADDLE_VELOCITY = 300.0; //pixels per second
constexpr real PADDLE_WIDTH = 60;
constexpr real PADDLE_HEIGHT = 10;

class BreakoutRunner: public BaseDemoRunner {
  GeometryRenderer geometryRenderer { defaultRenderer };
  PhysicsRunner *physics {(PhysicsRunner*) this->getContainer().getRequiredRunner(PhysicsRunner::ID)};

  LightResource light{ vector(0, 0, 0), vector(1, 1, 1), vector(1, 1, 1), vector(1, 1, 1), 1.0f };

  Background background { resourceManager, defaultRenderer };
  Border border { physics->getParticleManager(), geometryRenderer, DEPTH};
  Paddle paddle { physics->getParticleManager(), geometryRenderer, PADDLE_WIDTH, PADDLE_HEIGHT, DEPTH};
  Ball ball { physics->getParticleManager(), geometryRenderer, 10 };


  BreakoutLevel *level = null;
  std::vector<AABB *>bricks;

public:
  using BaseDemoRunner::BaseDemoRunner; //inherit constructors

  bool initialize() override {
    if (!BaseDemoRunner::initialize()) {
      return false;
    }

    this->getResourceManager().addAdapter(std::make_unique<BreakoutLevelAdapter>());

    background.initialize();
    border.initialize();
    paddle.initialize();
    ball.initialize();


    ParticleManager &particleManager = physics->getParticleManager();
    particleManager.getCollisionDetector().setRestitution(1.0);

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
    border.resize(width, height);
//    ball.resize(width, height);
    paddle.resize(width, height);


    if(level) {
      real halfDepth = DEPTH * 0.5;
      int margin = 1;

      int brick_height = height * 0.75 / level->getRows() - margin;
      int brick_width = width / level->getColumns() - margin;

      int left = -((int)width >> 1) + (width - (brick_width + margin) * level->getColumns()) / 2; //centered
      int top = ((int)height >> 1) - brick_height + margin;

      for(unsigned int i = 0; i < level->getRows(); i++) {
        for(unsigned int j = 0; j < level->getColumns(); j++) {
          this->bricks[i * level->getColumns() + j]->setHalfSizes(vector((int)brick_width >> 1, (int)brick_height >> 1, halfDepth));
          this->bricks[i * level->getColumns() + j]->setPosition(vector(left + (int)j * (margin + (int)brick_width), top - (int)i * (margin + (int)brick_height), -halfDepth));
        }
      }
    }
  }

  std::uniform_real_distribution<real> directionDistribution {10, 170};
  std::uniform_real_distribution<real> speedDistribution {-100, 100};

  bool reset() {
    camera.setPosition(vector(0, 0, -10));

    real direction = directionDistribution(mtRNE);
    real speed = BALL_VELOCITY + speedDistribution(mtRNE);

    //logger->info("Ball random values - angle [%.2f], module [%.2f]", angulo, modulo);
    real ballY = paddle.getPosition().y + paddle.getHalfSizes().y + ball.getRadius();
    ball.setPosition(vector(0, ballY, 0));
    ball.setVelocity(vector(speed * cos(radian(direction)), speed * sin(radian(direction)), 0));

    paddle.setPosition(vector(0, paddle.getPosition().y, 0));
    paddle.setVelocity(vector(0, 0, 0));
  }



  LoopResult doLoop() override {
    background.draw();
    border.draw();
    ball.draw();
    paddle.draw();

    for(auto &brick : bricks) {
      if(brick->getStatus()) {
        geometryRenderer.render(*brick);
      }
    }

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
        paddle.setVelocity(vector(0, 0, 0));
        break;
    }
  }

  virtual void onKeyDown(unsigned int key, unsigned int keyModifier) override {
    switch (key) {
      case SDLK_LEFT:
      case SDLK_A:
        paddle.setVelocity(vector(-PADDLE_VELOCITY, 0, 0));
        break;
      case SDLK_RIGHT:
      case SDLK_D:
        paddle.setVelocity(vector(PADDLE_VELOCITY, 0, 0));
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
  using Playground::Playground; //inherit constructors

  void initializePlayground() override {
    Playground::initializePlayground();
    this->addRunner<OpenGLRunner>();
    this->addRunner<AudioRunner>();
    this->addRunner<PhysicsRunner>();
    this->addRunner<BreakoutRunner>();
  }
};

int main(int argc, char **argv) {
  String repository = Paths::add(Paths::getDirname(argv[0]), "resources"); //assumes executable lies in playground/target folder
  Breakout playground(repository);
  playground.withName("Blockout");
  printf("\n\nRunning [%s]\n", playground.toString().c_str());
  playground.run();
  printf("done\n");
  return 0;
}
