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
    this->position = { collisionBox.x, collisionBox.y };
}

void Entity::SetTexture(Texture2D texture) {
    this->texture = texture;
}

Entity::Entity(Rectangle rect, bool isTrigger): collisionBox(rect), isTrigger(isTrigger), isTriggerActive(false), position({rect.x, rect.y}) {
    texture = Texture2D{};
}

Rectangle Entity::GetCollisionBox() const { return collisionBox; }

void Entity::SetPosition(Vector2 position) {
    this->position = position;
    this->collisionBox.x = position.x;
    this->collisionBox.y = position.y;
}

Vector2 Entity::GetPosition() {
    return position;
}

bool Entity::IsTrigger() const {
    return isTrigger;
}

void Entity::SetOnTriggerEnter(std::function<void()> action) {
    this->onTriggerEnter = std::move(action);
}

void Entity::Trigger() {
    if (onTriggerEnter) onTriggerEnter();
}

void Entity::UpdateTriggerState(bool isColliding) {
    if (isColliding && !isTriggerActive) {
        Trigger();
    }

    isTriggerActive = isColliding;
}
