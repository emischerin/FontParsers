#include "BdfFont.h"

BdfChar::BdfChar(){}

int BdfChar::GetEncoding()
{
    return m_encoding;
}

std::string BdfChar::GetTag()
{
        return m_char_tag;
}

std::list<int> BdfFont::GetBitmap()
{
    return m_bitmap;
}

BdfFont::BdfFont(std::string filepath)
{
    m_font_parser.SetPath(filepath);
    m_font_parser.ParseFont();
}

void BdfFont::DumpFont()
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