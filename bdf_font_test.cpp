#include "bdf_font.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

//BdfFont font("ter-u12b.bdf");
//BdfFont font("ter-u22b.bdf");
BdfFont font("ter-u32n.bdf");
sf::RenderWindow w(sf::VideoMode({800,600}),"BDF FONT TEST");

std::list<std::vector<sf::Vertex>> text;

int posx = 10;
int posy = 10;
int next_char_step = 10;

/*Initial position + position for moving enter*/

int init_lx = 10; 
int init_ly = 10;

bool show_cursor = true;


inline bool Bitset(unsigned int bit,int number)
{
    int filter = 0 | (1 << bit);
    return ((filter & number) != 0);
}

void ProcessKeyboardEvent(sf::Event& e)
{
    if(e.key.code == sf::Keyboard::Enter){
        init_ly += (font.GetBoundingBoxHeight() + 1);
        posx = init_lx;
        posy = init_ly;
    }else if (e.key.code == sf::Keyboard::BackSpace){
        std::cout << "START BACKSPACE\n";
        auto last_pos = text.back().begin();
        posx = last_pos->position.x;
        posy = last_pos->position.y;
        std::cout << "SET NEW POSITION!\n";
        text.pop_back();

    }

    
}


void PushCharToDrawingPipeline(sf::Event& e)
{
    
    sf::Uint32 c = e.text.unicode;
    BdfChar char_ = font.GetChar(c);
    std::vector<sf::Vertex> _new_char;
    size_t bitwidth = font.GetBitmapBitWith();

    if(char_.empty)        
        return;
    

    int yoff = 0;
    for(auto i = char_.m_bitmap.begin();i != char_.m_bitmap.end();++i){
                
        int xoff = 0;
        
        for(int j = (bitwidth - 1); j >= 0; --j){
            sf::Vertex v;
            v.position.x = posx + xoff;
            v.position.y = posy + yoff;
            
            if(Bitset(j,*i)){
                v.color = sf::Color::Green;
                
            }else{
                v.color.a = 1;
            }

            xoff++;
            _new_char.push_back(v);

        }
        yoff++;
                
    }

    posx += (font.GetBoundingBoxWidth() + 1);
    text.push_back(_new_char);
    

}

void DrawText()
{
    for(auto i = text.begin();i != text.end();++i){
        w.draw(i->data(),i->size(),sf::PrimitiveType::Points);
    }
}

void SimpleCharDraw(sf::Event& e)
{
    std::cout << "DRAWING CHAR!\n";
    sf::Uint32 c = e.text.unicode;

    std::cout << c << std::endl;

    BdfChar char_ = font.GetChar(c);

    std::cout << char_.m_encoding << " " << char_.m_char_tag << std::endl; 

    if(char_.empty){
        std::cout << "EMPTY CHAR!\n";
        return;
    }

    for(auto it = char_.m_bitmap.begin();it != char_.m_bitmap.end();++it){
                  

            for(int i = 0; i < 8; ++ i){
                for(int j = 0; j < char_.m_bitmap.size(); ++ j){
                    sf::Vertex v;
                    v.position.x = posx + j;
                    v.position.y = posy + i;

                    if(Bitset(j,*it)){
                    v.color = sf::Color::Black;
                    w.draw(&v,(size_t)1,sf::PrimitiveType::Points);

                    }

                }
            }
    }

}

void DrawCaret()
{   
    sf::Color c = sf::Color::Green;
    sf::RectangleShape r;
    r.setPosition({init_lx,init_ly});
    r.setSize({font.GetBoundingBoxHeight(),3});
    r.setFillColor(c);

    if(show_cursor){
        show_cursor = false;
    }else {
        show_cursor = true;
        c.a = 1;
    }

    w.draw(r);


}

void timer_handler(int sig, siginfo_t *si, void *uc) {
    DrawCaret();   
   
}

void SetTimerForCaret()
{
    std::cout << "SETTING TIMER!\n";
    timer_t timerid;
    struct sigevent sevp;
    struct itimerspec its;
    struct sigaction sa;
    
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sa.sa_sigaction = timer_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGRTMIN, &sa, NULL);
    
    sevp.sigev_notify = SIGEV_SIGNAL;
    sevp.sigev_signo = SIGRTMIN;
    sevp.sigev_value.sival_ptr = &timerid;
    if (timer_create(CLOCK_MONOTONIC, &sevp, &timerid) == -1) {
        perror("timer_create");
        exit(EXIT_FAILURE);
    }
    
    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 1;
    its.it_interval.tv_nsec = 0;

    if (timer_settime(timerid, 0, &its, NULL) == -1) {
        perror("timer_settime");
        exit(EXIT_FAILURE);
    }
    
    
}


int main()
{
    
    sf::Event e;
    SetTimerForCaret();
    

    while(w.isOpen()){
        w.waitEvent(e);
        
        if(e.type == sf::Event::Closed){
            w.close();
        }

        if(e.type == sf::Event::KeyPressed){
            ProcessKeyboardEvent(e);
        }

        if(e.type == sf::Event::TextEntered){
            PushCharToDrawingPipeline(e);
            //SimpleCharDraw(e);
            //TestOneVertexDrawing();
        }
        

        
        w.clear(sf::Color::Black);
        DrawText();

        w.display();

    }

    return 0;
}