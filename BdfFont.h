
#ifndef BDFFONT_H
#define BDFFONT_H

#include "BdfFontParser.h"
#include <list>

class BdfChar
{
    friend class BdfFontParser;
    public:
    BdfChar();
    int GetEncoding();
    

    std::string GetTag();
    

    std::list<int> GetBitmap();
    

    private:
        std::string m_char_tag; 
        int m_encoding;
        int m_bbox_x,m_bbox_y,m_bbox_offset_x,m_bbox_offset_y;
        std::list<int> m_bitmap;
};

class BdfFont
{
    friend class BdfFontParser;
    public:

    BdfFont(std::string filepath);
    

    void DumpFont();
    

    private:
    
    std::string m_family_name;
    int m_chars_count;
    int m_bbox_x,m_bbox_y,m_bbox_offset_x,m_bbox_offset_y;
    BdfFontParser m_font_parser;
    std::map<int,BdfChar> m_chars;


};

#endif