class BreakoutState {
public:
  virtual void enter(BreakoutRunner &breakoutRunner){}
  virtual BreakoutState &update(BreakoutRunner &breakoutRunner) { return this; }
  virtual BreakoutState &onKeyUp(BreakoutRunner &breakoutRunner, unsigned int key, unsigned int keyModifier) { return *this; }
  virtual BreakoutState &onKeyDown(BreakoutRunner &breakoutRunner, unsigned int key, unsigned int keyModifier) { return *this; }
  virtual ~BreakoutState() {}
};

class MenuState : public BreakoutState {
  unsigned int currentSelection = 0;
  std::vector<String>options {"New Game", "Exit"};
public:
  virtual void update(BreakoutRunner &breakoutRunner) {

    vector2 position=vector2(100, 100);
    for(int index = 0; index < options.size(); index++) {
      if(index == currentSelection) {
        breakoutRunner.textRenderer.print(">", position - vector(10, 0));
      }
      breakoutRunner.textRenderer.print(options[index], position);
      position.y -= 20;
    }
  }

  BreakoutState &onKeyUp(BreakoutRunner &breakoutRunner, unsigned int key, unsigned int keyModifier) override {
    switch (key) {
      case SDLK_UP:
        currentSelection = (currentSelection + 1) % options.size();
      break;
      case SDLK_DOWN:
        currentSelection = (currentSelection - 1) % options.size();
      break;
      case SDLK_ENTER:
        if(options[currentSelection] == "New Game") {
          breakoutRunner.reset();
          return new PlayingState();
        }
      break;
      case SDLK_ESC:
        return new PlayingState();
      break;
    }

    return *this;
  }
};

class PlayingState : public BreakoutState {
public:
  BreakoutState &onKeyDown(BreakoutRunner &breakoutRunner, unsigned int key, unsigned int keyModifier) override {
    switch (key) {
      case SDLK_LEFT:
      case SDLK_A:
        breakoutRunner.paddle.setVelocity(vector(-PADDLE_VELOCITY, 0, 0));
        break;
      case SDLK_RIGHT:
      case SDLK_D:
        breakoutRunner.paddle.setVelocity(vector(PADDLE_VELOCITY, 0, 0));
        break;
      case SDLK_SPACE:
        breakoutRunner.physics->setEnabled(!breakoutRunner.physics->getEnabled());
        break;
      case SDLK_BACKSPACE:
        breakoutRunner.reset();
        break;
    }

    return *this;
  }

  BreakoutState &onKeyUp(BreakoutRunner &breakoutRunner, unsigned int key, unsigned int keyModifier) override {
    switch (key) {
      case SDLK_LEFT:
      case SDLK_A:
      case SDLK_RIGHT:
      case SDLK_D:
        paddle.setVelocity(vector(0, 0, 0));
        break;
    }

    return this;
  }
};

class LevelTransition : public BreakoutState {

};
