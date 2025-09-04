#pragma once 

#include <ParticleManager.h>
#include <Geometry.h>
#include "GeometryRenderer.h"

class ParticleManagerRenderer : public GeometryRenderer {
public:
    const MaterialResource red = MaterialResource(vector(1, 0, 0), vector(1, 0, 0), vector(1, 0, 0), 1.0, 0.5);
    const MaterialResource green = MaterialResource(vector(0, 1, 0), vector(0, 1, 0), vector(0, 1, 0), 0.5);
    const MaterialResource blue = MaterialResource(vector(0, 0, 1), vector(0, 0, 1), vector(0, 0, 1), 0.5);
    const MaterialResource black {vector(0, 0, 0), vector(0, 0, 0), vector(0, 0, 0), 1.0, 0.2 };
    const MaterialResource white {vector(1, 1, 1), vector(1, 1, 1), vector(1, 1, 1), 1.0, 0.2 };
private:
    MaterialResource const *particleMaterial=&black;
    MaterialResource const *collidingParticleAMaterial=&red;
    MaterialResource const *collidingParticleBMaterial=&red;
    MaterialResource const *sceneryMaterial=&white;
    MaterialResource const *contactMaterial=&green;

    Logger *logger = LoggerFactory::getLogger("ParticleManagerRenderer");

public:
  ParticleManagerRenderer(DefaultRenderer &renderer) : GeometryRenderer(renderer)
  {    }

  void setParticleMaterial(MaterialResource *material) {
    this->particleMaterial = material;
  }

  void setCollidingParticleMaterial(MaterialResource *material) {
    this->collidingParticleAMaterial = material;
    this->collidingParticleBMaterial = material;
  }

  void setCollidingParticleAMaterial(MaterialResource *material) {
    this->collidingParticleAMaterial = material;
  }

  void setCollidingParticleBMaterial(MaterialResource *material) {
    this->collidingParticleBMaterial = material;
  }

  void setSceneryMaterial(MaterialResource *material) {
    this->sceneryMaterial = material;
  }

  void setContactMaterial(MaterialResource *material) {
    this->contactMaterial = material;
  }

  using GeometryRenderer::render;

  void render(const ParticleManager &particleManager) const {
    renderer.setMaterial(contactMaterial);
    for(auto &contact : particleManager.getContacts()) {
      this->renderContact(contact);
    }

    renderer.setMaterial(sceneryMaterial);
    for(auto &scenery : particleManager.getScenery()) {
      GeometryRenderer::render(*scenery);
    }

    for(auto &particle : particleManager.getParticles()) {
      if(particle->getStatus()) {
        renderer.setMaterial(particleMaterial);
        bool isColliding = false;
        for(auto &contact : particleManager.getContacts()) {
          if(contact.getParticleA() == particle.get()) {
            renderer.setMaterial(collidingParticleAMaterial);
            break;

          } else if (contact.getParticleB() ==  particle.get()) {
            renderer.setMaterial(collidingParticleBMaterial);
            break;
          }
        }


        GeometryRenderer::render(particle->getBoundingVolume(), isColliding);
      }
    }
  }

  void renderContact(const ParticleContact &contact) const {
        vector start = contact.getIntersection(); //contact.getParticleA()->getPosition();
        vector end = start + contact.getNormal() * contact.getPenetration();
        renderer.drawLine(matriz_4x4::identidad, start, end);
  }

};
