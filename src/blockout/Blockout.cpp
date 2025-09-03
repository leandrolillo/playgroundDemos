#include "../base/BaseDemo.h"

class BlockoutRunner: public BaseDemoRunner {
public:
  bool initialize() override {
    if (!BaseDemoRunner::initialize()) {
      return false;
    }

    return true;
  }

  LoopResult doLoop() override {
    defaultRenderer.clear();
    return LoopResult::CONTINUE;
  }
};

class Blockout: public Playground {
public:
  Blockout(const String &resourcesBasePath) :
      Playground(resourcesBasePath) {
  }
  void initializePlayground() override {
    Playground::initializePlayground();
    this->addRunner(std::make_unique<OpenGLRunner>());
    this->addRunner(std::make_unique<AudioRunner>());
    this->addRunner(std::make_unique<BlockoutRunner>());
  }
};

int main(int argc, char **argv) {
  String repository = Paths::add(Paths::getDirname(argv[0]), "resources"); //assumes executable lies in playground/target folder
  Blockout playground(repository);
  playground.withName("Blockout");
  printf("\n\nRunning playground [%s]\n", playground.toString().c_str());
  playground.run();
  printf("done\n");
  return 0;
}
