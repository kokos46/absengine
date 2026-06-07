//
// Created by Попехин Константин on 07.06.2026.
//

#ifndef ABSENGINE_ANIMATOR_H
#define ABSENGINE_ANIMATOR_H

#include <raylib.h>

class Animator {
    Texture2D spriteSheet;
    int frameCount;
    int frameWidth;
    int frameHeight;
    int row;
    float frameDuration;
    float timer;
    int currentFrame;
    bool ownsTexture;

public:
    Animator();
    Animator(Texture2D sheet, int frames, float duration);
    Animator(const char* texturePath, int frames, float duration);
    Animator(const char* texturePath, int columns, int rows, int row, int frames, float duration);
    ~Animator();

    Animator(const Animator&) = delete;
    Animator& operator=(const Animator&) = delete;

    void Update(float dt);
    void Reset();
    void Unload();
    bool IsValid() const;

    void Draw(Rectangle destination, bool flipX = false, Color tint = WHITE) const;
};

#endif //ABSENGINE_ANIMATOR_H
