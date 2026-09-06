#include "Breakout.h"

class Breakout: public Playground {
public:
  Breakout(const String &resourcesBasePath) : Playground(resourcesBasePath) {
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
