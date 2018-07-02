#include "GLShaderManager.h"

#include "GLTools.h"

#include <glut/glut.h>
#include "GLFrame.h"
#include "GLMatrixStack.h"
#include "GLGeometryTransform.h"
#include "GLFrustum.h"

GLShaderManager     shaderManager;
GLTriangleBatch     torusBatch;

// 相机
GLFrame             viewFrame;

// 矩阵
GLFrustum           viewFrustum;
GLMatrixStack       modelViewMatrix;
GLMatrixStack       projectionMatrix;
GLGeometryTransform transformPipeline;

int isCull = 0;
int isDepth = 0;

//为程序作一次性的设置

void SetupRC()

{
    
    //设置背影颜色
    
    glClearColor(0.0f,0.0f,1.0f,1.0f);
    
    //初始化着色管理器
    
    shaderManager.InitializeStockShaders();
    
    //观察者
    viewFrame.MoveForward(7.0f);
    
    //创建一个甜甜圈
    gltMakeTorus(torusBatch, 1.0f, 0.3f, 52, 26);
    
    //设置点大小
    glPointSize(4.0f);
    
}

//开始渲染

void RenderScene(void)

{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (isCull) {
        // 开启背面剔除功能
        glEnable(GL_CULL_FACE);
        // 指定模型
        glFrontFace(GL_CCW);
        // 执行剔除
        glCullFace(GL_BACK);
    } else {
        glDisable(GL_CULL_FACE);
    }
    
    //设置深度测试
    if (isDepth) {
        
        glEnable(GL_DEPTH_TEST);
    }else
    {
        glDisable(GL_DEPTH_TEST);
        
    }
    
    modelViewMatrix.PushMatrix(viewFrame);
    
    GLfloat vRed[] = {1.0f,0.0f,0.0f,1.0f};
    
    //默认光源着色器
    //参数1:GLT_SHADER_DEFAULT_LIGHT
    //参数2:模型视图矩阵
    //参数3:投影矩阵
    //参数4:颜色
    
    //transformPipeline 管理堆栈,模型视图堆栈\投影矩阵堆栈
    
    shaderManager.UseStockShader(GLT_SHADER_DEFAULT_LIGHT,
                                 transformPipeline.GetModelViewMatrix(),
                                 transformPipeline.GetProjectionMatrix(),vRed);
    
    //绘制
    torusBatch.Draw();
    
    //出栈
    modelViewMatrix.PopMatrix();
    
    glutSwapBuffers();
    
}

void ChangeSize(int w,int h)

{
    //设置视口窗口尺寸
    glViewport(0, 0, w, h);
    
    //setPerspective函数的参数是一个从顶点方向看去的视场角度（用角度值表示）
    // 设置透视模式，初始化其透视矩阵
    viewFrustum.SetPerspective(35.0f, float(w)/float(h), 1.0f, 100.0f);
    
    //把透视矩阵加载到透视矩阵对阵中
    //viewFrustum.GetProjectionMatrix() 获取投影矩阵
    //projectionMatrix.LoadMatrix(矩阵) 把矩阵加载到projectionMatrix中来
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    
    // 初始化渲染管线
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
    
}

void KeyPressFunc(unsigned char key, int x, int y)
{
    
}

void SpecialKeys(int key, int x, int y)
{
    //视觉班第二次已讲！
    if(key == GLUT_KEY_UP)
        viewFrame.RotateWorld(m3dDegToRad(-5.0), 1.0f, 0.0f, 0.0f);
    
    if(key == GLUT_KEY_DOWN)
        viewFrame.RotateWorld(m3dDegToRad(5.0), 1.0f, 0.0f, 0.0f);
    
    if(key == GLUT_KEY_LEFT)
        viewFrame.RotateWorld(m3dDegToRad(-5.0), 0.0f, 1.0f, 0.0f);
    
    if(key == GLUT_KEY_RIGHT)
        viewFrame.RotateWorld(m3dDegToRad(5.0), 0.0f, 1.0f, 0.0f);
    
    //重新刷新window
    glutPostRedisplay();
}

//鼠标点击事件
void MouseKey(int button,int state, int x,int y)
{
    //button:那个键位，左键(GLUT_LEFT_BUTTON)、右键(GLUT_RIGHT_BUTTON)
    //state:按下(GLUT_UP)/抬起(GLUT_DOWN)
    //x,y：光标位置
}

//右键菜单栏选项 点击右击菜单中的选项,则会调用到此处
void ProcessMenu(int value)
{
    switch (value) {
        case 1:
            isCull = !isCull;
            break;
            
        case 2:
            isDepth = !isDepth;
            break;
            
        case 3:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
            
        case 4:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
            
        case 5:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;
            
        default:
            break;
    }
    glutPostRedisplay();
    
}

int main(int argc,char* argv[])

{
    
    //设置当前工作目录，针对MAC OS X
    
    gltSetWorkingDirectory(argv[0]);
    
    //初始化GLUT库
    
    glutInit(&argc, argv);
    
    /*初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区*/
    
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
    
    //GLUT窗口大小，标题窗口
    
    glutInitWindowSize(800,600);
    
    glutCreateWindow("Triangle");
    
    //注册回调函数
    
    glutReshapeFunc(ChangeSize);
    glutKeyboardFunc(KeyPressFunc);
    glutSpecialFunc(SpecialKeys);
    glutMouseFunc(MouseKey);
    glutDisplayFunc(RenderScene);
    
    glutCreateMenu(ProcessMenu);
    
    glutAddMenuEntry("正面背面剔除", 1);
    glutAddMenuEntry("深度测试", 2);
    glutAddMenuEntry("Set Fill Mode", 3);
    glutAddMenuEntry("Set line Mode", 4);
    glutAddMenuEntry("Set Point Mode", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    //驱动程序的初始化中没有出现任何问题。
    
    GLenum err = glewInit();
    
    if(GLEW_OK != err) {
        
        fprintf(stderr,"glew error:%s\n",glewGetErrorString(err));
        
        return 1;
        
    }
    
    //调用SetupRC
    
    SetupRC();
    
    glutMainLoop();
    
    return 0;
    
}

