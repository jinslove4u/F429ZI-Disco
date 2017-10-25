//
//
//  Nokia LCD 5110  48x84 --> 2.4 QVGA 240x320
//
//
//
#include "Space_Invaders.h" //include the declaration for this class

#define leftEnd 310
#define rightEnd 14
#define bottomEnd 220


Ship::Ship(uint8_t newX,  uint8_t newY) {
    x = newX;
    y = newY;
    speed = 10;
    lives = 3;
}
Ship::~Ship() {
    delete &x;
    delete &y;
    delete &speed;
    delete &lives;
}

void Ship::move(uint8_t direction) {
    //uint8_t dir = direction < 0 ? -1 : 1;
    y += speed*direction;
    if (y < rightEnd) {
        y = rightEnd;        
    }

    else if (y > leftEnd) {
        y = leftEnd;    
    }
}
void Ship::setX(uint8_t newX) {
    x = newX;
}
void Ship::setY(uint8_t newY){
    y = newY;
}
uint8_t Ship::getX() {
    return x;
}
uint8_t Ship::getY() {
    return y;
}
void Ship::setLives(uint8_t newLives) {
    lives = newLives;
}
uint8_t Ship::getLives() {
    return lives;
}

Enemy::Enemy( uint8_t newX,  uint8_t newY, uint8_t newSpeed){
    x = newX;
    y = newY;
    alive = true;
///    speed = -2-newSpeed;
    speed = 10;
}

Enemy::Enemy() {
    alive = false;
}

Enemy::~Enemy() {
    delete &x;
    delete &y;
    delete &speed;
    //delete &alive;
}

char Enemy::move() {
    y += speed;
///    if (y < 5 || y > leftEnd ) {
    if (y < rightEnd || y > leftEnd ) {
        speed *= -1;
        y += speed;  // 움직이는 스피드 
        x += 10; // 아래쪽 내려옴 
        return 1;
    }
    return 0;
}
void Enemy::update(char ch) {
    speed *= -1;
    x += 10;  // 아래쪽 내려옴 
    if (ch == 1) {   // 1일 경우에만 스피드만큼 좌우로 움직임 
        y += speed;
    }
}
void Enemy::setDead() {
    alive = false;
}

uint8_t Enemy::getX() {
    return x;
}
uint8_t Enemy::getY() {
    return y;
}
bool Enemy::Alive() {
    return alive;
}
bool Enemy::Defeat() {
    return x >= 200;  // 적이 아래쪽까지 내려오면 끝. 
}

Bullet::Bullet(uint8_t newX, uint8_t newY, uint8_t newSpeed) {
    x = newX;
    y = newY;
    speed = newSpeed;
    exist = 1;
}
Bullet::Bullet() {
    exist = 0;
}
Bullet::~Bullet() {
    delete &x;
    delete &y;
    delete &speed;
    x = -1;
}
void Bullet::move() {
    x += speed+3;
    if (x < 0 || x > bottomEnd-10) {
        exist = 0; //outside LCD margin
    }
}
uint8_t Bullet::getSpeed() {
    return speed;
}

uint8_t Bullet::getX() {
    return x;
}
uint8_t Bullet::getY() {
    return y;
}

char Bullet::getExist() {
    return exist;
}

char Bullet::collisionShip(Ship* ship) {
///    if (x <= ship->getX() && x >= ship->getX()-3
///        && y >= ship->getY()-3 && y <= ship->getY()+3) {//collision detected
    if (x <= ship->getX() && x >= ship->getX()-5
        && y >= ship->getY()-23 && y <= ship->getY()+23) {//collision detected
            exist = 0;
            return 1;
        }
    return 0;
}

char Bullet::collisionEnemy(Enemy* enemy) {
///    if (enemy->Alive() && x <= enemy->getX() && x >= enemy->getX()-4
///        && y >= enemy->getY()-3 && y <= enemy->getY()+3) { //collison
    if (enemy->Alive() && x <= enemy->getX() && x >= enemy->getX()-14
        && y >= enemy->getY()-13 && y <= enemy->getY()+13) { //collison 
            exist = 0;
            return 1;
        }
    return 0;
}
