cmake_minimum_required(VERSION 3.15)
project(PyRayCasting CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Linux:   sudo apt install libglfw3-dev
# macOS:   brew install glfw
# Windows: vcpkg install glfw3   (ou MSYS2: pacman -S mingw-w64-x86_64-glfw)
# ---------------------------------------------------------------------------
find_package(glfw3 QUIET)
find_package(OpenGL QUIET)

set(HEADER_DIR "${CMAKE_SOURCE_DIR}/header")
set(SRC_DIR    "${CMAKE_SOURCE_DIR}/src")

if(glfw3_FOUND AND OpenGL_FOUND)

    # Alvo "raytracer": pipeline completo (Camera, Scene, SceneLoader,
    # Renderer, Display). Le todos os .cpp de src/, exceto:
    #   - "Janela 2.cpp"  -> duplicado de Janela.cpp (simbolo repetido)
    #   - "Tarefa1.cpp"   -> tem seu proprio main(), nao entra aqui
    file(GLOB RAYTRACER_SOURCES CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")
    list(FILTER RAYTRACER_SOURCES EXCLUDE REGEX ".*Janela 2\\.cpp$")
    list(FILTER RAYTRACER_SOURCES EXCLUDE REGEX ".*/Tarefa1\\.cpp$")

    add_executable(raytracer ${RAYTRACER_SOURCES})
    target_include_directories(raytracer PRIVATE "${HEADER_DIR}")
    target_link_libraries(raytracer PRIVATE glfw OpenGL::GL)

    # main.cpp resolve o caminho de scenes/scene1.txt a partir do proprio
    # __FILE__, entao nenhuma copia extra de arquivos e necessaria para
    # rodar a partir da raiz do projeto ou do diretorio de build.

    # Alvo "tarefa1": pinta uma esfera (hit/no-hit, sem Phong), salva em
    # canvas.ppm E mostra numa janela via Display::show
    add_executable(tarefa1
            "${SRC_DIR}/Tarefa1.cpp"
            "${SRC_DIR}/Vector3.cpp"
            "${SRC_DIR}/Ray.cpp"
            "${SRC_DIR}/Color.cpp"
            "${SRC_DIR}/Material.cpp"
            "${SRC_DIR}/Sphere.cpp"
            "${SRC_DIR}/Janela.cpp"
            "${SRC_DIR}/Framebuffer.cpp"
            "${SRC_DIR}/Display.cpp"
    )
    target_include_directories(tarefa1 PRIVATE "${HEADER_DIR}")
    target_link_libraries(tarefa1 PRIVATE glfw OpenGL::GL)

else()
    message(WARNING "GLFW e/ou OpenGL nao encontrados -- nenhum alvo sera criado (instale libglfw3-dev / brew install glfw).")
endif()