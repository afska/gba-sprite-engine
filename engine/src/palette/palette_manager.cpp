#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/gba/tonc_core.h>
#include <libgba-sprite-engine/palette/palette_manager.h>

const COLOR defaultPaletteData[PALETTE_MAX_SIZE] __attribute__((aligned(4))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

PaletteManager::PaletteManager()
    : PaletteManager(defaultPaletteData, PALETTE_MAX_SIZE) {}

int getBits(int number, int k, int p) {
  return (((1 << k) - 1) & (number >> p));
}

PaletteManager::PaletteManager(const COLOR* paletteData, int size)
    : data(paletteData), size(size) {}

void PaletteManager::persist() {
  dma3_cpy(this->paletteAddress(), this->data, this->size);
}

COLOR PaletteManager::change(int bank, int index, COLOR newColor) {
  auto palBank = this->paletteBank();
  COLOR oldColor = palBank[bank][index];
  palBank[bank][index] = newColor;
  return oldColor;
}

COLOR PaletteManager::color(u32 red, u32 green, u32 blue) {
  if (red > 31)
    red = 31;
  if (green > 31)
    green = 31;
  if (blue > 31)
    blue = 31;
  return red | (green << 5) | (blue << 10);
}

void PaletteManager::persistToBank(int bank) {
  auto palBank = this->paletteBank();
  dma3_cpy(palBank[bank], this->data, PALETTE_BANK_SIZE);
}