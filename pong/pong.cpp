#include <GL/glut.h>
#include <string>
#include <sstream>
#include <cmath>

/** 

1 - check
2 - check
3 - check
4 - check
5 - check
6 - check
7 - check
8 - 
9 - check

**/

// Dimensões da janela
int width = 800;
int heigh = 600;

struct ICoordinates {
    GLfloat x;
    GLfloat y;
};

struct IKeyBinding {
    char key;
    int specialKey;
    bool pressed;
};

struct IAction {
    IKeyBinding up;
    IKeyBinding down;
};

struct IPlayer {
    ICoordinates position;
    IAction action;
    bool scored;
    int score;
};

// Jogadores
IPlayer player1 = {
    { -0.9, 0.0 },
    {
        { 'w', 0, false },
        { 's', 0, false }
    },
    false,
    0
};

IPlayer player2 = {
    { 0.9, 0.0 },
    {
        { ' ', GLUT_KEY_UP, false },
        { ' ', GLUT_KEY_DOWN, false }
    },
    false,
    0
};

// Velocidades dos jogadores
float player_speed = 0.02;

struct IBall {
    ICoordinates position;
    ICoordinates speed;
    float radius;
};

float default_ball_speed = 0.01;
float speed_increment = 0.002;

// Bola
IBall ball = {
    { 0.0, 0.0 },
    { default_ball_speed, default_ball_speed },
    0.02,
};

bool paused = false;
bool endGame = false;
int winnerScore = 15;

void drawText(float x, float y, std::string texto) {
    glRasterPos2f(x, y);
    for (char c : texto) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
}

void drawPlayer(float x, float y) {
    glBegin(GL_QUADS);
    glVertex2f(x - 0.01, y - 0.1);
    glVertex2f(x + 0.01, y - 0.1);
    glVertex2f(x + 0.01, y + 0.1);
    glVertex2f(x - 0.01, y + 0.1);
    glEnd();
}

void drawBall(float x, float y, float raio) {
    int num_segments = 100;
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float cx = raio * cosf(theta);
        float cy = raio * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

void drawEndGameText(std::string winner_string) {
    std::string restart = "Restart [r]";
    std::string end = "End [ESC]";
    drawText(-0.022 * winner_string.length() / 2, 0.5, winner_string);
    drawText(-0.022 * restart.length() / 2, -0.5, restart);
    drawText(-0.022 * end.length() / 2, -0.6, end);
}

void verifyWinner() {
	if (player1.score == winnerScore) {
		std::string winner_string = "Left Player is the winner!";
        drawEndGameText(winner_string);
		paused = true;
	} else if (player2.score == winnerScore) {
		std::string winner_string = "Right Player is the winner!";
        drawEndGameText(winner_string);
		paused = true;
	}
}

void setBallPosition(IPlayer player, bool isLeft = true) {
    ball.position.x = player.position.x  + (isLeft ? 0.022 : -0.022);
    ball.position.y = player.position.y;
}

void update(int valor) {
    // Verifica se o jogo está pausado
    if (paused) {
        glutPostRedisplay();
        glutTimerFunc(15, update, 0);
        return;
    }

    // Atualiza a posição dos jogadores
    if (player1.action.up.pressed) {
        player1.position.y += player_speed;
    } else if (player1.action.down.pressed) {
        player1.position.y -= player_speed;
    }

    if (player2.action.up.pressed) {
        player2.position.y += player_speed;
    } else if (player2.action.down.pressed) {
        player2.position.y -= player_speed;
    }

    // Limita as posições dos jogadores
    if (player1.position.y > 1.0 - 0.1) {
        player1.position.y = 1.0 - 0.1;
    }
    if (player1.position.y < -1.0 + 0.1) {
        player1.position.y = -1.0 + 0.1;
    }

    if (player2.position.y > 1.0 - 0.1) {
        player2.position.y = 1.0 - 0.1;
    }
    if (player2.position.y < -1.0 + 0.1) {
        player2.position.y = -1.0 + 0.1;
    }

    // Verifica se algum jogador marcou ponto
    if (player1.scored) {
        glutPostRedisplay();
        glutTimerFunc(15, update, 0);
        setBallPosition(player1);
        return;
    }
    if (player2.scored) {
        glutPostRedisplay();
        glutTimerFunc(15, update, 0);
        setBallPosition(player2, false);
        return;
    }


    // Atualiza a posição da bola
    ball.position.x += ball.speed.x;
    ball.position.y += ball.speed.y;

    // Verifica colisões da bola com os jogadores
    if (ball.position.x - ball.radius < player1.position.x + 0.01 &&
        ball.position.x + ball.radius > player1.position.x - 0.01 &&
        ball.position.y - ball.radius < player1.position.y + 0.1 &&
        ball.position.y + ball.radius > player1.position.y - 0.1) {
        ball.speed.x = -ball.speed.x;
        ball.speed.x += (ball.speed.x > 0) ? speed_increment : -speed_increment;
    }

    if (ball.position.x - ball.radius < player2.position.x + 0.01 &&
        ball.position.x + ball.radius > player2.position.x - 0.01 &&
        ball.position.y - ball.radius < player2.position.y + 0.1 &&
        ball.position.y + ball.radius > player2.position.y - 0.1) {
        ball.speed.x = -ball.speed.x;
        ball.speed.x += (ball.speed.x > 0) ? speed_increment : -speed_increment;
    }

    // Verifica colisões da bola com as paredes
    if (ball.position.x + ball.radius > 1.0) {
        player1.scored = true;
        player1.score++;
        setBallPosition(player1);
        ball.speed.x = default_ball_speed;
        ball.speed.y = default_ball_speed;
    } else if (ball.position.x - ball.radius < -1.0) {
        player2.scored = true;
        setBallPosition(player2, false);
        ball.speed.x = default_ball_speed;
        ball.speed.y = default_ball_speed;
    }

    // Verifica colisões da bola com as paredes superiores e inferiores
    if (ball.position.y + ball.radius > 1.0 || ball.position.y - ball.radius < -1.0) {
        ball.speed.y = -ball.speed.y;
    }

    glutPostRedisplay();
    glutTimerFunc(15, update, 0);
}

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenha os jogadores
    glColor3f(1.0, 1.0, 1.0);
    drawPlayer(player1.position.x, player1.position.y);
    drawPlayer(player2.position.x, player2.position.y);

    // Desenha a bola
    glColor3f(1.0, 1.0, 1.0);
    drawBall(ball.position.x, ball.position.y, ball.radius);

    // Desenha o placar
    glColor3f(1.0, 1.0, 1.0);
    drawText(-0.06, 0.85, std::to_string(player1.score) + " - " + std::to_string(player2.score));

    // Verifica se algum jogador marcou ponto
    verifyWinner();

    // Desenha pause
    if (paused) {
        std::string pauseString = "PAUSED [SPACE]";
        glColor3f(1.0, 1.0, 1.0);
        drawText(-0.022 * pauseString.length() / 2, 0.2, pauseString);
    }
    glutSwapBuffers();
}

void handleKeyboardFunc(unsigned char key, int x, int y) {
    if (key == player1.action.up.key) {
        player1.action.up.pressed = true;
    } else if (key == player1.action.down.key) {
        player1.action.down.pressed = true;
    } else if (key == ' ') {
        paused = !paused;
    } else if (key == 13 && player1.scored) {
        player1.scored = false;
        // Talvez adicionar um fator de aleatoriedade na velocidade da bola
        ball.speed.x = default_ball_speed;
        ball.speed.y = default_ball_speed;
    } else if (key == 13 && player2.scored) {
        player2.scored = false;
        ball.speed.x = -default_ball_speed;
        ball.speed.y = -default_ball_speed;
    }
    
    
    else if (key == 27 && paused && (player1.score == winnerScore || player2.score == winnerScore)) {
        printf("End Game\n");
        glutDestroyWindow(glutGetWindow());
        exit(0);
    } else if (key == 'r') {
        printf("Restart Game\n");
        player1.score = 0;
        player2.score = 0;
        paused = true;
        player1.position.y = 0.0;
        player2.position.y = 0.0;
        ball.position.x = 0.0;
        ball.position.y = 0.0;
        ball.speed.x = default_ball_speed;
        ball.speed.y = default_ball_speed;
    }
}

void handleKeyboardUpFunc(unsigned char key, int x, int y) {
    if (key == player1.action.up.key) {
        player1.action.up.pressed = false;
    } else if (key == player1.action.down.key) {
        player1.action.down.pressed = false;
    }
}

void handleSpecialKeyboardFunc(int key, int x, int y) {
    if (key == player2.action.up.specialKey) {
        player2.action.up.pressed = true;
    } else if (key == player2.action.down.specialKey) {
        player2.action.down.pressed = true;
    } 
}

void handleSpecialKeyboardUpFunc(int key, int x, int y) {
    if (key == player2.action.up.specialKey) {
        player2.action.up.pressed = false;
    } else if (key == player2.action.down.specialKey) {
        player2.action.down.pressed  = false;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, heigh);
    glutCreateWindow("Pong");
    glutDisplayFunc(drawScene);
    glutTimerFunc(0, update, 0);
    glutKeyboardFunc(handleKeyboardFunc);
    glutKeyboardUpFunc(handleKeyboardUpFunc);
    glutSpecialFunc(handleSpecialKeyboardFunc);
    glutSpecialUpFunc(handleSpecialKeyboardUpFunc);
    glutMainLoop();
    return 0;
}
