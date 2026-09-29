#include "BdfFont.h"

int main()
{
    BdfFont fnt("ter-u12b.bdf");
    fnt.DumpFont();
    //BdfFontParser fnt("ter-u12b.bdf");
    //fnt.DumpMode();

    return 0;
}