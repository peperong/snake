#include "console.h"
#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>

#define BOARD_SIZE 20
#define MOVE_DELAY 15
#define WALL_VERTICAL_STRING "┃"
#define WALL_HORIZONTAL_STRING "━"
#define WALL_RIGHT_TOP_STRING "┓"
#define WALL_LEFT_TOP_STRING "┏"
#define WALL_RIGHT_BOTTOM_STRING "┛"
#define WALL_LEFT_BOTTOM_STRING "┗"
#define SNAKE_STRING "■"
#define SNAKE_BODY_STRING "■"
#define APPLE_STRING "●"

int x = 0;
int y = 0;
int win ;
int lose ;
int score = 0;
int snakex = 0;
int snakeLength = 1;
int snake[20];
int snaketail[20] ;
int directions[20];
double movetimer = 0.0;
int _direction = 0;
void drawBoard(void) {
    int i, j;
    for(i = 0; i < 20; i ++) {
        for(j = 0; j < 20; j ++) {
            if ((((i == 0) || (i == 19)) || (j == 0)) || (j == 19)) {
                if (i == 0 && j == 0) {
                    console::draw(0 , 0, WALL_LEFT_TOP_STRING);
                }
                else if ((i == 0) && (j == 19)) {
                    console::draw(19 , 0, WALL_RIGHT_TOP_STRING);
                }
                else if ((i == 19) && (j == 0)) {
                    console::draw(0 , 19, WALL_LEFT_BOTTOM_STRING);
                }
                else if ((i == 19) && (j == 19)) {
                    console::draw(19 , 19, WALL_RIGHT_BOTTOM_STRING);
                }
                else if ((j == 0) || (j == 19)) {
                    console::draw(i, j, WALL_HORIZONTAL_STRING);
                }
                else if ((i == 0) || (i == 19)) {
                    console::draw(i, j, WALL_VERTICAL_STRING);
                }
            }
        }
    }
    std::string scoreString = "Score: " + std::to_string(score);
    console::draw(BOARD_SIZE/2.5-4, BOARD_SIZE/1, scoreString);
}

void drawApple() {
    char i;
    if (i != '\x01') {
    console::draw(x, y, APPLE_STRING);
    }
}

void respawnApple() {
    // 랜덤한 사과 x,y 좌표 생성
    srand(time(NULL));
    if (APPLE_STRING == WALL_LEFT_TOP_STRING) {
        srand(time(NULL));
    }
    // 벽 안에서 랜덤한 사과 좌표 생성
    x = rand() % (BOARD_SIZE - 3) + 1;
    // 벽 안에서 랜덤한 사과 좌표 생성
    y = rand() % (BOARD_SIZE - 3) + 1;
}

void drawSnake(void) {
    int i;
    // 뱀의 머리
    console::draw(snake[0], snaketail[0], SNAKE_STRING);
    
    // 뱀의 꼬리
    for (i = 1; i < snakeLength; i++) {
        console::draw(snake[i], snaketail[i], SNAKE_BODY_STRING);
    }
}

enum Direction { UP, DOWN, LEFT, RIGHT };

Direction direction = RIGHT;
void handleInput() {
    if (snakeLength == 1) {
        if (console::key(console::K_LEFT)) {
            direction = LEFT;
        }
        if (console::key(console::K_RIGHT)) {
            direction = RIGHT;
        }
        if (console::key(console::K_UP)) {
            direction = UP;
        }
        if (console::key(console::K_DOWN)) {
            direction = DOWN;
        }
    }
    else if (snakeLength > 1){ // 꼬리가 생기면 꼬리부분으로는 못감
        if (console::key(console::K_LEFT) && direction != RIGHT)
            direction = LEFT;
        if (console::key(console::K_RIGHT) && direction != LEFT)
            direction = RIGHT;
        if (console::key(console::K_UP) && direction != DOWN)
            direction = UP;
        if (console::key(console::K_DOWN) && direction != UP)
            direction = DOWN;
    }
    if (console::key(console::K_ESC)) {
        console::clear();
        console::draw(BOARD_SIZE/2-5, BOARD_SIZE /2, "YOU LOSE!");
        console::draw(BOARD_SIZE/2-5, BOARD_SIZE /2+1, "Try again? (Enter)");
        console::Key(console::K_ENTER);
        console::init;
        console::wait();
        exit(0);
    }
    for (int i = snakeLength - 1; i > 0; --i) {
            snake[i] = snake[i - 1];
            snaketail[i] = snaketail[i - 1];
        }

    if (snake[0] == x && snaketail[0] == y) {
        score += 10 ;
        snakeLength++;
        snaketail[snakeLength - 1] = snaketail[snakeLength - 2];
        respawnApple();
    }
}
void moveSnake() {
  switch (direction) {
    case UP:
      snaketail[0]--;
      break;
    case DOWN:
      snaketail[0]++;
      break;
    case LEFT:
      snake[0]--;
      break;
    case RIGHT:
      snake[0]++;
      break;
  }
}

void restrictInScreen() {
    // x, y 위치를 화면의 최대 크기에서 벗어나지 않게 한다.
    if (x < 0)
        x = 0;
    if (x >= console::SCREEN_WIDTH)
        x = console::SCREEN_WIDTH - 2;
    if (y < 0)
        y = 0;
    if (y >= console::SCREEN_HEIGHT)
        y = console::SCREEN_HEIGHT - 2;
}

bool checkCollision() {
    // 머리와 꼬리가 충돌하는지 확인
    for (int i = 1; i < snakeLength; ++i) {
        if (snake[0] == snake[i] && snaketail[0] == snaketail[i])
            return true;
    }

    // 뱀이 화면 밖으로 나가면 충돌로 간주
    if (snake[0] < 0 || snake[0] >= BOARD_SIZE || snaketail[0] < 0 || snaketail[0] >= BOARD_SIZE)
        return true;

    return false;
}

bool gameOver() {
    return checkCollision();
}

void game() {
   respawnApple();
  // 콘솔 라이브러리를 초기화한다.
  console::init();
    snake[0] = BOARD_SIZE / 2;
    snaketail[0] = BOARD_SIZE / 2;
  while (true) {
    // 화면을 지운다.
    console::clear();

    handleInput();
    moveSnake();
    restrictInScreen();
    drawBoard();
    drawApple();
    drawSnake();
    if(gameOver()) {

        break;
        }
    // 화면을 갱신하고 다음 프레임까지 대기한다.
    for (int i = 0; i < MOVE_DELAY; i++)
        console::wait();
  }
}
