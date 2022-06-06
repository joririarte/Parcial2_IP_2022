//Counter en consola, juego de dos jugadores.
#include <iostream>
#include <Windows.h>
#include <ctime>
#include <conio.h>

using namespace std;
const int filas = 21, columnas = 29;
char mapa[filas][columnas];

struct APersonaje {
	string bando = " ";
	char repVisual = ' ', direccion = ' ';
	int posX = 0, posY = 0, vida, puntaje;
	bool creado = false;
};
APersonaje jugador, enemigo, bala, bala1;//actores

string elegirBando();
void crearActor(APersonaje& pers, string bando, char visual);
void reiniciarActor(APersonaje& pers);
void jugar(string bando);
void disparar(APersonaje personaje, APersonaje& proyectil, char direccion);
void addJugador(APersonaje& personaje);
void mover(APersonaje& personaje, char dir);
void crearMapa();
void mostrarMapa();
void recibirDmg(APersonaje& personaje, APersonaje& proyectil);
void accion(APersonaje& personaje, APersonaje& proyectil);
void ClearScreen();
void Respawn(APersonaje& personaje);

int main()
{
	srand(time(NULL));
	bool loop = true;
	string bando;
	do {
		system("cls");
		int opciones;
		cout << " ----> Counter en Consola <----\n\n";
		cout << " Bienvenidos a Counter en Consola\n\n";
		cout << " Menu:\n 1. Jugar\n 2. Instrucciones\n 3. Clasificaciones\n\n";
		cout << " Opcion: ";
		cin >> opciones;
		switch (opciones) {
		case 1://Jugar
			system("cls");
			bando = elegirBando();
			loop = false;
			break;
		case 2://instrucciones
			system("cls");
			cout << " ----> INSTRUCCIONES <----\n\n";
			cout << " Este juego es un shooter de dos jugadores\n\n";
			cout << " Muevete por el campo, dispara a tu enemigo y esquiva sus balas\n\n";
			cout << " CONTROLES:\n\n";
			cout << " --> JUGADOR 1 <--\n\n";
			cout << " W: mover arriba\n";
			cout << " S: mover abajo\n";
			cout << " A: mover izquierda\n";
			cout << " D: mover derecha\n\n";
			cout << " C: Disparar\n\n";
			cout << " --> JUGADOR 2 <--\n\n";
			cout << " 8: mover arriba\n";
			cout << " 5: mover abajo\n";
			cout << " 4: mover izquierda\n";
			cout << " 6: mover derecha\n\n";
			cout << " 0: Disparar\n\n";
			cout << " <---Volver al menu\n";
			system("pause");
			break;
		case 3://Clasificaciones
			system("cls");
			cout << " ----> CLASIFICACIONES <----\n\n";
			cout << " <---Volver al menu\n";
			system("pause");
			break;
		default:
			cout << "\n\n OPCION INVALIDA ELIGE BIEN!\n\n";
			system("pause");
			break;
		}
	} while (loop);
	jugar(bando);
	return 0;
}

string elegirBando() {
	int opciones;
	char confirmacion;
	do {
		system("cls");
		cout << " ----> ELEGIR BANDO <----\n\n";
		cout << " Elige tu bando (ingresa 1 o 2):\n 1. Counter\n 2. Terrorist\n\n";
		cout << " Opcion: ";
		cin >> opciones;
		if (opciones == 1) {
			cout << " Elegiste: Counter\n\n";
			cout << " Confirma tu eleccion apretando Y\n";
			cout << " Si te equivocaste presiona cualquier otra letra: ";
			cin >> confirmacion;
			if (toupper(confirmacion) == 'Y') {
				crearActor(jugador, "Counter", 'C');
				crearActor(enemigo, "Terrorist", 'T');
				return "Counter";
			}
		}
		else if (opciones == 2) {
			cout << " Elegiste: Terrorist\n\n";
			cout << " Confirma tu eleccion apretando Y\n";
			cout << " Si te equivocaste presiona cualquier otra letra: ";
			cin >> confirmacion;
			if (toupper(confirmacion) == 'Y') {
				crearActor(enemigo, "Counter", 'C');
				crearActor(jugador, "Terrorist", 'T');
				return "Terrorist";
			}
		}
		else {
			cout << "\n\n OPCION INVALIDA ELIGE BIEN!\n\n";
			system("pause");
		}
	} while (true);
}
void crearActor(APersonaje& pers, string bando, char visual) {
	pers.bando = bando;
	pers.repVisual = visual;
	pers.posX = 0;
	pers.posY = 0;
	pers.creado = true;
	pers.vida = 3;
}
void reiniciarActor(APersonaje& pers) {
	pers.creado = true;
	pers.vida = 3;
}
void jugar(string bando) {
	crearMapa();
	addJugador(jugador);
	addJugador(enemigo);
	char visual;
	do {
		//Respawnea al personaje que corresponda
		Respawn(jugador);
		Respawn(enemigo);
		//Reinicia la pantalla
		ClearScreen();
		//Deteccion de teclas del teclado
		if (_kbhit()) {
			char direccion = _getch();//obtiene la tecla.
			switch (toupper(direccion)) {//llama a la accion segun la tecla.
				case 'W'://llama al movimiento del jugador
				case 'A':
				case 'S':
				case 'D':
					mover(jugador, direccion);
					break;
				case '8'://llama a la movimiento del enemigo
				case '5':
				case '4':
				case '6':
					mover(enemigo, direccion);
					break;
				case 'C'://llama a disparar del jugador
					disparar(jugador, bala, jugador.direccion);
					addJugador(bala);
					break;
				case '0'://llama disparar del enemigo
					disparar(enemigo, bala1, enemigo.direccion);
					addJugador(bala1);
						break;
			}
		}
		accion(jugador, bala1);
		accion(enemigo, bala);
		mostrarMapa();
		cout << endl << "Vida " << enemigo.bando << ": " << enemigo.vida << " || Vida " << jugador.bando << ": " << jugador.vida << endl;
		Sleep(65);
	} while (true);
	//maneja todo el juego
	//movimiento, tiros, ganador, todo sale de aca
}
void accion(APersonaje& personaje, APersonaje& proyectil) {
	//mueve proyectiles
	//llama a recibir daño para verificar si en el frame se recibe daño o no
	//limpia el personaje que tenga vida menor estricto a 1 (0 o menos)
	if (proyectil.creado)
		mover(proyectil, proyectil.direccion);
	if (proyectil.creado)
		recibirDmg(personaje, proyectil);
	if (personaje.vida < 1) {
		mapa[personaje.posY][personaje.posX] = ' ';
		personaje.creado = false;
	}
}
void ClearScreen()
{
	//coloca el cursor de la consola en la posicion 0,0; arriba a la izquierda.
	//sirve para "limpiar" la pantalla
	//Elimina el flickering que hacia system("cls")
	COORD cursorPosition;
	cursorPosition.X = 0;
	cursorPosition.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}
void Respawn(APersonaje& personaje)
{
	//Respawnea el personaje si es que creado es false
	if (!personaje.creado) {
		reiniciarActor(personaje);
		addJugador(personaje);
	}
}
void disparar(APersonaje personaje, APersonaje& proyectil, char direccion) {
	//Crea un proyectil en la direccion que mira el jugador
	//se llamaria desde jugar
	proyectil.repVisual = '-';
	proyectil.bando = "bala";
	proyectil.direccion = direccion;
	proyectil.creado = true;
	switch (direccion) {
	case 'w':
		proyectil.posX = personaje.posX;
		proyectil.posY = personaje.posY - 1;
		proyectil.repVisual = '|';
		break;
	case 's':
		proyectil.posX = personaje.posX;
		proyectil.posY = personaje.posY + 1;
		proyectil.repVisual = '|';
		break;
	case 'a':
		proyectil.posX = personaje.posX - 1;
		proyectil.posY = personaje.posY;
		break;
	case 'd':
		proyectil.posX = personaje.posX + 1;
		proyectil.posY = personaje.posY;
		break;
	}
}
void addJugador(APersonaje& personaje) {
	//añade un jugador al mapa
	bool exito = false;
	do {
		if (personaje.bando != "bala") {//asigna posiciones rnd solo si el jugador no es una bala
			personaje.posX = rand() % 29;
			personaje.posY = rand() % 21;
		}
		if (mapa[personaje.posY][personaje.posX] == ' ') {//Coloca en el mapa solo si es un espacio vacio.
			mapa[personaje.posY][personaje.posX] = personaje.repVisual;
			exito = true;
		}
		else if (personaje.bando == "bala")//si el personaje es una bala siempre sale del ciclo, la haya colocado o no.
			exito = true;
	} while (!exito);//repite el ciclo hasta que el personaje se pueda colocar en la pantalla.
}
void mover(APersonaje& personaje, char dir) {
	//mueve el personaje jugable
	//se llamaria desde jugar
	int x = personaje.posX;//toma las posiciones iniciales del personaje
	int y = personaje.posY;
	switch (dir) {
	case 'W':
	case'w':
	case'8':
		if (mapa[personaje.posY - 1][personaje.posX] == ' ') {
			mapa[personaje.posY][personaje.posX] = ' ';
			personaje.posY--;
			mapa[personaje.posY][personaje.posX] = personaje.repVisual;
		}
		personaje.direccion = 'w';
		break;
	case 'S':
	case's':
	case'5':
		if (mapa[personaje.posY + 1][personaje.posX] == ' ') {
			mapa[personaje.posY][personaje.posX] = ' ';
			personaje.posY++;
			mapa[personaje.posY][personaje.posX] = personaje.repVisual;
		}
		personaje.direccion = 's';
		break;
	case 'A':
	case'a':
	case'4':
		if (mapa[personaje.posY][personaje.posX - 1] == ' ') {
			mapa[personaje.posY][personaje.posX] = ' ';
			personaje.posX--;
			mapa[personaje.posY][personaje.posX] = personaje.repVisual;
		}
		personaje.direccion = 'a';
		break;
	case 'D':
	case'd':
	case'6':
		if (mapa[personaje.posY][personaje.posX + 1] == ' ') {
			mapa[personaje.posY][personaje.posX] = ' ';
			personaje.posX++;
			mapa[personaje.posY][personaje.posX] = personaje.repVisual;
		}
		personaje.direccion = 'd';
		break;
	}
	//si las posiciones del personaje no sufrieron cambios pasando por el switch quiere decir que no se movio
	//por lo tanto está tocando una pared u otro personaje, entonces la bala se limpiará.
	if (personaje.bando == "bala" && x == personaje.posX && y == personaje.posY) {
		personaje.creado = false;
		mapa[personaje.posY][personaje.posX] = ' ';
	}
}
void crearMapa() {
	//crea el mapa donde se va a jugar
	//El mapa sera un arreglo bidimensional de caracteres.
	//se llamaria desde jugar
	for (int fil = 0; fil < filas; fil++) {
		for (int col = 0; col < columnas; col++) {
			if (fil == 0 || col == 0 || fil == filas - 1 || col == columnas - 1)
				mapa[fil][col] = '#';
			else if (fil == 1 && (col == 13 || col == 15 || col == 21 || col == 22))
				mapa[fil][col] = '#';
			else if (fil == 2 && (col == 5 || col == 8 || col > 12 && col < 16 || col == 21 || col == 22))
				mapa[fil][col] = '#';
			else if (fil == 3 && (col > 3 && col < 10 || col == 22 || col == 21 || col == 22))
				mapa[fil][col] = '#';
			else if (fil == 4 && (col == 4 || col > 5 && col < 8 || col == 9 || col == 10 || col == 21 || col == 22))
				mapa[fil][col] = '#';
			else if (fil == 5 && (col == 4 || col > 5 && col < 8 || col == 9 || col == 10))
				mapa[fil][col] = '#';
			else if (fil == 6 && (col > 3 && col < 10))
				mapa[fil][col] = '#';
			else if (fil == 7 && (col == 5 || col == 9 || col == 16 || col == 19 || col == 17 || col == 18))
				mapa[fil][col] = '#';
			else if (fil == 8 && (col == 6 || col == 7 || col > 15 && col < 20 || col == 24))
				mapa[fil][col] = '#';
			else if (fil == 9 && (col == 5 || col == 6 || col > 15 && col < 20 || col == 24))
				mapa[fil][col] = '#';
			else if (fil == 10 && (col == 4 || col == 6 || col > 15 && col < 20 || col == 24))
				mapa[fil][col] = '#';
			else if (fil == 11 && (col > 3 && col < 8 || col > 15 && col < 20 || col == 24))
				mapa[fil][col] = '#';
			else if (fil == 12 && (col == 4 || col == 6 || col == 12 || col == 24))
				mapa[fil][col] = '#';
			else if (fil == 13 && (col == 5 || col == 6 || col == 7 || col == 12 || col == 24))
				mapa[fil][col] = '#';
			else if (fil == 14 && (col == 6 || col == 12 || col == 20))
				mapa[fil][col] = '#';
			else if (fil == 15 && (col == 3 || col == 8 || col == 19 || col == 21 || col == 27 || col == 12))
				mapa[fil][col] = '#';
			else if (fil == 16 && (col == 3 || col == 7 || col == 8 || col == 9 || col > 11 && col < 16 || col == 19 || col > 24 && col < 28))
				mapa[fil][col] = '#';
			else if (fil == 17 && (col == 3 || col == 7 || col == 8 || col == 9 || col == 17 || col == 18 || col == 20 || col == 26 || col == 27))
				mapa[fil][col] = '#';
			else if (fil == 18 && (col == 8 || col == 17 || col == 19 || col == 27))
				mapa[fil][col] = '#';

			else
				mapa[fil][col] = ' ';

		}
	}
}
void mostrarMapa() {
	for (int fil = 0; fil < filas; fil++) {
		for (int col = 0; col < columnas; col++) {
			cout << mapa[fil][col] << "  ";
		}
		cout << endl;
	}
}
void recibirDmg(APersonaje& personaje, APersonaje& proyectil) {
	//recibe daño tiene problemas cuando la bala pasa por el costado, toma el daño igualmente.
	if (personaje.posX == proyectil.posX)
		if (personaje.posY == proyectil.posY - 1 || personaje.posY == proyectil.posY + 1)
			personaje.vida--;
	if (personaje.posY == proyectil.posY)
		if (personaje.posX == proyectil.posX - 1 || personaje.posX == proyectil.posX + 1)
			personaje.vida--;

}