#include "game.h"
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

struct termios originalTermios;

void enableRawMode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);           // Get current terminal attributes
    originalTermios = term;                   // Save original terminal attributes
    term.c_lflag &= ~(ICANON | ECHO);         // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);  // Apply new settings immediately
}

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &originalTermios); // Restore original attributes
}

int main() {

    Game *game = Init_Game();

    int knob1Value = 0;
    int knob2Value = 0;

    char c;

    // STATES:
    // 0: waiting for input to start game
    // 1: game play
    //    phase 1: get input
    //    phase 2: update game state
    //    phase 3: render
    // 2: missed ball
    // 3: winner

    enableRawMode(); // enable raw mode so we can read input without pressing enter
    
    while (1) {
        switch (game->next_state) {
            case 0:
                // TODO: replace with button press
                printf("\e[1;1H\e[2J"); // clear screen
                printf("Press 1 to start\n");
                c = getchar();
                if (c == '1') {
                    Update_Game_State(game);
                }
                break;
            case 1:
                // game play
                // phase 1: get input
                // TODO: replace with knob input
                c = getchar();
                if (c == 'w') {
                    knob1Value -= 40;
                } else if (c == 's') {
                    knob1Value += 40;
                } else if (c == 'i') {
                    knob2Value -= 40;
                } else if (c == 'k') {
                    knob2Value += 40;
                }

                // phase 2: update game state
                Player1_Paddle(game, knob1Value);
                Player2_Paddle(game, knob2Value);
                Move_Ball(game);

                // phase 3: render
                printf("\e[1;1H\e[2J"); // clear screen

                printf("P1_y: %d\n", game->p1_y);
                printf("P2_y: %d\n", game->p2_y);
                printf("Ball: (%d, %d)\n", game->ball_x, game->ball_y);

                printf("\n\nGame:\n");

                // TODO: replace with 7seg display
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 8; j++) {
                        printf(Is_Lit(game, j, i) ? "*" : " ");
                    }
                    printf("\n");
                }

                // TODO: replace with LED display
                printf("Score: ");
                int i;
                for (i = 15; i >= 0; i--) {
                    printf("%d", (game->score >> i) & 1);
                }
                printf("\n");

                // phase 4: update state
                break;
            case 2:
                // missed ball
                printf("\e[1;1H\e[2J"); // clear screen
                if (game->last_score == 1) {
                    printf("Player 1 scored!\n");
                } else if (game->last_score == 2) {
                    printf("Player 2 scored!\n");
                } else {
                    printf("Error: last_score is not 1 or 2\n");
                }
                Update_Game_State(game);
                break;
            case 3:
                printf("\e[1;1H\e[2J"); // clear screen
                printf("Game over!\n");
                Update_Game_State(game);
                break;
        }


        // phase 3: render
        // render 8x3 grid of pixels (print _ if pixel is on and nothing if pixel is off)
        // the left most column is the left paddle, the right most column is the right paddle
        // the middle column is the ball

        


    }

    disableRawMode();
    printf("Exiting...\n");
    return 0;
}