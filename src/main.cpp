
#include <iostream>
#include <exception>
#include <filesystem>

#include "SceneLoader.h"
#include "Framebuffer.h"
#include "Renderer.h"
#include "Display.h"


namespace fs = std::filesystem;


static fs::path getDefaultScenePath()
{
    /*
     * Estrutura esperada:
     *
     * RayTracer/
     *
     *     src/
     *         main.cpp
     *
     *     scenes/
     *         scene1.txt
     *
     *
     * __FILE__ aponta para:
     *
     * RayTracer/src/main.cpp
     *
     * Portanto:
     *
     * parent_path()             -> src
     * parent_path().parent()    -> RayTracer
     */

    fs::path sourceFile(__FILE__);

    fs::path projectDirectory = sourceFile.parent_path().parent_path();


    return projectDirectory/ "scenes"/ "scene1.txt";
}


int main(int argc, char* argv[])
{
    try
    {
        // -----------------------------------------------------
        // Resolução da imagem
        // -----------------------------------------------------

        constexpr int WIDTH  = 500;
        constexpr int HEIGHT = 500;


        // -----------------------------------------------------
        // Arquivo da cena
        // -----------------------------------------------------

        fs::path sceneFile;


        if (argc > 1)
        {
            /*
             * Permite passar outro arquivo no Xcode:
             *
             * Product
             *   -> Scheme
             *   -> Edit Scheme
             *   -> Run
             *   -> Arguments
             */

            sceneFile = fs::path(argv[1]);
        }
        else
        {
            sceneFile = getDefaultScenePath();
        }


        // -----------------------------------------------------
        // Verifica arquivo
        // -----------------------------------------------------

        if (!fs::exists(sceneFile))
        {
            std::cerr
                << "\nScene file not found:\n"
                << sceneFile
                << "\n";

            return 1;
        }


        std::cout
            << "\nLoading scene:\n"
            << sceneFile
            << "\n\n";


        // -----------------------------------------------------
        // Lê a cena
        // -----------------------------------------------------

        Scene scene = SceneLoader::loadFromFile(sceneFile.string());


        // -----------------------------------------------------
        // Cria framebuffer
        // -----------------------------------------------------

        Framebuffer framebuffer(WIDTH,HEIGHT);


        // -----------------------------------------------------
        // Ray Tracing
        //
        // TODO: alunos trabalham principalmente daqui para
        // baixo nas classes Renderer, Camera e Sphere.
        // -----------------------------------------------------

        Renderer renderer;


        std::cout
            << "Rendering "
            << WIDTH
            << " x "
            << HEIGHT
            << " pixels...\n";


        renderer.render(
            scene,
            framebuffer
        );


        std::cout << "Rendering completed.\n";


        // -----------------------------------------------------
        // OpenGL apenas mostra os pixels.
        // -----------------------------------------------------

        Display::show( framebuffer, "CPU Ray Tracer");
    }

    catch (
        const std::exception& e
    )
    {
        std::cerr
            << "\nERROR: "
            << e.what()
            << "\n";

        return 1;
    }


    return 0;
}
