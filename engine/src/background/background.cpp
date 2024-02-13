//
// Created by Wouter Groeneveld on 28/07/18.
//

#include <libgba-sprite-engine/allocator.h>
#include <libgba-sprite-engine/gba/tonc_memmap.h>

#include <libgba-sprite-engine/background/background.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/gba/tonc_core.h>

#define TRANSPARENT_TILE_NUMBER 0

void Background::persist() {
  dma3_cpy(char_block(charBlockIndex), this->data, this->size);

  if (this->map) {
    dma3_cpy(screen_block(screenBlockIndex), this->map, this->mapSize);
  }

  buildRegister();
}

void Background::render() {
  REG_BG_OFS[bgIndex].x = scrollX;
  REG_BG_OFS[bgIndex].y = scrollY;
}

void Background::clearData() {
  this->clearMap();
  int empty[this->size];
  dma3_cpy(char_block(charBlockIndex), empty, this->size);
}

void Background::clearMap() {
  volatile auto ptr = &se_mem[screenBlockIndex][0];
  for (int i = 0; i < this->mapSize; i++) {
    ptr[i] = TRANSPARENT_TILE_NUMBER;
  }
}

void Background::scroll(int x, int y) {
  scrollX = x;
  scrollY = y;
}

void Background::scrollNow(int x, int y) {
  scroll(x, y);
  render();
}

void Background::buildRegister() {
  *(vu16*)(REG_BASE + getBgControlRegisterIndex()) =
      priority |              /* priority, 0 is highest, 3 is lowest */
      (charBlockIndex << 2) | /* the char block the image data is stored in */
      ((mosaicEnabled ? 1 : 0) << 6) | /* the mosaic flag */
      (1 << 7) | /* color mode, 0 is 16 colors, 1 is 256 colors */
      (screenBlockIndex
       << 8) |    /* the screen block the tile data is stored in */
      (1 << 13) | /* wrapping flag */
      (mapLayout << 14);
}

u32 Background::getBgControlRegisterIndex() {
  switch (bgIndex) {
    case 0:
      return 0x0008;
    case 1:
      return 0x000A;
    case 2:
      return 0x000C;
    case 3:
      return 0x000E;
  }
  return 0;
}
