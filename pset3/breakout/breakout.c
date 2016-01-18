//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// paddle geometry
#define PADDLEY 550
#define PADDLEWIDTH 75
#define PADDLEHEIGHT 15

typedef struct vector
{
    double x;
    double y;
} vector;

static int hitCount = 0;

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);
int moveBall(GWindow window, GOval ball, GRect paddle, GLabel label, vector *velocity);
void movePaddle(GWindow window, GRect paddle, GEvent event);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);
    vector velocity = { 2.0, 2.0 };

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);
    updateScoreboard(window, label, 0);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // initial points
    int points = 0;

    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        int status = moveBall(window, ball, paddle, label, &velocity);

        if (status == 1)
        {
            points += 1;
            updateScoreboard(window, label, points);
        }
        else if (status == -1)
        {
            waitForClick();

            // New ball, but velocity
            // (and direction) maintained.
            removeGWindow(window, ball);
            ball = initBall(window);

            continue;
        }

        GEvent event = getNextEvent(MOUSE_EVENT);

        if (event != NULL && getEventType(event) == MOUSE_MOVED)
        {
            movePaddle(window, paddle, event);
        }

        pause(5); // 200 fps
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

void movePaddle(GWindow window, GRect paddle, GEvent event)
{
    int x = getX(event) - getWidth(paddle) / 2;
    int y = getY(paddle);

    if (x < 0)
    {
        x = 0;
    }
    else if (x > WIDTH - getWidth(paddle))
    {
        x = WIDTH - getWidth(paddle);
    }

    setLocation(paddle, x, y);
}

int moveBall(GWindow window,
              GOval ball,
              GRect paddle,
              GLabel label,
              vector *velocity)
{
    int status = 0;

    int x = getX(ball);
    int y = getY(ball);

    if (x + RADIUS * 2 > WIDTH)
    {
        velocity->x *= -1;
    }
    else if (x < 0)
    {
        velocity->x *= -1;
    }

    if (y + RADIUS * 2 > HEIGHT)
    {
        status = -1;
        return status;
    }
    else if (y < 0)
    {
        velocity->y *= -1;
    }

    GObject collider = detectCollision(window, ball);

    if (collider != NULL)
    {
        char* type = getType(paddle);
        bool isPaddle = collider == paddle;
        bool isBrick = collider != label && strcmp(type, "GRect") == 0;

        if (isPaddle)       // Hitting the paddle..
        {
            double positionOnPaddle = x + RADIUS - getX(paddle);
            bool isWithinPaddle = positionOnPaddle > 0 && positionOnPaddle < getWidth(paddle)
                                ? true : false;
            bool isAbovePaddle = (y + RADIUS * 2 > PADDLEY);
            bool isMovingDown = (velocity->y > 0);

            if (isWithinPaddle && isAbovePaddle && isMovingDown)
            {
                hitCount++;  // For debugging purposes..

                int maxSpeed = 5;

                velocity->y *= -1.01; // Increase speed

                if (velocity->y > maxSpeed)  velocity->y = maxSpeed;
                if (velocity->y < -maxSpeed) velocity->y = -maxSpeed;

                printf("%i: ", hitCount);
                printf("Paddle { %f, %f }\n", getX(paddle), getX(paddle) + getWidth(paddle));
                printf("   Ball { %f }\n", x + (double) RADIUS);
                printf("   Relative Position { %f }\n", positionOnPaddle);
                printf("   Velocity { %f, %f }\n", velocity->x, velocity->y);
            }
        }
        else if (isBrick)                           // Hitting a brick..
        {
            if (velocity->y < 0)                    // ..and moving upwards
            {
                velocity->y *= -1;                  // Switch direction..
                status = 1;
                removeGWindow(window, collider);    // ..and remove brick
            }
        }
    }

    setLocation(ball, x + velocity->x, y + velocity->y);

    return status;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    int spacing = 4;

    int width = WIDTH / COLS - spacing;
    int height = 10;
    int yOffset = 75;

    char* colors[] = {
        "PINK",
        "YELLOW",
        "MAGENTA",
        "ORANGE",
        "CYAN",
        "LIGHT_GRAY",
        "DARK_GRAY",
        "GRAY",
        "WHITE",
        "BLUE",
        "GREEN",
        "RED",
    };

    int colorsCount = sizeof(colors)/sizeof(colors[0]);

    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            int x = width * col + spacing * col + spacing / 2;
            int y = height * row + spacing * row + spacing / 2;

            // Pull everything down
            y += yOffset;

            GRect brick = newGRect(x, y, width, height);
            setColor(brick, colors[row % colorsCount]);
            setFilled(brick, true);
            add(window, brick);
        }
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    int x = WIDTH / 2 - RADIUS;
    int y = HEIGHT / 2 - RADIUS;
    
    GOval ball = newGOval(x, y, RADIUS * 2, RADIUS * 2);
    setColor(ball, "BLACK");
    setFilled(ball, true);
    add(window, ball);

    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect paddle = newGRect(WIDTH / 2 - PADDLEWIDTH / 2,
                            PADDLEY,
                            PADDLEWIDTH, PADDLEHEIGHT);

    setColor(paddle, "BLACK");
    setFilled(paddle, true);
    add(window, paddle);

    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel("0");
    setFont(label, "courier new-*-10");
    add(window, label);
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (WIDTH - getWidth(label)) / 2;
    double y = (HEIGHT - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
