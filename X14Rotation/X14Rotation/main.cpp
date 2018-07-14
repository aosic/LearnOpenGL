
#include "GLTools.h"	
#include "GLMatrixStack.h"
#include "GLFrame.h"
#include "GLFrustum.h"
#include "GLGeometryTransform.h"
#include "GLBatch.h"
#include "StopWatch.h"

#include <math.h>
#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

//视景体-投影矩阵通过它来设置
GLFrustum           viewFrustum;
//固定着色管理器
GLShaderManager     shaderManager;
//三角形批次类，绘制图形
GLTriangleBatch     torusBatch;


// 设置视口和投影矩阵
void ChangeSize(int w, int h)
{
 
    if (h==0) {
        h = 1;
    }
    glViewport(0, 0, w, h);
    
    //设置投影矩阵
    viewFrustum.SetPerspective(35.0f, float(w)/float(h), 1.0f, 1000.0f);
    
    
}


//召唤场景
void RenderScene(void)
{
   //建立一个基于时间变化的动画
    static CStopWatch rotTimer;
    
    //当前时间 * 60s
    float yRot = rotTimer.GetElapsedSeconds() * 60.0f;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //矩阵变量
    M3DMatrix44f mTranlate,mRotate,mModelView,mModelViewProjection;
    
    //将圆球像Z轴负方向移动2.5个单位长度
    m3dTranslationMatrix44(mTranlate, 0.0f , 0.0f,-2.5f);
    
    //旋转
    m3dRotationMatrix44(mRotate, m3dDegToRad(yRot), 0.0f, 1.0f, 0.0f);
    
    //将平移和旋转的矩阵进行叉乘,产生一个新的矩阵mModelView
    m3dMatrixMultiply44(mModelView, mTranlate, mRotate);
    
    //模型视图矩阵 和 投影矩阵
    //将投影矩阵 与 模型视图矩阵进行叉乘 ,将变化最终结果通过矩阵叉乘的方式应用到mModelViewProjection中来
    m3dMatrixMultiply44(mModelViewProjection, viewFrustum.GetProjectionMatrix(), mModelView);
    
    GLfloat vBlack[] = {0.0f,0.0f,0.0f,1.0f};
    
    //平面着色器来渲染图像
    shaderManager.UseStockShader(GLT_SHADER_FLAT,mModelViewProjection,vBlack);
    
    //开始绘图
    torusBatch.Draw();
    
    glutSwapBuffers();
    glutPostRedisplay();
    
    
    
}


void SetupRC()
{
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    
    glEnable(GL_DEPTH_TEST);
    
    shaderManager.InitializeStockShaders();
    
    gltMakeSphere(torusBatch, 0.4f, 10, 20);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
}



int main(int argc, char* argv[])
{
    gltSetWorkingDirectory(argv[0]);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("ModelViewProjection Example");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    
    
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    
    SetupRC();
    
    glutMainLoop();
    return 0;
}
