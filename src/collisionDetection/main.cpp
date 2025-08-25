#include <iostream>
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGLRunner.h>
#include <AudioRunner.h>

#include "ColDetDemo.h"


int main(int argc, char** argv){
	String repository = Paths::add(Paths::getDirname(argv[0]), "../../media"); //assumes executable lies in playground/target folder
	CollisionDetectionPlayground playground(repository);
	playground.withName("Collision detection demo");
	printf("\n\nRunning playground [%s]\n", playground.toString().c_str());
	playground.run();
    printf("done\n");
    return 0;
}
