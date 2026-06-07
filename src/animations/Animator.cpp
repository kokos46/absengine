#include "include/Animator.h"

Animator::Animator()
        : spriteSheet(Texture2D{}), frameCount(1), frameWidth(0), frameHeight(0),
          row(0), frameDuration(0.1f), timer(0.0f), currentFrame(0), ownsTexture(false) {
}

Animator::Animator(Texture2D sheet, int frames, float duration)
        : spriteSheet(sheet), frameCount(frames > 0 ? frames : 1), frameDuration(duration),
          timer(0.0f), currentFrame(0), ownsTexture(sheet.id > 0) {
    frameWidth = spriteSheet.width > 0 ? spriteSheet.width / frameCount : 0;
    frameHeight = spriteSheet.height;
    row = 0;
}

Animator::Animator(const char* texturePath, int frames, float duration)
        : Animator(LoadTexture(texturePath), frames, duration) {
}

Animator::Animator(const char* texturePath, int columns, int rows, int row, int frames, float duration)
        : spriteSheet(LoadTexture(texturePath)), frameCount(frames > 0 ? frames : 1),
          frameDuration(duration), timer(0.0f), currentFrame(0), ownsTexture(spriteSheet.id > 0) {
    int safeColumns = columns > 0 ? columns : frameCount;
    int safeRows = rows > 0 ? rows : 1;

    frameWidth = spriteSheet.width > 0 ? spriteSheet.width / safeColumns : 0;
    frameHeight = spriteSheet.height > 0 ? spriteSheet.height / safeRows : 0;
    this->row = row >= 0 && row < safeRows ? row : 0;

    if (frameCount > safeColumns) {
        frameCount = safeColumns;
    }
}

Animator::~Animator() {
    Unload();
}

void Animator::Update(float dt) {
    if (!IsValid() || frameCount <= 1 || frameDuration <= 0.0f) {
        return;
    }

    timer += dt;
    while (timer >= frameDuration) {
        timer -= frameDuration;
        currentFrame = (currentFrame + 1) % frameCount;
    }
}

void Animator::Reset() {
    timer = 0.0f;
    currentFrame = 0;
}

void Animator::Unload() {
    if (ownsTexture && spriteSheet.id > 0) {
        UnloadTexture(spriteSheet);
    }

    spriteSheet = Texture2D{};
    ownsTexture = false;
}

bool Animator::IsValid() const {
    return spriteSheet.id > 0 && frameWidth > 0 && frameHeight > 0;
}

void Animator::Draw(Rectangle destination, bool flipX, Color tint) const {
    if (!IsValid()) {
        DrawRectangleRec(destination, MAGENTA);
        return;
    }

    Rectangle source = {
        static_cast<float>(currentFrame * frameWidth),
        static_cast<float>(row * frameHeight),
        static_cast<float>(flipX ? -frameWidth : frameWidth),
        static_cast<float>(frameHeight)
    };

    DrawTexturePro(spriteSheet, source, destination, Vector2{ 0.0f, 0.0f }, 0.0f, tint);
}
