#define GL_SILENCE_DEPRECATION

#include "Display.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>


void Display::show(const Framebuffer& framebuffer, const char* title)
{
    // ---------------------------------------------------------
    // Inicializa GLFW
    // ---------------------------------------------------------

    if (!glfwInit())
    {
        throw std::runtime_error(
            "Could not initialize GLFW."
        );
    }


    // ---------------------------------------------------------
    // IMPORTANTE PARA macOS
    //
    // Não solicitar OpenGL Core Profile.
    //
    // Deixamos GLFW criar um contexto OpenGL legado,
    // necessário para glDrawPixels().
    // ---------------------------------------------------------

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);


    // ---------------------------------------------------------
    // Cria janela
    // ---------------------------------------------------------

    GLFWwindow* window = glfwCreateWindow(framebuffer.width(),
                                          framebuffer.height(),
                                          title,
                                          nullptr,
                                          nullptr);

    if (window == nullptr)
    {
        glfwTerminate();

        throw std::runtime_error(
            "Could not create GLFW window."
        );
    }


    // ---------------------------------------------------------
    // Torna o contexto OpenGL atual
    // ---------------------------------------------------------

    glfwMakeContextCurrent(window);


    // Sincroniza atualização com monitor
    glfwSwapInterval(1);


    // ---------------------------------------------------------
    // Configuração OpenGL extremamente simples
    // ---------------------------------------------------------

    glViewport( 0, 0, framebuffer.width(), framebuffer.height());

    // Não usamos iluminação OpenGL.
    // Não usamos depth buffer.
    // Não usamos shaders.
    // Não usamos texturas.

    glDisable(GL_DEPTH_TEST);

    glDisable(GL_LIGHTING);


    // Fundo preto
    glClearColor(
        0.0f,
        0.0f,
        0.0f,
        1.0f
    );


    // ---------------------------------------------------------
    // Configuração da projeção
    //
    // Coordenadas da janela:
    //
    // (0,0) ---------------- width
    //   |
    //   |
    //   |
    // height
    //
    // Assim usamos as mesmas coordenadas do framebuffer.
    // ---------------------------------------------------------

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    glOrtho(0.0, framebuffer.width(), framebuffer.height(), 0.0, -1.0, 1.0);


    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();


    // ---------------------------------------------------------
    // Alinhamento dos pixels
    // ---------------------------------------------------------

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


    // ---------------------------------------------------------
    // Loop de exibição
    // ---------------------------------------------------------

    while (!glfwWindowShouldClose(window))
    {
        int windowFramebufferWidth;
        int windowFramebufferHeight;

        glfwGetFramebufferSize(
            window,
            &windowFramebufferWidth,
            &windowFramebufferHeight
        );

        glViewport(
            0,
            0,
            windowFramebufferWidth,
            windowFramebufferHeight
        );

        glClear(GL_COLOR_BUFFER_BIT);

        // Razão entre framebuffer Retina e imagem do ray tracer
        float scaleX =
            static_cast<float>(windowFramebufferWidth)
            / static_cast<float>(framebuffer.width());

        float scaleY =
            static_cast<float>(windowFramebufferHeight)
            / static_cast<float>(framebuffer.height());

        // Posição inicial no canto superior esquerdo
        glRasterPos2i(0, 0);

        // Amplia a imagem e inverte verticalmente
        glPixelZoom(
            scaleX,
            -scaleY
        );

        glDrawPixels(
            framebuffer.width(),
            framebuffer.height(),
            GL_RGB,
            GL_UNSIGNED_BYTE,
            framebuffer.data()
        );

        glPixelZoom(1.0f, 1.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(
                window,
                GLFW_TRUE
            );
        }
    }

    // ---------------------------------------------------------
    // Finalização
    // ---------------------------------------------------------

    glfwDestroyWindow(window);

    glfwTerminate();
}
