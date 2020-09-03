#include "FractalRendererGL.h"

#include <fstream>

int main(int argc, char **argv)
{
    FractalRendererGL3 *renderer = 0x0;
    
    try
    {
        renderer = new FractalRendererGL3(1600, 900);
    }
    catch(std::runtime_error &error)
    {
        std::ofstream errorFile("error.log");
        errorFile<<error.what();
        return 0;
    }
    
    renderer->start();
    
    delete renderer;
    return 0;
}
