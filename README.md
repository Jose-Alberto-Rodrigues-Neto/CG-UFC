# Overview do projeto PyRayCasting

É um **ray tracer baseado em CPU**, que lê uma cena de um arquivo texto (`scenes/scene1.txt`), traça raios para cada pixel, calcula sombreamento pelo **modelo de Phong** (ambiente + difusa + especular) e mostra o resultado numa janela via **OpenGL legado (`glDrawPixels`) + GLFW**. Só existe um tipo de objeto geométrico: **esfera**.

## Estrutura de pastas

```
PyRayCasting/
├── CMakeLists.txt
├── header/     → declarações (.h) de todas as classes
├── src/        → implementações (.cpp)
└── scenes/     → arquivos de cena (scene1.txt)
```

Originalmente o projeto não tinha nenhum sistema de build (nem Makefile, nem CMakeLists.txt) — só o código-fonte e os headers. O `CMakeLists.txt` incluído neste pacote resolve isso.

## Classes, uma a uma

**`Vector3`** — vetor 3D (`x,y,z`). Operações: soma, subtração, negação, escalar, produto escalar (`dot`), produto vetorial (`cross`), normalização. É a base geométrica de tudo (posições, direções, normais).

**`Color`** — trio `r,g,b` em float (0–1). Suporta soma, multiplicação por escalar, multiplicação componente-a-componente (usada para `Ka*Ia`, `Kd*Il`, etc.) e `clamped()` para saturar em [0,1] antes de virar pixel.

**`Ray`** — raio com `origin` e `direction` (a direção é normalizada no construtor). `at(t)` retorna o ponto `origin + direction*t`.

**`Material`** — coeficientes de Phong: `ka` (ambiente), `kd` (difuso), `ks` (especular) e `shininess` (expoente de brilho).

**`HitRecord`** — "resultado de uma interseção": `t` (distância ao longo do raio), `point`, `normal`, ponteiro para o `material` e para o `object` atingidos. É o que passa de `intersect()` para o `Renderer`.

**`Object`** — classe abstrata (interface) com um único método puro virtual `intersect(Ray, tMin, tMax, HitRecord&)`. Qualquer geometria (aqui, só `Sphere`) implementa isso.

**`Sphere`** — implementa `Object`. Guarda centro, raio e material. `intersect()` resolve a equação quadrática raio-esfera (`a·t² + 2·hb·t + c = 0` via discriminante) e escolhe a raiz válida mais próxima dentro de `[tMin, tMax]`.

**`Camera`** — posição do olho (`eye_`), ponto observado (`lookAt_`), vetor `up_`, campo de visão (`fov_`), e a base ortonormal `u_,v_,w_` (calculada em `updateBasis()`, sistema "look-at" clássico). Tem `generateRay(px,py,largura,altura)` que gera o raio primário de um pixel — código alternativo ao que o `Renderer` realmente usa (veja observação abaixo).

**`Janela`** — classe simples que guarda `(largura, altura, distância d)` do plano de projeção — o "viewport" descrito no guia do `scene1.txt`.

**`PointLight`** — posição + intensidade (cor) de uma luz pontual.

**`Scene`** — agrega tudo: `camera`, `janela`, `ambientLight`, `background`, lista de `objects` (esferas) e lista de `lights`.

**`SceneLoader`** — parser de texto que lê o arquivo `.txt` linha a linha, reconhecendo os tokens `camera`, `janela`, `ambient`, `background`, `light`, `sphere` e monta um `Scene`. Ignora linhas começando com `#` (comentários).

**`Framebuffer`** — buffer de pixels RGB8 (`width*height*3` bytes). `setPixel()` recebe uma `Color` float, satura e converte para `uint8_t`.

**`Display`** — só exibe o framebuffer numa janela GLFW com `glDrawPixels`, sem shaders/depth/lighting do OpenGL (é puramente "blit" da imagem já calculada pela CPU). Fecha com ESC.

**`Renderer`** — o coração do ray tracer:

- `closestHit()`: testa o raio contra todos os objetos e guarda o hit mais próximo.
- `isInShadow()`: manda um raio da superfície até a luz; se algo bloquear antes de chegar, está em sombra.
- `shade()`: implementa Phong — `ambiente + Σ(difusa + especular)` por luz, pulando luzes em sombra.
- `render()`: loop duplo sobre todos os pixels do framebuffer, gera o raio primário e chama `closestHit`/`shade`.

**`main.cpp`** — resolve o caminho do `scene1.txt` (relativo ao `main.cpp`, assumindo a estrutura `src/`+`scenes/`), permite passar outro arquivo de cena por argumento, carrega a cena, cria o framebuffer (500×500), renderiza e chama `Display::show`.

## O modelo de Phong aplicado

Para cada ponto visível:

```
I = Iambient + Idiffuse + Ispecular

Iambient  = Ka * Ia
Idiffuse  = Kd * Il * max(N·L, 0)
Ispecular = Ks * Il * max(R·V, 0)^shininess
```

onde `Ka, Kd, Ks, shininess` vêm do material da esfera, `Ia` vem de `ambient`, `Il` vem de cada `light`, `N` é a normal da esfera, `L` aponta para a fonte, `V` aponta para a câmera e `R` é o vetor refletido — exatamente o que o `Renderer::shade()` implementa.

## Sobre os "STUDENT TODO"

Existem 6 marcações `// STUDENT TODO N` no código (`Sphere::intersect`, `Camera::generateRay`, `Renderer::closestHit`, `Renderer::isInShadow`, `Renderer::shade`, `Renderer::render`) — são os pontos que a disciplina espera que o aluno implemente. Nesta cópia analisada, **todos os 6 já estão preenchidos**, então é provavelmente um gabarito/versão de referência, ou uma etapa já avançada do projeto. Vale conferir se é essa mesma a versão a ser entregue.

Um detalhe importante: `Renderer::render()` **não usa** `Camera::generateRay()` — ele recalcula a posição do pixel na janela manualmente usando `Janela` e assume a câmera sempre olhando na direção -z a partir de `eye_`. Isso funciona para a cena de exemplo, mas ignora `lookAt_`/`up_`/`fov_` da câmera (o método `generateRay`, que usa a base `u_,v_,w_` e o FOV corretamente, fica sem uso). Se a disciplina pedir suporte a câmeras com outras orientações, esse é o ponto a revisar.

## Como rodar

### Opção A — usando CMake (recomendado, `CMakeLists.txt` incluso)

1. **Instalar o GLFW**
    - Linux (Ubuntu/Debian): `sudo apt install libglfw3-dev`
    - macOS (Homebrew): `brew install glfw`
    - Windows: `vcpkg install glfw3` (ou MSYS2: `pacman -S mingw-w64-x86_64-glfw`)

2. **Configurar e compilar**
   ```bash
   cmake -B build
   cmake --build build
   ```

3. **Executar**
   ```bash
   # Executar visualização do Raytracer
   ./build/raytracer
   # Sem argumentos, o programa localiza sozinho `scenes/scene1.txt`. 
   # Para testar outra cena:
   ./build/raytracer scenes/outra_cena.txt
   ```
   ```bash
   # Executar visualização da Tarefa 1
   ./build/tarefa1
   ```

**Atenção:** `src/*.cpp` vai incluir `Janela 2.cpp` junto com `Janela.cpp`, o que causa erro de linkagem. Apague ou mova `Janela 2.cpp` para fora de `src/` antes de rodar esse comando.

```bash
./raytracer
```