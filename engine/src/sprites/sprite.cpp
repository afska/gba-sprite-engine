#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/sprites/sprite.h>

#include <cstdlib>

// All the functions are inline in sprite.h

Sprite::Sprite(const Sprite& other)
    : Sprite(nullptr, 0, other.x, other.y, other.spriteSize) {
  tileIndex = other.tileIndex;
  animationDelay = other.animationDelay;
  numberOfFrames = other.numberOfFrames;
  currentFrame = other.currentFrame;
  animationCounter = other.animationCounter;
}

Sprite::Sprite(const void* imageData,
               int imageSize,
               int x,
               int y,
               SpriteSize size)
    : x(x),
      y(y),
      data(imageData),
      imageSize(imageSize),
      spriteSize(size),
      priority(0),
      affineId(0),
      doubleSize(false),
      animationDelay(0),
      numberOfFrames(0),
      beginFrame(0),
      currentFrame(0),
      animationCounter(0) {
  setAttributesBasedOnSize(size);
}