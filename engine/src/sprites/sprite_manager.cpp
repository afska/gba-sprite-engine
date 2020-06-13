//
// Created by Wouter Groeneveld on 26/07/18.
//

#include <libgba-sprite-engine/sprites/sprite_manager.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>

#include <libgba-sprite-engine/gba/tonc_core.h>
#include <libgba-sprite-engine/allocator.h>
#include <libgba-sprite-engine/background/text_stream.h>

#define MAX_SPRITE_SIZE 128
#define MAX_AFFINE_SIZE 31

void SpriteManager::set(std::vector<Sprite*> sprites) {
    initialized = false;

    this->sprites.clear();
    this->sprites.insert(this->sprites.end(), sprites.begin(), sprites.end());
}

void SpriteManager::add(Sprite* sprite) {
    if(sprites.size() == MAX_SPRITE_SIZE) {
        failure_gba(MaxSpriteSizeReached);
    }

    sprites.push_back(sprite);
    copyOverImageDataToVRAM(sprite);
}

void SpriteManager::persist() {
    copyOverImageDataToVRAM();
    initialized = true;
}

void SpriteManager::hideAll() {
    for(int i = 0; i < MAX_SPRITE_SIZE; i++) {
        oam_mem[i].attr0 = ATTR0_HIDE;
    }
}

void SpriteManager::copyOverImageDataToVRAM(Sprite *sprite) {
    if(!sprite->data && sprite->imageSize == 0) {
        // copy from the last sprite
        auto data = Allocator::allocatedSprites.at(Allocator::allocatedSprites.size() - 1);
        sprite->setData(data.pointer());
        sprite->setImageSize(data.size);
        sprite->buildOam(data.getTileLocation());
    } else {
        const auto allocated = Allocator::allocateObjectTiles(sprite->imageSize);
        dma3_cpy(allocated.pointer(), sprite->data, allocated.size);

        sprite->buildOam(allocated.getTileLocation());
    }
}

void SpriteManager::copyOverImageDataToVRAM() {
    for(auto sprite : sprites) {
        copyOverImageDataToVRAM(sprite);
    }
}
