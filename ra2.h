#ifndef RA2_H
#define RA2_H
#include <stdint.h>
struct ColorStruct {
    uint8_t rgb[3]; // R G B 0 1 2
};

enum {
    //ColorScheme indices, since they are hardcoded all over the exe, why shan't we do it as well?
    Yellow  =   3,
    White   =   5,
    Grey    =   7,
    Red     =  11,
    Orange  =  13,
    Pink    =  15,
    Purple  =  17,
    Blue    =  21,
    Sky     =  25,
    Green   =  29,
};

#define MAXPLAYERS 8
extern unsigned powerOutput[MAXPLAYERS];
extern unsigned powerDrain[MAXPLAYERS];
extern wchar_t UserName[MAXPLAYERS][0x20];
extern unsigned GIcount[MAXPLAYERS], ALLIDOGcount[MAXPLAYERS], SOVDOGcount[MAXPLAYERS];
extern unsigned ALLITANKcount[MAXPLAYERS], SOVTANKcount[MAXPLAYERS];

extern unsigned ALLIMinerCount[MAXPLAYERS], SOVMinerCount[MAXPLAYERS];

extern unsigned ALLIWarFactoryCount[MAXPLAYERS], SOVWarFactoryCount[MAXPLAYERS];
extern struct ColorStruct playerColor[MAXPLAYERS];

extern const char *allieCountries[];
extern const char *sovietCountries[];
extern bool validPlayer[MAXPLAYERS];
extern char countryName[MAXPLAYERS][0x40];
extern unsigned balance[MAXPLAYERS];

extern unsigned ALLIFlycount[MAXPLAYERS];
extern unsigned SOVSpidercount[MAXPLAYERS];
extern unsigned ALLIIFVcount[MAXPLAYERS];
extern unsigned SOVIFVcount[MAXPLAYERS];
extern unsigned SOVCONSCRIPTcount[MAXPLAYERS];

extern struct ColorStruct playerColor[MAXPLAYERS];
extern unsigned color[MAXPLAYERS];

extern unsigned ALLIBOATcount[MAXPLAYERS];
extern unsigned SOVBOATcount[MAXPLAYERS];

extern unsigned ALLIFISHcount[MAXPLAYERS];
extern unsigned SOVFISHcount[MAXPLAYERS];

extern unsigned ALLICARRIERcount[MAXPLAYERS];
extern unsigned SOVCARRIERcount[MAXPLAYERS];
#endif // RA2_H
