#include <iostream>


struct Window
{
    int w,h;
};
float WxToOGLX(int x,Window& w)
{
   
    
    
    float pixel_x_step = 2.0f / (float)(w.w );
    

    return -1.0f + (pixel_x_step * (float)x );


}

float WyToOGLY(int y,Window& w)
{
    
    
    float pixel_y_step = 2.0f / (float)(w.h );

    return (1.0f - (pixel_y_step * (float)y) );


}

int GlToWinX(float& x,Window& w)
{
    float pixel_x_step = 2.0f / (float)(w.w );
    int px = (x + 1.0f) / pixel_x_step;
    return px; 

}

int GlToWinY(float& y,Window& w)
{
    float pixel_y_step = 2.0f / (float)(w.h );
    int py = (y - 1.0f) / pixel_y_step;
    return -py; 

}

int main()
{
    
    Window w{640,480};
    
    int max_coord = 640;

    for(int i = 0; i <= max_coord;++i){
        std::cout << "=======TEST=========\n";
        int x = i;
        int y = i;
        std::cout << "X:" << x << " " << "Y:" << y << std::endl;
        float fx = WxToOGLX(x,w);
        float fy = WyToOGLY(y,w);
        std::cout << "GLX:" << fx << std::endl;
        std::cout << "GLY:" << fy << std::endl;

        int xreverse = GlToWinX(fx,w);
        int yreverse = GlToWinY(fy,w);

        std::cout << "REVERSE_X:" << xreverse << std::endl;
        std::cout << "REVERSE_Y:" << yreverse << std::endl;

        std::cout << "=======END TEST=========\n";

    }

    return 0;
}