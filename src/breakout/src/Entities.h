#include "DefaultRenderer.h"

class Entity {
public:
  virtual void draw(DefaultRenderer &renderer) = 0;
};

class Background: public Entity {
  real width;
  real height;
  TextureResource *texture;


public:
  Background(unsigned int width, unsigned int height) : texture(texture) {
    this->width = width;
    this->height = height;
  }

  void resize(unsigned int width, unsigned int height) {
    this->width = width;
    this->height = height;
  }

  void setTexture(TextureResource &texture) {
    this->texture = &texture;

  }

  void draw(DefaultRenderer &renderer) override {
    const TextureResource *previousTexture = renderer.setTexture(texture);
    renderer.drawBox(matriz_4x4::traslacion(0,  0, 0), vector(width, height, 1));
    renderer.setTexture(previousTexture);
  }
};

class Brick: public Entity {
  unsigned int hitsLeft;
};
