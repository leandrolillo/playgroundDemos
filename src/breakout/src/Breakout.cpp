#include "../../base/BaseDemo.h"
#include "PhysicsRunner.h"
#include "GeometryRenderer.h"
#include<random>

#include "Entities.h"
#include "BreakoutLevelAdapter.h"
#include "SpriteRenderer.h"

constexpr real zMin = -1;
constexpr real zMax = 1;

static std::mt19937 mtRNE(std::chrono::system_clock::now().time_since_epoch().count()); //random number engine

constexpr real DEPTH = 10;

//TODO: move this to levels
constexpr real BALL_VELOCITY = 500; //pixels per second
constexpr real PADDLE_VELOCITY = 300.0; //pixels per second
constexpr real PADDLE_WIDTH = 60;
constexpr real PADDLE_HEIGHT = 10;

class BreakoutRunner: public BaseDemoRunner {
  //GeometryRenderer geometryRenderer { defaultRenderer };
  SpriteRenderer spriteRenderer { video };

  PhysicsRunner *physics {(PhysicsRunner*) this->getContainer().getRequiredRunner(PhysicsRunner::ID)};

  LightResource light{ vector(0, 0, 0), vector(1, 1, 1), vector(1, 1, 1), vector(1, 1, 1), 1.0f };

  Background background { resourceManager };
  Border border { resourceManager, physics->getParticleManager(), DEPTH};
  Paddle paddle { resourceManager, physics->getParticleManager(), PADDLE_WIDTH, PADDLE_HEIGHT, DEPTH};
  Ball ball { resourceManager, physics->getParticleManager(), 10 };
  Level level { resourceManager, physics->getParticleManager(), DEPTH};

public:
  using BaseDemoRunner::BaseDemoRunner; //inherit constructors

  virtual void beforeLoop() override { //use sprite renderer instead
    spriteRenderer.clear();
  }

  virtual void afterLoop() override {
    spriteRenderer.render(camera);
  }


  bool initialize() override {
    if (!BaseDemoRunner::initialize()) {
      return false;
    }

    video.enable(VideoAttribute::RELATIVE_MOUSE_MODE);

    this->getResourceManager().addAdapter<BreakoutLevelAdapter>();

    ParticleManager &particleManager = physics->getParticleManager();
    particleManager.getCollisionDetector().setRestitution(1.0);

    background.initialize();
    border.initialize();
    paddle.initialize();
    ball.initialize();
    level.initialize();

    reset();

    return true;
  }

  virtual void onResize(unsigned int width, unsigned int height) override {
    //camera.setOrthographicProjection(0, 0, width, height, zMin, zMax);
    //camera.setOrthographicProjection(width * -0.5, height * -0.5, width * 0.5, height * 0.5, zMin, zMax);
    camera.setOrthographicProjection(width, height, zMin, zMax);

    background.onScreenResize(width, height);
    border.onScreenResize(width, height);
//    ball.resize(width, height);
    paddle.onScreenResize(width, height);
    level.onScreenResize(width, height);
  }

  std::uniform_real_distribution<real> directionDistribution {10, 170};
  std::uniform_real_distribution<real> speedDistribution {-100, 100};

  bool reset() {
    camera.setPosition(vector(0, 0, 0));

    real direction = directionDistribution(mtRNE);
    real speed = BALL_VELOCITY + speedDistribution(mtRNE);

    //logger->info("Ball random values - angle [%.2f], module [%.2f]", angulo, modulo);
    real ballY = paddle.getPosition().y + paddle.getSize().y * 0.5 + ball.getRadius();
    ball.setPosition(vector(0, ballY, 0));
    ball.setVelocity(vector(speed * cos(radian(direction)), speed * sin(radian(direction)), 0));

    paddle.setPosition(vector(0, paddle.getPosition().y, 0));
    paddle.setVelocity(vector(0, 0, 0));
  }



  LoopResult doLoop() override {
    background.draw(spriteRenderer);
    border.draw(spriteRenderer);
    ball.draw(spriteRenderer);
    paddle.draw(spriteRenderer);
    level.draw(spriteRenderer);


    return LoopResult::CONTINUE;
  }

  virtual void onMouseWheel(int wheel) override {
    logger->info("Camera before: [%s]", camera.toString().c_str());
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
