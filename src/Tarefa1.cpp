// =============================================================================
// Tarefa 1 - Pintar uma esfera no Canvas (sem iluminacao, so hit/no-hit)
//
// Reaproveita as classes ja existentes do projeto PyRayCasting:
//   Vector3, Ray, Material, Sphere, Janela, Color, Framebuffer
//
// Baseado nas Aulas 01/02 (revisao de vetores + intersecao raio-esfera) e
// Aula 03 (mapeamento Janela -> matriz de pixels do Canvas).
// =============================================================================

#include <fstream>
#include <iostream>
#include <limits>
#include <filesystem>

#include "Vector3.h"
#include "Ray.h"
#include "Material.h"
#include "Sphere.h"
#include "Janela.h"
#include "Color.h"
#include "Framebuffer.h"
#include "Display.h"

// Salva um Framebuffer em PPM (P6), alem de mostrar na tela via Display.
static void savePPM(const Framebuffer& fb, const std::string& path) {
    std::ofstream out(path, std::ios::binary);
    out << "P6\n" << fb.width() << " " << fb.height() << "\n255\n";
    out.write(reinterpret_cast<const char*>(fb.data()),
              static_cast<std::streamsize>(fb.width()) * fb.height() * 3);
}

int main() {
    // 1) Janela (plano de projecao), centrada em (0, 0, -dJanela)
    //    Janela ja existente no projeto guarda (largura, altura, d) num
    //    unico Vector3.
    const double wJanela = 4.0;   // largura da janela, em metros
    const double hJanela = 4.0;   // altura da janela, em metros
    const double dJanela = 2.0;   // distancia da janela ao olho, em metros

    Janela janela(Vector3(wJanela, hJanela, dJanela));

    // 2) Olho do pintor na origem
    const Vector3 E(0, 0, 0);

    // 3) e 4) Esfera: raio e centro sobre o eixo z, atras da janela
    //    (z_esfera < -(dJanela + rEsfera) garante que ela nao "atravessa"
    //    o plano da janela). Material eh irrelevante aqui (sem Phong nesta
    //    tarefa), entao usamos o construtor default de Material.
    const double rEsfera = 1.0;
    Vector3 cEsfera(0, 0, -(dJanela + rEsfera + 2.0));

    Sphere esfera(cEsfera, rEsfera, Material());

    // 5) e 6) Cores (mesma escala 0-1 usada em Color/Framebuffer)
    const Color esfColor(1.0f, 0.0f, 0.0f);
    const Color bgColor(100.f / 255.f, 100.f / 255.f, 100.f / 255.f);

    // -------------------------------------------------------------------
    // 7) Resolucao da matriz de cores do Canvas
    // -------------------------------------------------------------------
    const int nCol = 500;
    const int nLin = 500;

    Framebuffer canvas(nCol, nLin);

    // Tamanho de cada "retangulo" da tela de mosquito / quadriculado do canvas
    const double Dx = wJanela / nCol;
    const double Dy = hJanela / nLin;

    for (int l = 0; l < nLin; ++l) {
        // y do centro do retangulo (l, c) -- nao depende de c
        double y = hJanela / 2.0 - Dy / 2.0 - l * Dy;

        for (int c = 0; c < nCol; ++c) {
            // x do centro do retangulo (l, c)
            double x = -wJanela / 2.0 + Dx / 2.0 + c * Dx;

            // Ponto da janela atingido por este raio
            Vector3 pJanela(x, y, -dJanela);

            // Raio: parte do olho E, passa por pJanela.
            // Ray ja normaliza a direcao no construtor.
            Ray raio(E, pJanela - E);

            // -----------------------------------------------------------
            // Intersecao raio-esfera reaproveitando Sphere::intersect
            // (mesma equacao quadratica da Aula 02: delta<0 -> sem hit,
            // delta>=0 -> tangencia ou intersecao plena).
            // -----------------------------------------------------------
            HitRecord hit;
            bool acertou = esfera.intersect(raio, 1e-4f,
                                             std::numeric_limits<float>::infinity(),
                                             hit);

            canvas.setPixel(c, l, acertou ? esfColor : bgColor);
        }
    }

    if (!std::filesystem::exists("Tarefa1-Output")) {
        std::filesystem::create_directories("Tarefa1-Output");
    }
    savePPM(canvas, "Tarefa1-Output/canvas.ppm");
    std::cout << "Canvas " << nCol << "x" << nLin
              << " salvo em Tarefa1-Output/canvas.ppm\n";

    // Mostra o resultado numa janela (GLFW), igual ao raytracer.
    // Fecha com ESC.
    Display::show(canvas, "Tarefa 1 - Pinta Esfera");

    return 0;
}