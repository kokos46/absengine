#include "include/Entity.h"

void Entity::Move(float dx, float dy) {
    position.x += dx;
    position.y += dy;

    collisionBox.x = position.x;
    collisionBox.y = position.y;
}

void Entity::Draw() const {
    if (texture.id > 0) {
        DrawTexture(texture, (int)position.x, (int)position.y, WHITE);
    } else {
        DrawRectangleRec(collisionBox, DARKGRAY); // Заглушка, если нет спрайта
    }
}

void Entity::SetCollisionBox(Rectangle collisionBox) {
    this->collisionBox = collisionBox;
}

void Entity::SetTexture(Texture2D texture) {
    this->texture = texture;
}

Entity::Entity(Rectangle rect): collisionBox(rect), position({rect.x, rect.y}) {
    texture = { 0 };
}

Rectangle Entity::GetCollisionBox() const { return collisionBox; }