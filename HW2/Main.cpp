#include <iostream>
#include <vector>
#include "Scene.h"

using namespace std;

Scene *scene;

int main(int argc, char *argv[])
{
    // if (argc != 2)
    // {
    //     cout << "Please run the rasterizer as:" << endl
    //          << "\t./rasterizer <input_file_name>" << endl;
    //     return 1;
    // }
    // else
    // {
        // const char *xmlPath = argv[1];
        // const char *xmlPath = "./inputs_outputs/culling_enabled_inputs/empty_box.xml";
        const char *xmlPath = "./inputs_outputs/culling_disabled_inputs/horse_and_mug.xml";
        scene = new Scene(xmlPath);

        for (int i = 0; i < scene->cameras.size(); i++)
        {
            // initialize image with basic values
            scene->initializeImage(scene->cameras[i]);

            // do forward rendering pipeline operations
            scene->forwardRenderingPipeline(scene->cameras[i]); // TODO

            // generate PPM file
            scene->writeImageToPPMFile(scene->cameras[i]);

            // Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
            // Change/remove implementation if necessary.
            scene->convertPPMToPNG(scene->cameras[i]->outputFilename);
        }

    //     return 0;
    // }
}