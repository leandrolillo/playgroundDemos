/*
 * PhysicsDemo.h
 *
 *  Created on: Mar 22, 2021
 *      Author: leandro
 */
#include <iostream>
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include "../../base/BaseDemo.h"
#include <OpenGLRunner.h>
#include <AudioRunner.h>
#include<SkyboxRenderer.h>
#include<GridRenderer.h>
#include"../../base/ParticleManagerRenderer.h"

#include<Math3d.h>
#include<Gravity.h>

#include<Geometry.h>
#include"CollisionTester.h"
#include "ParticleManager.h"




class CollisionDetectionDemoRunner;

class CollidingParticle: public Particle {
private:
    bool _isSelected = false;

public:
    CollidingParticle(std::unique_ptr<Geometry> geometry) : Particle(std::move(geometry)) {
    }

    bool isSelected() const {
        return this->_isSelected;
    }

    void setSelected(bool selected) {
        this->_isSelected = selected;
    }
};

//class CameraParticle : public CollidingParticle {
//	Camera &camera;
//public:
//	CameraParticle(Camera &camera) : CollidingParticle(&camera.getFrustum()), camera(camera){
//	}
//
//	void setPosition(const vector &position) {
//		camera.setPosition(position);
//	}
//
//	const vector &getPosition() const {
//		return this->camera.getPosition();
//	}
//
//};

class CollisionDetectionDemoRunner: public BaseDemoRunner {
    ParticleManager particleManager;
    const CollisionTester &intersectionTester = particleManager.getCollisionDetector().getIntersectionTester();
    std::vector<CollidingParticle *>collidingParticles;
    Camera anotherCamera;

    vector2 startPosition;
    vector2 endPosition;

    SkyboxRenderer skyboxRenderer;
    GridRenderer gridRenderer;
    ParticleManagerRenderer particleManagerRenderer;

public:
    CollisionDetectionDemoRunner() :  particleManagerRenderer(defaultRenderer) {
    }

    virtual void onResize(unsigned int height, unsigned int width) override {
    	BaseDemoRunner::onResize(height, width);
      //camera.setProjectionMatrix(Camera::orthographicProjection(5.0, (double) width / (double) height, -20.0, 100.0));
    }


    void reset() {
        camera.setViewMatrix(matriz_4x4::traslacion(vector(0.0f, -0.5f, -10.0f)));
        anotherCamera.setViewMatrix(matriz_4x4::identidad);

        auto &collidingParticles = particleManager.getParticles();
        real radius = (real) 0.5;
        if(collidingParticles.size() > 0) {
            collidingParticles[0]->setPosition(vector(-1, 0, 0));
            ((Sphere &) collidingParticles[0]->getBoundingVolume()).setRadius(radius);
            collidingParticles[0]->setMass(M_PI * radius * radius);
            collidingParticles[0]->setVelocity(vector(1, 1, 0));
        }

        if(collidingParticles.size() > 1) {
            collidingParticles[1]->setPosition(vector(1, 0, 0));
            ((Sphere &) collidingParticles[1]->getBoundingVolume()).setRadius(radius);
            collidingParticles[1]->setMass(M_PI * radius * radius);
            collidingParticles[1]->setVelocity(vector(-1, -1, 0));
        }

        if(collidingParticles.size() > 2) {
            collidingParticles[2]->setPosition(vector(1, 0, 0));
            ((AABB &) collidingParticles[2]->getBoundingVolume()).setHalfSizes(vector(0.5, 0.5, 0.5));
            collidingParticles[2]->setMass(1);
            collidingParticles[2]->setVelocity(vector(-1, -1, 0));
        }

        if(collidingParticles.size() > 3) {
            collidingParticles[3]->setPosition(vector(1, 2, 0));
            ((AABB &) collidingParticles[3]->getBoundingVolume()).setHalfSizes(vector(1, 1, 1));
            collidingParticles[3]->setMass(1);
            collidingParticles[3]->setVelocity(vector(0, 0, 0));
        }

        if(collidingParticles.size() > 4) {
            collidingParticles[4]->setPosition(vector(-2, 1, 0));
            collidingParticles[4]->setMass(1);
            collidingParticles[4]->setVelocity(vector(0, 0, 0));
        }
    }

    bool initialize() override {
    	BaseDemoRunner::initialize();

        this->video->resize(800, 600);

        logger->debug("Initializing renderers");
        gridRenderer.setVideoRunner(*video);
//	    skyboxRenderer.setVideoRunner(video);
//	    skyboxRenderer.setSize(200);

        logger->debug("Setting up video %d", video);
        video->enable(BLEND, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        /**
         * Add all particles to particle manager and colliding particles to collidingParticles list for internal management
         */
        collidingParticles.push_back(
            (CollidingParticle *)&particleManager.addParticle(std::make_unique<CollidingParticle>(std::make_unique<Sphere>(vector(0, 0, 0), (real) 0.5))));
        collidingParticles.push_back(
            (CollidingParticle *)&particleManager.addParticle(std::make_unique<CollidingParticle>(std::make_unique<Sphere>(vector(0, 0, 0), (real) 0.5))));
        collidingParticles.push_back(
            (CollidingParticle *)&particleManager.addParticle(std::make_unique<CollidingParticle>(std::make_unique<AABB>(vector(0, 0, 0), vector(0.5, 0.5, 0.5)))));
        collidingParticles.push_back(
            (CollidingParticle *)&particleManager.addParticle(std::make_unique<CollidingParticle>(std::make_unique<AABB>(vector(0, 0, 0), vector(1, 1, 1)))));

        auto hierarchicalGeometry = std::make_unique<HierarchicalGeometry>(std::make_unique<AABB>(vector(0, 0, 0), vector(1, 0.5, 0.5)));
        hierarchicalGeometry->addChildren(std::make_unique<Sphere>(vector(-0.5, 0, 0), 0.5));
        hierarchicalGeometry->addChildren(std::make_unique<Sphere>(vector(0.5, 0, 0), 0.5));
        collidingParticles.push_back(
            (CollidingParticle *)&particleManager.addParticle(std::make_unique<CollidingParticle>(std::move(hierarchicalGeometry))));

        particleManager.addParticle(std::make_unique<Particle>(std::make_unique<Plane>(vector(0, 0, 0), vector(0, 1, 0)))).setInverseMass((real)0);


//        collidingParticles.push_back(std::unique_ptr<CollidingParticle>(new CameraParticle(anotherCamera)));
//        particleManager.addParticle(collidingParticles.back().get());


        reset();

        logger->debug("Completed initialization");
        return true;
    }


    LoopResult doLoop() override {
        defaultRenderer.drawAxes(matriz_4x4::identidad);
        defaultRenderer.drawLine(matriz_4x4::identidad, vector(-1, 0, 0), vector(1, 0, 0));
        defaultRenderer.drawLine(matriz_4x4::identidad, vector(0, -1, 0), vector(0, 1, 0));
        defaultRenderer.drawLine(matriz_4x4::identidad, vector(0, 0, -1), vector(0, 0, 1));

        particleManager.detectCollisions();
        std::vector<ParticleContact> contacts = particleManager.getContacts();

        particleManagerRenderer.render(particleManager);
        //particleManagerRenderer.render(&anotherCamera.getFrustum());

        skyboxRenderer.render(camera);
        gridRenderer.render(camera);

        return LoopResult::CONTINUE;
    }


//    void onCollision(CollidingParticle *sphereParticle) {
//        //sphereParticle->setIsColliding(true);
//    }
//
//    void afterIntegrate(CollidingParticle *particle) {
//        if (particle->getPosition().modulo() > 100) {
//            particle->setStatus(false);
//        }
//    }

    void onMouseMove(int x, int y, int dx, int dy, unsigned int buttons) override {
            Line line(camera.getPosition(),
                    camera.getRayDirection((unsigned int) x, (unsigned int) y, video->getScreenWidth(), video->getScreenHeight()));

            if (!equalsZeroAbsoluteMargin(line.getDirection().z)) {
                for(auto &particle : collidingParticles) {

                if (particle->isSelected()) {
                    vector origin = particle->getBoundingVolume().getOrigin();

                    real t = (origin.z - line.getOrigin().z) / line.getDirection().z;
                    particle->setPosition(line.getOrigin() + t * line.getDirection());
                }
            }
        }
    }

    void onMouseButtonUp(unsigned char button, int x, int y) override {
        if (button == SDL_BUTTON_LEFT) {
            for (auto &particle : collidingParticles) {
                particle->setSelected(false);
            }
        }
    }

	void onMouseWheel(int wheel) override {
		logger->debug("Mouse wheel %d", wheel);
		camera.setPosition(camera.getPosition() - vector(0.0, 0.0, std::min(1.0, 0.1 * wheel)));
		logger->debug("Camera position: %s", camera.getPosition().toString().c_str());
	}


    void onMouseButtonDown(unsigned char button, int x, int y) override {
        if (button == SDL_BUTTON_LEFT) {
            this->startPosition = vector2(x, y);

            Line line(camera.getPosition(),
                    camera.getRayDirection((unsigned int) x, (unsigned int) y, video->getScreenWidth(), video->getScreenHeight()));

            for(auto &particle : collidingParticles) {
                if (intersectionTester.intersects(particle->getBoundingVolume(), (Geometry &)line)) {
                    particle->setSelected(true);
                }
            }
        }
    }

    void onKeyDown(unsigned int key, unsigned int keyModifier) override {
        switch (key) {
					case '+':
					case SDLK_KP_PLUS:
						camera.setPosition(camera.getPosition() - vector(0.0, 0.0, 0.1));
					break;
					case '-':
					case SDLK_KP_MINUS:
						camera.setPosition(camera.getPosition() + vector(0.0, 0.0, 0.1));
					break;
					case SDLK_LEFT:
						camera.setPosition(camera.getPosition() - vector(0.1, 0.0, 0.0));
					break;
					case SDLK_RIGHT:
						camera.setPosition(camera.getPosition() + vector(0.1, 0.0, 0.0));
					break;
					case SDLK_UP:
						camera.setPosition(camera.getPosition() + vector(0.0, 0.1, 0.0));
					break;
					case SDLK_DOWN:
						camera.setPosition(camera.getPosition() - vector(0.0, 0.1, 0.0));
						break;
					case SDLK_BACKSPACE:
							reset();
							break;
					case SDLK_SPACE:
						particleManager.resolveContacts(this->getStopWatch().getElapsedTime());
							break;
					case 'w':
					case 'W':
						anotherCamera.setPosition(anotherCamera.getPosition() + vector(0.0, 10.0, 0.0) * this->getStopWatch().getElapsedTime());
						break;
					case 's':
					case 'S':
						anotherCamera.setPosition(anotherCamera.getPosition() - vector(0.0, 10.0, 0.0) * this->getStopWatch().getElapsedTime());
						break;
					case 'a':
					case 'A':
						anotherCamera.setPosition(anotherCamera.getPosition() - vector(10.0, 0.0, 0.0) * this->getStopWatch().getElapsedTime());
						break;
					case 'd':
					case 'D':
						anotherCamera.setPosition(anotherCamera.getPosition() + vector(10.0, 0.0, 0.0) * this->getStopWatch().getElapsedTime());
						break;
					case 'q':
					case 'Q':
						anotherCamera.setOrientation(anotherCamera.getOrientation() * matriz_3x3::matrizRotacion(0.0, radian(10 * this->getStopWatch().getTotalTime()), 0.0));
						break;
					case 'e':
					case 'E':
						anotherCamera.setOrientation(matriz_3x3::matrizRotacion(0.0, -10 * radian(this->getStopWatch().getElapsedTime()), 0.0) * anotherCamera.getOrientation());
						break;
					case 'z':
					case 'Z':
						anotherCamera.setOrientation(matriz_3x3::matrizRotacion(radian(10 * this->getStopWatch().getElapsedTime()), 0.0, 0.0) * anotherCamera.getOrientation());
						break;
					case 'c':
					case 'C':
						anotherCamera.setOrientation(matriz_3x3::matrizRotacion(radian(10 * this->getStopWatch().getElapsedTime()), 0.0, 0.0) * anotherCamera.getOrientation());
						break;


        }
//        logger->info("Another camera position: %s - dt: %f", anotherCamera.getPosition().toString().c_str(), this->getStopWatch().getElapsedTime());
//        logger->debug("Camera position: %s", camera.getPosition().toString().c_str());
    }

    virtual String toString() const override {
    		return "CollisionDetectionDemoRunner(id:" + std::to_string(this->getId()) + ")\n    * " + this->intersectionTester.toString();
    }
};

//void CollidingParticle::setRunner(CollisionDetectionDemoRunner *runner) {
//    this->runner = runner;
//}

//void CollidingParticle::afterIntegrate(real dt) {
//    if (runner != null) {
//        runner->afterIntegrate(this);
//    }
//}
//
//void CollidingParticle::onCollision(const ParticleContact &contact) {
//    if (runner != null) {
//        runner->onCollision(this);
//    }
//
//}

class CollisionDetectionPlayground: public Playground {
public:
    CollisionDetectionPlayground(const String &resourcesBasePath) : Playground(resourcesBasePath) {
    }
    void initializePlayground() override {
        Playground::initializePlayground();
        this->addRunner(std::make_unique<OpenGLRunner>());
        this->addRunner(std::make_unique<AudioRunner>());
        this->addRunner(std::make_unique<CollisionDetectionDemoRunner>());
    }
};

int main(int argc, char** argv){
  String repository = Paths::add(Paths::getDirname(argv[0]), "resources"); //assumes executable lies in playground/target folder
  CollisionDetectionPlayground playground(repository);
  playground.withName("Collision detection demo");
  printf("\n\nRunning playground [%s]\n", playground.toString().c_str());
  playground.run();
    printf("done\n");
    return 0;
}
