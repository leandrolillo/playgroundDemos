#include "Breakout.h"

/**************
 * Menu State *
 **************/
void MenuState::enter(BreakoutRunner &runner) {
  runner.freeze();
}

void MenuState::update(BreakoutRunner &breakoutRunner) {
//  vector2 position = vector2(
//      (breakoutRunner.getVideo().getScreenWidth() - 100) / 2,
//      (breakoutRunner.getVideo().getScreenHeight() - 100) / 2);

  vector2 position = vector2(-100 , 100); //TODO: review SpriteRenderer & TextRenderer coordinate frames

  for (int index = 0; index < options.size(); index++) {
    if (index == currentSelection) {
      breakoutRunner.print(">", position - vector2(30, 0));
    }
    breakoutRunner.print(options[index], position);
    position.y -= 40;
  }
}

void MenuState::onKeyDown(BreakoutRunner &breakoutRunner, unsigned int key, unsigned int keyModifier) {
  switch (key) {
  case SDLK_UP:
    currentSelection = (currentSelection - 1 + options.size()) % options.size();
    break;
  case SDLK_DOWN:
    currentSelection = (currentSelection + 1) % options.size();
    break;
  case SDLK_RETURN:
    if (options[currentSelection] == "New Game") {
      //breakoutRunner.reset();
      breakoutRunner.setState(std::make_unique<StandByState>(breakoutRunner.getLevelNames()[0], true));
    } else if (options[currentSelection] == "Continue") {
      breakoutRunner.setState(std::make_unique<PlayingState>()); //TODO: Handle case were previous state was standby
    } else {
      breakoutRunner.exit();
    }
    break;
  case SDLK_ESCAPE:
    if(withContinue) {
      breakoutRunner.setState(std::make_unique<PlayingState>()); //TODO: Handle case were previous state was standby
    } else {
      breakoutRunner.exit();
    }
    break;
  }
}

/*****************
 * Standby State *
 *****************/

void StandByState::enter(BreakoutRunner &runner) {
  runner.unfreeze();

  //TODO: disable ball so that physics manager skips it.
  runner.setPaddleVelocity(vector(0, 0, 0));

  if(!this->levelName.empty()) {
    runner.setCurrentLevelName(this->levelName, this->forceLoad);
  }
}

void StandByState::update(BreakoutRunner &breakoutRunner) {
  vector2 position = vector2(-100 , 100);

  String message = this->levelName; //remove .json from message

  auto lastIndex = message.rfind(".");
  if(lastIndex != String::npos) {
    message.erase(lastIndex);
  }
  std::replace(message.begin(), message.end(), '-', ' ');

  breakoutRunner.print(message, position);

  breakoutRunner.syncBallWithPaddle();
}

void StandByState::onKeyDown(BreakoutRunner &breakoutRunner, unsigned int key, unsigned int keyModifier) {
  switch (key) {
  case SDLK_LEFT:
    case SDLK_A:
    breakoutRunner.setPaddleVelocity(vector(-PADDLE_VELOCITY, 0, 0));
    break;
  case SDLK_RIGHT:
    case SDLK_D:
    breakoutRunner.setPaddleVelocity(vector(PADDLE_VELOCITY, 0, 0));
    break;
  }
}


void StandByState::onKeyUp(BreakoutRunner &breakoutRunner, unsigned int key, unsigned int keyModifier) {
  switch (key) {
  case SDLK_SPACE:
    breakoutRunner.playBall();
    breakoutRunner.setState(std::make_unique<PlayingState>());
    break;
  case SDLK_ESCAPE:
    breakoutRunner.setState(std::make_unique<MenuState>(true));
    break;
  case SDLK_LEFT:
  case SDLK_A:
  case SDLK_RIGHT:
  case SDLK_D:
    breakoutRunner.setPaddleVelocity(vector(0, 0, 0));
    break;
  }
}

/*****************
 * Playing State *
 *****************/

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
    breakoutRunner.setState(std::make_unique<MenuState>(true));
    break;

  }
}


/*****************
 * Transitioning State *
 *****************/

void TransitioningState::enter(BreakoutRunner &runner) {
  runner.freeze();
  this->elapsedTime=0;
}

void TransitioningState::update(BreakoutRunner &breakoutRunner) {
  elapsedTime += breakoutRunner.getStopWatch().getElapsedTime();
  if(elapsedTime >= this->timeout) {
    breakoutRunner.setState(std::move(this->nextState));
  }
  vector2 position = vector2(-100 , 100);
  breakoutRunner.print(this->message, position);
}

void TransitioningState::onKeyDown(BreakoutRunner &breakoutRunner, unsigned int key, unsigned int keyModifier) {
  if(elapsedTime > 1 || elapsedTime > this->timeout) {
    breakoutRunner.setState(std::move(this->nextState));
  }
}


class Breakout: public Playground {
public:
  using Playground::Playground; //inherit constructors

  void initializePlayground() override {
    Playground::initializePlayground();
    this->addRunner<OpenGLRunner>();
    this->addRunner<OpenALRunner>();
    this->addRunner<PhysicsRunner>();
    this->addRunner<BreakoutRunner>();
  }
};

int main(int argc, char **argv) {
  String repository = Paths::add(Paths::getDirname(argv[0]), "resources"); //assumes executable lies in playground/target folder
  Breakout playground(repository);
  playground.withName("Breakout");
  printf("\n\nRunning [%s]\n", playground.toString().c_str());
  playground.run();
  printf("done\n");
  return 0;
}
