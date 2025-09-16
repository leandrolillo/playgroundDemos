#include "../../base/BaseDemo.h"
#include "GeometryRenderer.h"

constexpr real zMin = -100;
constexpr real zMax = 100;

class MinkowskiDifferenceRunner: public BaseDemoRunner {
  GeometryRenderer geometryRenderer;

  AABB left {vector(0, 0, 0), vector(100, 100, 10)};
  AABB right {vector(0, 0, 0), vector(100, 200, 10)};

  const MaterialResource red {vector(1, 0, 0), vector(1, 0, 0), vector(1, 0, 0), 1.0, 0.5};
  const MaterialResource green {vector(0, 1, 0), vector(0, 1, 0), vector(0, 1, 0), 0.5};
  const MaterialResource blue {vector(0, 0, 1), vector(0, 0, 1), vector(0, 0, 1), 0.5};
  const MaterialResource black {vector(0, 0, 0), vector(0, 0, 0), vector(0, 0, 0), 1.0, 0.2};
  const MaterialResource white {vector(1, 1, 1), vector(1, 1, 1), vector(1, 1, 1), 1.0, 0.2};

public:
  MinkowskiDifferenceRunner() : geometryRenderer(defaultRenderer) {
  }

  bool initialize() override {
    if (!BaseDemoRunner::initialize()) {
      return false;
    }

    reset();

    return true;
  }


  bool reset() {
    camera.setPosition(vector(0, 0, 2));

    left.setOrigin(vector(0, 0, 0));
    right.setOrigin(vector(0, 0, 0));
  }

  LoopResult doLoop() override {
    defaultRenderer.setMaterial(&blue);
    geometryRenderer.render(left);

    defaultRenderer.setMaterial(&green);
    geometryRenderer.render(right);

    AABB md = left.minkowskiDifference(right);
    defaultRenderer.setMaterial(&white);
    geometryRenderer.render(md);

    return LoopResult::CONTINUE;
  }

  virtual void onResize(unsigned int height, unsigned int width) override {
    //camera.setPerspectiveProjectionFov(45.0, (GLfloat) width / (GLfloat) height, 0.1, zMax);
    camera.setOrthographicProjection(height, width, zMin, zMax);
  }

  virtual void onMouseWheel(int wheel) override {
    camera.setPosition(camera.getPosition() - vector(0.0f, 0.0f, wheel));
    logger->info("Camera [%s]", camera.getPosition().toString().c_str());
  }

  virtual void onMouseMove(int x, int y, int dx, int dy, unsigned int buttons) override {
    if (dx != 0 || dy != 0) {
      ;
    }
  }

  virtual void onKeyUp(unsigned int key, unsigned int keyModifier) override {
  }

  virtual void onKeyDown(unsigned int key, unsigned int keyModifier) override {
    switch (key) {
    case SDLK_LEFT:
      left.setOrigin(left.getOrigin() + vector(-10, 0, 0));
      break;
    case SDLK_RIGHT:
      left.setOrigin(left.getOrigin() + vector(10, 0, 0));
      break;
    case SDLK_UP:
      left.setOrigin(left.getOrigin() + vector(0, 10, 0));
      break;
    case SDLK_DOWN:
      left.setOrigin(left.getOrigin() + vector(0, -10, 0));
      break;

    case SDLK_A:
      right.setOrigin(right.getOrigin() + vector(-10, 0, 0));
      break;
    case SDLK_D:
      right.setOrigin(right.getOrigin() + vector(10, 0, 0));
      break;
    case SDLK_W:
      right.setOrigin(right.getOrigin() + vector(0, 10, 0));
      break;
    case SDLK_S:
      right.setOrigin(right.getOrigin() + vector(0, -10, 0));
      break;

    case SDLK_SPACE:
        reset();
        break;
      default:
        BaseDemoRunner::onKeyUp(key, keyModifier);
        break;
    }
  }


};

class MinkowskiDifference: public Playground {
public:
  MinkowskiDifference(const String &resourcesBasePath) :
      Playground(resourcesBasePath) {
  }
  void initializePlayground() override {
    Playground::initializePlayground();
    this->addRunner(std::make_unique<OpenGLRunner>());
    this->addRunner(std::make_unique<AudioRunner>());
    this->addRunner(std::make_unique<MinkowskiDifferenceRunner>());
  }
};

int main(int argc, char **argv) {
  String repository = Paths::add(Paths::getDirname(argv[0]), "resources"); //assumes executable lies in playground/target folder
  MinkowskiDifference playground(repository);
  playground.withName("MinkowskiDifference");
  printf("\n\nRunning playground [%s]\n", playground.toString().c_str());
  playground.run();
  printf("done\n");
  return 0;
}
