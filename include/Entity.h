#ifndef ABSENGINE_ENTITY_H
#define ABSENGINE_ENTITY_H
#include <raylib.h>


class Entity {
    Rectangle collisionBox;
    Texture2D texture;

    Vector2 position;
public:
    Entity(Rectangle rect);

    void Draw() const;
    void Move(float dx, float dy);
    void SetCollisionBox(Rectangle collisionBox);
    void SetTexture(Texture2D texture);
    Rectangle GetCollisionBox() const;

};


#endif //ABSENGINE_ENTITY_H
