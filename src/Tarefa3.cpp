// =============================================================================
// Tarefa 3 - Esfera + dois planos, luz pontual + luz ambiente
//
// Evolucao da Tarefa 2 (so esfera): agora a cena tem tres objetos (Object*):
// a esfera, o plano do chao e o plano de fundo. Para cada raio, escolhemos
// o objeto com o MENOR t positivo (o mais proximo do olho). Para pontos
// vistos nos PLANOS, so acrescentamos a parcela difusa/especular se o
// segmento ate a fonte de luz nao estiver obstruido pela esfera.
//
// Cor final em cada ponto de interseccao P_I:
//
//   cor(P_I) = I_A @ Ka  +  [ se nao obstruido ]( I_d + I_e )
//
//   I_d = (I_F @ Kd) * max(0, l.n)
//   I_e = (I_F @ Ks) * (max(0, v.r))^m
//
//   n = normal do objeto em P_I
//   l = normalize(P_F - P_I)         direcao da fonte de luz
//   v = -d_r                          direcao de volta para o olho
//   r = 2*(l.n)*n - l                 reflexao de l em torno de n
//
// Reaproveita as classes ja existentes do projeto PyRayCasting:
//   Vector3, Ray, Object, HitRecord, Material, Sphere, Color, Framebuffer
// Um novo tipo, Plane, e definido aqui mesmo (implementa Object).
// =============================================================================

#include <fstream>
#include <iostream>
#include <limits>
#include <cmath>
#include <vector>
#include <filesystem>

#include "Vector3.h"
#include "Ray.h"
#include "Object.h"
#include "HitRecord.h"
#include "Material.h"
#include "Sphere.h"
#include "Color.h"
#include "Framebuffer.h"
#include "Display.h"

// -----------------------------------------------------------------------
// Plano infinito, definido por um ponto conhecido do plano e um vetor
// normal unitario. Mesma interface do Sphere (implementa Object).
// -----------------------------------------------------------------------
class Plane : public Object {
    Vector3  point_;
    Vector3  normal_;
    Material material_;

public:
    Plane(const Vector3& point, const Vector3& normal, const Material& material)
        : point_(point), normal_(normal.normalized()), material_(material) {}

    bool intersect(const Ray& ray, float tMin, float tMax, HitRecord& hit) const override {
        // Equacao do plano: (P - point_) . normal_ = 0
        // Equacao do raio:   P(t) = origin + t*direction
        // Substituindo e isolando t:
        //   t = (point_ - origin) . normal_ / (direction . normal_)
        float denom = ray.direction.dot(normal_);
        if (std::fabs(denom) < 1e-6f) return false; // raio paralelo ao plano

        float t = (point_ - ray.origin).dot(normal_) / denom;
        if (t < tMin || t > tMax) return false; // obs 1: t_i > 0 (tMin cuida disso)

        hit.t        = t;
        hit.point    = ray.at(t);
        hit.normal   = normal_;
        hit.material = &material_;
        hit.object   = this;
        return true;
    }
};

// Salva um Framebuffer em PPM (P6), alem de mostrar na tela via Display.
static void savePPM(const Framebuffer& fb, const std::string& path) {
    std::ofstream out(path, std::ios::binary);
    out << "P6\n" << fb.width() << " " << fb.height() << "\n255\n";
    out.write(reinterpret_cast<const char*>(fb.data()),
              static_cast<std::streamsize>(fb.width()) * fb.height() * 3);
}

static float clamp0(float x) { return x > 0.f ? x : 0.f; }

// -----------------------------------------------------------------------
// Testa o raio contra todos os objetos da cena e devolve o hit com o
// MENOR t positivo (obs 3: "o ponto de intersecao visto ... e aquele
// que tiver o menor t_i positivo"). Reduzir tMax a cada acerto garante
// isso automaticamente.
// -----------------------------------------------------------------------
static bool closestHit(const Ray& ray, float tMin, float tMax,
                        const std::vector<const Object*>& objects,
                        HitRecord& outHit) {
    HitRecord temp;
    bool acertouAlgo = false;
    float maisProximo = tMax;

    for (const Object* obj : objects) {
        if (obj->intersect(ray, tMin, maisProximo, temp)) {
            acertouAlgo = true;
            maisProximo = temp.t;
            outHit = temp;
        }
    }
    return acertouAlgo;
}

int main() {
    // -------------------------------------------------------------------
    // 1) a 3) Janela e Canvas (valores em cm, conforme enunciado)
    // -------------------------------------------------------------------
    const double wJanela = 60.0; // W_J
    const double hJanela = 60.0; // H_J
    const double dJanela = 30.0; // d  (z da janela = -d)

    const int nCol = 500; // W_C
    const int nLin = 500; // H_C

    // -------------------------------------------------------------------
    // Olho do observador na origem
    // -------------------------------------------------------------------
    const Vector3 E(0, 0, 0);

    // -------------------------------------------------------------------
    // 4) e 5) Esfera
    // -------------------------------------------------------------------
    const float rEsfera = 40.0f;
    const Vector3 cEsfera(0, 0, -100.0f);

    const Color kEsfera(0.7f, 0.2f, 0.2f); // Ka = Kd = Ks
    const float mEsfera = 10.0f;

    Sphere esfera(cEsfera, rEsfera, Material(kEsfera, kEsfera, kEsfera, mEsfera));

    // -------------------------------------------------------------------
    // 6) e 7) Plano do chao
    // -------------------------------------------------------------------
    const Vector3 pChao(0, -rEsfera, 0);   // P_pi = (0, -R, 0)
    const Vector3 nChao(0, 1, 0);
    const Color   kdaChao(0.2f, 0.7f, 0.2f); // Kd = Ka
    const Color   ksChao(0.0f, 0.0f, 0.0f);
    const float   mChao = 1.0f;

    Plane planoChao(pChao, nChao, Material(kdaChao, kdaChao, ksChao, mChao));

    // -------------------------------------------------------------------
    // 8) e 9) Plano de fundo
    // -------------------------------------------------------------------
    const Vector3 pFundo(0, 0, -200.0f);   // P_pi = (0, 0, -200)
    const Vector3 nFundo(0, 0, 1);
    const Color   kdaFundo(0.3f, 0.3f, 0.7f); // Kd = Ka
    const Color   ksFundo(0.0f, 0.0f, 0.0f);
    const float   mFundo = 1.0f;

    Plane planoFundo(pFundo, nFundo, Material(kdaFundo, kdaFundo, ksFundo, mFundo));

    // Lista de objetos da cena (usada para achar o hit mais proximo)
    const std::vector<const Object*> objetos = {&esfera, &planoChao, &planoFundo};

    // -------------------------------------------------------------------
    // 10) Fonte de luz pontual
    // -------------------------------------------------------------------
    const Color   IF(0.7f, 0.7f, 0.7f);
    const Vector3 PF(0.0f, 60.0f, -30.0f);

    // -------------------------------------------------------------------
    // 11) Luz ambiente
    // -------------------------------------------------------------------
    const Color IA(0.3f, 0.3f, 0.3f);

    // Cor de fundo -- so usada se, por algum motivo, o raio nao acertar
    // nenhum dos tres objetos (na pratica nao deve acontecer aqui, ja
    // que o plano de fundo cobre toda a janela, mas mantemos por seguranca).
    const Color bgColor(0.0f, 0.0f, 0.0f);

    Framebuffer canvas(nCol, nLin);

    const double Dx = wJanela / nCol;
    const double Dy = hJanela / nLin;

    const float T_MIN = 1e-3f; // obs 1: intersecao valida so com t_i > 0
    const float T_MAX = std::numeric_limits<float>::infinity();

    for (int l = 0; l < nLin; ++l) {
        double y = hJanela / 2.0 - Dy / 2.0 - l * Dy;

        for (int c = 0; c < nCol; ++c) {
            double x = -wJanela / 2.0 + Dx / 2.0 + c * Dx;

            Vector3 pJanela(x, y, -dJanela);
            Ray raio(E, pJanela - E);

            HitRecord hit;
            bool acertou = closestHit(raio, T_MIN, T_MAX, objetos, hit);

            if (!acertou) {
                canvas.setPixel(c, l, bgColor);
                continue;
            }

            const Vector3& PI = hit.point;
            const Vector3& n  = hit.normal;

            // Termo ambiente -- sempre presente, nunca fica em sombra.
            Color corAmbiente = IA * hit.material->ka;

            // -----------------------------------------------------------
            // obs 4: se o ponto de intersecao esta em um dos PLANOS,
            // so somamos difusa/especular se o segmento P_I -> P_F nao
            // estiver obstruido pela esfera. Pontos na propria esfera
            // nao precisam desse teste.
            // -----------------------------------------------------------
            bool emUmPlano = (hit.object != &esfera);
            bool obstruido = false;

            Vector3 vetorParaLuz = PF - PI;
            float   distLuz      = vetorParaLuz.length();
            Vector3 l_dir        = vetorParaLuz / distLuz; // normalizado

            if (emUmPlano) {
                Ray raioSombra(PI + n * 1e-3f, l_dir); // pequeno deslocamento (evita auto-intersecao)
                HitRecord hitSombra;
                if (esfera.intersect(raioSombra, T_MIN, distLuz - 1e-3f, hitSombra)) {
                    obstruido = true;
                }
            }

            Color corDireta(0.0f, 0.0f, 0.0f);
            if (!obstruido) {
                Vector3 v     = -raio.direction;
                Vector3 r_dir = n * (2.0f * l_dir.dot(n)) - l_dir;

                float ln = clamp0(l_dir.dot(n));
                float vr = clamp0(v.dot(r_dir));

                Color Id = (IF * hit.material->kd) * ln;
                Color Ie = (IF * hit.material->ks) * std::pow(vr, hit.material->shininess);

                corDireta = Id + Ie;
            }

            Color cor = (corAmbiente + corDireta).clamped();
            canvas.setPixel(c, l, cor);
        }
    }

    if (!std::filesystem::exists("Tarefa3-Output")) {
        std::filesystem::create_directories("Tarefa3-Output");
    }

    savePPM(canvas, "Tarefa3-Output/canvas.ppm");
    std::cout << "Canvas " << nCol << "x" << nLin
              << " salvo em Tarefa3-Output/canvas.ppm\n";

    Display::show(canvas, "Tarefa 3 - Esfera + Planos");

    return 0;
}