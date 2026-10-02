#include "bdf_font.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


//BdfFont font("ter-u12b.bdf");
BdfFont font("ter-u22b.bdf");
sf::RenderWindow w(sf::VideoMode({800,600}),"BDF FONT TEST");

std::list<std::vector<sf::Vertex>> text;

int posx = 10;
int posy = 10;
int next_char_step = 10;

/*Initial position + position for moving enter*/

int init_lx = 10; 
int init_ly = 10;



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
                v.color = sf::Color::Black;
                
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
            
            std::cout << "j loop\n";
            
             

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

void TestOneVertexDrawing()
{
    static int y = 10;
    static int x = 10;

    std::cout << "DP:" << " X:" << x << std::endl;
    sf::Vertex v;
    v.position.x = x;
    v.position.y = y;
    v.color = sf::Color::Black; 
    std::vector<sf::Vertex> vec;

    vec.push_back(sf::Vertex({10,y},sf::Color::Black));
    vec.push_back(sf::Vertex({11,y},sf::Color::Black));
    vec.push_back(sf::Vertex({12,y},sf::Color::Black));
    vec.push_back(sf::Vertex({13,y},sf::Color::Black));
    vec.push_back(sf::Vertex({14,y},sf::Color::Black));
    vec.push_back(sf::Vertex({15,y},sf::Color::Black));
    vec.push_back(sf::Vertex({16,y},sf::Color::Black));
    vec.push_back(sf::Vertex({17,y},sf::Color::Black));
    vec.push_back(sf::Vertex({18,y},sf::Color::Black));
    vec.push_back(sf::Vertex({19,y},sf::Color::Black));
    
    w.draw(vec.data(),vec.size(),sf::PrimitiveType::LinesStrip);

    y++;
    
}

int main()
{
    
    sf::Event e;

    

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
        

        
        w.clear(sf::Color::White);
        DrawText();

        w.display();

    }

    return 0;
}