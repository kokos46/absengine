#ifndef ABSENGINE_ENTITY_H
#define ABSENGINE_ENTITY_H
#include <raylib.h>
#include <functional>


class Entity {
    Rectangle collisionBox;
    Texture2D texture;
    bool isTrigger;
    bool isTriggerActive;
    Vector2 position;

    std::function<void()> onTriggerEnter;

public:
    Entity(Rectangle rect, bool isTrigger = false);

    void Draw() const;
    void Move(float dx, float dy);
    void SetCollisionBox(Rectangle collisionBox);
    void SetTexture(Texture2D texture);
    Rectangle GetCollisionBox() const;
    void SetPosition(Vector2 position);
    Vector2 GetPosition();

    bool IsTrigger() const;

    void SetOnTriggerEnter(std::function<void()> onTriggerEnter);
    void Trigger();
    void UpdateTriggerState(bool isColliding);

};


#endif //ABSENGINE_ENTITY_H
