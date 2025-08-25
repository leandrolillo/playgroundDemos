/*
 * TerrainDemo.h
 *
 *  Created on: Mar 26, 2021
 *      Author: leandro
 */

#ifndef SRC_DEMO_TERRAINDEMO_H_
#define SRC_DEMO_TERRAINDEMO_H_

#include "../base/BaseDemo.h"

#include<vector>

#include<TerrainRenderer.h>
#include<SkyboxRenderer.h>
#include<GeometryRenderer.h>


#include "../inputController/FPSInputController.h"
#include "../inputController/ThirdPersonInputController.h"

#include<PhysicsRunner.h>
#include<Gravity.h>



class TerrainFPSInputController : public FPSInputController {
protected:
    TerrainResource *terrain = null;

public:
    TerrainFPSInputController(Camera &cameraReference, matriz &playerTransformReference) : FPSInputController(cameraReference, playerTransformReference) {
    }

    void setTerrain(TerrainResource *terrain) {
        this->terrain = terrain;
    }

protected:
    vector constrainPosition(const vector &position) override {
        vector result = position;
        if(terrain != null) {
            real altitude = 0.5 + terrain->getHeightMap()->heightAt(
                    position.x - floor(position.x / terrain->getHeightMap()->getWidth()) * terrain->getHeightMap()->getWidth(),
                    position.z - floor(position.z / terrain->getHeightMap()->getDepth()) * terrain->getHeightMap()->getDepth()
                    );

            if(result.y < altitude) {
                result.y = altitude;
            }
        }
        return result;
    }
};

class TerrainThirdPersonInputController : public ThirdPersonInputController {
protected:
    TerrainResource *terrain = null;

public:
    TerrainThirdPersonInputController(Camera &cameraReference, matriz &playerTransformReference) : ThirdPersonInputController(cameraReference, playerTransformReference) {
    }

    void setTerrain(TerrainResource *terrain) {
        this->terrain = terrain;
    }

protected:
    vector constrainPosition(const vector &position) override {
        vector result = position;
        if(terrain != null) {
            real altitude = 0.5 + terrain->getHeightMap()->heightAt(
                    position.x - floor(position.x / terrain->getHeightMap()->getWidth()) * terrain->getHeightMap()->getWidth(),
                    position.z - floor(position.z / terrain->getHeightMap()->getDepth()) * terrain->getHeightMap()->getDepth()
                    );

            if(result.y < altitude) {
                result.y = altitude;
            }
        }
        return result;
    }
};


class TerrainDemoRunner: public BaseDemoRunner {
private:
    /**
     * Playground Stuff
     */
    PhysicsRunner *physics = nullptr;

    bool debug = false;

    /**
     * Input controllers
     */

    TerrainFPSInputController fpsInputController;
    TerrainThirdPersonInputController thirdPersonController;
    InputController *inputController = &fpsInputController;

    matriz playerTransform;

    /**
     * This demo stuff
     */

	LightResource light;

	TerrainRenderer terrainRenderer;
	SkyboxRenderer skyboxRenderer;
	GeometryRenderer geometryRenderer;

	VertexArrayResource *tree = null;
	TextureResource *treeTexture = null;
	std::vector<matriz_4x4> treePositions;
	std::vector<std::unique_ptr<AABB>> treeBoundingVolumes;

	TerrainResource *terrain = null;
	std::unique_ptr<HierarchicalGeometry> terrainBoundingVolume;

	std::vector<std::unique_ptr<BulletParticle>> particles;
	Gravity gravity = Gravity(vector(0.0, -9.8, 0.0));

    MaterialResource black {vector(0, 0, 0), vector(0, 0, 0), vector(0, 0, 0), 1.0, 0.2 };
    MaterialResource white {vector(1, 1, 1), vector(1, 1, 1), vector(1, 1, 1), 1.0, 0.2 };
	MaterialResource red {vector(1, 0, 0), vector(1, 0, 0), vector(1, 0, 0), 1.0, 0.2 };
	MaterialResource green {vector(0, 1, 0), vector(0, 1, 0), vector(0, 1, 0), 1.0, 0.2 };
	MaterialResource blue {vector(0, 0, 1), vector(0, 0, 1), vector(0, 0, 1), 1.0, 0.2 };

	MaterialResource *materials[3] = { &red, &green, &blue};

public:
	TerrainDemoRunner() : light(vector(0, 0, 0),
			vector(0.2f, 0.2f, 0.2f), vector(0.2f, 0.2f, 0.2f),
			vector(0.1f, 0.1f, 0.1f), 1.0f),
            fpsInputController(camera, playerTransform),
            thirdPersonController(camera, playerTransform),
			geometryRenderer(defaultRenderer) {
	}

    virtual void onResize(unsigned int height, unsigned int width) override {
    	/**
    	 * perspective projection zFar needs to be at least the size of skybox hypotenuse (sqrt(skybox size^2 + skybox size^2)
    	 */
        camera.setPerspectiveProjectionFov(45.0, (GLfloat) width / (GLfloat) height, 0.1, 600.0);
    }

    virtual bool init() override {
    	if(!BaseDemoRunner::init()) {
    		return false;
    	}

    	physics = (PhysicsRunner *)this->getContainer()->getRequiredRunner(PhysicsRunner::ID);

        video->enable(RELATIVE_MOUSE_MODE, 0);
        video->enable(BLEND, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ResourceManager *resourceManager = this->getResourceManager();

        treeTexture = (TextureResource *)resourceManager->load("images/lowPolyTree.png", MimeTypes::TEXTURE);

        /**
         * If the object has no name in the obj file, we put the filename as the object name, thus we have to request it with a duplicated name.
         * Review the single object use case to see if there's a better option
         */
        if((tree = (VertexArrayResource *)resourceManager->load("geometry/lowPolyTree.obj/lowPolyTree.obj", MimeTypes::VERTEXARRAY)) == null) {
        	logger->error("Could not load tree model");
        	return false;
        }

        terrain = (TerrainResource *)resourceManager->load("geometry/terrain/terrain.json", "video/terrain");
        if(!terrain) {
        	throw Exception("Could not load terrain geometry/terrain/terrain.json");
        }

        fpsInputController.setTerrain(terrain);
        thirdPersonController.setTerrain(terrain);

        terrainRenderer.setVideoRunner(video);
        terrainRenderer.setLight(&light);
        terrainRenderer.addTerrain(vector(0, 0, 0), terrain);
        terrainRenderer.addTerrain(vector(-terrain->getHeightMap()->getWidth(), 0, 0), terrain);
        terrainRenderer.addTerrain(vector(0, 0, -terrain->getHeightMap()->getDepth()), terrain);
        terrainRenderer.addTerrain(vector(-terrain->getHeightMap()->getWidth(), 0, -terrain->getHeightMap()->getDepth()), terrain);


        /**
         * Setup terrain hierarchical bounding volume - aabbs per heightmap are actually unnecessary since its current intersection test does aabb check as well.
         */
        terrainBoundingVolume = std::unique_ptr<HierarchicalGeometry>(new HierarchicalGeometry(
        		new AABB(vector(0, 0, 0),
        				vector(terrain->getHeightMap()->getWidth(),
        						terrain->getHeightMap()->getHeight(), terrain->getHeightMap()->getDepth()))));

        vector terrainHalfSizes = vector(terrain->getHeightMap()->getWidth() * 0.5,
                terrain->getHeightMap()->getHeight() * 0.5,
                terrain->getHeightMap()->getDepth() * 0.5);

        terrainBoundingVolume->addChildren(new HeightMapGeometry(vector(0, 0, 0), terrain->getHeightMap()));
        terrainBoundingVolume->addChildren(new HeightMapGeometry(vector(-terrain->getHeightMap()->getWidth(), 0, 0), terrain->getHeightMap()));
        terrainBoundingVolume->addChildren(new HeightMapGeometry(vector(0, 0, -terrain->getHeightMap()->getDepth()), terrain->getHeightMap()));
        terrainBoundingVolume->addChildren(new HeightMapGeometry(vector(-terrain->getHeightMap()->getWidth(), 0, -terrain->getHeightMap()->getDepth()), terrain->getHeightMap()));

        skyboxRenderer.setVideoRunner(video);
        skyboxRenderer.setSize(300);

        physics->getParticleManager()->addForce(&this->gravity);
        physics->getParticleManager()->addScenery(terrainBoundingVolume.get());

        for(int index = 0; index < numberOfParticles; index++) {
            particles.push_back(std::unique_ptr<BulletParticle>(new BulletParticle()));
            particles.back()->setStatus(false);
            //particles.back()->setRunner(this);

            physics->getParticleManager()->addParticle(particles.back().get());
        }

        for(int index = 0; index < 20; index++) {
        	real x = rrand() * terrain->getHeightMap()->getWidth();
        	real z = rrand() * terrain->getHeightMap()->getDepth();

        	vector position(terrain->getHeightMap()->positionAt(x, z));

        	this->treePositions.push_back(matriz_4x4::traslacion(position) * matriz_4x4::rotacion(0, radian(rrand() * 360.0), 0));

        	vector halfsizes(tree->getSize() * 0.5);
        	this->treeBoundingVolumes.push_back(std::unique_ptr<AABB>(new AABB(position + vector(0, halfsizes.y, 0), vector(halfsizes.x * 0.15, halfsizes.y, halfsizes.z * 0.15))));
        	physics->getParticleManager()->addScenery(treeBoundingVolumes.back().get());
        }


        reset();

        logger->info("Done configuring!");
        return true;
    }

	void reset() {
		//light.setPosition(position);
		inputController->reset();

        for(auto &particle : this->particles) {
            particle->setStatus(false);
        }

	}

	virtual LoopResult doLoop() override {
	    defaultRenderer.clear();
	    defaultRenderer.drawAxes(matriz_4x4::identidad);

	    // draw avatar
	    defaultRenderer.drawSphere(playerTransform, 0.5);

	    // draw spheres when not debugging. If debugging they will be rendered exactly the same when rendering the particle manager data
	    if(!debug) {
			for(auto &particle : this->particles)
			{
				if(particle->getStatus() == true) {
					geometryRenderer.render(particle->getBoundingVolume());
				}
			}
	    }

        //draw trees
	    defaultRenderer.setTexture(treeTexture);
        for(auto &position : this->treePositions) {
        	defaultRenderer.drawObject(position, tree);
        }


        if(debug){
        	defaultRenderer.setTexture(null);
        	geometryRenderer.render(physics->getParticleManager());
        }

		terrainRenderer.render(camera);
		skyboxRenderer.render(camera);
	  defaultRenderer.render(camera);

		inputController->update(this->getStopWatch().getElapsedTime());

		return LoopResult::CONTINUE;
	}

    void fire(const vector &position, bool booble = false) {
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

            if(booble) {
                bullet->setVelocity(camera.getOrientation().columna(2).normalizado() * -20);
                bullet->setMass(0.1);
                bullet->setDamping(0.99f);
            } else {
                bullet->setVelocity(camera.getOrientation().columna(2).normalizado() * -4);
                bullet->setDamping(0.8f);
                bullet->setMass(-0.001);
            }

            bullet->setAcceleration(vector(0, 0, 0));

            bullet->setStatus(true);

            //logger->info("bullet at position: %s", bullet->getPosition().toString("%.2f").c_str());

        } else {
            logger->debug("no particle found");
        }
    }

	void onMouseButtonDown(unsigned char button, int x, int y) override
	{
	    if(button == SDL_BUTTON_LEFT) {
	        fire(camera.getPosition(), true);
	    } else {
	        fire(camera.getPosition());
	    }

	    inputController->mouseButtonDown(button, x, y);
	}

	void onMouseButtonUp(unsigned char button, int x, int y) override
	{
	    inputController->mouseButtonUp(button, x, y);
	}

	virtual void onMouseMove(int x, int y, int dx, int dy, unsigned int buttons) override {
		if(dx != 0 || dy != 0) {
				inputController->mouseMove(x, y, dx, dy);
				this->video->setMousePosition(video->getScreenWidth() >> 1, video->getScreenHeight() >> 1);
		}
	}

	void onMouseWheel(int wheel) override {
	    inputController->mouseWheel(wheel);
	}

    virtual void onKeyDown(unsigned int key, unsigned int keyModifier) override {
        inputController->keyDown(key, keyModifier);
    }

    virtual void onKeyUp(unsigned int key, unsigned int keyModifier) override {
        inputController->keyUp(key, keyModifier);
        switch (key) {
        	case SDLK_F1:
        		this->debug = !this->debug;
        		break;
            case '1':
                this->inputController = &fpsInputController;
                break;
            case '2':
                this->inputController = &thirdPersonController;
                break;
            case SDLK_SPACE:
                reset();
                break;
            case SDLK_BACKSPACE:
            	if(physics->getEnabled()) {
            		physics->stop();
            	} else {
            		physics->start();
            	}
            	break;
            case SDLK_RIGHT:
            	physics->step(0.01f);
            	break;
            case SDLK_LEFT:
				physics->step(-0.01f);
				break;
        }
    }
};

class PlaygroundTerrainDemo: public Playground {
public:
	PlaygroundTerrainDemo(const String &resourcesBasePath) :
			Playground(resourcesBasePath) {
	}
	void init() override {
		Playground::init();
		this->addRunner(new OpenGLRunner());
		this->addRunner(new AudioRunner());
        this->addRunner(new PhysicsRunner());
        this->addRunner(new TerrainDemoRunner());
	}
};


#endif /* SRC_DEMO_TERRAINDEMO_H_ */
