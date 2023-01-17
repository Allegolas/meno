// Autor: Nedeljko Tesanovic
// Opis: Sablon projekat (ANI) - Neizmjenjen otvara crni prozor

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <math.h>
#include "cglm/cglm.h"
#include "model.h"

/**
 * @brief Compiles GLSL shader
 *
 * @param type Type of shader: GL_VERTEX, GL_FRAGMENT
 * @param source Shader source code
 */
static unsigned CompileShader(GLenum type, const char* source);

/**
 * @brief Creates GLSL shader program
 *
 * @param vertexShaderSource Vertex shader source code
 * @param fragmentShaderSource Fragment shader source code
 */
static unsigned CreateShader(const char* vertexShaderSource, const char* fragmentShaderSource);

/**
 * @brief Sets mat4 uniform in GLSL shader
 *
 * @param programId GLSL Shader program ID
 * @param uniform Uniform name
 * @param m Pointer to matrix
 *
 */
static void SetUniform4m(unsigned programId, const char* uniform, const mat4* m);

int main(void)
{
    // GLFW INIT
    if (!glfwInit())
    {
        printf("GLFW Biblioteka se nije ucitala! :(\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // WINDOW INIT
    GLFWwindow* window;
    float scaleWindow = 0.8;
    unsigned int wWidth = 2560 * scaleWindow;
    unsigned int wHeight = 1440 * scaleWindow;
    const char wTitle[] = "Egipat - AI 11/2021 - Tommy";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);
    if (window == NULL) 
    {
        printf("Prozor nije napravljen! :(\n");
        glfwTerminate(); 
        return 1;
    }
    glfwMakeContextCurrent(window);
    
    // GLEW INIT
    if (glewInit() != GLEW_OK)
    {
        printf("GLEW nije mogao da se ucita! :'(\n");
        return 1;
    }

    //------------------------------------------------------------------------------------

    // SETTINGS
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    float intensity = 0.2f;
    glClearColor(0.3f * intensity, 0.5f * intensity, 1.0f * intensity, 1.0f);

    // SHADER PROGRAM
    unsigned int unifiedShader = CreateShader("shaders/basic.vert", "shaders/basic.frag");


    // VERTEX DATA
    float lighting, r, g, b;
    lighting = 1.0f;
    r = 92; g = 74; b = 52;
    float plane_data[] = {
        -150.0f,  0.0f, -15.0f,   r / 255.0f * lighting,  g / 255.0f * lighting,  b / 255.0f * lighting,
        -150.0f,  0.0f,  150.0f, 96 / 255.0f * lighting, 74 / 255.0f * lighting, 53 / 255.0f * lighting,
         150.0f,  0.0f,  150.0f, 96 / 255.0f * lighting, 74 / 255.0f * lighting, 53 / 255.0f * lighting,
        -150.0f,  0.0f, -15.0f,   r / 255.0f * lighting,  g / 255.0f * lighting,  b / 255.0f * lighting,
         150.0f,  0.0f,  150.0f, 96 / 255.0f * lighting, 74 / 255.0f * lighting, 53 / 255.0f * lighting,
         150.0f,  0.0f, -15.0f,   r / 255.0f * lighting,  g / 255.0f * lighting,  b / 255.0f * lighting,

    };
    lighting = 1.0f;
    r = 104 * 1.0f; g = 81 * 1.0f; b = 57 * 1.0f;
    float pyramid_data[] = {
         0.6f,  0.0f,  0.6f, r / 255.0f * lighting,         g / 255.0f * lighting,         b / 255.0f * lighting,
         0.6f,  0.0f, -0.6f, r / 255.0f * lighting,         g / 255.0f * lighting,         b / 255.0f * lighting,
         0.0f,  1.0f,  0.0f, r / 255.0f * lighting,         g / 255.0f * lighting,         b / 255.0f * lighting,
         0.6f,  0.0f, -0.6f, r / 255.0f * lighting * 0.95f, g / 255.0f * lighting * 0.95f, b / 255.0f * lighting * 0.95f,
        -0.6f,  0.0f, -0.6f, r / 255.0f * lighting * 0.95f, g / 255.0f * lighting * 0.95f, b / 255.0f * lighting * 0.95f,
         0.0f,  1.0f,  0.0f, r / 255.0f * lighting * 0.95f, g / 255.0f * lighting * 0.95f, b / 255.0f * lighting * 0.95f,
         
        -0.6f,  0.0f, -0.6f, r / 255.0f * lighting,         g / 255.0f * lighting,         b / 255.0f * lighting,
        -0.6f,  0.0f,  0.6f, r / 255.0f * lighting,         g / 255.0f * lighting,         b / 255.0f * lighting,
         0.0f,  1.0f,  0.0f, r / 255.0f * lighting,         g / 255.0f * lighting,         b / 255.0f * lighting,
        -0.6f,  0.0f,  0.6f, r / 255.0f * lighting,         g / 255.0f * lighting,         b / 255.0f * lighting,
         0.6f,  0.0f,  0.6f, r / 255.0f * lighting,         g / 255.0f * lighting,         b / 255.0f * lighting,
         0.0f,  1.0f,  0.0f, r / 255.0f * lighting,         g / 255.0f * lighting,         b / 255.0f * lighting,
         
    };

    lighting = 1.0f;
    r = 200 * 1.0f; g = 241 * 1.0f; b = 255 * 1.0f;

    float moon_data[] = {
       0.6f,  0.0f,  0.6f, r / 255.0f * lighting * 0.6f, g / 255.0f * lighting * 0.9f, b / 255.0f * lighting * 0.9f,
       0.6f,  0.0f, -0.6f, r / 255.0f * lighting * 0.6f, g / 255.0f * lighting * 0.9f, b / 255.0f * lighting * 0.9f,
       0.0f,  1.0f,  0.0f, r / 255.0f * lighting * 0.6f, g / 255.0f * lighting * 0.9f, b / 255.0f * lighting * 0.9f,
       0.6f,  0.0f, -0.6f, r / 255.0f * lighting * 0.7f, g / 255.0f * lighting * 0.9f, b / 255.0f * lighting * 1.0f,
      -0.6f,  0.0f, -0.6f, r / 255.0f * lighting * 0.7f, g / 255.0f * lighting * 0.9f, b / 255.0f * lighting * 1.0f,
       0.0f,  1.0f,  0.0f, r / 255.0f * lighting * 0.7f, g / 255.0f * lighting * 0.9f, b / 255.0f * lighting * 1.0f,

      -0.6f,  0.0f, -0.6f, r / 255.0f * lighting,        g / 255.0f * lighting,        b / 255.0f * lighting,
      -0.6f,  0.0f,  0.6f, r / 255.0f * lighting,        g / 255.0f * lighting,        b / 255.0f * lighting,
       0.0f,  1.0f,  0.0f, r / 255.0f * lighting,        g / 255.0f * lighting,        b / 255.0f * lighting,
      -0.6f,  0.0f,  0.6f, r / 255.0f * lighting,        g / 255.0f * lighting,        b / 255.0f * lighting,
       0.6f,  0.0f,  0.6f, r / 255.0f * lighting,        g / 255.0f * lighting,        b / 255.0f * lighting,
       0.0f,  1.0f,  0.0f, r / 255.0f * lighting,        g / 255.0f * lighting,        b / 255.0f * lighting,

    };

    r = 43 / 255.0f;
    g = 150 / 255.0f;
    b = 180 / 255.0f;

   float r1 = 17 / 255.0f;
   float g1 = 120 / 255.0f;
   float b1 = 140 / 255.0f;

    float r2 = 41 / 255.0f;
    float g2 = 209 / 255.0f;
    float b2 = 255 / 255.0f;

    //41, 209, 


    float carpet_data[] = {
        
        -1.0f,  0.0f, -1.0f, 1.0f * r1, 1.0f * g1, 1.0f * b1,
         1.0f,  0.0f, -1.0f, 1.0f * r1, 1.0f * g1, 1.0f * b2,
        -1.0f,  0.0f,  1.0f, 1.0f * r1, 1.0f * g1, 1.0f * b1,

         1.0f,  0.0f, -1.0f, 1.0f * r1, 1.0f * g2, 1.0f * b1,
         1.0f,  0.0f,  1.0f, 1.0f * r2, 1.0f * g2, 1.0f * b2,
        -1.0f,  0.0f,  1.0f, 1.0f * r2, 1.0f * g2, 1.0f * b2,
                             
        -1.0f,  0.1f, -1.0f, 1.0f * r2, 1.0f * g2, 1.0f * b2,
        -1.0f,  0.1f,  1.0f, 1.0f * r1, 1.0f * g1, 1.0f * b1,
         1.0f,  0.1f, -1.0f, 1.0f , 1.0f * g1, 1.0f * b1,

         1.0f,  0.1f, -1.0f, 1.0f , 1.0f * g1, 1.0f * b1,
        -1.0f,  0.1f,  1.0f, 1.0f * r1, 1.0f * g1, 1.0f * b1,
         1.0f,  0.1f,  1.0f, 1.0f * r2, 1.0f * g2, 1.0f * b2,
        
        -1.0f,  0.0f,  1.0f, 1.0f * r, 1.0f * g, 1.0f * b,
         1.0f,  0.1f,  1.0f, 1.0f * r, 1.0f * g, 1.0f * b,
        -1.0f,  0.1f,  1.0f, 1.0f * r, 1.0f * g, 1.0f * b,

        -1.0f,  0.0f,  1.0f, 1.0f * r, 1.0f * g, 1.0f * b,
         1.0f,  0.0f,  1.0f, 1.0f * r, 1.0f * g, 1.0f * b,
         1.0f,  0.1f,  1.0f, 1.0f * r, 1.0f * g, 1.0f * b,
                             
         1.0f,  0.0f, -1.0f, 1.0f, 1.0f * g, 1.0f * b,
         1.0f,  0.1f, -1.0f, 1.0f, 1.0f * g, 1.0f * b,
         1.0f,  0.1f,  1.0f, 1.0f * r, 1.0f * g, 1.0f * b,

         1.0f,  0.0f, -1.0f, 1.0f * r, 1.0f * g, 1.0f * b,
         1.0f,  0.1f,  1.0f, 1.0f * r, 1.0f * g, 1.0f * b,
         1.0f,  0.0f,  1.0f, 1.0f * r, 1.0f * g, 1.0f * b,
    };

    // VAOs, VBOs
    unsigned int plane_vao, plane_vbo;
    {
        glGenVertexArrays(1, &plane_vao);
        glBindVertexArray(plane_vao);

        glGenBuffers(1, &plane_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, plane_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(plane_data), plane_data, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    unsigned int pyramid_vao, pyramid_vbo;
    {
        glGenVertexArrays(1, &pyramid_vao);
        glBindVertexArray(pyramid_vao);

        glGenBuffers(1, &pyramid_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, pyramid_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_data), pyramid_data, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    unsigned int carpet_vao, carpet_vbo;
    {
        glGenVertexArrays(1, &carpet_vao);
        glBindVertexArray(carpet_vao);

        glGenBuffers(1, &carpet_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, carpet_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(carpet_data), carpet_data, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    unsigned int moon_vao, moon_vbo;
    {
        glGenVertexArrays(1, &moon_vao);
        glBindVertexArray(moon_vao);

        glGenBuffers(1, &moon_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, moon_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(moon_data), moon_data, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    }


    // .obj MODEL
    Model camile;
    if (!LoadModel("kamila.obj", &camile)) printf("Failed to open \"*.obj\"");

    // TRANSFORMATIONS
    mat4 model, view, projection;
    glm_mat4_identity(&model);
    glm_mat4_identity(&view);
    glm_mat4_identity(&projection);


    vec3 pos = { 0.0f, 1.0f, 0.0f };
    vec3 dir = { 0.0f, -1.0f, 0.0 }; // (0, 0, 0) !!?
    //vec3 pos = { 4.1f,  4.0f, -1.0f };
    //vec3 dir = { 0.0f,  -1.0f, 0.1f };
    vec3 up = { 1.0f,  0.0f,  0.0f };
    vec3 camile_start_pos = { 0.0f, 2.011f, 10.0f };
    vec3 camile_end_pos = { pos[0], pos[1], pos[2] - 0.4f};

    float translate_vector[] = { 0.0f, 0.0f, 0.0f };
    float scale_vector[] = { 1.0f, 1.0f, 1.0f };

    //glm_translate(&model, translate_vector);
    glm_look(pos, dir, up, &view);
    float t = 0.90f, speed = 0.001f, time = 0.0f;

    // MAIN LOOP
    while (!glfwWindowShouldClose(window))
    {
        time += 0.02f;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) t -= speed;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) t += speed;
        if (t < 0.0f) t = 0.0f;
        else if (t > 1.0f) t = 1.0f;

        // GET WINDOW SIZE, UPDATE ASPECT RATIO
        glfwGetWindowSize(window, &wWidth, &wHeight);
        glViewport(0, 0, wWidth, wHeight);
        glm_perspective(glm_rad(47.0f), (float)wWidth / (float)wHeight, 0.1f, 100.0f, &projection);

        // USE SHADER PROGRAM, PASS TRANSFORMATION MATRICES AS UNIFORMS
        glUseProgram(unifiedShader);
        SetUniform4m(unifiedShader, "uView", view);
        SetUniform4m(unifiedShader, "uProjection", projection);


        //// RENDER
        // PLANE
        glBindVertexArray(plane_vao);
        {
            glm_mat4_identity(&model);
            glm_rotate(model, glm_rad(180.0f), up);
            translate_vector[0] = 0.0f;
            translate_vector[1] = 0.0f;
            translate_vector[2] = -100.0f;
            glm_translate(&model, translate_vector);
            SetUniform4m(unifiedShader, "uModel", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);

        }
        // PYRAMIDS
        glBindVertexArray(pyramid_vao);
        // PYRAMID 1
        {
            glm_mat4_identity(&model);
            translate_vector[0] = 0.0f;
            translate_vector[1] = 0.0f;
            translate_vector[2] = 10.0f;
            glm_translate(&model, translate_vector);
            scale_vector[0] = 2.0f;
            scale_vector[1] = 2.0f;
            scale_vector[2] = 2.0f;
            glm_scale(&model, scale_vector);
            SetUniform4m(unifiedShader, "uModel", model);
            glDrawArrays(GL_TRIANGLES, 0, 12);
        }
        // PYRAMID 2
        {
            glm_mat4_identity(&model);
            translate_vector[0] = -2.8f;
            translate_vector[1] = 0.0f;
            translate_vector[2] = 8.2f;
            glm_translate(&model, translate_vector);
            scale_vector[0] = 1.2f;
            scale_vector[1] = 1.2f;
            scale_vector[2] = 1.2f;
            glm_scale(&model, scale_vector);
            SetUniform4m(unifiedShader, "uModel", model);
            glDrawArrays(GL_TRIANGLES, 0, 12);
        }
        // PYRAMID 3
        {
            glm_mat4_identity(&model);
            translate_vector[0] = 3.0f;
            translate_vector[1] = 0.0f;
            translate_vector[2] = 8.4f;
            glm_translate(&model, translate_vector);
            scale_vector[0] = 1.4f;
            scale_vector[1] = 1.4f;
            scale_vector[2] = 1.4f;
            glm_scale(&model, scale_vector);
            SetUniform4m(unifiedShader, "uModel", model);
            glDrawArrays(GL_TRIANGLES, 0, 12);
        }
        // PYRAMID 4
        {
            glm_mat4_identity(&model);
            translate_vector[0] = -4.0f;
            translate_vector[1] = 0.0f;
            translate_vector[2] = 6.0f;
            glm_translate(&model, translate_vector);
            SetUniform4m(unifiedShader, "uModel", model);
            glDrawArrays(GL_TRIANGLES, 0, 12);
        }
        // PYRAMID 5
        {
            glm_mat4_identity(&model);
            translate_vector[0] = 4.3f;
            translate_vector[1] = 0.0f;
            translate_vector[2] = 4.5f;
            glm_translate(&model, translate_vector);
            scale_vector[0] = 0.9f;
            scale_vector[1] = 0.9f;
            scale_vector[2] = 0.9f;
            glm_scale(&model, scale_vector);
            SetUniform4m(unifiedShader, "uModel", model);
            glDrawArrays(GL_TRIANGLES, 0, 12);

        }

        //MOON
        {
            glBindVertexArray(moon_vao);
            glm_mat4_identity(&model);
            translate_vector[0] = 2.0f;
            translate_vector[1] = 8.0f;
            translate_vector[2] = 50.0f;
            glm_translate(&model, translate_vector);
            scale_vector[0] = 2.0f *1.5;
            scale_vector[1] = 1.42f * 1.5;
            scale_vector[2] = 2.0f * 1.5;
            glm_scale(&model, scale_vector);
            vec3 axis = { 1.0f, 0.0f, 0.0f };
            glm_rotate(model, glm_rad(30.0f), axis);

            SetUniform4m(unifiedShader, "uModel", model);
            glDrawArrays(GL_TRIANGLES, 0, 12);
            axis[0] = 1.0f;
            axis[1] = 0.0f;
            axis[2] = 1.0f;
            glm_rotate(model, glm_rad(180.0f), axis);
            SetUniform4m(unifiedShader, "uModel", model);
            glDrawArrays(GL_TRIANGLES, 0, 12);




        }


        // CARPET
        {
            glm_mat4_identity(&model);
            translate_vector[0] = camile_start_pos[0] + t * (camile_end_pos[0] - camile_start_pos[0]) - 0.0f;
            translate_vector[1] = camile_start_pos[1] + t * (camile_end_pos[1] - camile_start_pos[1]) - 0.013f + sin(time) * 0.002f;
            translate_vector[2] = camile_start_pos[2] + t * (camile_end_pos[2] - camile_start_pos[2]) - 0.04f;
            glm_translate(&model, translate_vector);
            glm_rotate(model, glm_rad(80.0f), up);
            scale_vector[0] = 0.06f;
            scale_vector[1] = 0.01f;
            scale_vector[2] = 0.04f;
            glm_scale(&model, scale_vector);
            glBindVertexArray(carpet_vao);
            SetUniform4m(unifiedShader, "uModel", model);
            glDrawArrays(GL_TRIANGLES, 0, 24);
        }
        // CAMEL - MODEL(.obj)
        {
            glm_mat4_identity(&model);
            translate_vector[0] = camile_start_pos[0] + t * (camile_end_pos[0] - camile_start_pos[0]) - 0.0f;
            translate_vector[1] = camile_start_pos[1] + t * (camile_end_pos[1] - camile_start_pos[1]) - 0.0f + sin(time) * 0.002f;
            translate_vector[2] = camile_start_pos[2] + t * (camile_end_pos[2] - camile_start_pos[2]) - 0.0f;
            glm_translate(&model, translate_vector);
            scale_vector[0] = 0.001f;
            scale_vector[1] = 0.001f;
            scale_vector[2] = 0.001f;
            glm_scale(&model, scale_vector);
            SetUniform4m(unifiedShader, "uModel", model);
            RenderModel(&camile);
        }

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}

unsigned CompileShader(GLenum type, const char* source) {
    unsigned int id, result, logLength;
    char* sourceCode = NULL, * errorMessage = NULL;
    long shaderSize;
    FILE* inputFile = 0;

    id = glCreateShader(type);
    inputFile = fopen(source, "r");
    if (!inputFile)
    {
        fprintf(stderr, "ERROR: %s file not found.\n", source);
        return 0;
    }

    fseek(inputFile, 0L, SEEK_END);
    shaderSize = ftell(inputFile);
    fseek(inputFile, 0L, SEEK_SET);
    sourceCode = (char*)calloc(shaderSize, sizeof(char));

    if (!sourceCode) {
        fprintf(stderr, "ERROR: Could not load source code.\n");
        return 0;
    }

    fread(sourceCode, sizeof(char), shaderSize, inputFile);
    fclose(inputFile);

    glShaderSource(id, 1, &sourceCode, NULL);
    glCompileShader(id);

    free(sourceCode);

    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
        errorMessage = (char*)calloc(logLength, sizeof(char));
        glGetShaderInfoLog(id, logLength, &logLength, errorMessage);
        fprintf(stderr, "ERROR compiling shader: %s\n", errorMessage);
        free(errorMessage);
    }

    return id;
}

unsigned CreateShader(const char* vertexShaderSource, const char* fragmentShaderSource) {
    unsigned int program, vertexShader, fragmentShader;

    program = glCreateProgram();
    vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

static void SetUniform4m(unsigned programId, const char* uniform, const mat4* m) {
    glUniformMatrix4fv(glGetUniformLocation(programId, uniform), 1, GL_FALSE, (float*)m);
}