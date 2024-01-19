#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shaderClasses/shader_s.h>

#include <iostream>
#include <cmath>

#include "stb_image.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

float mixCoefficient = 0.5f;

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRightAxis = glm::normalize(glm::cross(cameraUp, cameraDirection));
glm::vec3 cameraUpAxis = glm::cross(cameraDirection, cameraRightAxis);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);


glm::mat4 viewMatrix;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

int main()
{

    //float yaw = -90.0f;
    //glm::vec3 direction;
    //direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    //direction.y = sin(glm::radians(pitch));
    //direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));


    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Capture mouse cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // Get the resolution of the primary monitor
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    // Calculate the center position
    int xPos = (mode->width - SCR_WIDTH) / 2;
    int yPos = (mode->height - SCR_HEIGHT) / 2;

    // Set the window position on the center of the screen
    glfwSetWindowPos(window, xPos, yPos);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("vertexShader.glsl", "fragmentShader.glsl"); // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,     0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,	  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,     0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 0.0f,     0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 0.0f,     0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 0.0f,     0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,     1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,     0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,     0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 0.0f,     0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 0.0f,     1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,     1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,     0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,     0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 0.0f,     0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,     1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,     0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,     1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 0.0f,     0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,     0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,     0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,     1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,     1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 0.0f,     0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,     0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    //unsigned int indices[] = {
    //    0, 1, 3, // first triangle
    //    1, 2, 3  // second triangle
    //};
    unsigned int VBO, VAO
        //, EBO
        ;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coords attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int texture1,texture2;

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Enabling depth buffer
    glEnable(GL_DEPTH_TEST);

    // load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("uv_grid_opengl.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    ourShader.use();
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // set it manually
    ourShader.setInt("texture2", 1); // or with shader class

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.58f, 0.54f, 0.59f, 1.0f);

        // clearing the buffers so that the informations of the previous frames don't stay in the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double timeValue = glfwGetTime();


        // camera variables

        //const float radius = 10.0f;
        //float camX = sin(glfwGetTime()) * radius;
        //float camZ = cos(glfwGetTime()) * radius;
        //cameraPosition = glm::vec3(camX, 0.0, camZ);

        viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // render the triangle
        ourShader.use();

        //glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        //transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        //transform = glm::rotate(transform, (float)timeValue, glm::vec3(0.0f, 0.0f, 1.0f));
        float redValue = static_cast<float>(sin(timeValue * 3.5 ) / 2.0 + 0.5);
        float greenValue = static_cast<float>(sin(timeValue * 1.5) / 2.0 + 0.5);
        float blueValue = static_cast<float>(sin(timeValue * 2.5) / 2.0 + 0.5);

        // update shader uniform
        ourShader.setFloat4f("varyingColor", redValue, greenValue, blueValue, 1.0f);

        ourShader.setFloat("mixCoefficient", mixCoefficient);

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        //glm::mat4 viewMatrix = glm::mat4(1.0f);
        glm::mat4 projectionMatrix = glm::mat4(1.0f);

        modelMatrix = glm::rotate(modelMatrix, (float)timeValue * glm::radians(45.0f), glm::vec3(0.7f, 1.0f, 0.2f));
        // note that we're translating the scene in the reverse direction of where we want to move
        //viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -8.0f));
        projectionMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        ourShader.setMat4("modelMatrix", modelMatrix);
        ourShader.setMat4("viewMatrix", viewMatrix);
        ourShader.setMat4("projectionMatrix", projectionMatrix);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // render the triangles
        glBindVertexArray(VAO);

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, (float)timeValue * glm::radians(45.0f), glm::vec3(0.7f, 1.0f, 0.2f));

            float angle = 20.0f * i;
            if(i%3)
                model = glm::rotate(model, glm::radians(-angle), glm::vec3(1.0f, 0.0f, 0.5f));
            else
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("modelMatrix", model);

            float redValue = static_cast<float>(sin(timeValue * 3.5 * (i + 0.1)/3) / 2.0 + 0.5);
            float greenValue = static_cast<float>(sin(timeValue * 1.5 * (i + 0.1)/3) / 2.0 + 0.5);
            float blueValue = static_cast<float>(sin(timeValue * 2.5 * (i + 0.1)/3) / 2.0 + 0.5);

            // update shader uniform
            ourShader.setFloat4f("varyingColor", redValue, greenValue, blueValue, 1.0f);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }        
        
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //transform = glm::mat4(1.0f);
        //transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
        //float scaleAmount = static_cast<float>(sin(glfwGetTime()));
        //transform = glm::scale(transform, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixCoefficient += 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (mixCoefficient >= 1.0f)
            mixCoefficient = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixCoefficient -= 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (mixCoefficient <= 0.0f)
            mixCoefficient = 0.0f;
    }

    float cameraSpeed = static_cast<float>(2.5 * deltaTime);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPosition += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPosition -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}