#pragma once

#include "Playground.h"

class BreakoutRunner;

class BreakoutState {
public:
  virtual void enter(BreakoutRunner &breakoutRunner){}
  virtual void update(BreakoutRunner &breakoutRunner) {}
  virtual bool onKeyUp(BreakoutRunner &breakoutRunner, unsigned int key, unsigned int keyModifier) { return true; } //return true if handled the event
  virtual bool onKeyDown(BreakoutRunner &breakoutRunner, unsigned int key, unsigned int keyModifier) { return true; } //return true if handled the event
  virtual ~BreakoutState() {}
};


/**
 * Game states:
 * - Menu
 *    - welcome (new game / exit). No "continue" option. Escape exits the game. New game -> standby at level 0
 *    - paused (similar to welcome but with additional continue option). Continue (or ESC key) go back to previous {playing or standby}
 * - standby (ball is on paddle, label with current level. Can move paddle with arrows. Can launch ball with space)
 * - playing (playing ball. Escape goes to menu)
 * - transitioning state (display message, timeout based, with pointer to next state. Any key to continue)
 */
class MenuState : public BreakoutState {
  int currentSelection = 0;
  bool withContinue;
  std::vector<String>options;
public:
  MenuState(bool withContinue) : withContinue(withContinue) {
    if(withContinue) {
      options = {"New Game", "Continue", "Exit"};
    } else {
      options = {"New Game", "Exit"};
    }
  }

  void enter(BreakoutRunner &runner) override;
  void update(BreakoutRunner &breakoutRunner) override;
  bool onKeyDown(BreakoutRunner &breakoutRunner, unsigned int key, unsigned int keyModifier) override;
};

class StandByState : public BreakoutState {
  String levelName;
  bool forceLoad;
public:
  StandByState(const String &levelName, bool forceLoad = false) : levelName(levelName), forceLoad(forceLoad) {
  }

  void enter(BreakoutRunner &runner) override;
  void update(BreakoutRunner &breakoutRunner) override;
  bool onKeyDown(BreakoutRunner &breakoutRunner, unsigned int key, unsigned int keyModifier) override;
  bool onKeyUp(BreakoutRunner &breakoutRunner, unsigned int key, unsigned int keyModifier) override;
};

class PlayingState : public BreakoutState {
public:
  void enter(BreakoutRunner &runner) override;
  bool onKeyDown(BreakoutRunner &breakoutRunner, unsigned int key, unsigned int keyModifier) override;
  bool onKeyUp(BreakoutRunner &breakoutRunner, unsigned int key, unsigned int keyModifier) override;
};

class TransitioningState : public BreakoutState {
  String message;
  real timeout;
  real elapsedTime=0;
  std::unique_ptr<BreakoutState> nextState;
public:
  TransitioningState(const String &message, real timeout, std::unique_ptr<BreakoutState> nextState) : message(message), timeout(timeout), nextState(std::move(nextState)) {
  }
  void enter(BreakoutRunner &runner) override;
  void update(BreakoutRunner &breakoutRunner) override;
  bool onKeyDown(BreakoutRunner &breakoutRunner, unsigned int key, unsigned int keyModifier) override;
};
