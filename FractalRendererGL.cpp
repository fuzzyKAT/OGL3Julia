#include "FractalRendererGL.h"
#include <fstream>
#include <cmath>

FractalRendererGL3::FractalRendererGL3(GLint width, GLint height)
{
    // Инициализация GLFW
    glfwInit();
    // Задается минимальная требуемая версия OpenGL. 
    // Мажорная 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // Минорная
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Установка профайла для которого создается контекст
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Выключение возможности изменения размера окна
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    // Создаем окно
    window = glfwCreateWindow(width, height, "FractalRendererGL", nullptr, nullptr);

    if(window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create a window\n");
    }
    
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    
    if(glewInit() != GLEW_OK)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to init GLEW\n");
    }
    
    //
    glViewport(0, 0, width, height);
    
    // Загрузка шейдеров
    std::string vrtShaderSrc = readFile("vrtShader.glsl");
    std::string fgtShaderSrc = readFile("fgtShader.glsl");
    const char *vrtShaderStr = vrtShaderSrc.c_str();
    const char *fgtShaderStr = fgtShaderSrc.c_str();
    
    GLuint vrtShader = 0;
    GLuint fgtShader = 0;
    
    try
    {
        vrtShader = loadShader(vrtShaderStr, GL_VERTEX_SHADER);
        fgtShader = loadShader(fgtShaderStr, GL_FRAGMENT_SHADER);    
        shaderProgram = linkShaderProgram(vrtShader, fgtShader);
    }
    catch(std::runtime_error &error)
    {
        glfwTerminate();
        throw std::runtime_error("Error : Failed to load shaders");
    }
    
    glDeleteShader(vrtShader);
    glDeleteShader(fgtShader);
    
    // Вершинные данные
    GLfloat vertData[] = 
    {
         // Координаты    // Текстурные координаты
         1.0f,  1.0f,     1.0f, 1.0f,   // Верхний правый
         1.0f, -1.0f,     1.0f, 0.0f,   // Нижний правый
        -1.0f, -1.0f,     0.0f, 0.0f,   // Нижний левый
        -1.0f,  1.0f,     0.0f, 1.0f    // Верхний левый
    };
    
    GLuint indices[] = 
    {
        0, 1, 3,   // Первый треугольник
        1, 2, 3    // Второй треугольник
    };
    
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertData), vertData, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
    
    // Настрйка входных параметров вершинного шейдера
    // Параметр координаты
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Параметр текстурной координаты
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)(2*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
 
    glBindVertexArray(0);
    
    // Параметры привязки текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // Параметры фильтрации текстур
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

FractalRendererGL3::~FractalRendererGL3()
{
    glfwTerminate();
}

void FractalRendererGL3::start()
{
    GLint addCoeffs = glGetUniformLocation(shaderProgram, "addCoeffs");
    GLint maxIter = glGetUniformLocation(shaderProgram, "maxIter");
    GLint initPoint = glGetUniformLocation(shaderProgram, "initPoint");
    
    glUseProgram(shaderProgram);
    glUniform3f(addCoeffs, 37.0f, 35.0f, 31.5f);
    glUniform1i(maxIter, 1000);
    
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        
        GLfloat ct = glfwGetTime();
        glUniform2f(initPoint, 0.7f*sin(ct/8), 0.7f*cos(ct/8));
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
    }
}

std::string FractalRendererGL3::readFile(const char *fname)
{
    std::ifstream ifs(fname);
    std::string str;
    char c;
    
    while(ifs)
    {
        ifs.get(c);
        if(ifs)
        {
            str += c;
        }
    }
    
    return str;
}

GLuint FractalRendererGL3::loadShader(const GLchar *source, GLenum type)
{
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        throw std::runtime_error(infoLog);
    }

    return shader;
}

GLuint FractalRendererGL3::linkShaderProgram(GLuint vrtShader, GLuint fgtShader)
{
    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vrtShader);
    glAttachShader(shaderProgram, fgtShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        throw std::runtime_error(infoLog);
    }

    return shaderProgram;
}