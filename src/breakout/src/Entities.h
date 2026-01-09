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

  virtual void initialize() = 0;
  virtual void draw(DefaultRenderer &renderer) = 0;
};

class Background: public Entity {
  TextureResource *texture;
public:
  Background() : Entity(0, 0), texture(texture) {
  }

  void setTexture(TextureResource &texture) {
    this->texture = &texture;

  }

  void initialize() override {

  }

  void draw(DefaultRenderer &renderer) override {
    const TextureResource *previousTexture = renderer.setTexture(texture);
    renderer.drawBox(matriz_4x4::traslacion(0,  0, 0), vector(width, height, 1));
    renderer.setTexture(previousTexture);
  }
};

class Border: public Entity {
  void resize(unsigned int width, unsigned int height) {
    this->width = width;
    this->height = height;
  }

  void draw(DefaultRenderer &renderer) override {
  }
};

class Level: public Entity {

};
class Brick: public Entity {
  unsigned int hitsLeft;
};
