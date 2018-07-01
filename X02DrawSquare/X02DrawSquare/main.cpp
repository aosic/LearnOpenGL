#include "GLShaderManager.h"

#include "GLTools.h"

#include <glut/glut.h>

GLBatch triangleBatch;

GLShaderManager shaderManager;

// 正方形边长/2
GLfloat blockSize = 0.25f;

// 正方形顶点
GLfloat vVerts[] =  {
    -blockSize,-blockSize,0.0f,
    -blockSize,+blockSize,0.0f,
    +blockSize,+blockSize,0.0f,
    +blockSize,-blockSize,0.0f
};

//窗口大小改变时接受新的宽度和高度，其中0,0代表窗口中视口的左下角坐标，w，h代表像素

void ChangeSize(int w,int h)

{
    
    glViewport(0,0, w, h);
    
}

//为程序作一次性的设置

void SetupRC()

{
    
    //设置背影颜色
    
    glClearColor(0.0f,0.0f,1.0f,1.0f);
    
    //初始化着色管理器
    
    shaderManager.InitializeStockShaders();
    
    //批次处理
    
    triangleBatch.Begin(GL_TRIANGLE_FAN,4);
    
    triangleBatch.CopyVertexData3f(vVerts);
    
    triangleBatch.End();
    
}

void SpecialKeys(int key, int x, int y){
    
    // 定义步长
    GLfloat stepSize = 0.1f;
    
    // 相对点
    GLfloat blockX = vVerts[0];
    GLfloat blockY = vVerts[1];
    
    if (key == GLUT_KEY_UP) {
        blockY += stepSize;
    }
    
    if (key == GLUT_KEY_DOWN) {
        blockY -= stepSize;
    }
    
    if (key == GLUT_KEY_LEFT) {
        blockX -= stepSize;
    }
    
    if (key == GLUT_KEY_RIGHT) {
        blockX += stepSize;
    }
    
    // 触碰到边界（4个边界）的处理
    if (blockX < -1.0f) {
        blockX = -1.0f;
    }
    
    if (blockX > (1.0 - blockSize * 2)) {
        blockX = 1.0f - blockSize * 2;
    }
    
    if (blockY > 1.0f - blockSize * 2 ) {
        blockY = 1.0f - blockSize * 2;
    }
    
    if (blockY < -1.0f) {
        blockY = -1.0f;
    }
    
    // 更新点数据
    vVerts[0] = blockX;
    vVerts[1] = blockY;
    
    vVerts[3] = blockX;
    vVerts[4] = blockY + blockSize * 2;
    
    vVerts[6] = blockX + blockSize * 2;
    vVerts[7] = blockY + blockSize * 2;
    
    vVerts[9] = blockX + blockSize * 2;
    vVerts[10] = blockY;
    
    // 提交新顶点数据
    triangleBatch.CopyVertexData3f(vVerts);
    
    glutPostRedisplay();
}

//开始渲染

void RenderScene(void)

{
    
    //清除一个或一组特定的缓冲区
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    
    //设置一组浮点数来表示红色
    
    GLfloat vRed[] = {1.0f,0.0f,0.0f,1.0f};
    
    //传递到存储着色器，即GLT_SHADER_IDENTITY着色器，这个着色器只是使用指定颜色以默认笛卡尔坐标第在屏幕上渲染几何图形
    
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vRed);
    
    //提交着色器
    
    triangleBatch.Draw();
    
    //将在后台缓冲区进行渲染，然后在结束时交换到前台
    
    glutSwapBuffers();
    
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
    
    glutDisplayFunc(RenderScene);
    
    //注册特殊函数
    glutSpecialFunc(SpecialKeys);
    
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

