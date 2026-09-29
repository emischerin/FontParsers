#include "TokenHandler.h"


BaseTokenHandler::BaseTokenHandler(BdfFontParser* owner) : m_owner(owner){};

BitmapTokenHandler::BitmapTokenHandler(BdfFontParser* owner) : BaseTokenHandler(owner){}

void BitmapTokenHandler::HandleToken(std::ifstream& f, std::vector<std::string>& current_line) 
{
    //if(current_line[0] != "BITMAP") return;

        std::list<int> bitmap;
        for(;;){
            std::string line;
            std::getline(f,line);
            
            if(line == "ENDCHAR"){
                m_owner->SetTmpCharBitmap(bitmap);
                m_owner->InvokeEndCharHandler(f,current_line);
                break;
            } 
            
            int map_from_hex = std::stoi(line,0,16);
            bitmap.push_back(map_from_hex);
            std::cout << current_line[0] << " " << map_from_hex << std::endl;
                       

            
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

     if(current_line.size() >= 2){
        m_owner->SetTmpCharTag(current_line[1]);
     }
     

}   

EncodingTokenHandler::EncodingTokenHandler(BdfFontParser* owner) : BaseTokenHandler(owner){}

void EncodingTokenHandler::HandleToken(std::ifstream& f, std::vector<std::string>& current_line) 
{
    
    if(current_line[0] != "ENCODING") return;

      if(current_line.size() >=2 ){
        int encoding = std::stoi(current_line[1],0,10);
        m_owner->SetTmpCharEncoding(encoding);
      }
      

            
}

BBXTokenHandler::BBXTokenHandler(BdfFontParser* owner): BaseTokenHandler(owner){}

void BBXTokenHandler::HandleToken(std::ifstream& f, std::vector<std::string>& current_line)
{
    if(current_line[0] != "BBX") return;

    if(current_line.size() < 5){
        m_owner->SetTmpCharBBX(0,0,0,0);
    }else{
        int x = std::stoi(current_line[1],0);
        int y = std::stoi(current_line[2],0);
        int x_offset = std::stoi(current_line[3],0);
        int y_offset = std::stoi(current_line[4],0);

        m_owner->SetTmpCharBBX(x,y,x_offset,y_offset);
    }

}
