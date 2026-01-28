#include "DefaultRenderer.h"
#include "BreakoutLevel.h"

class Entity {
protected:
  real width = 0;
  real height = 0;
public:
  Entity(real width, real height){
    resize(width, height);
  }

  virtual void resize(unsigned int width, unsigned int height) {
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
  void resize(unsigned int width, unsigned int height) override {
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

class Object : public Entity {
protected:
  Particle &particle;
  GeometryRenderer &renderer;
public:
  Object(Particle &particle, GeometryRenderer &renderer) : Entity(0, 0), particle(particle), renderer(renderer) {
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

  void draw() override {
    renderer.render(particle.getBoundingVolume());
  }
};

class Paddle : public Object {
public:
  Paddle(ParticleManager &particleManager, GeometryRenderer &renderer, real width, real height, real depth) :
    Object(particleManager.addParticle(std::make_unique<Particle>(std::make_unique<AABB>(vector(0, 0, 0), vector(width, height, depth * 0.5)))),
        renderer) {
    particle.setInverseMass(0.0);
  }

  void resize(unsigned int width, unsigned int height) override {
    Entity::resize(width, height);

    particle.setPosition(vector(
        particle.getPosition().x,
        height * -0.5 + 2.0 * ((AABB &)particle.getBoundingVolume()).getHalfSizes().y,
        particle.getPosition().z));
  }

  const vector &getHalfSizes() {
    return ((AABB &)particle.getBoundingVolume()).getHalfSizes();
  }
};

class Ball : public Object {
public:

  Ball(ParticleManager &particleManager, GeometryRenderer &renderer, real radius) :
    Object(particleManager.addParticle(std::make_unique<Particle>(std::make_unique<Sphere>(vector(0, 0, 0), radius))),
        renderer)
  {
    particle.setDamping(1.0);
  }

  real getRadius() {
    return ((Sphere &)particle.getBoundingVolume()).getRadius();
  }
};

class Brick: public Entity {
  unsigned int i;
  unsigned int j;
  unsigned int hitsLeft;
  AABB &boundingBox;
public:
  Brick(const Brick &brick) = delete;
  Brick(Brick &&brick) = default;

  Brick(unsigned int i, unsigned int j, unsigned int hitsLeft, AABB &boundingBox) :
    Entity(2, 2),
    boundingBox(boundingBox),
    hitsLeft(hitsLeft) {

    this->i = i;
    this->j = j;
    boundingBox.setOnCollisionHandler([this](GeometryContact &contact) {
      this->onCollision();
//      this->hitsLeft--;
//      if(contact.getGeometryA()) {
//        contact.getGeometryA()->setStatus(this->hitsLeft <= 0);
//      }
      //this->boundingBox.setStatus(this->hitsLeft <= 0);
      //this->boundingBox.setStatus(false);
    });
  }

  void onCollision() {
    this->hitsLeft--;
    this->setStatus(this->hitsLeft <= 0);
  }

  unsigned int getI() {
    return this->i;
  }

  unsigned int getJ() {
    return this->j;
  }

  void setPosition(const vector &position) {
    this->boundingBox.setPosition(position);
  }

  void setHalfSizes(const vector &halfSizes) {
    this->boundingBox.setHalfSizes(halfSizes);
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
};

class Level: public Entity {
  unsigned int rows = 0;
  unsigned int columns = 0;
  real halfDepth;

  ResourceManager &resourceManager;
  ParticleManager &particleManager;
  GeometryRenderer &renderer;

  std::vector<std::unique_ptr<Brick>>bricks;

public:
  Level(ResourceManager &resourceManager, ParticleManager &particleManager, GeometryRenderer &renderer, real depth) :
    Entity(0, 0),
    resourceManager(resourceManager),
    particleManager(particleManager),
    renderer(renderer),
    halfDepth(depth * 0.5)
  {


  }

  void resize(unsigned int width, unsigned int height) override {
    Entity::resize(width, height);
    int margin = 1;

    int brick_height = height * 0.75 / rows - margin;
    int brick_width = width / columns - margin;

    int left = -((int)width >> 1) + (width - (brick_width + margin) * columns) / 2; //centered
    int top = ((int)height >> 1) - brick_height + margin;


    for(auto &brick : bricks) {
      brick->setHalfSizes(vector((int)brick_width >> 1, (int)brick_height >> 1, halfDepth));
      brick->setPosition(vector(left + (int)brick->getJ() * (margin + (int)brick_width), top - (int)brick->getI() * (margin + (int)brick_height), -halfDepth));
    }

//    for(unsigned int i = 0; i < level->getRows(); i++) {
//      for(unsigned int j = 0; j < level->getColumns(); j++) {
//        this->bricks[i * level->getColumns() + j]->setHalfSizes(vector((int)brick_width >> 1, (int)brick_height >> 1, halfDepth));
//        this->bricks[i * level->getColumns() + j]->setPosition(vector(left + (int)j * (margin + (int)brick_width), top - (int)i * (margin + (int)brick_height), -halfDepth));
//      }
//    }
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
            this->bricks.push_back(std::make_unique<Brick>(i, j, levelDescription->getBrickAt(i, j), (AABB &)particleManager.addScenery(std::make_unique<AABB>(vector(0, 0, 0), vector(1, 1, 1)))));
          }
        }
      }
    }
  }

  void draw() override {
    for(auto &brick : bricks) {
      if(brick->getStatus()) {
        renderer.render(brick->getBoundingBox());
      }
    }
  }

};

