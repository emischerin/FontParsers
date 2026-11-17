#include "bdf_font.h"
#include "glad.h"
#include <GLFW/glfw3.h> 
#include <iostream>
#include <vector>
#include <thread>




int win_x = 640;
int win_y = 480;

const int max_buffer_size = 1024;
BdfFont font("ter-u22b.bdf");
int pos_x = 10;
int pos_y = 10;

int lix = 10; // line int x
int liy = 10; //line int y

using namespace std::chrono_literals;
//  g++ *.cpp *.c -lglfw -lGL -o gl.exe
const char* vertex_shader =
"#version 410 core\n"
"layout (location = 0) in vec2 vp;"
"out vec4 vertex_out_color;"
"void main() {"
    "gl_Position = vec4( vp, 1.0, 1.0 );"
    "vertex_out_color = vec4(1.0,1.0,1.0,1.0);"
"}";

const char* fragment_shader =
"#version 410 core\n"
"in vec4  vertex_out_color;"
"out vec4 FragColor;"
"void main() {"
    "FragColor = vertex_out_color;"
    
"}";


GLuint vbo = 0;GLuint vao = 0;



std::vector<GLfloat> identity = {1.0f,0.0f,0.0f,
                                 0.0f,1.0f,0.0f,
                                 0.0f,0.0f,1.0f}; 




std::vector<GLfloat> vertices;
std::list<unsigned int> chars_entered; // history for redrawing                                 

inline bool Bitset(unsigned int bit,int number)
{
    int filter = 0 | (1 << bit);
    return ((filter & number) != 0);
}


void OnMouseClick(GLFWwindow* w,int button,int action,int mods)
{
        std::cout << "=======MOUSE CLICKED LOG=========\n";
        GLfloat mx;
        GLfloat my;
        
        int win_h,win_w;
        glfwGetWindowSize(w,&win_w,&win_h);
        float pixel_w_step = 2.0f / (float)(win_w );
        float pixel_h_step = 2.0f / (float)(win_h );
        std::cout << "PIXX" << pixel_w_step << std::endl;
        std::cout << "PIXY" << pixel_h_step << std::endl; 
        std::cout << "WINW:" << win_w << std::endl;
        std::cout << "WINH:" << win_h << std::endl;
        double xpos, ypos;
        glfwGetCursorPos(w, &xpos, &ypos);

        std::cout << "GLFW_X:" << xpos << std::endl;
        std::cout << "GLFW_Y:" << ypos << std::endl;



        mx = -1.0f + (pixel_w_step * (float)xpos );
        my =  (1.0f - (pixel_h_step * (float)ypos) );
        

        

        std::cout << "GLX:" << mx << std::endl;
        std::cout << "GLY:" << my << std::endl;

        std::cout << "======= END MOUSE CLICKED LOG=========\n";

        vertices.push_back(mx);
        vertices.push_back(my);


}  

void KeyPressedCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        int yinc = font.GetBoundingBoxHeight() + 1;

        pos_y += yinc;  //height increment
        pos_x = lix;
    }
}

void WindowResizeCallback(GLFWwindow* window, int width, int height) 
{
    float oxr = 2.0f / (float)win_x; //old xratio
    float oyr = 2.0f / (float)win_y; // old yratio

    float nxr = 2.0f / (float)width;
    float nyr = 2.0f / (float)height;

    float xr = nxr / oxr;
    float yr = nyr / oyr;

    for(int i = 0; i < vertices.size();i += 2){
        vertices[i] = vertices[i] * xr;
        vertices[i+1] = vertices[i+1] * yr;
    }

    win_x = width;
    win_y = height;
    
}



inline float WxToOGLX(int x,GLFWwindow* w)
{
   
    int win_h,win_w;
    glfwGetWindowSize(w,&win_w,&win_h);
    float pixel_x_step = 2.0f / (float)(win_w );
    float pixel_h_step = 2.0f / (float)(win_h );

    return -1.0f + (pixel_x_step * (float)x );


}

inline float WyToOGLY(int y,GLFWwindow* w)
{
   
    int win_h,win_w;
    glfwGetWindowSize(w,&win_w,&win_h);
    float pixel_x_step = 2.0f / (float)(win_w );
    float pixel_y_step = 2.0f / (float)(win_h );

    return (1.0f - (pixel_y_step * (float)y) );


}

inline float GetGLPxStep(GLFWwindow* w)
{
    int win_h,win_w;
    glfwGetWindowSize(w,&win_w,&win_h);
    return 2.0f / (float)(win_w );
}

inline float GetGlPyStep(GLFWwindow* w)
{
    int win_h,win_w;
    glfwGetWindowSize(w,&win_w,&win_h);
    return 2.0f / (float)(win_h );
}

void TextEnteredCallback(GLFWwindow* window, unsigned int codepoint)
{
    BdfChar c = font.GetChar(codepoint);
    size_t bitw = font.GetBitmapBitWith();

    if(c.empty) return;

    chars_entered.push_back(codepoint);

    int yoff = 0;
    for(auto i = c.m_bitmap.begin();i != c.m_bitmap.end();++i){
        int xoff = 0;
        for(int j = (bitw - 1) ; j >=0; --j ){
            if(Bitset(j,*i)){
                float x = WxToOGLX(pos_x + xoff,window);
                float y = WyToOGLY(pos_y + yoff,window);
                vertices.push_back(x);
                vertices.push_back(y);
                
            }
            xoff++;
        }

        yoff++;
    }

    pos_x += (bitw + 1);


}

int main(void)
{
    
    GLFWwindow* window; // Declare a pointer to a GLFW window object
    int err;
    /* Initialize the library */
    if (!glfwInit()) // Attempt to initialize GLFW
        return -1; // Return -1 if initialization fails

    /* Create a windowed mode window and its OpenGL context */
    // Creates a 640x480 window with the title "Hello World"
    window = glfwCreateWindow(win_x, win_y, "Hello World", NULL, NULL); 
    if (!window) // Check if window creation was successful
    {
        glfwTerminate(); // Terminate GLFW if window creation fails
        return -1; // Return -1
    }

    
    
    /* Make the window's context current */
    // Sets the newly created window's OpenGL context as the current one
    glfwMakeContextCurrent(window);

    int glad_v = gladLoadGL();
    
    if(glad_v){
        std::cout << "Loaded OpenGL!" << std::endl;
    }
    /*Vertices*/
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindVertexArray(vao);
    
    glBufferData(GL_ARRAY_BUFFER,max_buffer_size * sizeof(float),NULL,GL_STATIC_DRAW);
    
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 2* sizeof(float), (void*)0 );
    
    
    glEnableVertexAttribArray( 0 );
    
        

    GLuint vs = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vs, 1, &vertex_shader, NULL );
    glCompileShader( vs );
    GLuint fs = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fs, 1, &fragment_shader, NULL );
    glCompileShader( fs );

    GLuint shader_program = glCreateProgram();
    glAttachShader( shader_program, vs );
    glAttachShader( shader_program, fs );
    
    glLinkProgram( shader_program );
    
    std::cout << glGetError() << std::endl;
    
    glfwSetMouseButtonCallback(window,OnMouseClick);
    glfwSetCharCallback(window,TextEnteredCallback);
    glfwSetKeyCallback(window, KeyPressedCallback);
    glfwSetWindowSizeCallback(window,WindowResizeCallback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) // Loop while the window should not close
    {

        
        glfwWaitEvents();

        

        

        /* Render here */
        // Clears the color buffer, preparing for new drawing
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
              
        
        glUseProgram(shader_program);
        
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glBindVertexArray(vao);

        glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(float),vertices.data(),GL_STATIC_DRAW);
        err = glGetError();

        if(err != 0){
            std::cout << "BUFFER CHECK GLERR:" << err << std::endl;
        }

        glDrawArrays(GL_POINTS,0,vertices.size());
        
        
    
        err = glGetError();

        if(err != 0){
            std::cout << "GLERR:" << err << std::endl;
        }
        

        glfwSwapBuffers(window);
      
        
    }

    glfwTerminate(); // Terminate GLFW when the loop ends
    return 0; // Return 0, indicating successful execution
}