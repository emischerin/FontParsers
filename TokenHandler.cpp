#include "TokenHandler.h"


BaseTokenHandler::BaseTokenHandler(BdfFontParser* owner) : m_owner(owner){};

BitmapTokenHandler::BitmapTokenHandler(BdfFontParser* owner) : BaseTokenHandler(owner){}

void BitmapTokenHandler::HandleToken(std::ifstream& f, std::vector<std::string>& current_line) 
{
    if(current_line[0] != "BITMAP") return;

        for(;;){
            std::getline(f,current_line[0]);
            if(current_line[0] == "ENDCHAR") break;
            
            int map_from_hex = std::stoi(current_line[1],0,16);
            std::cout << current_line[1] << " " << map_from_hex << std::endl;
                       

            
        }
       
}

EndcharTokenHandler::EndcharTokenHandler(BdfFontParser* owner) : BaseTokenHandler(owner){}

void EndcharTokenHandler::HandleToken(std::ifstream& f, std::vector<std::string>& current_line) 
{
    if(current_line[0] != "ENDCHAR") return;

    m_owner->AddCharToFont();

            
}

StartCharTokenHandler::StartCharTokenHandler(BdfFontParser* owner) : BaseTokenHandler(owner){}

void StartCharTokenHandler::HandleToken(std::ifstream& f, std::vector<std::string>& current_line) 
{
    if(current_line[0] != "STARTCHAR") return;

     m_owner->ReinitTmpChar();
     m_owner->SetTmpCharTag(current_line[1]);

}   

EncodingTokenHandler::EncodingTokenHandler(BdfFontParser* owner) : BaseTokenHandler(owner){}

void EncodingTokenHandler::HandleToken(std::ifstream& f, std::vector<std::string>& current_line) 
{
    if(current_line[0] != "ENCODING") return;

      int encoding = std::stoi(current_line[1],0,10);
      m_owner->SetTmpCharEncoding(encoding);

            
}