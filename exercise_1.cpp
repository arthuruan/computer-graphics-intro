// Autor: Arthur Ruan Bizerra Florentino
// Matricula: 2018083114

// g++ openGL_1.cpp -o 1_opengl -lGL -lGLU -lglut

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

GLint WINDOW_WIDTH = 500, WINDOW_HEIGH = 500;

int clickCount = 0;

struct coordinates {
    GLint x;
    GLint y;
};

coordinates initialCoordinates;
coordinates finalCoordinates;

enum menuOptions {
    Clear = 0,
    Point = 1,
    Line = 2,
    Rectangle = 3,
    Circle = 4
};

menuOptions menuSelected;

void initializeGlut(int argc, char *argv[]){
    // inicializa
    glutInit(&argc, argv);

    // define qual sera o modo inicial de display
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // definir a posição inicial da janela, passando canto superior esquerdo
    glutInitWindowPosition(0, 0);

    // define largura e altura da janela
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGH);

    // criar janela
    glutCreateWindow("Hello World OpenGL!!");
}

void initializeGl() {
    // define a cor de fundo
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // matrix que usaremos para operações
    glMatrixMode(GL_MODELVIEW);

    // esquerdo, direito, baixo, cima
    gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGH, 0);

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 1.0f);
}

void drawPoint(GLfloat x, GLfloat y) {
    glPointSize(5.0f);
    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
    glFlush();
}

void drawLine(coordinates initCoor, coordinates finalCoor) {
    glLineWidth(5.0f);
    glBegin(GL_LINES);
        glVertex2f(initCoor.x, initCoor.y);
        glVertex2f(finalCoor.x, finalCoor.y);
    glEnd();

    glFlush();
}

void drawReactangle(coordinates initCoor, coordinates finalCoor) {
    glBegin(GL_QUADS);
        glVertex2f(initCoor.x, initCoor.y);
        glVertex2f(initCoor.x, finalCoor.y);
        glVertex2f(finalCoor.x, finalCoor.y);
        glVertex2f(finalCoor.x, initCoor.y);
    glEnd();

    glFlush();
}

void drawCircle(coordinates initCoor, coordinates finalCoor) {
    GLint xDelta = finalCoor.x - initCoor.x;
    GLint yDelta = finalCoor.y - initCoor.y;

    GLint x = initCoor.x + (xDelta / 2);
    GLint y = finalCoor.y - (yDelta / 2);
    float r = xDelta / 2;
    int numSegments = 20;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < numSegments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);

        float cx = r * cosf(theta);
        float cy = r * sinf(theta);

        glVertex2f(cx + x, cy + y);
    }
    glEnd();

    glFlush();
}

void doubleClickAction(
    GLint x, 
    GLint y,
    void (*callback)(coordinates initCoor, coordinates finalCoor)
) {
    clickCount ++;
    switch (clickCount) {
        case 1:
            initialCoordinates.x = x;
            initialCoordinates.y = y;
            break;
        case 2:
            finalCoordinates.x = x;
            finalCoordinates.y = y;
            callback(initialCoordinates, finalCoordinates);
            clickCount = 0;
            break;
        default:
            clickCount = 0;
            break;
    }
}

void handleMouse(GLint button, GLint action, GLint x, GLint y) {
    if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
        switch (menuSelected) {
            case Point:
                drawPoint(x, y);
                break;
            case Line:
                doubleClickAction(x, y, drawLine);
                break;
            case Rectangle:
                doubleClickAction(x, y, drawReactangle);
                break;
            case Circle:
                doubleClickAction(x, y, drawCircle);
                break;
        }
    }
}

void handleMenu(GLint menuIndex) {
    menuOptions normalizedMenuIdx = menuOptions(menuIndex);
    
    if (normalizedMenuIdx == Clear) {
        glClear(GL_COLOR_BUFFER_BIT);
    } else {
        menuSelected = normalizedMenuIdx;
    }

    glutPostRedisplay();
}

void createMenu() {
    GLint sub1 = glutCreateMenu(handleMenu);
    glutAddMenuEntry("Ponto", Point);
    glutAddMenuEntry("Linha", Line);
    glutAddMenuEntry("Retangulo", Rectangle);
    glutAddMenuEntry("Circulo", Circle);
    glutCreateMenu(handleMenu);
    glutAddSubMenu("Desenhar", sub1);
    
    glutAddMenuEntry("Limpar", Clear);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void handleKeyboard(GLubyte key, GLint x, GLint y) {
    if (key == 'r') {
        glColor3f(1.0f, 0.0f, 0.0f);
    }
    if (key == 'g') {
        glColor3f(0.0f, 1.0f, 0.0f);
    }
    if (key == 'b') {
        glColor3f(0.0f, 0.0f, 1.0f);
    }
}

void handleMotion(GLint x, GLint y) {
    drawPoint(x, y);
}

// void draw() {
//     glFlush();
// }

int main(int argc, char *argv[]) {
    initializeGlut(argc, argv);
    initializeGl();

    // glutDisplayFunc(draw);

    glutMouseFunc(handleMouse);
    glutKeyboardFunc(handleKeyboard);
    glutMotionFunc(handleMotion);

    createMenu();

    glutMainLoop();
    return EXIT_SUCCESS;
}