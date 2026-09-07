// =============================================================================
// Tarefa 2 - Esfera iluminada por uma fonte de luz pontual
//
// Evolucao da Tarefa 1: em vez de pintar a esfera com uma cor fixa quando o
// raio acerta, a cor no ponto de intersecao P_I passa a ser a energia
// luminosa refletida na direcao do olho, composta por:
//
//   I_d = (I_F @ Kd) * max(0, l.n)                 (difusa)
//   I_e = (I_F @ Ks) * (max(0, v.r))^m              (especular)
//   cor(P_I) = I_d + I_e
//
// onde (ver Aula 06 / revisao de vetores e o quadro do modelo de Phong):
//   n = (P_I - C) / R                 normal da esfera no ponto de hit
//   l = normalize(P_F - P_I)          direcao da fonte de luz
//   v = -d_r                          direcao de volta para o olho
//   r = 2*(l.n)*n - l                 reflexao de l em torno de n
//
// Reaproveita as classes ja existentes do projeto PyRayCasting:
//   Vector3, Ray, Material, Sphere, Janela, Color, Framebuffer
// =============================================================================

#include <fstream>
#include <iostream>
#include <limits>
#include <cmath>
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

// max(0, x), aplicado componente a componente nao e necessario aqui porque
// os produtos escalares (l.n, v.r) resultam em um unico float.
static float clamp0(float x) { return x > 0.f ? x : 0.f; }

int main() {
    // -------------------------------------------------------------------
    // 1) Janela (plano de projecao), centrada em (0, 0, -dJanela)
    // -------------------------------------------------------------------
    const double wJanela = 4.0;   // largura da janela, em metros
    const double hJanela = 4.0;   // altura da janela, em metros
    const double dJanela = 2.0;   // distancia da janela ao olho, em metros

    Janela janela(Vector3(wJanela, hJanela, dJanela));

    // -------------------------------------------------------------------
    // 2) Olho do pintor na origem
    // -------------------------------------------------------------------
    const Vector3 E(0, 0, 0);

    // -------------------------------------------------------------------
    // 3) e 4) Esfera: raio e centro sobre o eixo z, atras da janela.
    //    Agora o material importa: Kd e Ks definem a cor da esfera sob
    //    luz difusa e especular (mantive o mesmo material vermelho usado
    //    no scene2.txt, pra ficar comparavel com as tarefas anteriores).
    // -------------------------------------------------------------------
    const double rEsfera = 1.0;
    Vector3 cEsfera(0, 0, -(dJanela + rEsfera + 2.0));

    const Color kd(0.80f, 0.10f, 0.10f);
    const Color ks(1.0f, 1.0f, 1.0f);
    const float shininess = 64.0f; // m

    Sphere esfera(cEsfera, rEsfera, Material(Color(), kd, ks, shininess));

    // -------------------------------------------------------------------
    // Fonte de luz pontual (dados do enunciado da Tarefa 2)
    // -------------------------------------------------------------------
    const Color   IF(0.7f, 0.7f, 0.7f);   // intensidade da fonte
    const Vector3 PF(0.0f, 5.0f, 0.0f);   // posicao da fonte

    // -------------------------------------------------------------------
    // 6) Cor de background (usada quando o raio nao acerta a esfera)
    // -------------------------------------------------------------------
    const Color bgColor(100.f / 255.f, 100.f / 255.f, 100.f / 255.f);

    // -------------------------------------------------------------------
    // 7) Resolucao da matriz de cores do Canvas
    // -------------------------------------------------------------------
    const int nCol = 500;
    const int nLin = 500;

    Framebuffer canvas(nCol, nLin);

    const double Dx = wJanela / nCol;
    const double Dy = hJanela / nLin;

    for (int l = 0; l < nLin; ++l) {
        double y = hJanela / 2.0 - Dy / 2.0 - l * Dy;

        for (int c = 0; c < nCol; ++c) {
            double x = -wJanela / 2.0 + Dx / 2.0 + c * Dx;

            Vector3 pJanela(x, y, -dJanela);
            Ray raio(E, pJanela - E); // dr = direcao do raio, ja normalizada

            HitRecord hit;
            bool acertou = esfera.intersect(raio, 1e-4f,
                                             std::numeric_limits<float>::infinity(),
                                             hit);

            if (!acertou) {
                canvas.setPixel(c, l, bgColor);
                continue;
            }

            // ---------------------------------------------------------
            // Vetores do modelo de Phong no ponto de intersecao P_I
            // ---------------------------------------------------------
            const Vector3& PI = hit.point;
            const Vector3& n  = hit.normal;               // ja normalizada

            Vector3 l_dir = (PF - PI).normalized();        // direcao da luz
            Vector3 v     = -raio.direction;                // direcao do olho
            Vector3 r_dir = n * (2.0f * l_dir.dot(n)) - l_dir; // reflexao de l

            float ln = clamp0(l_dir.dot(n));
            float vr = clamp0(v.dot(r_dir));

            Color Id = (IF * hit.material->kd) * ln;
            Color Ie = (IF * hit.material->ks) * std::pow(vr, shininess);

            Color cor = (Id + Ie).clamped();

            canvas.setPixel(c, l, cor);
        }
    }

    if (!std::filesystem::exists("Tarefa2-Output")) {
        std::filesystem::create_directories("Tarefa2-Output");
    }

    savePPM(canvas, "Tarefa2-Output/canvas.ppm");
    std::cout << "Canvas " << nCol << "x" << nLin
              << " salvo em canvas.ppm\n";

    Display::show(canvas, "Tarefa 2 - Esfera Iluminada");

    return 0;
}