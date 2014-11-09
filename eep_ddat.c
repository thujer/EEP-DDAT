#include <stdio.h>
#include <conio.h>
#include "keyb.c"

//
//  |  InfoSector  |  Data table  |  Data blocks  |
//
//
//  Infosector
//   00 01 02 03 04 05 06 07 08 09
//  +--+--+--+--+--+--+--+--+--+--+
//  |D |D |0 |9 |0A|20|00|30|01|00|
//  +--+--+--+--+--+--+--+--+--+--+
//   --+-- --+-- +- +- --+-- +--+--
//     +-----|---|--|----|------|--  Signatura
//           +---|--|----|------|--  Verze
//               +--|----|------|--  Adresa tabulky dat
//                  +----|------|--  Velikost tabulky dat
//                       +------|--  Adresa prvniho bloku dat
//                              +--  Velikost pole dat
//  Data table
//  +--+--+--+--+--+--+--+---+
//  |A |B |C |D |15|20|01|CRC|
//  +--+--+--+--+--+--+--+---+
//   ----------  +- +- +-
//   Block name  +--|--|--------  Zacatek dat
//                  +--|--------  Velikost
//                     +--------  Typ dat
//
//  Data blocks
//  +--+--+--+--+--+--+---+
//  |D |A |T |A | . . .
//  +--+--+--+--+--+--+---+
//

#define uchar unsigned char
#define uint unsigned int


union
{ uchar Array[1024];
  struct
  { uchar Infosector[9];
    uchar DataTable[20];
    uchar Data[1000];
  } Str;
} EEPROM;


uchar Pos,Bytes;
uint  eep_index,eep_datasize,eep_datatype,eep_databegin,i;

void ShowData(uchar Begin, uchar End)
{ uchar i;

  textattr(7);
  for(i=Begin; i<End; i++)
  { cprintf("  %.2X ",i);
  }
  gotoxy(wherex()-(End-Begin)*5,wherey()+1);
  for(i=Begin; i<End; i++)
  { cprintf("-----");
  }
  gotoxy(wherex()-(End-Begin)*5,wherey()+1);
  for(i=Begin; i<End; i++)
  { cprintf("| %.2X ",EEPROM.Array[i]);
  }
  gotoxy(wherex()-(End-Begin)*5,wherey()+1);
  for(i=Begin; i<End; i++)
  { if(EEPROM.Array[i]>15)
      cprintf("| %2c ",EEPROM.Array[i]);
    else cprintf("| .  ",EEPROM.Array[i]);
  }
  cprintf("|");
  gotoxy(wherex()-(End-Begin)*5-1,wherey()+1);
  for(i=Begin; i<End; i++)
  { cprintf("-----");
  }
}


void EEP_Format()
{ EEPROM.Str.Infosector[0]='D';   // Signatura
  EEPROM.Str.Infosector[1]='D';   // --||--
  EEPROM.Str.Infosector[2]='0';   // Verze
  EEPROM.Str.Infosector[3]='9';   // --||--
  EEPROM.Str.Infosector[4]=0x0A;  // Adresa DataTable
  EEPROM.Str.Infosector[5]=20;    // Velikost DataTable
  EEPROM.Str.Infosector[6]=0;     // Adresa BlokuDat
  EEPROM.Str.Infosector[7]=30;    // --||--
  EEPROM.Str.Infosector[8]=1;     // Velikost pole dat
  EEPROM.Str.Infosector[9]=0;     // --||--
}


void EEP_WriteData(uchar NAME[8],uchar *Pointer,uchar Bytes)
{ uint i,index;

  if((EEPROM.Array[0]=='D') &&
     (EEPROM.Array[1]=='D'))
  { cprintf("DDAT OK.\r\n");

    index=EEPROM.Array[4];    // Nastav index na zacatek DataTable
    while(1)
    { cprintf("Index: %d \r\n",index);
      cprintf(": %X \r\n",EEPROM.Array[index]);
      if(EEPROM.Array[index]) // Pokud je na pozici nejaky zaznam...
      { index+=EEPROM.Array[index+8]; // ...posun index o jeho velikost
        cprintf("%d ",index);
      }
      else break;
    }

    for(i=0; i<sizeof(NAME); i++)
    { EEPROM.Str.DataTable[i]=NAME[i];
    }
    EEPROM.Str.DataTable[]=24;
    EEPROM.Str.DataTable[]=5;
    EEPROM.Str.DataTable[]=5;
    EEPROM.Str.Data[0]=1;
    EEPROM.Str.Data[1]=2;
    EEPROM.Str.Data[2]=3;
    EEPROM.Str.Data[3]=4;
    if(Pointer);
    if(Bytes);
  }
  else { cprintf("Bad structure DDAT!"); }
}

void EEP_ReadData(uchar NAME[8],uchar *Pointer,uchar MaxBytes)
{ uint index;

  if((EEPROM.Array[0]=='D') &&
     (EEPROM.Array[1]=='D'))
  { cprintf("DDAT OK.\r\n");

    index=EEPROM.Array[4];
    while(1)
    { if(EEPROM.Array[index]==NAME[0])
      { cprintf("ReadData: %c%c%c%c",NAME[0],NAME[1],NAME[2],NAME[3]);
        break;
      }
      else index++;
      if(index>1000) break;
    }

  }
  else cprintf("Bad DDAT structure!\r\n");


  /*
  cprintf("\r\n\nSeznam bloku dat:\r\n");
  eep_index=0;
  if(EEPROM.Str.DataTable[eep_index]>0)
  cprintf("%c",EEPROM.Str.DataTable[eep_index]); eep_index++;
  cprintf("%c",EEPROM.Str.DataTable[eep_index]); eep_index++;
  cprintf("%c",EEPROM.Str.DataTable[eep_index]); eep_index++;
  cprintf("%c",EEPROM.Str.DataTable[eep_index]); eep_index++;
  cprintf("%c",EEPROM.Str.DataTable[eep_index]); eep_index++;
  cprintf("%c",EEPROM.Str.DataTable[eep_index]); eep_index++;
  cprintf("%c ... ",EEPROM.Str.DataTable[eep_index]); eep_index++;
  eep_databegin=EEPROM.Str.DataTable[eep_index];
  cprintf("Begin:%Xh  ",eep_databegin); eep_index++;
  eep_datasize=EEPROM.Str.DataTable[eep_index];
  cprintf("Size:%d  ",eep_datasize); eep_index++;
  eep_datatype=EEPROM.Str.DataTable[eep_index];
  cprintf("Type:%d  ",eep_datatype);

  cprintf("Data: ");
  for(i=eep_databegin; i<eep_databegin+eep_datasize; i++)
  { cprintf("%.2X ",EEPROM.Array[i]);
  }*/

  if(NAME[0]);
  if(Pointer);
  if(MaxBytes);
}

uchar Buf[100];

void main()
{ clrscr();
  textattr(9); gotoxy(30,1); cprintf("EEPROM DDAT\r\n");
  textattr(7);

  EEP_Format();
  EEP_WriteData("Test0001",Buf,sizeof(Buf));
  cprintf("\r\n");
  //EEP_ReadData("Test0001",Buf,sizeof(Buf));

  Pos=0; Bytes=15;

  while(1)
  { if(kbhit())
    { GetKey();
      gotoxy(1,8);
      if(kbENTER) ShowData(Pos,Pos+Bytes);
      if(kbRIGHT) { Pos++; ShowData(Pos,Pos+Bytes); }
      if(kbLEFT)  { if(Pos)Pos--; ShowData(Pos,Pos+Bytes); }
      if(kbHOME)  { Pos=0; ShowData(Pos,Pos+Bytes); }
      if(kbESC) break;
    }
  }
}