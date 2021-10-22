#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Pool.h"

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(900, 540, "Pool", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

    gladLoadGL();

    Pool pool(window, 900.0f, 540.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.5f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        int resetButton = glfwGetKey(window, GLFW_KEY_R);

        if (resetButton == GLFW_PRESS)
            pool.ResetGame();

        pool.DrawPool();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}