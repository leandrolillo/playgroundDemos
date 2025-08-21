/*
 * PhysicsDemo.h
 *
 *  Created on: Mar 22, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICSDEMO_H_
#define SRC_PHYSICSDEMO_H_

#include <iostream>
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGLRunner.h>
#include <AudioRunner.h>
#include<PhysicsRunner.h>

#include<renderers/SkyboxRenderer.h>
#include<renderers/GridRenderer.h>

#include<forces/Gravity.h>

#include<Geometry.h>

#include "base/BaseDemo.h"

constexpr unsigned int numberOfParticles = 60;

class PhysicsDemoRunner;

class BulletParticle : public Particle
{
private:
    PhysicsDemoRunner *runner = null;

public:
    BulletParticle() : Particle(new Sphere(vector(0, 0, 0), 0.1)) {

    }
    void setRunner(PhysicsDemoRunner *runner);
	void afterIntegrate(real dt);
	void onCollisionResolved(const ParticleContact &contact);
};

class PhysicsDemoRunner: public BaseDemoRunner {
	PhysicsRunner *physics = null;

    /**
     * This demo stuff
     */
	Source *gunshotSource = null;
	Source *bounceSource = null;

	std::vector<std::unique_ptr<BulletParticle>> particles;
	Gravity gravity = Gravity(vector(0.0, -9.8, 0.0));
	//Plane ground = Plane(vector(0, 0, 0), vector(0, 1, 0));
	Particle ground;
	Particle spherePlatform;
	Particle aabbPlatform;

	unsigned long to = 0;
	real invPerformanceFreq = 1.0f;

	/**
	 * Renderers - defaultRenderer inherited from base demo
	 */
	SkyboxRenderer skyboxRenderer;
	GridRenderer gridRenderer;

	TextureResource *textureResource = null;
	MaterialResource materials[3] = {MaterialResource(vector(1, 0.5, 0.5), vector(1, 0.5, 0.5), vector(1, 1, 1), 32),
	        MaterialResource(vector(0.5, 1, 0.5), vector(0.5, 1, 0.5), vector(1, 1, 1), 32),
	        MaterialResource(vector(0.5, 0.5, 1), vector(0.5, 0.5, 1), vector(1, 1, 1), 32)};

	LightResource light = LightResource(vector(0, 0, 3), vector(0.4f, 0.4f, 0.4f), vector(0.5f, 0.5f, 0.5f), vector(1, 1, 1), 1.0);
	MaterialResource material = MaterialResource(vector(0.5, 0.5, 0.5), vector(0.7, 0.7, 0.7), vector(1, 1, 1), 32);

	MeshResource *basketball = null;
public:
	PhysicsDemoRunner() :   ground(new Plane(vector(0, 0, 0), vector(0, 1, 0))),
	                        spherePlatform(new Sphere(vector(0.0, 0.0, 0.0), 0.1)),
	                        aabbPlatform(new AABB(vector(0.0, 1.0, 0.0), vector(0.5, 0.05, 0.05))) {
	}

    bool init() override {
    	BaseDemoRunner::init();

        physics = (PhysicsRunner *)this->getContainer()->getRequiredRunner(PhysicsRunner::ID);
        //physics->setPlaybackSpeed(0.3);

        gunshotSource = audio->createSource("audio/handgunfire.wav", vector(0, 0, 0), vector(0, 0, 0), false);
        bounceSource = audio->createSource("audio/twang3.wav", vector(0, 0, 0), vector(0, 0, 0), false);

        textureResource = (TextureResource *)this->getContainer()->getResourceManager()->load("images/basketball.png", MimeTypes::TEXTURE);
        basketball = (MeshResource *)this->getResourceManager()->load("geometry/basketball.json", MimeTypes::MESH);

        gridRenderer.setVideoRunner(video);
        skyboxRenderer.setVideoRunner(video);
        skyboxRenderer.setSize(200);

        defaultRenderer.setLight(&light);

        ground.setInverseMass(0.0); // this is actually the default value
        spherePlatform.setInverseMass(0.0);
        aabbPlatform.setInverseMass(0.0);

        //physics->getParticleManager()->getCollisionDetector().addScenery(&ground);
        for(int index = 0; index < numberOfParticles; index++) {
            particles.push_back(std::unique_ptr<BulletParticle>(new BulletParticle()));
            particles.back()->setStatus(false);
            particles.back()->setRunner(this);


            physics->getParticleManager()->addParticle(particles.back().get());
        }

        physics->getParticleManager()->addParticle(&spherePlatform);
        physics->getParticleManager()->addParticle(&aabbPlatform);
        physics->getParticleManager()->addParticle(&ground);

        physics->getParticleManager()->addForce(&this->gravity);

        reset();

        return true;
    }


	void reset() {
	    for(auto &particle : this->particles) {
	        particle->setStatus(false);
	    }

		video->setMousePosition(video->getScreenWidth() >> 1, video->getScreenHeight() >> 1);

		camera.setPosition(vector(1.0f, 0.0f, 5.0f));
		spherePlatform.setPosition(vector(0, 0.5, 0));
		aabbPlatform.setPosition(vector(0, 1.0, 0.0));
	}

	LoopResult doLoop() override {
		defaultRenderer.clear();
		defaultRenderer.drawAxes(matriz_4x4::identidad);
		defaultRenderer.drawLine(matriz_4x4::identidad, vector(-1, 0, 0), vector(1, 0, 0));
		defaultRenderer.drawLine(matriz_4x4::identidad, vector(0, -1, 0), vector(0, 1, 0));
		defaultRenderer.drawLine(matriz_4x4::identidad, vector(0, 0, -1), vector(0, 0, 1));


		//defaultRenderer.drawObject(matriz_4x4::traslacion(spherePlatform.getPosition()) * matriz_4x4::zoom(0.1, 0.1, 0.1), basketball);
		defaultRenderer.setTexture(textureResource);
		defaultRenderer.drawBox(matriz_4x4::traslacion(aabbPlatform.getPosition()),
						2.0 * ((AABB *)aabbPlatform.getBoundingVolume())->getHalfSizes().x,
						2.0 * ((AABB *)aabbPlatform.getBoundingVolume())->getHalfSizes().y,
						2.0 * ((AABB *)aabbPlatform.getBoundingVolume())->getHalfSizes().z);

		//defaultRenderer.drawSphere(matriz_4x4::traslacion(spherePlatform.getPosition()) * matriz_4x4::zoom(0.1, 0.1, 0.1));
		defaultRenderer.drawObject(matriz_4x4::traslacion(spherePlatform.getPosition()) * matriz_4x4::zoom(0.1, 0.1, 0.1), basketball);
		for(auto &particle : this->particles)
		{
				if(particle->getStatus() == true) {
					//defaultRenderer.setMaterial(&materials[(particleIterator - particles.begin()) % 3]);
					defaultRenderer.setTexture(textureResource);
					defaultRenderer.setMaterial(&material);
//				defaultRenderer.drawSphere(matriz_4x4::traslacion(particle->getPosition()) * matriz_4x4::zoom(0.1, 0.1, 0.1));

					defaultRenderer.drawObject(matriz_4x4::traslacion(particle->getPosition()) * matriz_4x4::zoom(0.1, 0.1, 0.1), basketball);
				}
		}
		defaultRenderer.render(camera);
		skyboxRenderer.render(camera);
		gridRenderer.render(camera);

		spherePlatform.setVelocity(vector(sin(this->getStopWatch().getTotalTime() + M_PI_2), 0, 0));
		aabbPlatform.setVelocity(vector(-sin(this->getStopWatch().getTotalTime() + M_PI_2), 0, 0));

		return LoopResult::CONTINUE;
	}

	void onCollision(BulletParticle *bulletParticle, const ParticleContact &contact) {
		//logger->info("Collision relative speed: %f - sound: %s", contact.getRelativeSpeed(), contact.getRelativeSpeed() < -0.1 ? "on" : "off");

		if(contact.getRelativeSpeed() < -0.5) {
			//logger->info("Collision relative speed: %f - sound on", contact.getRelativeSpeed());
			bounceSource->setPosition(bulletParticle->getPosition());
			audio->updateSource(bounceSource);
			audio->playSource(bounceSource);
		}
	}

	void afterIntegrate(BulletParticle *bulletParticle) {
        if(bulletParticle->getPosition().modulo() > 100) {
            bulletParticle->setStatus(false);
        }
	}

	void fire(const vector &position, bool isStatic = false) {
		BulletParticle *bullet = null;

		logger->debug("Iterating particles");
		for(auto &particle : particles)
        {
            if(!particle->getStatus()) {
                bullet = particle.get();
                break;
            }
        }

		if(bullet != null) {
			bullet->setPosition(position);
			if(isStatic) {
			    bullet->setVelocity(vector(0, 0, 0));
			    bullet->setDamping(0.99f);
			} else {
			    bullet->setVelocity(camera.getOrientation().columna(2).normalizado() * -35);
			    bullet->setDamping(0.99f);
			}
			bullet->setAcceleration(vector(0, 0, 0));
			bullet->setMass(0.1);

			bullet->setStatus(true);

			gunshotSource->setPosition(position);
			audio->updateSource(gunshotSource);
			audio->playSource(gunshotSource);

			//logger->info("bullet at position: %s", bullet->getPosition().toString("%.2f").c_str());

		} else {
			logger->debug("no particle found");
		}
	}

	void onMouseWheel(int wheel) override {
        camera.setPosition(camera.getPosition() - vector(0.0f, 0.0f, wheel));
        audio->updateListener(camera.getPosition());
        //logger->debug("camera: %s", camera.getPosition().toString("%.2f").c_str());
	}

	void onMouseMove(int x, int y, int dx, int dy, unsigned int buttons) override {
        camera.setPosition(camera.getPosition() - vector(0.1f * dx, 0.1f * dy, 0));
        audio->updateListener(camera.getPosition());
        //logger->debug("camera: %s", camera.getPosition().toString("%.2f").c_str());
	}

	void onMouseButtonDown(unsigned char button, int x, int y) override {
		fire(camera.getPosition());

		float randomDx = ((real)rand()/(real)RAND_MAX * 0.1 - 0.05);
		//logger->info("RandomDx %f", randomDx);
		fire(vector(randomDx, 2.0, 0.0), true);
	}

    virtual void onKeyDown(unsigned int key, unsigned int keyModifier) override {
        switch (key) {
            case SDLK_SPACE:
                reset();
                break;
        }
    }
};

class PhysicsPlayground: public Playground {
public:
	PhysicsPlayground(const String &resourcesBasePath) :
			Playground(resourcesBasePath) {
	}
	void init() override {
		Playground::init();
		this->addRunner(new OpenGLRunner());
		this->addRunner(new AudioRunner());
		this->addRunner(new PhysicsRunner());
		this->addRunner(new PhysicsDemoRunner());
	}
};

void BulletParticle::setRunner(PhysicsDemoRunner *runner) {
    this->runner = runner;
}

void BulletParticle::afterIntegrate(real dt) {
    if(runner != null) {
        runner->afterIntegrate(this);
    }
}

void BulletParticle::onCollisionResolved(const ParticleContact &contact) {
    if(runner != null) {
        runner->onCollision(this, contact);
    }

}




#endif /* SRC_PHYSICSDEMO_H_ */
