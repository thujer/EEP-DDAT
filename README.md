EEP-DDAT
========

C++ app - EEPROM filesystem

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
