#include "DefaultRenderer.h"

class Entity {
protected:
  real width = 0;
  real height = 0;
public:
  Entity(real width, real height){
    resize(width, height);
  }

  void resize(unsigned int width, unsigned int height) {
    this->width = width;
    this->height = height;
  }

  virtual void initialize() {};
  virtual void draw() {};
  virtual ~Entity() {};
};

class Background: public Entity {
  TextureResource *texture = null;
  ResourceManager &resourceManager;
  DefaultRenderer &renderer;
public:
  Background(ResourceManager &resourceManager, DefaultRenderer &renderer) :
    Entity(0, 0),
    resourceManager(resourceManager),
    renderer(renderer) {
  }

  void setTexture(TextureResource &texture) {
    this->texture = &texture;

  }

  void initialize() override {
    texture = (TextureResource *)resourceManager.load("background.png", MimeTypes::TEXTURE);
  }

  void draw() override {
    const TextureResource *previousTexture = renderer.setTexture(texture);

    renderer.drawBox(matriz_4x4::traslacion(0,  0, 0), vector(width, height, 1));
    renderer.setTexture(previousTexture);
  }
};

class Border: public Entity {
  GeometryRenderer &renderer;
  //Level limits
  AABB &top;
  AABB &bottom;
  AABB &left;
  AABB &right;

  real halfDepth = 0;

public:
  Border(ParticleManager &particleManager, GeometryRenderer &renderer, real depth) : Entity(0, 0),
    top((AABB &)particleManager.addScenery(std::make_unique<AABB>(vector(0, 0, 0), vector(2, 1, 0.1)))),
    bottom((AABB &)particleManager.addScenery(std::make_unique<AABB>(vector(0, 0, 0), vector(2, 1, 0.1)))),
    left((AABB &)particleManager.addScenery(std::make_unique<AABB>(vector(0, 0, 0), vector(1, 2, 0.1)))),
    right((AABB &)particleManager.addScenery(std::make_unique<AABB>(vector(0, 0, 0), vector(1, 2, 0.1)))),
    halfDepth(depth * 0.5),
    renderer(renderer) {
  }
  void resize(unsigned int width, unsigned int height) {
    Entity::resize(width, height);

    top.setOrigin(vector(0, height * 0.51 + halfDepth, 0));
    top.setHalfSizes(vector(width * 0.5, halfDepth, halfDepth));

    bottom.setOrigin(vector(0, height * -0.51 - halfDepth, 0));
    bottom.setHalfSizes(vector(width * 0.5, halfDepth, halfDepth));

    left.setOrigin(vector(width * -0.51 - halfDepth, 0, 0));
    left.setHalfSizes(vector(halfDepth, height * 0.5, halfDepth));

    right.setOrigin(vector(width * 0.51 + + halfDepth, 0, 0));
    right.setHalfSizes(vector(halfDepth, height *  0.5, halfDepth));
  }

  void draw() override {
    renderer.render(top);
    renderer.render(bottom);
    renderer.render(left);
    renderer.render(right);
  }
};

class Level: public Entity {

};
class Brick: public Entity {
  unsigned int hitsLeft;
};
