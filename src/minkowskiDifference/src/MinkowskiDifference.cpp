#include "../../base/BaseDemo.h"
#include "GeometryRenderer.h"
#include "CollisionTester.h"

constexpr real zMin = -100;
constexpr real zMax = 100;

class SelectableGeometry {
private:
  std::unique_ptr<Geometry> geometry;
  bool _isSelected {false};
public:
  SelectableGeometry(std::unique_ptr<Geometry> geometry) {
    if(geometry) {
      this->geometry = std::move(geometry);
    } else {
      throw std::invalid_argument("geometry can not be null");
    }
  }

  Geometry &getGeometry() {
    return *geometry.get();
  }

  void setPosition(const vector &position) {
    geometry->setOrigin(position);
  }

  vector getPosition() const {
    return geometry->getOrigin();
  }

  void select() {
    this->_isSelected = true;
  }

  void unselect() {
    this->_isSelected = false;
  }

  bool isSelected() const {
    return this->_isSelected;
  }
};

class MinkowskiDifferenceRunner: public BaseDemoRunner {
  CollisionTester intersectionTester;
  GeometryRenderer geometryRenderer;

  SelectableGeometry left {std::make_unique<AABB>(vector(0, 0, 0), vector(40, 40, 50))};
  SelectableGeometry right {std::make_unique<AABB>(vector(0, 0, 0), vector(40, 80, 50))};

  AABB md { vector(0, 0, 0), vector(0, 0, 0)};


  const MaterialResource red {vector(1, 0, 0), vector(1, 0, 0), vector(1, 0, 0), 1.0, 0.5};
  const MaterialResource green {vector(0, 1, 0), vector(0, 1, 0), vector(0, 1, 0), 0.5};
  const MaterialResource blue {vector(0, 0, 1), vector(0, 0, 1), vector(0, 0, 1), 0.5};
  const MaterialResource black {vector(0, 0, 0), vector(0, 0, 0), vector(0, 0, 0), 1.0, 0.2};
  const MaterialResource white {vector(1, 1, 1), vector(1, 1, 1), vector(1, 1, 1), 1.0, 0.2};

  /**
   * Mouse click and drag tracking
   */
  vector2 startPosition;

public:
  MinkowskiDifferenceRunner() : geometryRenderer(defaultRenderer) {
  }

  bool initialize() override {
    if (!BaseDemoRunner::initialize()) {
      return false;
    }

    video->enable(BLEND, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    reset();

    return true;
  }


  bool reset() {
    camera.setPosition(vector(0, 0, 2));

    left.setPosition(vector(0, 0, 0));
    right.setPosition(vector(0, 0, 0));

    md = ((AABB &)left.getGeometry()).minkowskiDifference((AABB &)right.getGeometry());

  }

  LoopResult doLoop() override {
    left.isSelected() ? defaultRenderer.setMaterial(&red) : defaultRenderer.setMaterial(&blue);
    geometryRenderer.render(left.getGeometry());

    right.isSelected() ? defaultRenderer.setMaterial(&red) : defaultRenderer.setMaterial(&green);
    geometryRenderer.render(right.getGeometry());

    md.contains(vector(0, 0, 0)) ? defaultRenderer.setMaterial(&red) : defaultRenderer.setMaterial(&white);
    geometryRenderer.render(md);

    return LoopResult::CONTINUE;
  }

  virtual void onResize(unsigned int height, unsigned int width) override {
    //camera.setPerspectiveProjectionFov(45.0, (GLfloat) width / (GLfloat) height, 0.1, zMax);
    camera.setOrthographicProjection(height, width, zMin, zMax);
  }

  void onMouseButtonDown(unsigned char button, int x, int y) override {
    if (button == SDL_BUTTON_LEFT) {
      this->startPosition = vector2(x, y);

      //TODO: We should find a way to conciliate mouse picking in persepective projection and orthographic projection.
      //Line line(vector(x, y, camera.getPosition().z), -camera.getOrientation().columna(2));

      vector origin = camera.unproject(x, y, video->getScreenWidth(), video->getScreenHeight());
      origin.z = -origin.z;
      vector direction = vector(0, 0, -1);
      Line line(origin, direction);

      if (intersectionTester.intersects(left.getGeometry(), (Geometry&) line)) {
        left.select();
      }

      if (intersectionTester.intersects(right.getGeometry(), (Geometry&) line)) {
        right.select();
      }
    }
  }

  void onMouseButtonUp(unsigned char button, int x, int y) override {
    if (button == SDL_BUTTON_LEFT) {
      left.unselect();
      right.unselect();
    }
  }


  virtual void onMouseWheel(int wheel) override {
    camera.setPosition(camera.getPosition() - vector(0.0f, 0.0f, wheel));
    logger->info("Camera [%s]", camera.getPosition().toString().c_str());
  }

  virtual void onMouseMove(int x, int y, int dx, int dy, unsigned int buttons) override {
    if (dx != 0 || dy != 0) {
      vector origin = camera.unproject(x, y, video->getScreenWidth(), video->getScreenHeight());
      origin.z = -origin.z;
      vector direction = vector(0, 0, -1);
      Line line(origin, direction);

      if (!equalsZeroAbsoluteMargin(line.getDirection().z)) {
        if(left.isSelected()) {
          vector origin = left.getPosition();
          real t = (origin.z - line.getOrigin().z) / line.getDirection().z;
          left.setPosition(line.getOrigin() + t * line.getDirection());
          md = ((AABB &)left.getGeometry()).minkowskiDifference((AABB &)right.getGeometry());
        }

        if(right.isSelected()) {
          vector origin = right.getPosition();
          real t = (origin.z - line.getOrigin().z) / line.getDirection().z;
          right.setPosition(line.getOrigin() + t * line.getDirection());
          md = ((AABB &)left.getGeometry()).minkowskiDifference((AABB &)right.getGeometry());
        }
      }
    }
  }

  virtual void onKeyUp(unsigned int key, unsigned int keyModifier) override {
  }

  virtual void onKeyDown(unsigned int key, unsigned int keyModifier) override {
    switch (key) {
    case SDLK_LEFT:
      left.setPosition(left.getPosition() + vector(-10, 0, 0));
      md = ((AABB &)left.getGeometry()).minkowskiDifference((AABB &)right.getGeometry());
      break;
    case SDLK_RIGHT:
      left.setPosition(left.getPosition() + vector(10, 0, 0));
      md = ((AABB &)left.getGeometry()).minkowskiDifference((AABB &)right.getGeometry());
      break;
    case SDLK_UP:
      left.setPosition(left.getPosition() + vector(0, 10, 0));
      md = ((AABB &)left.getGeometry()).minkowskiDifference((AABB &)right.getGeometry());
      break;
    case SDLK_DOWN:
      left.setPosition(left.getPosition() + vector(0, -10, 0));
      md = ((AABB &)left.getGeometry()).minkowskiDifference((AABB &)right.getGeometry());
      break;

    case SDLK_A:
      right.setPosition(right.getPosition() + vector(-10, 0, 0));
      md = ((AABB &)left.getGeometry()).minkowskiDifference((AABB &)right.getGeometry());
      break;
    case SDLK_D:
      right.setPosition(right.getPosition() + vector(10, 0, 0));
      md = ((AABB &)left.getGeometry()).minkowskiDifference((AABB &)right.getGeometry());
      break;
    case SDLK_W:
      right.setPosition(right.getPosition() + vector(0, 10, 0));
      md = ((AABB &)left.getGeometry()).minkowskiDifference((AABB &)right.getGeometry());
      break;
    case SDLK_S:
      right.setPosition(right.getPosition() + vector(0, -10, 0));
      md = ((AABB &)left.getGeometry()).minkowskiDifference((AABB &)right.getGeometry());
      break;
    case SDLK_SPACE:
      if(md.contains(vector(0, 0, 0))) {
        vector penetrationVector = md.closestSurfacePoint(vector(0, 0, 0));
        logger->info("Penetration Vector: %s", penetrationVector.toString().c_str());

        right.setPosition(right.getPosition() + penetrationVector);
        md = ((AABB &)left.getGeometry()).minkowskiDifference((AABB &)right.getGeometry());
      }
      break;
    case SDLK_BACKSPACE:
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
