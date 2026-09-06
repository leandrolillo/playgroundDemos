#pragma once

#include<random>
#include <cassert>

#include "../../base/BaseDemo.h"
#include "OpenALRunner.h"
#include "PhysicsRunner.h"
#include "GeometryRenderer.h"

#include "Entities.h"
#include "BreakoutLevelAdapter.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "BreakoutStates.h"

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

  std::vector<String> levelNames = { "Level-0.json", "Level-1.json", "Level-2.json"};
  std::vector<String> softThemes = {"audio/forest.ogg", "audio/PathToLakeLand.ogg", "audio/the_field_of_dreams.mp3" };
  std::vector<String> dynamicThemes = {"audio/awesomeness.wav", "audio/background.ogg", "audio/CyberpunkMoonlightSonata.mp3", "audio/song18.mp3"};

  SpriteRenderer spriteRenderer { video };
  TextRenderer textRenderer { video };

  PhysicsRunner *physics {(PhysicsRunner*) this->getContainer().getRequiredRunner(PhysicsRunner::ID)};

  Background background { resourceManager };
  Border border { resourceManager, physics->getParticleManager(), DEPTH};
  Paddle paddle { resourceManager, physics->getParticleManager(), PADDLE_WIDTH, PADDLE_HEIGHT, DEPTH};
  Ball ball { resourceManager, physics->getParticleManager(), 10 };
  Level level { levelNames[0], resourceManager, physics->getParticleManager(), DEPTH};

  FontResource *font = null;
  std::unique_ptr<BreakoutState> state;

  unsigned short livesLeft = 3;


  std::vector<std::unique_ptr<AudioSource>> softBackgroundMusic;
  std::vector<std::unique_ptr<AudioSource>> dynamicBackgroundMusic;

public:
  BreakoutRunner(Playground &container) : BaseDemoRunner(container) {
    video.enable(VideoAttribute::RELATIVE_MOUSE_MODE);
    video.enable(VideoAttribute::BLEND, VideoAttribute::SRC_ALPHA, VideoAttribute::ONE_MINUS_SRC_ALPHA);

    this->getResourceManager().addAdapter<BreakoutLevelAdapter>();

    ParticleManager &particleManager = physics->getParticleManager();
    particleManager.getCollisionDetector().setRestitution(1.0);

    background.initialize();
    border.initialize();
    border.setOnBottomCollisionHandler(
        [this](GeometryContact &contact) {
                  this->livesLeft--;
                  if(livesLeft <= 0) {
                    this->setState(std::make_unique<TransitioningState>("You died", 10.0f, std::make_unique<MenuState>(false)));
                  } else {
                    this->setState(std::make_unique<TransitioningState>("Some message making fun of you lack of spatial perception!", 10.0f, std::make_unique<StandByState>(level.getName())));
                  }
                }
        );
    paddle.initialize();
    ball.initialize();

    level.initialize();
    level.setOnCompletedHandler(
        [this]() {
          String nextLevel;

          auto it = std::find(levelNames.begin(), levelNames.end(), level.getName());
          if (it != levelNames.end()) {
            auto nextIt = std::next(it);

            if (nextIt == levelNames.end()) {
              nextLevel = levelNames.front();
            } else {
              nextLevel = *nextIt;
            }
          }

          if(!nextLevel.empty()) {
            this->setState(std::make_unique<TransitioningState>("Level complete!", 10, std::make_unique<StandByState>(nextLevel)));
          } else {
            this->exit();
          }
        });

    textRenderer.initialize();
    textRenderer.print("This is some beautiful text with lots of letters, no question gathered per now!", vector2(0, 100));

    for( auto &audioResourceName : softThemes ) {
      auto audioResource = audio.createSource(audioResourceName);
      if(audioResource) {
        softBackgroundMusic.emplace_back(std::move(audioResource));
      }
    }

    for( auto &audioResourceName : dynamicThemes ) {
      auto audioResource = audio.createSource(audioResourceName);
      if(audioResource) {
        dynamicBackgroundMusic.emplace_back(std::move(audioResource));
      }
    }

    camera.setPosition(vector(0, 0, 0));

    setState(std::make_unique<MenuState>(false));
  }

  virtual void onResize(unsigned int width, unsigned int height) override {
    //camera.setOrthographicProjection(0, 0, width, height, zMin, zMax);
    //camera.setOrthographicProjection(width * -0.5, height * -0.5, width * 0.5, height * 0.5, zMin, zMax);
    camera.setOrthographicProjection(width, height, zMin, zMax);

    background.onScreenResize(width, height);
    border.onScreenResize(width, height);
    ball.onScreenResize(width, height);
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

    return true;
  }


  virtual void beforeLoop() override {
    spriteRenderer.clear();
    textRenderer.clear();
  }

  LoopResult doLoop() override {
    background.draw(spriteRenderer);
    border.draw(spriteRenderer);
    level.draw(spriteRenderer);
    ball.draw(spriteRenderer);
    paddle.draw(spriteRenderer);

    textRenderer.print("Lives: " + std::to_string(this->livesLeft), vector2(this->getVideo().getScreenWidth() * -0.5 + 20, this->getVideo().getScreenHeight() * 0.5 - 40));

    state->update(*this);

    return LoopResult::CONTINUE;
  }

  virtual void afterLoop() override {
    spriteRenderer.render(camera);
    textRenderer.render(camera);
  }

  //  virtual void onMouseWheel(int wheel) override {
  //    logger->info("Camera before: [%s]", camera.toString().c_str());
  //    vector position = camera.getPosition() - vector(0.0, 0.0, std::min(1.0, 0.1 * wheel));
  //    position.z = std::max(zMin + 1, std::min(zMax - 1, position.z));
  //    camera.setPosition(position);
  //    logger->info("[%s]", camera.toString().c_str());
  //  }
  //
  //  virtual void onMouseMove(int x, int y, int dx, int dy, unsigned int buttons) override {
  //    if (dx != 0 || dy != 0) {
  //      camera.setPosition(camera.getPosition() + vector(dx, dy, 0));
  //    }
  //  }

  virtual void onKeyUp(unsigned int key, unsigned int keyModifier) override {
    if(!state->onKeyUp(*this, key, keyModifier)) {
      BaseDemoRunner::onKeyUp(key, keyModifier);
    }
  }

  virtual void onKeyDown(unsigned int key, unsigned int keyModifier) override {
    if(!state->onKeyDown(*this, key, keyModifier)) {
      BaseDemoRunner::onKeyDown(key, keyModifier);
    }
  }
  virtual void onMouseWheel(int wheel) override {

  }

  void setState(std::unique_ptr<BreakoutState> state) {
    assert(state.get() != null);
    this->state = std::move(state);
    this->state->enter(*this);
  }

  void print(const String &text, vector2 position) {
    this->textRenderer.print(text, position);
  }

  void shuffleSoftBackgroundMusic() {
    if(!softBackgroundMusic.empty()) {
      std::uniform_int_distribution<unsigned long> distribution {0, softBackgroundMusic.size() - 1};
      unsigned long index = distribution(mtRNE);
      logger->info("Using soft background music [%d]/[%d]", index, softBackgroundMusic.size() - 1);

      softBackgroundMusic.at(index)->play();
    }
  }

  void shuffleDynamicBackgroundMusic() {
    if(!softBackgroundMusic.empty()) {
      std::uniform_int_distribution<unsigned long> distribution {0, dynamicBackgroundMusic.size() - 1};
      unsigned long index = distribution(mtRNE);
      logger->info("Using dynamic background music [%d]/[%d]", index, dynamicBackgroundMusic.size() - 1);

      dynamicBackgroundMusic.at(index)->play();
    }
  }

  void freeze() {
    this->physics->setEnabled(false);
    this->shuffleSoftBackgroundMusic();
  }

  void unfreeze() {
    this->physics->setEnabled(true);
    this->shuffleDynamicBackgroundMusic();
  }

//  void standBy() {
//    this->ball.setStatus(false);
//    this->physics->setEnabled(true);
//  }

  void syncBallWithPaddle() {
    this->ball.setStatus(false);
    ball.setPosition(paddle.getPosition() + vector(0, paddle.getSize().y * 0.5 + ball.getRadius(), 0));
  }

  void playBall() {
    real direction = directionDistribution(mtRNE);
    real speed = BALL_VELOCITY + speedDistribution(mtRNE);

    //logger->info("Ball random values - angle [%.2f], module [%.2f]", angulo, modulo);
    ball.setPosition(paddle.getPosition() + vector(0, paddle.getSize().y * 0.5 + ball.getRadius(), 0));
    ball.setVelocity(vector(speed * cos(radian(direction)), speed * sin(radian(direction)), 0));

    this->ball.setStatus(true);
    this->physics->setEnabled(true);
  }

  void exit() {
    this->getContainer().stop();
  }

  void setPaddleVelocity(const vector &velocity) {
    this->paddle.setVelocity(velocity);
  }

  const std::vector<String> &getLevelNames() const {
    return this->levelNames;
  }
  const String &getCurrentLevelName() const {
    return this->level.getName();
  }
  void setCurrentLevelName(const String &levelName, bool forceLoad = false) {
    if(std::any_of(this->levelNames.begin(), this->levelNames.end(), [&levelName](auto &availableLevelName) { return availableLevelName == levelName; })) {
      if(level.getName() != levelName || forceLoad) {
        this->level.load(levelName);
        this->level.onScreenResize(this->getVideo().getScreenWidth(), this->getVideo().getScreenHeight());
      }
    }
  }
};
