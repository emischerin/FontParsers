#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <list>

const std::string FLUSH_TERMINAL = "\033[2J\033[1;1H"; 

class BdfFontParser;
class BdfChar;

class BdfFont
{
    friend class BdfFontParser;
    public:

    BdfFont(std::string filepath)
    {
        m_font_parser.SetPath(filepath);
        m_font_parser.ParseFont();
    }

    void DumpFont()
    {
        for(auto i = m_chars.begin();i != m_chars.end();++i){
            std::cout << "====CHAR======" << std::endl;
            std::cout << i->second.GetTag();

            auto bmap_begin = i->second.GetBitmap().begin();
            auto bmap_end = i->second.GetBitmap().end(); 

            for(;bmap_begin != bmap_end;++bmap_begin){
                std::cout << *bmap_begin << ' ';
            }

            std::cout << "\n";

            std::cout << "====END CHAR======" << std::endl;


        }
    }

    private:
    
    std::string m_family_name;
    int m_chars_count;
    int m_bbox_x,m_bbox_y,m_bbox_offset_x,m_bbox_offset_y;
    BdfFontParser m_font_parser;
    std::map<int,BdfChar> m_chars;


};

class BdfChar
{
    friend class BdfFontParser;
    public:
    BdfChar(){}
    int GetEncoding()
    {
        return m_encoding;
    }

    std::string GetTag()
    {
        return m_char_tag;
    }

    std::list<int> GetBitmap()
    {
        return m_bitmap;
    }

    private:
        std::string m_char_tag; 
        int m_encoding;
        int m_bbox_x,m_bbox_y,m_bbox_offset_x,m_bbox_offset_y;
        std::list<int> m_bitmap;
};



class BaseTokenHandler
{
    public:
    BaseTokenHandler(BdfFontParser* owner) : m_owner(owner){};
    virtual void HandleToken(std::ifstream& f, std::vector<std::string>& current_line) = 0;        
    
        
    virtual ~BaseTokenHandler() = default;
    protected:
        BdfFontParser* m_owner = nullptr;
    
};

class BitmapTokenHandler : public BaseTokenHandler
{
    public:
    BitmapTokenHandler(BdfFontParser* owner) : BaseTokenHandler(owner){}
    void HandleToken(std::ifstream& f, std::vector<std::string>& current_line) override
    {
        if(current_line[0] != "BITMAP") return;

        for(;;){
            std::getline(f,current_line[0]);
            if(current_line[0] == "ENDCHAR") break;
            
            int map_from_hex = std::stoi(current_line[1],0,16);
            std::cout << current_line[1] << " " << map_from_hex << std::endl;
                       

            
        }

        std::string junk;
        std::getline(std::cin,junk);
    }
};

class EndcharTokenHandler : public BaseTokenHandler
{
    public:
        EndcharTokenHandler(BdfFontParser* owner) : BaseTokenHandler(owner){}

        void HandleToken(std::ifstream& f, std::vector<std::string>& current_line) override
        {
            if(current_line[0] != "ENDCHAR") return;

            m_owner->AddCharToFont();

            
        }
};

class StartCharTokenHandler : public BaseTokenHandler
{
    public:
        StartCharTokenHandler(BdfFontParser* owner) : BaseTokenHandler(owner){}

        void HandleToken(std::ifstream& f, std::vector<std::string>& current_line) override
        {
            if(current_line[0] != "STARTCHAR") return;

            m_owner->ReinitTmpChar();
            m_owner->SetTmpCharTag(current_line[1]);

            
        }
};

class EncodingTokenHandler : public BaseTokenHandler
{
    public:
        EncodingTokenHandler(BdfFontParser* owner) : BaseTokenHandler(owner){}

        void HandleToken(std::ifstream& f, std::vector<std::string>& current_line) override
        {
            if(current_line[0] != "ENCODING") return;

            int encoding = std::stoi(current_line[1],0,10);
            m_owner->SetTmpCharEncoding(encoding);

            
        }
};


class BdfFontParser
{
    friend class BdfFont;
    public: 
        

        void SetPath(std::string filepath){m_file_path = filepath;}
        void SetOwner(BdfFont* font_owner){ m_owner = font_owner;}

        void AddCharToFont()
        {
            m_owner->m_chars[m_tmp_char.m_encoding] = m_tmp_char;
        }

        void ReinitTmpChar()
        {
            m_tmp_char.m_bitmap.clear();
        }
        
        void SetTmpCharTag(std::string tag)
        {
            m_tmp_char.m_char_tag = tag;
        }

        void SetTmpCharEncoding(int encoding)
        {
            m_tmp_char.m_encoding = encoding;
        }
    private:

        BdfFontParser(){}
        BdfFontParser(std::string path) : m_file_path(path)
        {
            ParseFont();
        }

        void ParseFont()
        {
            std::ifstream f(m_file_path);

            if(!f.is_open()) 
                throw std::runtime_error("File is not opened!");

            
            std::string line;
            while(std::getline(f,line)){
                //m_file_lines.push_back(line);
                std::vector<std::string> split = Split(line,' ');
                auto h = m_token_handlers.find(split[0]);
                if(h != m_token_handlers.end()){
                    h->second->HandleToken(f,split);
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

std::vector<std::string> Split(std::string& str,char stoken)
{
  std::vector<std::string> result;
  std::list<char> current;
  for(int i = 0; i < str.size();++i){
        
    if(str[i] == stoken || str[i] == '\n' ){
      
      if(current.size() > 0){
        std::string res_str(current.begin(),current.end());
        result.push_back(res_str);
        current.clear();
      }
      
    }else{
      current.push_back(str[i]);
      
    }
   
  
  }
   
     return result;
}       
        BdfFont* m_owner = nullptr;
        BdfChar m_tmp_char;
        std::vector<std::string> m_file_lines;
        std::string m_file_path;
        std::map<std::string,BaseTokenHandler*> m_token_handlers = 
        {
            {"STARTCHAR",new StartCharTokenHandler(this)},
            {"ENCODING",new EncodingTokenHandler(this)},
            {"BITMAP",new BitmapTokenHandler(this)},
            {"ENDCHAR",new EndcharTokenHandler(this)}
        };

};



int main()
{
    BdfFont fnt("ter-u12b.bdf");
    fnt.DumpFont();
    //BdfFontParser fnt("ter-u12b.bdf");
    //fnt.DumpMode();

    return 0;
}