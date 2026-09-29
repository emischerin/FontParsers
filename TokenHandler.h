#ifndef TOKENHANDLER_H
#define TOKENHANDLER_H
#include "BdfFontParser.h"

class BaseTokenHandler
{
    public:
    BaseTokenHandler(BdfFontParser* owner); 
    virtual void HandleToken(std::ifstream& f, std::vector<std::string>& current_line) = 0;        
    
        
    virtual ~BaseTokenHandler() = default;
    protected:
        BdfFontParser* m_owner = nullptr;
    
};

class BitmapTokenHandler : public BaseTokenHandler
{
    public:
    BitmapTokenHandler(BdfFontParser* owner);
    void HandleToken(std::ifstream& f, std::vector<std::string>& current_line) override;
   
};

class EndcharTokenHandler : public BaseTokenHandler
{
    public:
        EndcharTokenHandler(BdfFontParser* owner);

        void HandleToken(std::ifstream& f, std::vector<std::string>& current_line) override;
        
};

class StartCharTokenHandler : public BaseTokenHandler
{
    public:
        StartCharTokenHandler(BdfFontParser* owner); 

        void HandleToken(std::ifstream& f, std::vector<std::string>& current_line) override;
        
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


#endif