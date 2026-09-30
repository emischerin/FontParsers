#include <list>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include<string>
struct BdfChar
{
    std::string m_char_tag; 
    int m_encoding;
    int m_bbox_x,m_bbox_y,m_bbox_offset_x,m_bbox_offset_y;
    std::list<int> m_bitmap;
};

class BdfFont
{
    
    public:

    BdfFont(std::string filepath) : m_file_path(filepath){
        this->ParseFont();
    }
    


    void BdfFont::DumpFont()
    {
        std::cout << "FONT SUMMARY\n";
        std::cout << m_chars.size() << std::endl;
        for(auto i = m_chars.begin();i != m_chars.end();++i){
            std::cout << "====CHAR======" << std::endl;
            std::cout << i->second.m_char_tag << std::endl;

            auto bmap_begin = i->second.m_bitmap.begin();
            auto bmap_end = i->second.m_bitmap.end(); 

            for(;bmap_begin != bmap_end;++bmap_begin){
                std::cout << *bmap_begin << ' ';
            }

            std::cout << "\n";

            std::cout << "====END CHAR======" << std::endl;


        }
    }
    

    private:
    
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
//   std::cout << "SPLITTED!\n";
     return result;
    }
    
    void ParseFont()
    {
    std::cout << "Started Parsing Font\n";
    std::ifstream f(m_file_path);

    if(!f.is_open()) 
        throw std::runtime_error("File is not opened!");

            
            std::string line;
            BdfChar tmp_char;
            while(std::getline(f,line)){
                //m_file_lines.push_back(line);
                std::vector<std::string> split = Split(line,' ');
                if(split.size() < 1) continue;

                std::string current_token = split[0];

                if(current_token == "STARTCHAR"){
                    tmp_char.m_char_tag = split[1];
                }else if(current_token == "ENCODING"){
                    int e = std::stoi(split[1],nullptr,10);
                    tmp_char.m_encoding = e;
                }else if(current_token == "BBX"){
                    tmp_char.m_bbox_x = std::stoi(split[1]);
                    tmp_char.m_bbox_y = std::stoi(split[2]);
                    tmp_char.m_bbox_offset_x = std::stoi(split[3]);
                    tmp_char.m_bbox_offset_x = std::stoi(split[4]);
                }else if(current_token == "BITMAP"){
                    for(;;){
                        std::getline(f,line);
                        if(line == "ENDCHAR"){
                            this->m_chars[tmp_char.m_encoding] = tmp_char;
                            break;
                        }
                        tmp_char.m_bitmap.push_back(std::stoi(line));
                    }
                }else if(current_token == "FAMILY_NAME"){
                    this->m_family_name = split[1];
                }
                else if(current_token == "ENDFONT"){
                    break;
                }

            }

            f.close();
            
                
    }

    

    std::string m_family_name;
    int m_chars_count;
    int m_bbox_x,m_bbox_y,m_bbox_offset_x,m_bbox_offset_y;
    std::string m_file_path;
    std::map<int,BdfChar> m_chars;


};

int main()
{
    BdfFont fnt("ter-u12b.bdf");
    fnt.DumpFont();
    //BdfFontParser fnt("ter-u12b.bdf");
    //fnt.DumpMode();

    return 0;
}