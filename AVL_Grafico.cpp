// Graficar el AVL con insert y remove

#include <GL/glut.h>
#include <iostream>
#include <cmath>

struct Nodo 
{
    int valor, altura;
    Nodo* izquierdo;
    Nodo* derecho;
    Nodo(int val) : valor(val), altura(1), izquierdo(nullptr), derecho(nullptr) {}
};

class ArbolAVL 
{
private:
    Nodo* raiz;

    int obtenerAltura(Nodo* n) 
    {
        return n ? n->altura : 0;
    }

    int obtenerFactorEquilibrio(Nodo* n) 
    {
        return n ? obtenerAltura(n->izquierdo) - obtenerAltura(n->derecho) : 0;
    }

    void actualizarAltura(Nodo* n) 
    {
        n->altura = 1 + std::max(obtenerAltura(n->izquierdo), obtenerAltura(n->derecho));
    }

    Nodo* rotarDerecha(Nodo* y) 
    {
        Nodo* x = y->izquierdo;
        Nodo* T = x->derecho;
        x->derecho = y;
        y->izquierdo = T;
        actualizarAltura(y);
        actualizarAltura(x);
        return x;
    }

    Nodo* rotarIzquierda(Nodo* x) 
    {
        Nodo* y = x->derecho;
        Nodo* T = y->izquierdo;
        y->izquierdo = x;
        x->derecho = T;
        actualizarAltura(x);
        actualizarAltura(y);
        return y;
    }

    Nodo* equilibrar(Nodo* n) 
    {
        actualizarAltura(n);
        int factorEquilibrio = obtenerFactorEquilibrio(n);

        if (factorEquilibrio > 1) {
            if (obtenerFactorEquilibrio(n->izquierdo) < 0)
                n->izquierdo = rotarIzquierda(n->izquierdo);
            return rotarDerecha(n);
        }
        if (factorEquilibrio < -1) {
            if (obtenerFactorEquilibrio(n->derecho) > 0)
                n->derecho = rotarDerecha(n->derecho);
            return rotarIzquierda(n);
        }
        return n;
    }

    Nodo* insertar(Nodo* n, int val) 
    {
        if (!n) return new Nodo(val);
        if (val < n->valor)
            n->izquierdo = insertar(n->izquierdo, val);
        else if (val > n->valor)
            n->derecho = insertar(n->derecho, val);
        else
            return n;
        return equilibrar(n);
    }

    Nodo* encontrarMinimo(Nodo* n) 
    {
        return n->izquierdo ? encontrarMinimo(n->izquierdo) : n;
    }

    Nodo* eliminar(Nodo* n, int val) 
    {
        if (!n) return nullptr;
        if (val < n->valor) {
            n->izquierdo = eliminar(n->izquierdo, val);
        }
        else if (val > n->valor) {
            n->derecho = eliminar(n->derecho, val);
        }
        else {
            if (!n->izquierdo || !n->derecho) {
                Nodo* temp = n->izquierdo ? n->izquierdo : n->derecho;
                delete n;
                return temp;
            }
            else {
                Nodo* nodoMinimo = encontrarMinimo(n->derecho);
                n->valor = nodoMinimo->valor;
                n->derecho = eliminar(n->derecho, nodoMinimo->valor);
            }
        }
        return equilibrar(n);
    }

    void dibujarArbol(Nodo* n, float x, float y, float offsetX, float offsetY) 
    {
        if (!n) return;

        // Dibujar nodo
        glColor3f(0.8f, 0.6f, 0.2f); // Color naranja para los nodos
        dibujarCirculo(x, y, 0.05f);

        // Dibujar valor
        glColor3f(0.0f, 0.0f, 0.0f); // Color negro para el texto
        glRasterPos2f(x - 0.02f, y);
        char texto[10];
        sprintf_s(texto, sizeof(texto), "%d", n->valor); // Usar sprintf_s para evitar errores
        for (char* c = texto; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

        // Dibujar l�neas de conexi�n y sub�rboles
        if (n->izquierdo) 
        {
            glColor3f(0.0f, 0.0f, 0.5f); // Color azul oscuro para las l�neas
            glBegin(GL_LINES);
            glVertex2f(x, y);
            glVertex2f(x - offsetX, y - offsetY);
            glEnd();
            dibujarArbol(n->izquierdo, x - offsetX, y - offsetY, offsetX * 0.7f, offsetY);
        }
        if (n->derecho) 
        {
            glColor3f(0.0f, 0.0f, 0.5f);
            glBegin(GL_LINES);
            glVertex2f(x, y);
            glVertex2f(x + offsetX, y - offsetY);
            glEnd();
            dibujarArbol(n->derecho, x + offsetX, y - offsetY, offsetX * 0.7f, offsetY);
        }
    }

    void dibujarCirculo(float x, float y, float radio) 
    {
        int segmentos = 100;
        float anguloPaso = 360.0f / segmentos;

        glBegin(GL_TRIANGLE_FAN);
        for (int i = 0; i <= segmentos; i++) 
        {
            float angulo = anguloPaso * i;
            float radian = angulo * (3.14159265f / 180.0f);
            glVertex2f(x + cos(radian) * radio, y + sin(radian) * radio);
        }
        glEnd();
    }

public:
    ArbolAVL() : raiz(nullptr) {}

    void insertar(int val) 
    {
        raiz = insertar(raiz, val);
    }

    void eliminar(int val) 
    {
        raiz = eliminar(raiz, val);
    }

    void dibujar() 
    {
        dibujarArbol(raiz, 0.0f, 0.8f, 0.4f, 0.2f);
    }
};

ArbolAVL arbol;

void mostrar() 
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    arbol.dibujar();

    glFlush();
}

int main(int argc, char** argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("�rbol AVL");

    glClearColor(1.0, 1.0, 1.0, 1.0);
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

    arbol.insertar(40);
    arbol.insertar(20);
    arbol.insertar(50);
    arbol.insertar(10);
    arbol.insertar(30);
    arbol.insertar(5);
    arbol.insertar(25);
    arbol.insertar(27);

    
    /*arbol.eliminar(10);*/

    glutDisplayFunc(mostrar);
    glutMainLoop();

    return 0;
}