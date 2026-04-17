#include "Breakout.h"

void MenuState::enter(BreakoutRunner &runner) {
  runner.freeze();
}

void MenuState::update(BreakoutRunner &breakoutRunner) {
//  vector2 position = vector2(
//      (breakoutRunner.getVideo().getScreenWidth() - 100) / 2,
//      (breakoutRunner.getVideo().getScreenHeight() - 100) / 2);

  vector2 position = vector2(-100 , 100);

  for (int index = 0; index < options.size(); index++) {
    if (index == currentSelection) {
      breakoutRunner.print(">", position - vector2(30, 0));
    }
    breakoutRunner.print(options[index], position);
    position.y -= 40;
  }
}

void MenuState::onKeyUp(BreakoutRunner &breakoutRunner, unsigned int key, unsigned int keyModifier) {
  switch (key) {
  case SDLK_UP:
    currentSelection = (currentSelection + 1) % options.size();
    break;
  case SDLK_DOWN:
    currentSelection = (currentSelection - 1) % options.size();
    break;
  case SDLK_RETURN:
    if (options[currentSelection] == "New Game") {
      breakoutRunner.reset();
      breakoutRunner.setState(std::make_unique<PlayingState>());
    } else {
      breakoutRunner.getContainer().stop();
    }
    break;
  case SDLK_ESCAPE:
    breakoutRunner.setState(std::make_unique<PlayingState>());
    break;
  }
}

void PlayingState::enter(BreakoutRunner &runner) {
  runner.unfreeze();
}

void PlayingState::onKeyDown(BreakoutRunner &breakoutRunner, unsigned int key, unsigned int keyModifier) {
  switch (key) {
  case SDLK_LEFT:
    case SDLK_A:
    breakoutRunner.setPaddleVelocity(vector(-PADDLE_VELOCITY, 0, 0));
    break;
  case SDLK_RIGHT:
    case SDLK_D:
    breakoutRunner.setPaddleVelocity(vector(PADDLE_VELOCITY, 0, 0));
    break;
//    case SDLK_BACKSPACE:
//      breakoutRunner.reset();
//      break;
  }
}

void PlayingState::onKeyUp(BreakoutRunner &breakoutRunner, unsigned int key, unsigned int keyModifier) {
  switch (key) {
  case SDLK_LEFT:
    case SDLK_A:
    case SDLK_RIGHT:
    case SDLK_D:
    breakoutRunner.setPaddleVelocity(vector(0, 0, 0));
    break;
  case SDLK_ESCAPE:
    breakoutRunner.setState(std::make_unique<MenuState>());
    break;

  }
}

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
