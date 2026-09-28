#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <functional>

const std::string FLUSH_TERMINAL = "\033[2J\033[1;1H"; 

struct BdfChar
{

};

class BaseHandler
{
    public:
    BaseHandler(){};
    virtual void HandleToken(std::ifstream& f, std::string& current_line) = 0;        
    
        
    ~BaseHandler() = default;
    
    
};

class BitmapTokenHandler : public BaseHandler
{
    void HandleToken(std::ifstream& f, std::string& current_line) override
    {
        if(current_line != "BITMAP") return;

        for(;;){
            std::getline(f,current_line);
            if(current_line == "ENDCHAR") break;
            
            int map_from_hex = std::stoi(current_line,0,16);
            std::cout << current_line << " " << map_from_hex << std::endl;
            

            

            
        }

        std::string junk;
        std::getline(std::cin,junk);
    }
};

class BdfFont
{
    public: 
        BdfFont(std::string path) : m_file_path(path)
        {
            ParseFont();
        }

        void DumpMode()
        {
            size_t lines = 0;
            size_t step = 10;

            DumpLines(lines,step);
            for(;;){
                
                std::string input;
                std::getline(std::cin,input);
                if(input == "n"){
                    lines += step;
                    DumpLines(lines,step);
                }else if(input == "p"){
                    lines -= step;
                    DumpLines(lines,step);
                }
            }
        }

    private:
        void ParseFont()
        {
            std::ifstream f(m_file_path);

            if(!f.is_open()) 
                throw std::runtime_error("File is not opened!");

            
            std::string line;
            while(std::getline(f,line)){
                m_file_lines.push_back(line);
                 auto h = m_token_handlers.find(line);
                 if(h != m_token_handlers.end()){
                    h->second->HandleToken(f,line);
                 }

            }
            
                
        }

        

        void DumpLines(int offset,int lines_count)
        {
            std::cout << FLUSH_TERMINAL;

            int last_index = offset+ lines_count;
            int c = offset;
            while(c <= last_index){
                std::cout << m_file_lines[c] << std::endl;
                c++;
            }
        }

        
        std::vector<std::string> m_file_lines;
        std::string m_file_path;
        std::map<std::string,BaseHandler*> m_token_handlers = 
        {
            {"BITMAP",new BitmapTokenHandler}
        };

};



int main()
{
    BdfFont fnt("ter-u12b.bdf");
    fnt.DumpMode();

    return 0;
}