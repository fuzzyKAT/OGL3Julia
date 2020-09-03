#ifndef FRACTAL_RENDERER_GL_H_
#define FRACTAL_RENDERER_GL_H_
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class FractalSet
{
public:
    
    // Виртуальный деструктор
    virtual ~FractalSet() {}
    
    // Генерация фрактала
    virtual void createSet(GLfloat *pixels, int width, int height) = 0;
};

class FractalRendererGL3
{
public:
    
    // Конструктор
    FractalRendererGL3(GLint width, GLint height);
    
    // Деструктор
    ~FractalRendererGL3();
    
    // Запуск цикла отрисовки
    void start();
    
private:

    // Чтение файла с именем fname
    std::string readFile(const char *fname);
    
    // Компиляция шейдера из исходника source
    GLuint loadShader(const GLchar *source, GLenum type);
    
    // Сборка шейдерной программы
    GLuint linkShaderProgram(GLuint vrtShader, GLuint fgtShader);

    // Указатель на объект окна
    GLFWwindow *window;
    
    // Шейдерная программа
    GLuint shaderProgram;
    
    // Объект текстуры
    GLuint fractTexture;
    
    // Объекты буферов OpenGL
    GLuint VAO, VBO, EBO;
};

#endif