// Autor: Arthur Ruan Bizerra Florentino
// Autor: Jorge Luiz Pereira Da Silva Filho

// g++ main.cpp -o main -lGL -lGLU -lglut

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <iostream>

using namespace std;

GLint WINDOW_WIDTH = 500, WINDOW_HEIGHT = 500;

int count = 0;

struct point {
    GLint x;
    GLint y;
};

struct VERTICE {
    int x;
    int y;
};

struct OBJETO {
    VERTICE *vertices;
    int num_vertices;
};


OBJETO *objeto;
int tx = 0;
int ty = 0;
double angulo = 0;
int ex = 1;
int ey = 1;

OBJETO* criar_objeto() {
    OBJETO *obj = (OBJETO*)malloc(sizeof(OBJETO));
    obj->num_vertices = 3;
    obj->vertices = (VERTICE*)malloc(sizeof(VERTICE)*obj->num_vertices);
    obj->vertices[0].x = 300;
    obj->vertices[0].y = 200;

    obj->vertices[1].x = 350;
    obj->vertices[1].y = 275;

    obj->vertices[2].x = 250;
    obj->vertices[2].y = 275;

    return obj;
}

point initialpose;
point finalpose;

VERTICE translacao2d(VERTICE v, int x, int y) {
    VERTICE v_transladado;
    int sum = 0;
    int matriz_translacao[3][3] = {
        1, 0, x,
        0, 1, y,
        0, 0, 1
    };
    int coord_homogeneas[3] = {v.x, v.y, 1};
    int coord_finais[3] = {0, 0, 0};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            sum += matriz_translacao[i][j] * coord_homogeneas[j];
        }
        coord_finais[i] = sum;
        sum = 0; 
    }
    v_transladado.x = coord_finais[0];
    v_transladado.y = coord_finais[1];
    
    return v_transladado;
}

VERTICE rotacao2d(VERTICE v, int ang) {
    VERTICE v_rotacionado;
    double s = sin(((ang)*M_PI)/180.0);
    double c = cos(((ang)*M_PI)/180.0);
    double sum;
    double matriz_rotacao[3][3] = {
        c, -s, 0,
        s, c, 0,
        0, 0, 1
    };

    double coord_homogeneas[3] = {(double)v.x, (double)v.y, 1};
    double coord_finais[3] = {0, 0, 0};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            sum += matriz_rotacao[i][j] * coord_homogeneas[j];
        }
        coord_finais[i] = sum;
        sum = 0;
    }
    v_rotacionado.x = (int)coord_finais[0];
    v_rotacionado.y = (int)coord_finais[1];
    
    return v_rotacionado;
}

VERTICE escala2d(VERTICE v, int x, int y) {
     VERTICE v_escala;
    int sum = 0;
    int matriz_escala[3][3] = {
        x, 0, 0,
        0, y, 0,
        0, 0, 1
    };
    int coord_homogeneas[3] = {v.x, v.y, 1};
    int coord_finais[3] = {0, 0, 0};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            sum += matriz_escala[i][j] * coord_homogeneas[j];
        }
        coord_finais[i] = sum;
        sum = 0; 
    }
    v_escala.x = coord_finais[0];
    v_escala.y = coord_finais[1];
    
    return v_escala;
}

VERTICE calcularCentroide(OBJETO *obj) {
    VERTICE centroide;
    int sum_x = 0;
    int sum_y = 0;
    for (int i = 0; i < obj->num_vertices; i++) {
        sum_x += obj->vertices[i].x;
        sum_y += obj->vertices[i].y;
    }
    centroide.x = sum_x / obj->num_vertices;
    centroide.y = sum_y / obj->num_vertices;
    return centroide;
}

void desenharObjeto() {
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);

    VERTICE v;
    VERTICE centroide = calcularCentroide(objeto);
    for (int i = 0; i < objeto->num_vertices; i++) {
        v = translacao2d(objeto->vertices[i], -centroide.x, -centroide.y);
        v = rotacao2d(v, angulo);
        v = escala2d(v, ex, ey);
        v = translacao2d(v, centroide.x, centroide.y);
        v = translacao2d(v, tx, ty);
        glVertex2i(v.x, v.y);
    }

    glEnd();
}

void initializeGlut(int argc, char *argv[]) {
    //Inicializa a biblioteca GLUT e negocia uma 
    //seção com o sistema de janelas
    glutInit(&argc, argv);

    //Define qual será o modo inicial de display
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    //Definir a posição inicial da janela, passando 
    //canto superior esquerdp
    glutInitWindowPosition(100,100);

    //Define a largura e altura da janela
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    //Cria a janela, passando o titulo
    glutCreateWindow("Meu primeiro programa OpenGL");

}

void initialize() {
    //Define a cor de fundo
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    //Matriz que usaremos para opperacoes
    glMatrixMode(GL_MODELVIEW);

    //erquerdo, direito, baixo, cima
    gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    objeto = criar_objeto();
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    desenharObjeto();

    glFlush();
}

void handle_keyboard(GLubyte key, GLint x, GLint y) {
    switch(key) {
        case 'w':
            ty -= 2;
            break;
        case 's':
            ty += 2;
            break;
        case 'a':
            tx -= 2;
            break;
        case 'd':
            tx += 2;
            break;
        case '4':
            angulo --;
            break;
        case '6':
            angulo ++;
            break;
    }

    draw();
    glFlush();
}

void handle_special(GLint key, GLint x, GLint y) {
    switch (key) {
        case GLUT_KEY_UP:
            ey ++;
            break;
        case GLUT_KEY_DOWN:
            if (ey > 1) {
                ey --;
            }
            break;
        case GLUT_KEY_LEFT:
            if (ex > 1) {
                ex --;
            }
            break;
        case GLUT_KEY_RIGHT:
            ex ++;
            break;
    }

    draw();
    glFlush();
}

int main(int argc, char *argv[]) {   
    initializeGlut(argc, argv);
    initialize();
    glutDisplayFunc(draw);
    glutKeyboardFunc(handle_keyboard);
    glutSpecialFunc(handle_special);
    glutMainLoop();
    
    return EXIT_SUCCESS;
}
