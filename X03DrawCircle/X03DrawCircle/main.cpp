#include <iostream>
#include <GLUT/GlUT.h>
#include "math3d.h"

//开始渲染

void RenderScene(void)

{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINE_LOOP);
    const int n = 55;
    const float R = 0.5f;
    const GLfloat PI = 3.1415926f;
    for (int i = 0; i < n; i++) {
        glVertex2d(R *cos(2 * PI / n * i), R * sin(2 * PI / n * i));
    }
    glEnd();
    glFlush();
    
}

int main(int argc,char* argv[])

{
    
    glutInit(&argc, argv);
    
    glutCreateWindow("Triangle");
    
    glutDisplayFunc(RenderScene);
    
    glutMainLoop();
    
    return 0;
    
}

