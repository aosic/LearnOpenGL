#include "GLShaderManager.h"

#include "GLTools.h"

#include <glut/glut.h>


GLShaderManager shaderManager;

//窗口大小改变时接受新的宽度和高度，其中0,0代表窗口中视口的左下角坐标，w，h代表像素

void ChangeSize(int w,int h)

{
    
    glViewport(0,0, w, h);
    
}

//为程序作一次性的设置

void SetupRC()

{
    
}

//开始渲染

void RenderScene(void)

{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    //一 现在裁剪出小红色分区
    //1.指定裁剪区的颜色为红色--设置!
    glClearColor(1.0f, 0.0f, 0.0f,1.0f);
    
    //2.设置裁剪尺寸
    glScissor(100,100,600, 400);
    
    //3.开启裁剪
    glEnable(GL_SCISSOR_TEST);
    
    //4.开启清屏,执行裁剪
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    //二 裁剪另外一个绿色的小矩形
    //1.指定裁剪区的颜色为绿色--设置!
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    
    //2.设置裁剪尺寸
    glScissor(200,200,400, 200);
    
    //3.开启裁剪
    glEnable(GL_SCISSOR_TEST);
    
    //4.开启清屏,执行裁剪
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    //关闭裁剪测试
    glDisable(GL_SCISSOR_TEST);
    
    //强制执行缓存区
    glutSwapBuffers();
    
}

void KeyPressFunc(unsigned char key, int x, int y)
{
    
}

void SpecialKeys(int key, int x, int y)
{

}

//鼠标点击事件
void MouseKey(int button,int state, int x,int y)
{
    //button:那个键位，左键(GLUT_LEFT_BUTTON)、右键(GLUT_RIGHT_BUTTON)
    //state:按下(GLUT_UP)/抬起(GLUT_DOWN)
    //x,y：光标位置
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

