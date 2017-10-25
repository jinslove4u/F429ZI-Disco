
/*
void drawShip(byte ShipX, byte ShipY) {
  display.fillRect(ShipX-2, ShipY-3, 3, 2, LTDC_GREEN);
  display.fillRect(ShipX-2, ShipY+2, 3, 2, LTDC_GREEN);
  display.fillRect(ShipX-1, ShipY-1, 2, 3, LTDC_GREEN);
  display.drawFastHLine(ShipX-4, ShipY, 3, LTDC_GREEN);
}

void drawEnemy(byte x, byte y) {
  display.fillRect(x-2, y-3, 3, 7, LTDC_BLUE);
  display.drawPixel(x-3, y-3, LTDC_BLUE);
  display.drawPixel(x-3, y+3, LTDC_BLUE);
  display.drawPixel(x-4, y, LTDC_BLUE);
  display.drawFastVLine(x-3, y-1, 3, LTDC_BLUE);
  display.drawPixel(x-1, y-1, LTDC_WHITE);
  display.drawPixel(x-1, y+1, LTDC_WHITE);
///  display.display();
}

*/
void drawShip(byte ShipX, byte ShipY) {
    display.fillRect(ShipX-3, ShipY-13, 10, 10, LTDC_WHITE); // right
    display.fillRect(ShipX-3, ShipY+3, 10, 10, LTDC_GREEN);  // left
    display.fillRect(ShipX+3, ShipY-3, 6, 6, LTDC_BLUE);     // booster
    display.fillRect(ShipX-8, ShipY-3, 10, 6, LTDC_RED);     // con
//  display.drawFastHLine(ShipX-4, ShipY, 13, LTDC_RED);     // con
}


void drawEnemy(byte x, byte y) {
  display.fillRect(x-3, y-3, 6, 6, LTDC_BLUE);
  display.fillRect(x-3, y+3, 6, 3, LTDC_RED);
  display.drawPixel(x-1, y-2, LTDC_WHITE);
  display.drawPixel(x-1, y+2, LTDC_WHITE);
/*  
  display.fillRect(x-2, y-3, 3, 7, LTDC_BLUE);
  display.drawPixel(x-3, y-3, LTDC_BLUE);
  display.drawPixel(x-3, y+3, LTDC_BLUE);
  display.drawPixel(x-4, y, LTDC_BLUE);
  display.drawFastVLine(x-3, y-1, 3, LTDC_BLUE);
  display.drawPixel(x-1, y-1, LTDC_WHITE);
  display.drawPixel(x-1, y+1, LTDC_WHITE);
*/
}


void initGame(byte lvl) {
  if (lvl == 1) {
    ship =  new Ship(220, 160);
    score = 0;
///    shiftOut(dataB, CP, MSBFIRST, 0);
  }
  else {  
    ship->setX(220); 
    ship->setY(160); //reset ship position
  }
  //Enemy initialization
  for (byte i = 1; i <= 4; i++) {
    for (byte j = 1; j <= 5; j++) {
///   enemy[(i-1)*3+j-1] = new Enemy(10*i+6, 300 - 9*(j-1), lvl);
      enemy[(i-1)*5+j-1] = new Enemy(20*i+6, 100 - 20*(j-1), lvl);  // 적들 초기 배열 
    }
  }
  //draw the level
  display.fillScreen(LTDC_BLACK);
  drawShip(ship->getX(), ship->getY());
  drawLives(ship->getLives());
  for (byte i=0; i<nr_enemy; i++) {
    drawEnemy(enemy[i]->getX(), enemy[i]->getY());
  }
}

void moveShip() {
  joyX = analogRead(MISSILE);  // <------- joystic 

///  short int dir = map(joyX, 0, 1023, -5, 5);
  short int dir = map(joyX, 0, 1023, -5, 5);  // 아날로그입력값 최대값 1023, 초소값 32, 중간값 ??? 
  if (dir != 0) { 
    dir = dir < 0 ? -1 : 1;
///    display.fillRect(ship->getX()-6, ship->getY()-7, 7, 15, LTDC_BLACK);
    display.fillRect(ship->getX()-8, ship->getY()-13, 17, 28, LTDC_BLACK);  // 움직임후 잔상 지움. 
    ship->move(dir);
    drawShip(ship->getX(), ship->getY());
  }
}

void moveBullet() {
  if (ShipBullet->getExist() == 1) {//the bullet exists
///    display.drawFastHLine(ShipBullet->getX(), ShipBullet->getY(), 3, LTDC_BLACK);
       display.fillRect(ShipBullet->getX(), ShipBullet->getY(), 5, 2, LTDC_BLACK);     // Missile shadows
///    display.display();
    ShipBullet->move();
///    display.drawFastHLine(ShipBullet->getX(), ShipBullet->getY(), 3, LTDC_RED);
       display.fillRect(ShipBullet->getX(), ShipBullet->getY(), 5, 2, LTDC_WHITE);     //  Missile
///    display.display();
    for (byte i=0; i<nr_enemy; i++) {
      if (ShipBullet->collisionEnemy(enemy[i]) == 1) {
        enemy[i]->setDead();
        score += 100;
      }
    }
  }
  if (EnemyBullet->getExist() == 1) {//the bullet exists
///    display.drawFastHLine(EnemyBullet->getX(), EnemyBullet->getY(), 3, LTDC_BLACK);
       display.fillRect(EnemyBullet->getX(), EnemyBullet->getY(), 5, 2, LTDC_BLACK);     // Missile shadows
    EnemyBullet->move();
///    display.drawFastHLine(EnemyBullet->getX(), EnemyBullet->getY(), 3, LTDC_RED);
       display.fillRect(EnemyBullet->getX(),  EnemyBullet->getY(), 5, 2, LTDC_RED);     //  Missile
    if (EnemyBullet->collisionShip(ship) == 1) {
      ship->setLives(ship->getLives()-1);
      if(ship->getLives() !=0) {
      ship->setX(220);  // 충돌후 초기화 
      ship->setY(160);  // 충돌후 초기화 
      }
    }
  }
}

byte remaining_enemys() {
  byte k=0;
  for (byte i=0; i<nr_enemy; i++) {
    if(enemy[i]->Alive()) {
      k++;
    }
  }
  return k;
}


void moveEnemy() {
  bounce = 0;
  for (byte i=0; i<nr_enemy; i++) { // 적의 수만큼 루프 
    if (bounce == 0 && enemy[i]->Alive() == true && enemy[i]->move() == 1) {  // 살아있는 적 대상 
      for (byte j=0; j<nr_enemy; j++) { //schimb directia la toti ceilalti
        if(j < i) {
          enemy[j]->update(1);
        }
        else if (j > i) {
          enemy[j]->update(0);
        }
      }
      bounce = 1;
    }          
  }
  display.fillScreen(LTDC_BLACK);
  drawShip(ship->getX(), ship->getY());
  drawLives(ship->getLives());
  for (byte i=0; i<nr_enemy; i++) { //적 배열만큼 조회 
    if (enemy[i]->Alive()) { // 만약 적들이 남아 있다면.. 
      drawEnemy(enemy[i]->getX(), enemy[i]->getY()); //  적 움직임. 
      if (enemy[i]->Defeat()) {
        play = 2;  // 게임오버 
      }
      if((int)random(chance_shoot) == 0) {//enemy shoots bullet
        addBullet(enemy[i]->getX(), enemy[i]->getY(), 3, 0);  // 적 미사일발사 
      }
    }
  }
}

void drawLives(unsigned short int lives) {
  for (byte i=0; i<lives; i++) { 
    drawL(5, 300 - i*7);
  }
} 

void drawL(int x, int y) {
  display.fillRect(x+1, y, 3, 5, LTDC_YELLOW);
  display.drawPixel(x, y+1, LTDC_YELLOW);
  display.drawPixel(x, y+3, LTDC_YELLOW);
  display.drawFastVLine(x+4, y+1, 3, LTDC_YELLOW);
  display.drawPixel(x+5, y+2, LTDC_YELLOW);
///  display.display();
}

void buttonsPressed() {
/*  
//  char b = getButton(BPIN, oldB);
  char b = getButton(MISSILE, oldB);

///  if (b == 0 && oldB == 1) {
  if (b == 0 ) {
    oldB = 0;
    if (play == 0) {
      addBullet(ship->getX()-8, ship->getY(), -3, 1);
    }
    else if (play == 2) {
      play = 0;
      level = 1;
      initGame(level);
    }
  }
  else if(b == 1 && oldB == 0) {
    oldB = 1;
  }

//  char b1 = getButton(JPIN, oldJB);
  char b1 = getButton(PAUSE, oldJB);
  
  if (b1 == 0 && oldJB == 1) {
    oldJB = 0;
    if (play == 0) {
      play = 1;
    }
    else if (play == 1) {
      play = 0;
    }
    else if (play == 2) {
      play = 0;
      level = 1;
      initGame(level);
    }
  }
  else if(b1 == 1 && oldJB == 0) {
    oldJB = 1;
  }
 */ 
}

void showMenu() {
///  display.clearDisplay();
  display.drawBitmap(100, 155, game, 8, 23, LTDC_YELLOW);
  display.drawBitmap(115, 155, over, 8, 23, LTDC_YELLOW);
///  display.display();
}

void showPause() {
///  display.clearDisplay();
  display.drawBitmap(100, 150, pause, 8, 32, LTDC_RED);
///  display.display();
}


