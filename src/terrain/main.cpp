#include <iostream>
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGLRunner.h>
#include <AudioRunner.h>

#include "TerrainDemo.h"


int main(int argc, char** argv){
	String repository = Paths::add(Paths::getDirname(argv[0]), "../../media"); //assumes executable lies in playground/target folder
	PlaygroundTerrainDemo playground(repository);
	playground.withName("Terrain Demo");
	printf("\n\nRunning playground [%s]\n", playgroundName.c_str());
	playground.run();
    printf("done\n");
    return 0;
}
