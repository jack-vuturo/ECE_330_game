#ifndef GAME_H
#define GAME_H


struct Game {
   int score;
   int last_score; // 0 for none, 1 for p1, 2 for p2
   int p1_y;   // value between 0 and 256
   int p2_y;   // value between 0 and 256
   int ball_x; // value between 0 and 1536
   int ball_y; // value between 0 and 256
   int ball_speed_x;
   int ball_speed_y;
   int next_state;
   int screen_width;
   int screen_height;
   int cell_width;
   int cell_height;
};


typedef struct Game Game;


Game* Init_Game();
void Reset_Game(Game *game);
int Is_Lit(Game *game, int x, int y);
void Move_Ball(Game *game);
void Player1_Paddle(Game *game, int value);
void Player2_Paddle(Game *game, int value);
void Update_Game_State(Game *game);


#endif // GAME_H
