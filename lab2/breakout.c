/***************************************************************************************************
 * DON'T REMOVE THE VARIABLES BELOW THIS COMMENT                                                   *
 **************************************************************************************************/
unsigned long long __attribute__((used)) VGAaddress = 0xc8000000; // Memory storing pixels
unsigned int __attribute__((used)) red = 0x0000F0F0;
unsigned int __attribute__((used)) green = 0x00000F0F;
unsigned int __attribute__((used)) blue = 0x000000FF;
unsigned int __attribute__((used)) white = 0x0000FFFF;
unsigned int __attribute__((used)) black = 0x0;

// Don't change the name of this variables
#define NCOLS 10 // <- Supported value range: [1,18]
#define NROWS 18 // <- This variable might change.
#define TILE_SIZE 15 // <- Tile size, might change.

char *won = "You Won";       // DON'T TOUCH THIS - keep the string as is
char *lost = "You Lost";     // DON'T TOUCH THIS - keep the string as is
unsigned short height = 240; // DON'T TOUCH THIS - keep the value as is
unsigned short width = 320;  // DON'T TOUCH THIS - keep the value as is
char font8x8[128][8];        // DON'T TOUCH THIS - this is a forward declaration
unsigned char tiles[NROWS][NCOLS] __attribute__((used)) = { 0 }; // DON'T TOUCH THIS - this is the tile map
/**************************************************************************************************/

/***
 * TODO: Define your variables below this comment
 */
unsigned int __attribute__((used)) pink = 0x0000F81F;
/***
 * You might use and modify the struct/enum definitions below this comment
 */
typedef struct _block
{
    unsigned char destroyed;
    unsigned char deleted;
    unsigned int pos_x;
    unsigned int pos_y;
    unsigned int color;
} Block;

typedef enum _gameState
{
    Stopped = 0,
    Running = 1,
    Won = 2,
    Lost = 3,
    Exit = 4,
} GameState;
GameState currentState = Stopped;

typedef struct _ball
{
    unsigned int x; // ball x-coord
    unsigned int y; // ball y-coord
    unsigned int old_x;
    unsigned int old_y;
    int dx;
    int dy;
    unsigned int color;
} Ball;



typedef struct _bar
{
    unsigned int y;
    unsigned int old_y;
    unsigned int height;
    unsigned int width;
    unsigned int t_b_color;
    unsigned int mid_color;
} Bar;

/***
 * Here follow the C declarations for our assembly functions
 */

// TODO: Add a C declaration for the ClearScreen assembly procedure
// Don't modify the function headers
void SetPixel(unsigned int x_coord, unsigned int y_coord, unsigned int color);
int ReadUart();
void WriteUart(char c);
void ClearScreen();
void write(char *str);

/***
 * Now follow the assembly implementations
 */
// It must only clear the VGA screen, and not clear any game state
asm("ClearScreen: \n\t"
    "    PUSH {LR} \n\t"
    "    PUSH {R4-R7} \n\t"
    "    LDR R4, =VGAaddress\n\t"
    "    LDR R4, [R4]\n\t" // dereferencing
    "    LDR R5, =white\n\t"
    "    LDR R5, [R5]\n\t"
    "    MOV R7, #0\n\t" // y-coord
    "   y_loop: \n\t"
        "    CMP R7, #240\n\t"
        "    BGE done_loop\n\t"
        "    MOV R6, #0\n\t" // x-coord
    "   x_loop: \n\t"
        "   CMP R6, #320\n\t"
        "   BGE next_row\n\t"
        "   MOV R0, R6\n\t"
        "   MOV R1, R7\n\t"
        "   MOV R2, R5\n\t"
        "   BL SetPixel\n\t"
        "   ADD R6, R6, #1\n\t"
        "   B x_loop\n\t"
    "   next_row: \n\t"
        "   ADD R7, R7, #1\n\t"
        "   B y_loop\n\t"
    // TODO: Add ClearScreen implementation in assembly here
    "   done_loop: \n\t"
        "    POP {R4-R7}\n\t"
        "    POP {LR} \n\t"
        "    BX LR");

// assumes R0 = x-coord, R1 = y-coord, R2 = colorvalue
asm("SetPixel: \n\t"
    "LDR R3, =VGAaddress \n\t"
    "LDR R3, [R3] \n\t"
    "LSL R1, R1, #10 \n\t"
    "LSL R0, R0, #1 \n\t"
    "ADD R1, R0 \n\t"
    "STRH R2, [R3,R1] \n\t"
    "BX LR");

asm("ReadUart:\n\t"
    "LDR R1, =0xFF201000 \n\t"
    "LDR R0, [R1]\n\t"
    "BX LR");

// TODO: Add the WriteUart assembly procedure here that respects the WriteUart C declaration on line 46
asm("WriteUart:\n\t"
    "    LDR R1, =0xFF201000\n\t"
    "    STR R0, [R1]\n\t"
    "    BX LR\n\t");

// init game elements
static Block field[NROWS][NCOLS];
Ball ball = {120, 120, 120, 120, 1, 0, 0x0}; // initializing ball
Bar bar = {98, 98, 45, 7, 0x000000FF, 0x0000F81F}; // initializing bar

// TODO: Implement the C functions below
// Don't modify any function header
void draw_block(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int color)
{
    for(unsigned int row = 0; row < height; row++){
        for(unsigned int col = 0; col < width; col++){
            unsigned int px = x + col;
            unsigned int py = y + row;
            // Only draw if within screen bounds
            if(px < 320 && py < 240){
                SetPixel(px, py, color);
            }
        }
    }
}

void delay(int time) {
    for (volatile int i = 0; i < time; i++) { }
}


void draw_bar(unsigned int y)
{
    draw_block(0, y, bar.width, 15, bar.t_b_color);
    draw_block(0, y+15, bar.width, 15, bar.mid_color);
    draw_block(0, y+30, bar.width, 15, bar.t_b_color);
}

void clear_bar()
{
    draw_block(0, bar.old_y, bar.width, 15, 0x0000FFFF);
    draw_block(0, bar.old_y+15, bar.width, 15, 0x0000FFFF);
    draw_block(0, bar.old_y+30, bar.width, 15, 0x0000FFFF);
}

void draw_ball() 
{
    for (int dy = -3; dy <= 3; dy++) {
        int row_width = 7 - 2 * (dy < 0 ? -dy : dy);
        int start_x = ball.x - row_width / 2;
        int y = ball.y + dy;

        for (int dx = 0; dx < row_width; dx++) {
            SetPixel(start_x + dx, y, pink);
        }
    }
}

// nearly the same as draw_ball, but this is the same color as the background
void clear_ball() 
{
    for (int dy = -3; dy <= 3; dy++) {
        int row_width = 7 - 2 * (dy < 0 ? -dy : dy); // diamond row width
        int start_x = ball.old_x - row_width / 2;
        int y = ball.old_y + dy;

        for (int dx = 0; dx < row_width; dx++) {
            SetPixel(start_x + dx, y, 0x0000FFFF);
        }
    }
}

void clear_block(unsigned int row, unsigned int col)
{
    draw_block(field[row][col].pos_x, field[row][col].pos_y, TILE_SIZE, TILE_SIZE, 0x0000FFFF);
    field[row][col].destroyed = 1;
}

void init_playing_field() {
    unsigned int left_edge = width - (NCOLS * TILE_SIZE);
    for (int row = 0; row < NROWS; row++) {
        for (int col = 0; col < NCOLS; col++) {
            field[row][col].pos_x = left_edge + col * TILE_SIZE;
            field[row][col].pos_y = row * TILE_SIZE;
            field[row][col].destroyed = 0;
            switch ((row + col) % 3) {
                case 0: field[row][col].color = red; break;
                case 1: field[row][col].color = green; break;
                case 2: field[row][col].color = blue; break;
            }
        }
    }
}



void draw_playing_field()
{
    for(int i = 0; i < NROWS; i++){
        for(int j = 0; j < NCOLS; j++){
            if(field[i][j].destroyed)
            {
                continue;
            }
            draw_block(field[i][j].pos_x, field[i][j].pos_y, TILE_SIZE, TILE_SIZE, field[i][j].color);
        }
    }
}

void corner_checks()
{
    // coordinates of the ball's corners
    int corners_x[4] = {ball.x, ball.x+3, ball.x, ball.x-3};
    int corners_y[4] = {ball.y-3, ball.y, ball.y+3, ball.y};

    // iterating over each corner of the ball
    for(int c = 0; c < 4; c++){
        int cx = corners_x[c];
        int cy = corners_y[c];
        for(int i = 0; i < NROWS; i++){
            for(int j = 0; j < NCOLS; j++){
                if(field[i][j].destroyed == 1){
                    continue;
                }

                // checking if the corner is within the block's borders
                if(cx >= field[i][j].pos_x && cx < field[i][j].pos_x + TILE_SIZE && cy >= field[i][j].pos_y && cy < field[i][j].pos_y + TILE_SIZE){
                    field[i][j].destroyed = 1;
                    clear_block(i, j); // drawing a white block in its place
                    switch(c)
                    {
                        // 0 and 2 are the top and bottom corners
                        case 0:
                        case 2: 
                            ball.dy = -ball.dy; 
                            break;
                        // 1 and 3 are the right and left corners
                        case 1:
                        case 3: 
                            ball.dx = -ball.dx; 
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
}

void handle_wall_collisions() 
{
    // top wall
    if (ball.y <= 3) {
        ball.y = 3;
        ball.dy = -ball.dy;
    }

    // bottom wall
    if (ball.y >= height - 4) {
        ball.y = height - 4;
        ball.dy = -ball.dy;
    }
}

void handle_bar_collisions()
{
    int ball_left_x = ball.x - 3;
    if(ball_left_x <= bar.width)
    {
        if(ball.y >= bar.y && ball.y < bar.y + bar.height)
        {
            int hit_pos = ball.y - bar.y;
            if(hit_pos < 15)
            {
                ball.dx = -ball.dx;
                ball.dy = -1;
            }
            else if(hit_pos < 30)
            {
                ball.dx = -ball.dx;
				ball.dy = 0;
            }
            else
            {
                ball.dx = -ball.dx;
                ball.dy = 1;
            }
            ball.x = bar.width + 3;
        }
    }
}


void update_game_state()
{
;
    if (currentState != Running)
    {
        return;
    } 
    // remove old position
    clear_ball();

    // save current position as old position
    ball.old_x = ball.x;
    ball.old_y = ball.y;

    // move the ball 
    ball.x += ball.dx;
    ball.y += ball.dy;

    // check if any corner has hit the wall, if so delete the block
    corner_checks();

    // check if any corner hit the top or bottom border
    handle_wall_collisions();

    // checking if any collisions with the bar have happened
    handle_bar_collisions();

    // draw the ball in the new position
    draw_ball();
    
    // game lost condition
    if(ball.x < 7){
        currentState = Lost;
        return;
    }
    // game win condition
    if(ball.x >= 320){
        currentState = Won;
        return;
    }
    
}

void update_bar_state()
{
    bar.old_y = bar.y;
    while(1){
        int remaining = 0;
        unsigned int readin = ReadUart();
        if(!(readin & 0x8000)){
            return;
        }
        char c = readin & 0xFF;
        // moving bar depending on char c
        if(c == 'w'){
            if(bar.y > 0){
                clear_bar();
                bar.y -= 15;
                draw_bar(bar.y);
            }
        }
        else if(c == 's'){
            if(bar.y + bar.height < height){
                clear_bar();
                bar.y += 15;
                draw_bar(bar.y);
            }
        }
        remaining = (readin & 0x00FF0000) >> 16;
        if (remaining == 0) return;                   // done for this frame
    }
}

void def_values()
{
    ball.x = width/2;
    ball.y = height/2;
    ball.dx = 1;
    ball.dy = 0;
    ball.color = black;
    bar.y = (height - bar.height)/2;

    // initialize field
    init_playing_field();
    draw_ball();
    draw_bar(bar.y);
    currentState = Stopped;
}

void write(char *str)
{
    // TODO: Use WriteUart to write the string to JTAG UART
    while(*str){
        WriteUart(*str++);
    }
}

void play()
{
    // HINT: This is the main game loop
    init_playing_field();
    draw_bar(bar.y);
    while (1)
    {
        update_game_state();
        update_bar_state();
        if (currentState != Running)
        {
            break;
        }
        // drawing new positions
        draw_playing_field();
        
    }
    if (currentState == Won)
    {
        write(won);
    }
    else if (currentState == Lost)
    {
        write(lost);
    }
    else if (currentState == Exit)
    {
        return;
    }
    currentState = Stopped;
}

// It must initialize the game
void reset()
{
    // Hint: This is draining the UART buffer
    int remaining = 0;
    do {
        unsigned int out = ReadUart();
        if (!(out & 0x8000)) break;                     // stop draining, but continue reset
        remaining = (out & 0x00FF0000) >> 16;           // correct field + mask
    } while (remaining > 0);

    // resetting ball, bar and playing field
	ClearScreen();
    def_values();
}

void wait_for_start()
{
    // waiting behaviour until the user presses either w/s
    while (1)
    {
        int remaining = 0;
        do
        {
            unsigned long long out = ReadUart();
            if (!(out & 0x8000))
            {
                // not valid - abort reading
                break;
            }
            remaining = (out & 0xFF0000) >> 16;
            // get the character
            char c = out & 0xFF;
            // w or s -> start the game
            if (c == 0x77 || c == 0x73)
            {
                currentState = Running;
                return;
            }
        } while (remaining > 0);
    }
}

int main(int argc, char *argv[])
{
    ClearScreen();
    // HINT: This loop allows the user to restart the game after loosing/winning the previous game
    while (1)
    {
        wait_for_start();
        play();
        reset();
        if (currentState == Exit)
        {
            break;
        }
    }
    return 0;
}
