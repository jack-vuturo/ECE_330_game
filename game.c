#include "game.h"
#include <stdlib.h>

Game *Init_Game()
{
    Game *game = (Game *)malloc(sizeof(Game));
    game->screen_width = 1536;
    game->screen_height = 256;
    game->cell_width = game->screen_width / 8;
    game->cell_height = game->screen_height / 3;
    Reset_Game(game);
    return game;
}

void Reset_Game(Game *game)
{
    game->score = 0b00000011000000;
    game->last_score = 0;
    game->ball_x = 768;
    game->ball_speed_x = 198;
    game->ball_speed_y = rand() % 2 == 1 ? -40 : 40;
    game->ball_y = 128;
    game->p1_y = 128;
    game->p2_y = 128;
    game->next_state = 1;
}

// x input is 0 - 7
// y input is 0 - 2
// return 0 if not lit, 1 if lit
int Is_Lit(Game *game, int x, int y)
{

    int y_value = 0;
    int x_value = 0;

    if (x == 0)
    {
        y_value = game->p1_y;
        x_value = 0;
    }
    else if (x == 7)
    {
        y_value = game->p2_y;
        x_value = game->screen_width - 1;
    }
    else
    {
        y_value = game->ball_y;
        x_value = game->ball_x;
    }

    if (x * game->cell_width > x_value || (x + 1) * game->cell_width <= x_value)
    {
        return 0;
    }
    if (y * game->cell_height > y_value || (y + 1) * game->cell_height <= y_value)
    {
        return 0;
    }

    return 1;
}

void Move_Ball(Game *game)
{
    game->ball_x += game->ball_speed_x;
    game->ball_y += game->ball_speed_y; // Adding vertical movement here

    // Horizontal boundary checks
    if (game->ball_x < 0)
    {
        game->ball_x = 0;
        game->ball_speed_x = -game->ball_speed_x;
    }
    if (game->ball_x > 1535)
    { // Example max boundary
        game->ball_x = 1535;
        game->ball_speed_x = -game->ball_speed_x;
    }

    // Vertical boundary checks (example, adapt as needed)
    if (game->ball_y < 0)
    {
        game->ball_y = 0;
        game->ball_speed_y = -game->ball_speed_y;
    }
    if (game->ball_y > game->screen_height)
    { // Example max y-boundary
        game->ball_y = game->screen_height;
        game->ball_speed_y = -game->ball_speed_y;
    }

    // Paddle collision checks
    if (game->ball_x == 0)
    {
        if (game->p1_y <= game->ball_y && game->ball_y <= game->p1_y + game->cell_height)
        {
            game->ball_speed_x = -game->ball_speed_x;
        }
    }
    if (game->ball_x == 1535)
    {
        if (game->p2_y <= game->ball_y && game->ball_y <= game->p2_y + game->cell_height)
        {
            game->ball_speed_x = -game->ball_speed_x;
        }
    }

    // Score checks
    if (game->ball_x == 0 || game->ball_x == 1535)
    {
        game->last_score = game->ball_x == 0 ? 2 : 1;
        Update_Game_State(game);
    }
    
}

void Player1_Paddle(Game *game, int value)
{
    if (value >= 0 && value <= game->screen_height)
    {
        game->p1_y = value;
    }
}

void Player2_Paddle(Game *game, int value)
{
    if (value >= 0 && value <= game->screen_height)
    {
        game->p2_y = value;
    }
}

void Update_Game_State(Game *game)
{
    switch (game->next_state)
    {
    case 0: // button pressed -> game start
        game->next_state = 1;
        break;
    case 1: // game play (someone scored) -> missed ball / won
        game->score = game->last_score == 1 ? game->score << 1 : game->score >> 1;
        game->next_state = game->score == 0b00000000000001 || game->score == 0b10000000000000 ? 3 : 2;
        break;
    case 2: // missed ball -> game play
        game->ball_speed_x *= -1;
        game->ball_x = game->screen_width / 2;
        game->ball_y = game->screen_height / 2;
        game->next_state = 1;
        break;
    case 3: // won -> game start
        Reset_Game(game);
        game->next_state = 0;
        break;
    }
}
