#include "BdfFontParser.h"
#include "TokenHandler.h"
#include "BdfFont.h"

BdfFontParser::BdfFontParser()
{
    this->Init();
    
}
BdfFontParser::BdfFontParser(std::string path) : m_file_path(path)
{
    this->Init();
    ParseFont();
}

void BdfFontParser::ParseFont()
{
    std::cout << "Started Parsing Font\n";
    std::ifstream f(m_file_path);

    if(!f.is_open()) 
        throw std::runtime_error("File is not opened!");

            
            std::string line;
            while(std::getline(f,line)){
                m_file_lines.push_back(line);
                std::vector<std::string> split = Split(line,' ');
                if(split.size() < 1) continue;
                std::cout << split[0] << "\n";
                auto h = m_token_handlers.find(split[0]);
                if(h != m_token_handlers.end()){
                    std::cout << h->first << " invoked!\n";
                    h->second->HandleToken(f,split);
                 }

            }
            
                
}


void BdfFontParser::SetPath(std::string filepath){m_file_path = filepath;}
void BdfFontParser::SetOwner(BdfFont* font_owner) { m_owner = font_owner;}

void BdfFontParser::AddCharToFont()  
{
    //std::cout << m_tmp_char->m_encoding << std::endl;
    //m_owner->m_chars[m_tmp_char->m_encoding] = *m_tmp_char;
    m_owner->m_chars.try_emplace(m_tmp_char->m_encoding,*m_tmp_char);
}

void BdfFontParser::ReinitTmpChar()
{
    m_tmp_char->m_bitmap.clear();
}
        
void BdfFontParser::SetTmpCharTag(std::string tag)
{
    m_tmp_char->m_char_tag = tag;
}

void BdfFontParser::SetTmpCharEncoding(int encoding)
{
    m_tmp_char->m_encoding = encoding;
    std::cout << m_tmp_char->m_encoding << std::endl;
}
       

void BdfFontParser::DumpLines(int offset,int lines_count)
{
   std::cout << FLUSH_TERMINAL;

            int last_index = offset+ lines_count;
            int c = offset;
            while(c <= last_index){
                std::cout << m_file_lines[c] << std::endl;
                c++;
            }
        }

std::vector<std::string> BdfFontParser::Split(std::string& str,char stoken)
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
//   std::cout << "SPLITTED!\n";
     return result;
}       
void BdfFontParser::SetTmpCharBBX(int x,int y,int ox,int oy)
{
    m_tmp_char->m_bbox_x = x;
    m_tmp_char->m_bbox_y = y;
    m_tmp_char->m_bbox_offset_x = ox;
    m_tmp_char->m_bbox_offset_y = oy;


}

void BdfFontParser::Init()
{
    m_tmp_char = new BdfChar();
    m_token_handlers["STARTCHAR"] = new StartCharTokenHandler(this);
    m_token_handlers["ENCODING"] = new EncodingTokenHandler(this);
    m_token_handlers["BITMAP"] = new BitmapTokenHandler(this);
    m_token_handlers["ENDCHAR"] = new EndcharTokenHandler(this);
    m_token_handlers["BBX"] = new BBXTokenHandler(this);
}


void BdfFontParser::InvokeEndCharHandler(std::ifstream& f, std::vector<std::string>& current_line)
{
    m_token_handlers["ENDCHAR"]->HandleToken(f,current_line);
}

void BdfFontParser::SetTmpCharBitmap(std::list<int>& bitmap)
{
    m_tmp_char->m_bitmap = bitmap;
}