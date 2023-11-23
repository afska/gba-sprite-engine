//
// Created by Wouter Groeneveld on 05/08/18.
//

#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/palette/palette_manager.h>

void CombinedPalette::changeBrightness(PaletteManager& palette,
                                       int bank,
                                       int index,
                                       int intensity) {
  auto current = palette.get(bank, index);
  auto next = PaletteManager::modify(current, intensity);

  palette.change(bank, index, next);
}

void CombinedPalette::changeBrightness(int intensity) {
  if (abs(intensity) > 31) {
    failure_gba(Brightness_Intensity_Too_High);
    return;
  }

  for (int bank = 0; bank < PALETTE_BANK_SIZE; bank++) {
    for (int index = 0; index < PALETTE_BANK_SIZE; index++) {
      changeBrightness(palette1, bank, index, intensity);
      changeBrightness(palette2, bank, index, intensity);
    }
  }
}