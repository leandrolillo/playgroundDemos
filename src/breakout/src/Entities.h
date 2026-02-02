#include "BreakoutLevel.h"
#include "SpriteRenderer.h"

class Entity {
protected:
  ResourceManager &resourceManager;
  real width = 0;
  real height = 0;
public:
  Entity(ResourceManager &resourceManager):
    resourceManager(resourceManager)
  {
  }

  virtual void onScreenResize(unsigned int width, unsigned int height) {
    this->width = width;
    this->height = height;
  }

  virtual void initialize() {};
  virtual void draw(SpriteRenderer &renderer) {};
  virtual ~Entity() {};
};

class Background: public Entity {
  Sprite sprite;
public:
  Background(ResourceManager &resourceManager) :
    Entity(resourceManager),
    sprite(null, vector2(0, 0), vector2(0, 0), 0)
  {
  }

  void onScreenResize(unsigned int width, unsigned int height) override {
    sprite.setPosition(vector2(width * -0.5, height * -0.5));
    sprite.setSize(vector2(width * 0.5, height * 0.5));
  }

  void initialize() override {
    sprite.setTexture((TextureResource *)resourceManager.load("background.png", MimeTypes::TEXTURE));
  }

  void draw(SpriteRenderer &renderer) override {
    renderer.draw(sprite);
    renderer.draw(*sprite.getTexture(), sprite.getPosition() + sprite.getSize(), sprite.getSize(), 0);
  }
};

class Border: public Entity {
  TextureResource *texture = null;

  //Level limits
  AABB &top;
  AABB &bottom;
  AABB &left;
  AABB &right;

  real halfDepth = 0;

public:
  Border(ResourceManager &resourceManager, ParticleManager &particleManager, real depth) :
    Entity(resourceManager),
    top((AABB &)particleManager.addScenery(std::make_unique<AABB>(vector(0, 0, 0), vector(2, 1, 0.1)))),
    bottom((AABB &)particleManager.addScenery(std::make_unique<AABB>(vector(0, 0, 0), vector(2, 1, 0.1)))),
    left((AABB &)particleManager.addScenery(std::make_unique<AABB>(vector(0, 0, 0), vector(1, 2, 0.1)))),
    right((AABB &)particleManager.addScenery(std::make_unique<AABB>(vector(0, 0, 0), vector(1, 2, 0.1)))),
    halfDepth(depth * 0.5)
  {
  }
  void onScreenResize(unsigned int width, unsigned int height) override {
    top.setOrigin(vector(0, height * 0.51 + halfDepth, 0));
    top.setHalfSizes(vector(width * 0.5, halfDepth, halfDepth));

    bottom.setOrigin(vector(0, height * -0.51 - halfDepth, 0));
    bottom.setHalfSizes(vector(width * 0.5, halfDepth, halfDepth));

    left.setOrigin(vector(width * -0.51 - halfDepth, 0, 0));
    left.setHalfSizes(vector(halfDepth, height * 0.5, halfDepth));

    right.setOrigin(vector(width * 0.51 + + halfDepth, 0, 0));
    right.setHalfSizes(vector(halfDepth, height *  0.5, halfDepth));
  }

  void initialize() override {
    texture = (TextureResource *)resourceManager.load("background.png", MimeTypes::TEXTURE);
  }

  void draw(SpriteRenderer &renderer) override {
    renderer.draw(*texture, top.getTopLeft().xy(), top.getSize().xy(), 0);
    renderer.draw(*texture, bottom.getTopLeft().xy(), bottom.getSize().xy(), 0);
    renderer.draw(*texture, left.getTopLeft().xy(), left.getSize().xy(), 0);
    renderer.draw(*texture, right.getTopLeft().xy(), right.getSize().xy(), 0);
  }
};

class Object : public Entity {
protected:
  Particle &particle;
  TextureResource *texture = null;
public:
  Object(ResourceManager &resourceManager, Particle &particle) :
    Entity(resourceManager),
    particle(particle) {
  }

  void setVelocity(const vector &velocity) {
    this->particle.setVelocity(velocity);
  }

  void setPosition(const vector &position) {
      this->particle.setPosition(position);
  }

  const vector &getPosition() const {
    return this->particle.getPosition();
  }

//  void draw(SpriteRenderer &renderer) override {
//    //renderer.render(particle.getBoundingVolume());
//  }
};

class Paddle : public Object {
public:
  Paddle(ResourceManager &resourceManager, ParticleManager &particleManager, real width, real height, real depth) :
    Object(resourceManager, particleManager.addParticle(std::make_unique<Particle>(std::make_unique<AABB>(vector(0, 0, 0), vector(width, height, depth * 0.5)))))
  {
    particle.setInverseMass(0.0);
  }

  void onScreenResize(unsigned int width, unsigned int height) override {
    particle.setPosition(vector(
        particle.getPosition().x,
        height * -0.5 + 2.0 * ((AABB &)particle.getBoundingVolume()).getHalfSizes().y,
        particle.getPosition().z));
  }

  const vector &getSize() {
    return ((AABB &)particle.getBoundingVolume()).getSize();
  }

  void initialize() override {
    texture = (TextureResource *)resourceManager.load("images/paddle.png", MimeTypes::TEXTURE);
  }

  void draw(SpriteRenderer &renderer) override {
    AABB &boundingBox = (AABB &)particle.getBoundingVolume();

    renderer.draw(*texture, boundingBox.getTopLeft().xy(), boundingBox.getSize().xy(), 0);
  }
};

class Ball : public Object {
public:

  Ball(ResourceManager &resourceManager, ParticleManager &particleManager, real radius) :
    Object(resourceManager, particleManager.addParticle(std::make_unique<Particle>(std::make_unique<Sphere>(vector(0, 0, 0), radius))))
  {
    particle.setDamping(1.0);
  }

  real getRadius() {
    return ((Sphere &)particle.getBoundingVolume()).getRadius();
  }

  void initialize() override {
    texture = (TextureResource *)resourceManager.load("images/awesomeface.png", MimeTypes::TEXTURE);
  }

  void draw(SpriteRenderer &renderer) override {
    Sphere &boundingSphere = (Sphere &)particle.getBoundingVolume();

    vector2 halfSizes(boundingSphere.getRadius(), boundingSphere.getRadius());

    renderer.draw(*texture, boundingSphere.getOrigin().xy() - halfSizes, halfSizes * 2, 0, vector3(1.0, 1.0, 1.0));
  }
};

class Brick: public Entity { //could be an object if added as particle instead of scenery
  ParticleManager &particleManager;

  unsigned int i;
  unsigned int j;
  unsigned int hitsLeft;
  AABB &boundingBox;

  TextureResource *texture = null;
  TextureResource *unbreakableTexture = null;

  inline static const auto brickColors = std::array {vector3(1.0, 0.0, 0.0), vector3(0.0, 1.0, 0.0), vector3(0.0, 0.0, 1.0), vector3(1.0, 1.0, 0.0), vector3(1.0, 0.0, 1.0), vector3(0.0, 1.0, 1.0)};
public:
  Brick(ResourceManager &resourceManager, ParticleManager &particleManager, unsigned int i, unsigned int j, unsigned int hitsLeft) :
    Entity(resourceManager),
    particleManager(particleManager),
    boundingBox((AABB &)particleManager.addScenery(std::make_unique<AABB>(vector(0, 0, 0), vector(1, 1, 1)))),
    hitsLeft(hitsLeft) {

    this->i = i;
    this->j = j;
    boundingBox.setOnCollisionHandler([this](GeometryContact &contact) {
      this->onCollision();
    });
  }

  void onCollision() {
    this->hitsLeft--;
    this->setStatus(this->hitsLeft > 0);
  }

  unsigned int getI() {
    return this->i;
  }

  unsigned int getJ() {
    return this->j;
  }

  void setTopLeft(const vector &position) {
    this->boundingBox.setTopLeft(position);
  }

  void setSize(const vector &size) {
    this->boundingBox.setHalfSizes(size * 0.5);
  }

  AABB &getBoundingBox() const {
    return this->boundingBox;
  }

  bool getStatus() const {
    return this->boundingBox.getStatus();
  }

  void setStatus(bool status) {
    this->boundingBox.setStatus(status);
  }

  void initialize() override {
      texture = (TextureResource *)resourceManager.load("images/block.png", MimeTypes::TEXTURE);
      unbreakableTexture = (TextureResource *)resourceManager.load("images/block_solid.png", MimeTypes::TEXTURE);
    }

  void draw(SpriteRenderer &renderer) override {
    if(this->getStatus()) {
      if(this->hitsLeft > 0) {
        renderer.draw(*texture, boundingBox.getTopLeft().xy(), boundingBox.getSize().xy(), 0, brickColors[this->hitsLeft % brickColors.size()]);
      } else if (hitsLeft < 0) {
        renderer.draw(*unbreakableTexture, boundingBox.getTopLeft().xy(), boundingBox.getSize().xy(), 0);
      }
    }
  }
};

class Level: public Entity {
  unsigned int rows = 0;
  unsigned int columns = 0;
  real halfDepth;

  ParticleManager &particleManager;

  std::vector<std::unique_ptr<Brick>>bricks;

public:
  Level(ResourceManager &resourceManager, ParticleManager &particleManager, real depth) :
    Entity(resourceManager),
    particleManager(particleManager),
    halfDepth(depth * 0.5)
  {


  }

  void onScreenResize(unsigned int width, unsigned int height) override {
    int margin = 1;

    int brick_height = height * 0.75 / rows - margin;
    int brick_width = width / columns - margin;

    int left = -((int)width >> 1) + (width - (brick_width + margin) * columns) / 2; //centered
    int top = ((int)height >> 1) - brick_height + margin;


    for(auto &brick : bricks) {
      brick->setSize(vector(brick_width, brick_height, halfDepth * 2));
      brick->setTopLeft(vector(
                          left + (int)brick->getJ() * (margin + (int)brick_width),
                          top - (int)brick->getI() * (margin + (int)brick_height),
                          -halfDepth));
    }
  }

  void initialize() override {
    for(auto &brick : bricks) {
      particleManager.removeScenery(brick->getBoundingBox());
    }
    this->bricks.clear();

    BreakoutLevel *levelDescription = (BreakoutLevel *)resourceManager.load("level-0.json", BreakoutLevel::MimeType);
    if(levelDescription) {
      this->rows = levelDescription->getRows();
      this->columns = levelDescription->getColumns();

      for(unsigned int i = 0; i < levelDescription->getRows(); i++) {
        for(unsigned int j = 0; j < levelDescription->getColumns(); j++) {
          if(levelDescription->getBrickAt(i, j) != 0) {
            this->bricks.push_back(std::make_unique<Brick>(resourceManager, particleManager, i, j, levelDescription->getBrickAt(i, j)));
          }
        }
      }
    }
  }

  void draw(SpriteRenderer &renderer) override {
    for(auto &brick : bricks) {
      brick->draw(renderer);
    }
  }

};

