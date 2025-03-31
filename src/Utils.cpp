#include "Utils.h"

namespace Utils {
    float getFPS()
    {
        static double previousTime = 0.0;
        static int frameCount = 0;
        static float fps = 0.0f;

        double currentTime = glfwGetTime(); 
        frameCount++;

        if (currentTime - previousTime >= 1.0)
        {
            fps = frameCount / (currentTime - previousTime);
            frameCount = 0;
            previousTime = currentTime;
        }

        return fps;
    }
}