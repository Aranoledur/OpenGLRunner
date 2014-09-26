#include "s3e.h"
#include "IwDebug.h"
#include "IwGL.h"
#include <iostream>
#include "Game.h"



// Main entry point for the application
int main()
{
	EasyGodzilla::Game game;

    //Initialise graphics system(s)
    if(!IwGLInit())
    {
        IwTrace(GL, ("IwGLInit failed"));
        return 1;
	}

	// Get dimensions from IwGL
	int w = IwGLGetInt(IW_GL_WIDTH);
	int h = IwGLGetInt(IW_GL_HEIGHT);

	glViewport(0, 0, w, h);

	game.Init();
    

    // Loop forever, until the user or the OS performs some action to quit the app
    while (!s3eDeviceCheckQuitRequest())
    {
        //Update the input systems
        s3eKeyboardUpdate();
        s3ePointerUpdate();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
        
		game.DrawFrame();

        // Call IwGL swap instead of egl directly
        IwGLSwapBuffers();

        // Sleep for 0ms to allow the OS to process events etc.
        s3eDeviceYield(0);
    }

	game.Release();

    //Terminate modules being used
    IwGLTerminate();
    
    // Return
    return 0;
}
