//
// Created by Wouter Groeneveld on 04/08/18.
//

#include <libgba-sprite-engine/effects/fade_out_scene.h>

FadeOutScene::FadeOutScene(int speed) : type(FadeOutType::ToBlack), timesUpdated(0), speed(speed) {}
FadeOutScene::FadeOutScene(int speed, FadeOutType type) : type(type), timesUpdated(0), speed(speed) {}

void FadeOutScene::update() {
  if (!this->palette.get()) {
    auto fgPalette = sceneToAffect->getForegroundPalette();
    auto bgPalette = sceneToAffect->getBackgroundPalette();

    this->palette = std::unique_ptr<CombinedPalette>(*fgPalette + *bgPalette);
  }

  this->palette.get()->changeBrightness(speed * (type == FadeOutType::ToWhite ? 1 : -1));
  timesUpdated++;
}