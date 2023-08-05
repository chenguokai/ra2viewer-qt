/*
    Copyright (C) 2023 Guokai Chen

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, version 3 of the
    License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h> //for PROCESSENTRY32, needs to be included after windows.h
#include <stdint.h>
#include "ra2.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */


#define INVALIDCLASS 0xffffffffu
#define INFOFFSET 0x557c

#define ALLIDOGOFFSET 0x1c
#define SOVDOGOFFSET 0x9

#define TANKOFFSET 0x5568
#define ALLITANKOFFSET 0x9
#define SOVTANKOFFSET 0x3
#define ALLIMINEROFFSET (0x84 / 4)
#define SOVMINEROFFSET (0x4 / 4) // or 0x4 / 4

#define BUILDINGOFFSET 0x5554
#define ALLIWARFACTORYOFFSET (0x1c / 4)
#define SOVWARFACTORYOFFSET (0x38 / 4)


#define CREDITSPENT_OFFSET 0x2dc
#define BALANCEOFFSET 0x30c
#define USERNAMEOFFSET 0x1602a
#define ISWINNEROFFSET 0x1f7
#define ISLOSEROFFSET 0x1f8

#define POWEROUTPUTOFFSET 0x53a4
#define POWERDRAINOFFSET 0x53a8

#define PRODUCINGBUILDINGINDEXOFFSET 0x564c
#define PRODUCINGUNITINDEXOFFSET 0x5650

#define HOUSETYPECLASSBASEOFFSET 0x34
#define COUNTRYSTRINGOFFSET 0x24

#define COLOROFFSET 0x56fC
#define COLORSCHEMEOFFSET 0x16054

#define ROCKETEEROFFSET 0x04
#define SPIDEROFFSET 0x10

#define IFVOFFSET 0x26
#define FLAKTRACKOFFSET 0x11

#define CONSCRIPTOFFSET 0x01
#define GIOFFSET 0x0

unsigned ALLIFlycount[MAXPLAYERS];
unsigned SOVSpidercount[MAXPLAYERS];
unsigned ALLIIFVcount[MAXPLAYERS];
unsigned SOVIFVcount[MAXPLAYERS];
unsigned SOVCONSCRIPTcount[MAXPLAYERS];

unsigned counts[0x2000];

unsigned color[MAXPLAYERS];

bool inited = false;
int validHouses = 0;
unsigned *HouseBase[MAXPLAYERS];

unsigned fixedPoint = (0xa8b230);
unsigned fixedPointValue;
unsigned classBasePtr[MAXPLAYERS];

unsigned classBaseArrayPtr = 0xa8022c;
unsigned classBaseArray;
unsigned realClassBasePtr[MAXPLAYERS];
unsigned realClassBase[MAXPLAYERS];
unsigned itemPtr[MAXPLAYERS];
unsigned itemArrayBase[MAXPLAYERS];

unsigned balancePtr[MAXPLAYERS];
unsigned spentCreditPtr[MAXPLAYERS];
unsigned balance[MAXPLAYERS];
unsigned spentCredit[MAXPLAYERS];

unsigned userNamePtr[MAXPLAYERS];
wchar_t UserName[MAXPLAYERS][0x20];

unsigned isWinnerPtr[MAXPLAYERS];
unsigned isLoserPtr[MAXPLAYERS];
bool isWinner[MAXPLAYERS];
bool isLoser[MAXPLAYERS];

unsigned powerOutputPtr[MAXPLAYERS];
unsigned powerDrainPtr[MAXPLAYERS];
unsigned powerOutput[MAXPLAYERS];
unsigned powerDrain[MAXPLAYERS];

unsigned producingBuildingIndexPtr[MAXPLAYERS];
unsigned producingUnitIndexPtr[MAXPLAYERS];
unsigned producingBuildingIndex[MAXPLAYERS];
unsigned producingUnitIndex[MAXPLAYERS];

unsigned houseTypeClassBasePtr[MAXPLAYERS];
unsigned houseTypeClassBase[MAXPLAYERS];

unsigned countryNamePtr[MAXPLAYERS];
char countryName[MAXPLAYERS][0x40];

unsigned GIcount[MAXPLAYERS], ALLIDOGcount[MAXPLAYERS], SOVDOGcount[MAXPLAYERS];
unsigned ALLITANKcount[MAXPLAYERS], SOVTANKcount[MAXPLAYERS];

unsigned ALLIMinerCount[MAXPLAYERS], SOVMinerCount[MAXPLAYERS];

unsigned ALLIWarFactoryCount[MAXPLAYERS], SOVWarFactoryCount[MAXPLAYERS];

unsigned playerColorPtr[MAXPLAYERS];
struct ColorStruct playerColor[MAXPLAYERS];

bool validPlayer[MAXPLAYERS];


const char *allieCountries[] = {
    "Americans",
    "Alliance", // Korea
    "French",
    "Germans",
    "British"
};
const char *sovietCountries[] = {
    "Africans", // Libya
    "Arabs", // Iraq
    "Confederation", // Cuba
    "Russians"
};

DWORD findPidByName(const char* name)
{
    HANDLE h;
    PROCESSENTRY32 singleProcess;
    h = CreateToolhelp32Snapshot( //takes a snapshot of specified processes
        TH32CS_SNAPPROCESS, //get all processes
        0); //ignored for SNAPPROCESS

    singleProcess.dwSize = sizeof(PROCESSENTRY32);

    do {

        if (strcmp(singleProcess.szExeFile, name) == 0)
        {
            DWORD pid = singleProcess.th32ProcessID;
            printf("PID Found: %lu\n", pid);
            CloseHandle(h);
            return pid;
        }

    } while (Process32Next(h, &singleProcess));

    CloseHandle(h);

    return 0;
}

void ReadClassBase() {
    HANDLE handle = OpenProcess(
        PROCESS_QUERY_INFORMATION | //Needed to get a process' token
            PROCESS_CREATE_THREAD |    //for obvious reasons
            PROCESS_VM_OPERATION |    //required to perform operations on address space of process (like WriteProcessMemory)
            PROCESS_VM_READ,  //required for read data
        FALSE,      //don't inherit handle
        findPidByName("gamemd-spawn.exe"));

    if (handle == NULL)
    {
        fprintf(stderr, "Could not open process\n");
        return;
    }

    ReadProcessMemory(handle,
                      (const void *)fixedPoint,
                      &fixedPointValue,
                      4,
                      NULL);

    ReadProcessMemory(handle,
                      (const void *)classBaseArrayPtr,
                      &classBaseArray,
                      4,
                      NULL);
    //printf("AAA classBaseArray %x\n", classBaseArray);

    unsigned classbasearray = (fixedPointValue + 1120 * 4);
    for (int i = 0; i < MAXPLAYERS; i++) {
        ReadProcessMemory(handle,
                          (const void *)classbasearray,
                          &classBasePtr[i],
                          4,
                          NULL);
        classbasearray += 4;
        if (classBasePtr[i] != INVALIDCLASS) {
            validPlayer[i] = true;
            //printf("debug classBasePtr %u\n", classBasePtr);
            realClassBasePtr[i] = classBasePtr[i] * 4 + classBaseArray;
            printf("real classbasearray %x\n", realClassBasePtr[i]);

            ReadProcessMemory(handle,
                              (const void *)realClassBasePtr[i],
                              &realClassBase[i],
                              4,
                              NULL);

            // Balance
            balancePtr[i] = realClassBase[i] + BALANCEOFFSET;
            ReadProcessMemory(handle,
                              (const void *)balancePtr[i],
                              &balance[i],
                              4,
                              NULL);
            printf("Player %d balance %u\n", i, balance[i]);

            // Spent money
            spentCreditPtr[i] = realClassBase[i] + CREDITSPENT_OFFSET;
            ReadProcessMemory(handle,
                              (const void *)spentCreditPtr[i],
                              &spentCredit[i],
                              4,
                              NULL);
            printf("Player %d spent %u\n", i, spentCredit[i]);

            // User name
            userNamePtr[i] = realClassBase[i] + USERNAMEOFFSET;
            ReadProcessMemory(handle,
                              (const void *)userNamePtr[i],
                              &UserName[i],
                              0x20,
                              NULL);
            printf("Player %d name %ls\n", i, UserName[i]);

            // IsWinner
            isWinnerPtr[i] = realClassBase[i] + ISWINNEROFFSET;
            ReadProcessMemory(handle,
                              (const void *)isWinnerPtr[i],
                              &isWinner[i],
                              1,
                              NULL);
            printf("Player %d isWinner %u\n", i, isWinner[i]);

            // IsLoser
            isLoserPtr[i] = realClassBase[i] + ISLOSEROFFSET;
            ReadProcessMemory(handle,
                              (const void *)isLoserPtr[i],
                              &isLoser[i],
                              1,
                              NULL);
            printf("Player %d isLoser %u\n", i, isLoser[i]);

            // Power output
            powerOutputPtr[i] = realClassBase[i] + POWEROUTPUTOFFSET;
            ReadProcessMemory(handle,
                              (const void *)powerOutputPtr[i],
                              &powerOutput[i],
                              4,
                              NULL);
            printf("Player %d powerOutput %u\n", i, powerOutput[i]);

            // Power drain
            powerDrainPtr[i] = realClassBase[i] + POWERDRAINOFFSET;
            ReadProcessMemory(handle,
                              (const void *)powerDrainPtr[i],
                              &powerDrain[i],
                              4,
                              NULL);
            printf("Player %d powerDrain %u\n", i, powerDrain[i]);


            unsigned colorPtr = realClassBase[i] + COLORSCHEMEOFFSET;
            ReadProcessMemory(handle,
                              (const void *)colorPtr,
                              &color[i],
                              4,
                              NULL);
            printf("Player %d colorScheme %u\n", i, color);
            /*
      // currently unusable
      // Producing building
      producingBuildingIndexPtr = realClassBase + PRODUCINGBUILDINGINDEXOFFSET;
      ReadProcessMemory(handle,
        (const void *)producingBuildingIndexPtr,
        &producingBuildingIndex,
        4,
        NULL);
      printf("Player %d producingBuildingIndex %u\n", i, producingBuildingIndex);
      */
            // HouseTypeClassBase
            houseTypeClassBasePtr[i] = realClassBase[i] + HOUSETYPECLASSBASEOFFSET;
            ReadProcessMemory(handle,
                              (const void *)houseTypeClassBasePtr[i],
                              &houseTypeClassBase[i],
                              4,
                              NULL);
            printf("Player %d houseTypeClassBase %x\n", i, houseTypeClassBase[i]);

            // Country name
            countryNamePtr[i] = houseTypeClassBase[i] + COUNTRYSTRINGOFFSET;
            ReadProcessMemory(handle,
                              (const void *)countryNamePtr[i],
                              &countryName[i],
                              25,
                              NULL);
            printf("Player %d countryName %s\n", i, countryName[i]);

            playerColorPtr[i] = houseTypeClassBase[i] + COLOROFFSET;
            ReadProcessMemory(handle,
                              (const void *)playerColorPtr[i],
                              &playerColor[i],
                              3,
                              NULL);
            printf("Player %d color r %u g %u b %u\n", i, playerColor[i].rgb[0], playerColor[i].rgb[1], playerColor[i].rgb[2]);

            // building part
            itemPtr[i] = realClassBase[i] + BUILDINGOFFSET;
            ReadProcessMemory(handle,
                              (const void *)itemPtr[i],
                              &itemArrayBase[i],
                              4,
                              NULL);
            printf("Building itemPtr %x\n", itemPtr[i]);
            printf("Building itemArrayBase %x\n", itemArrayBase[i]);

            // War Factory
            ReadProcessMemory(handle,
                              (const void *)(itemArrayBase[i] + 4 * ALLIWARFACTORYOFFSET),
                              &ALLIWarFactoryCount[i],
                              4,
                              NULL);
            printf("Player %d ALLIWARFACTORYcount %u\n", i, ALLIWarFactoryCount[i]);

            ReadProcessMemory(handle,
                              (const void *)(itemArrayBase[i] + 4 * SOVWARFACTORYOFFSET),
                              &SOVWarFactoryCount[i],
                              4,
                              NULL);
            printf("Player %d SOVWARFACTORYcount %u\n", i, SOVWarFactoryCount[i]);




            // infantry part
            itemPtr[i] = realClassBase[i] + INFOFFSET;
            ReadProcessMemory(handle,
                              (const void *)itemPtr[i],
                              &itemArrayBase[i],
                              4,
                              NULL);
            printf("Infantry itemPtr %x\n", itemPtr[i]);
            printf("Infantry itemArrayBase %x\n", itemArrayBase[i]);
            // GI
            ReadProcessMemory(handle,
                              (const void *)(itemArrayBase[i] + 4 * GIOFFSET),
                              &GIcount[i],
                              4,
                              NULL);
            // Conscript
            ReadProcessMemory(handle,
                              (const void *)(itemArrayBase[i] + 4 * CONSCRIPTOFFSET),
                              &SOVCONSCRIPTcount[i],
                              4,
                              NULL);
            // rocket fly
            ReadProcessMemory(handle,
                              (const void *)(itemArrayBase[i] + 4 * ROCKETEEROFFSET),
                              &ALLIFlycount[i],
                              4,
                              NULL);
            // spider
            ReadProcessMemory(handle,
                              (const void *)(itemArrayBase[i] + 4 * SPIDEROFFSET),
                              &SOVSpidercount[i],
                              4,
                              NULL);


            // ALLIDOG
            ReadProcessMemory(handle,
                              (const void *)(itemArrayBase[i] + 4 * ALLIDOGOFFSET),
                              &ALLIDOGcount[i],
                              4,
                              NULL);
            // SOVDOG
            ReadProcessMemory(handle,
                              (const void *)(itemArrayBase[i] + 4 * SOVDOGOFFSET),
                              &SOVDOGcount[i],
                              4,
                              NULL);

            printf("Player %d SOVDOG count %u ALLIDOG count %u\n", i, SOVDOGcount[i], ALLIDOGcount[i]);
            // (unit)tank part
            itemPtr[i] = realClassBase[i] + TANKOFFSET;
            ReadProcessMemory(handle,
                              (const void *)itemPtr[i],
                              &itemArrayBase[i],
                              4,
                              NULL);
            printf("Tank itemPtr %x\n", itemPtr[i]);
            printf("Tank itemArrayBase %x\n", itemArrayBase[i]);
            // IFV
            ReadProcessMemory(handle,
                              (const void *)(itemArrayBase[i] + 4 * IFVOFFSET),
                              &ALLIIFVcount[i],
                              4,
                              NULL);
            // SOV IFV
            ReadProcessMemory(handle,
                              (const void *)(itemArrayBase[i] + 4 * FLAKTRACKOFFSET),
                              &SOVIFVcount[i],
                              4,
                              NULL);
            // ALLITank
            ReadProcessMemory(handle,
                              (const void *)(itemArrayBase[i] + 4 * ALLITANKOFFSET),
                              &ALLITANKcount[i],
                              4,
                              NULL);
            // SOVTank
            ReadProcessMemory(handle,
                              (const void *)(itemArrayBase[i] + 4 * SOVTANKOFFSET),
                              &SOVTANKcount[i],
                              4,
                              NULL);
            printf("Player %d Sov Tank count %u AlliTank count %u\n", i, SOVTANKcount[i], ALLITANKcount[i]);

            // ALLIMiner
            ReadProcessMemory(handle,
                              (const void *)(itemArrayBase[i] + 4 * ALLIMINEROFFSET),
                              &ALLIMinerCount[i],
                              4,
                              NULL);
            printf("Player %d Alli Miner count %u\n", i, ALLIMinerCount[i]);
            // SOVMiner
            ReadProcessMemory(handle,
                              (const void *)(itemArrayBase[i] + 4 * SOVMINEROFFSET),
                              &SOVMinerCount[i],
                              4,
                              NULL);
            printf("Player %d Sov Miner count %u\n", i, SOVMinerCount[i]);
            /*
      for (int i = 0; i < 0x200; i++) {
        if (counts[i] == 9) {
            printf("find offset %u\n", i);
          }
      }*/

        } else {
            // empty player, skip
            validPlayer[i] = false;
        }
    }


}



void* ra2_main(void* arg) {
    while (!findPidByName("gamemd-spawn.exe")) {
        printf("Waiting game process\n");
        Sleep(1000);
    }
    while (1) {
        ReadClassBase();
        Sleep(1000); // sleep 1s
    }
    return 0;
}
