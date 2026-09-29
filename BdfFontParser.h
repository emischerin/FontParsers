

#ifndef BDFFONTPARSER_H
#define BDFFONTPARSER_H
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <fstream>
#include <iostream>


class BaseTokenHandler;
class BdfFont;
class BdfChar;

class BdfFontParser
{
    friend class BdfFont;
    public: 
        BdfFontParser();
        BdfFontParser(std::string path);
        void SetPath(std::string filepath);
        void SetOwner(BdfFont* font_owner);
        void AddCharToFont();
        void ReinitTmpChar();
        void SetTmpCharTag(std::string tag);
        void SetTmpCharEncoding(int encoding);
        void DumpLines(int offset,int lines_count);
        std::vector<std::string> Split(std::string& str,char stoken);

    private:
    void ParseFont();

    BdfFont* m_owner = nullptr;
    BdfChar* m_tmp_char = nullptr;
    std::vector<std::string> m_file_lines;
    std::string m_file_path;
    const std::string FLUSH_TERMINAL = "\033[2J\033[1;1H";
    std::map<std::string,BaseTokenHandler*> m_token_handlers;
        
        
};

#endif