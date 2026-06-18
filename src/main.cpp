#include <iostream>
#include <cstdint>
#include <fstream>
#include <bitset>
#include <string>
#include <ios>

#pragma pack(push, 1) // Avoid adding hidden memory alignment padding
struct TTFHeader{
  uint32_t sfntVersion;
  uint16_t numTables;
  uint16_t searchRange;
  uint16_t entrySelector;
  uint16_t rangeShift;
};

struct TableDirectory{
  char tag[4];
  uint32_t checkSum;
  uint32_t offset;
  uint32_t length;
};

struct GlyphHeader{
  int16_t numContours;
  int16_t xMin;
  int16_t yMin;
  int16_t xMax;
  int16_t yMax;
};
#pragma pack(pop)

uint16_t bigEndian16_t(uint16_t var){
  return (var >> 8) | (var << 8);
}

uint32_t bigEndian32_t(uint32_t var){
  return ((var & 0x000000FF) << 24) | ((var & 0x0000FF00) << 8) | ((var & 0x00FF0000) << 8) | ((var & 0xFF000000) << 24);
}

int main(int argc, char *argv[]){
  if(argc < 2){
    std::cerr << "Usage: " << argv[0] << " <path-to-ttf-font-file>\n";
    return 1;
  }

  std::string fontPath = argv[1];
  std::ifstream font(fontPath, std::ios::binary);

  TTFHeader ttfHeader;
  font.read(reinterpret_cast<char*>(&ttfHeader), sizeof(TTFHeader));

  ttfHeader.sfntVersion   = bigEndian32_t(ttfHeader.sfntVersion);
  ttfHeader.numTables     = bigEndian16_t(ttfHeader.numTables);
  ttfHeader.searchRange   = bigEndian16_t(ttfHeader.searchRange);
  ttfHeader.entrySelector = bigEndian16_t(ttfHeader.entrySelector);
  ttfHeader.rangeShift    = bigEndian16_t(ttfHeader.rangeShift);

  std::cout << "--- TTF Header ---\n";
  std::cout << "SFNT Version  : 0x" << std::bitset<32>(ttfHeader.sfntVersion) << '\n';
  std::cout << "Tables Count  : " << ttfHeader.numTables << '\n';
  std::cout << "Search Range  : " << ttfHeader.searchRange << '\n';
  std::cout << "Entry Selector: " << ttfHeader.entrySelector << '\n';
  std::cout << "Range Shift   : " << ttfHeader.rangeShift << '\n';

  return 0;
}
