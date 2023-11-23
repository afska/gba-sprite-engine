//
// Created by Wouter Groeneveld on 28/07/18.
//

#include <libgba-sprite-engine/allocator.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>

u16 GBAEngine::readKeys() {
  return ~REG_KEYS & KEY_ANY;
}

GBAEngine::GBAEngine() {
  REG_DISPCNT = DCNT_MODE0 | DCNT_OBJ | DCNT_OBJ_1D | DCNT_BG0 | DCNT_BG1 |
                DCNT_BG2 | DCNT_BG3;
  REG_SNDDSCNT = 0;
  disableTextBg = false;
  Allocator::free();
}

void GBAEngine::transitionIntoScene(Scene* scene, SceneEffect* effect) {
  sceneToTransitionTo = scene;
  currentEffectForTransition = effect;
  currentEffectForTransition->setSceneToAffect(this->currentScene);
}

void GBAEngine::cleanupPreviousScene() {
  for (auto bg : currentScene->backgrounds()) {
    bg->clearData();
  }

  delete currentScene;
  sceneToTransitionTo = nullptr;
  delete currentEffectForTransition;
  currentEffectForTransition = nullptr;
}

void GBAEngine::setScene(Scene* scene) {
  if (this->currentScene) {
    cleanupPreviousScene();
    if (!this->disableTextBg) {
      TextStream::instance().clear();
    }
  }
  spriteManager.hideAll();
  scene->load();

  auto fgPalette = scene->getForegroundPalette();
  if (!fgPalette) {
    failure_gba(NoFgPaletteDefined);
  }
  fgPalette->persist();
  auto bgPalette = scene->getBackgroundPalette();
  if (!bgPalette) {
    failure_gba(NoBgPaletteDefined);
  }
  bgPalette->persist();

  if (!this->disableTextBg) {
    TextStream::instance().persist();
  }

  for (const auto bg : scene->backgrounds()) {
    bg->persist();
  }

  this->currentScene = scene;
  updateSpritesInScene();
}

void GBAEngine::updateSpritesInScene() {
  Allocator::free();
  spriteManager.set(currentScene->sprites());
  spriteManager.persist();
}