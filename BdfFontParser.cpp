#include "BdfFontParser.h"

void BdfFontParser::SetPath(std::string filepath){m_file_path = filepath;}
void BdfFontParser::SetOwner(BdfFont*font_owner) { m_owner = font_owner;}

void BdfFontParser::AddCharToFont()  
{
    m_owner->m_chars[m_tmp_char.m_encoding] = m_tmp_char;
}

void BdfFontParser::ReinitTmpChar()
{
    m_tmp_char.m_bitmap.clear();
}
        
void BdfFontParser::SetTmpCharTag(std::string tag)
{
    m_tmp_char.m_char_tag = tag;
}

void BdfFontParser::SetTmpCharEncoding(int encoding)
{
    m_tmp_char.m_encoding = encoding;
}
    

BdfFontParser::BdfFontParser(){}
BdfFontParser::BdfFontParser(std::string path) : m_file_path(path)
{
    ParseFont();
}

void BdfFontParser::ParseFont()
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
   
     return result;
}       



