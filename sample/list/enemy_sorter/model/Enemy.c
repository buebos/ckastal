#ifndef __ENEMY_C__
#define __ENEMY_C__

#include "../../../../include/syntax_utils.h"

typedef struct {
    int x;
    int y;
    int z;
} Position;

typedef enum EnemyType {
    ZOMBIE = 0,
    SKELETON = 1,
    CREEPER = 2,
} EnemyType;

typedef struct Enemy {
    int id;
    int health;

    EnemyType type;
    Position position;
} Enemy;

char* enemy_type_tags[3] = {"Zombie", "Skeleton", "Creeper"};

Enemy* enemy_new(int id, int health, EnemyType type, Position position) {
    Enemy* enemy = (Enemy*)calloc(1, sizeof(Enemy));
    enemy->id = id;
    enemy->health = health;
    enemy->position = position;
    enemy->type = type;

    return enemy;
}

void* enemy_copy(void* data) {
    Enemy* original = cast(data, Enemy*);
    Enemy* copy = enemy_new(original->id, original->health, original->type, original->position);
    return (void*)copy;
}

#endif /** __ENEMY_C__ */