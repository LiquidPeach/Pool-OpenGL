#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Pool.h"

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(900, 606, "Pool", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

    gladLoadGL();

    Pool pool(window, 900, 606);

    while (!glfwWindowShouldClose(window))
    {
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