//Counter en consola, juego de dos jugadores.
#include <iostream>
#include <Windows.h>
#include <ctime>
#include <conio.h>
#include <iomanip>

using namespace std;
struct APersonaje {
	string bando = " ", nombre="pichichu";
	char repVisual = ' ', direccion = ' ';
	int posX = 0, posY = 0, vida, puntaje, muertes=0,dado=2;
	bool creado = false;
};

//Variables globales
const int filas = 21, columnas = 29;
char mapa[filas][columnas];
int puntajes[10];
string jugadores[10] = { "noobmaster69","xXToretoXx","ApuNahasapee","santi.godoy","elChidoxD","Pipita123","Messi2022","[ARG]Rambo87","vegeta777","elKun9" };
APersonaje jugador1, jugador2, bala, bala1;//actores

//Funciones: DEFINICIONES
//Menús
void MenuInicio();
bool MenuPausa();
void Instrucciones();

//Inicializaciones
void crearMapa();
void elegirBando(APersonaje& jugador, APersonaje& enemigo);
void crearActor(APersonaje& pers, string bando, char visual);
void addJugador(APersonaje& personaje);
void Respawn(APersonaje& personaje);
void initPuntajes();
string crearNombres();

//Acciones
void disparar(APersonaje personaje, APersonaje& proyectil, char direccion);
void mover(APersonaje& personaje, char dir);
void accion(APersonaje& personaje, APersonaje& proyectil);

//Sistema
void ClearScreen();
void mostrarMapa();
void mostrarClasificaciones();
void OrdClasificaciones();
void addClasificacion(APersonaje player);
void mostrarStats(APersonaje jugador, APersonaje enemigo);

//Modo de Juego
void jugar(APersonaje& jugador, APersonaje& enemigo);
void sumarPuntos(APersonaje& player, APersonaje& enemy);
void recibirDmg(APersonaje& personaje, APersonaje& proyectil);


//Programa
int main()
{
	//Inicia la semilla para el random
	srand(time(NULL));
	//inicia los puntajes de la grilla de clasificaciones
	initPuntajes();
	//ordena los puntajes con los nombres
	OrdClasificaciones();
	//inicio del juego
	cout << "\n\n --->COUNTER EN CONSOLA <---\n\n\n";
	system("pause");
	do {
		//abre menu inicio
		MenuInicio();
		//mueve con WASD el jugador que le toca el dado = 1, es como el jugador 1, entonces inicia el juego con ese jugador en primer lugar
		if (jugador1.dado == 1)
			jugar(jugador1, jugador2);
		else if (jugador2.dado == 1)
			jugar(jugador2, jugador1);

		//termina el juego y muestra las stats
		system("cls");
		cout << "\n-----> Fin del Juego! <-----\n\n";
		mostrarStats(jugador1, jugador2);
		system("pause");
		cout << "\n\n Las puntuaciones pueden estar en el top 10!\n";
		cout << " Veamos si alguno logro posicionarse...\n\n";
		system("pause");
		//añade las clasificaciones a la grilla
		addClasificacion(jugador1);
		addClasificacion(jugador2);
		mostrarClasificaciones();

	} while (true);
	//termina el programa
	return 0;
}

//Funciones: DECLARACIONES

//Menús
void MenuInicio()
{
	bool loop = true;
	do {
		system("cls");
		int opciones,dado;
		cout << " ----> Counter en Consola <----\n\n";
		cout << " Bienvenidos a Counter en Consola\n\n";
		cout << "Ingresa un numero de opcion.\n\n";
		cout << " 1. Jugar\n";
		cout << " 2. Instrucciones\n";
		cout << " 3. Clasificaciones\n";
		cout << " 4. Salir del juego\n\n";
		cout << " Opcion: ";
		cin >> opciones;
		switch (opciones) {
		case 1://Jugar
			//Eleccion de nombres, alias
			system("cls");
			cout << "-------> GAMERTAG <------\n\n";
			cout << " Jugador 1 ";
			jugador1.nombre = crearNombres();
			cout << " \n Jugador 2 ";
			jugador2.nombre = crearNombres();
			cout << " \n\n Ahora un dado define quien elige el bando\n";
			Sleep(1000);
			//tirar dados para que jugador elige el bando
			dado = rand() % 6 + 1;
			//si el dado cae del 1 al 3 elige el jugador 2 y si cae del 4 al 6 el jugador 1
			if (dado >= 4) {
				cout << " \n Elige " << jugador1.nombre<<endl<<endl;
				system("pause");
				elegirBando(jugador1, jugador2);
				jugador1.dado = 1;
			}
			else {
				cout << " \n Elige " << jugador2.nombre<<endl<<endl;
				system("pause");
				elegirBando(jugador2, jugador1);
				jugador2.dado = 1;
			}
			loop = false;
			break;
		case 2://instrucciones
			Instrucciones();
			break;
		case 3://Clasificaciones
			mostrarClasificaciones();
			break;
		case 4:
			exit(0);
			break;
		default:
			cout << "\n\n OPCION INVALIDA ELIGE BIEN!\n\n";
			system("pause");
			break;
		}
	} while (loop);
}
bool MenuPausa()
{
	//muestra un menu de pausa durante el juego
	//durante el juego se aprietea P para desplegar.
	//se define como bool para tener un metodo para salir del juego actual y volver al menu principal
	int opcion;
	do {
		system("cls");
		cout << " ----> JUEGO EN PAUSA <----\n\n";
		cout << " Ingresa un numero de opcion\n\n";
		cout << " 1. Volver al Juego\n";
		cout << " 2. Instrucciones\n";
		cout << " 3. Clasificaciones\n";
		cout << " 4. Salir del juego\n\n";
		cout << " Opcion: ";
		cin >> opcion;
		switch (opcion)
		{
		case 1://vuelve al juego
			return true;
			break;
		case 2://despliega instrucciones
			Instrucciones();
			return true;
			break;
		case 3://muestra la grilla de clasificaciones.
			mostrarClasificaciones();
			return true;
			break;
		case 4:
			return false;
			break;
		}
	} while (true);
}
void Instrucciones()
{
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
}

//Inicializaciones
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
void elegirBando(APersonaje& jugador, APersonaje& enemigo) {
	int opciones;
	char confirmacion;
	do {
		system("cls");
		cout << " ----> ELEGIR BANDO <----\n\n";
		cout <<" Jugador: "<< jugador.nombre<<"\n\n Elige tu bando (ingresa 1 o 2):\n 1. Counter\n 2. Terrorist\n\n";
		cout << " Opcion: ";
		cin >> opciones;
		if (opciones == 1) {
			cout << " Elegiste: Counter\n\n";
			cout << " Confirma tu eleccion apretando Y\n";
			cout << " Si te equivocaste presiona cualquier otra letra: ";
			cin >> confirmacion;
			//toupper para garantizar que lo que se ingresa esta en mayusculas
			//en codigo ASCII las mayusculas y minusculas son diferentes 'y' es diferente a 'Y'
			if (toupper(confirmacion) == 'Y') {
				crearActor(jugador, "Counter", 'C');
				crearActor(enemigo, "Terrorist", 'T');
				return;//usa return para cortar el loop infinito
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
				return;
			}
		}
		else {
			cout << "\n\n OPCION INVALIDA ELIGE BIEN!\n\n";
			system("pause");
		}
	} while (true);
}
void crearActor(APersonaje& pers, string bando, char visual) {
	//inicializa las variables principales del actor
	pers.bando = bando;
	pers.repVisual = visual;
	pers.posX = 0;
	pers.posY = 0;
	pers.creado = true;
	pers.vida = 3;
	pers.muertes = 0;
	pers.puntaje = 0;
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
void Respawn(APersonaje& personaje)
{
	//Respawnea el personaje si es que creado es false
	//si creado es false, significa que el personaje esta muerto, por ende tiene sentido respawnearlo.
	if (!personaje.creado) {
		personaje.creado = true;
		personaje.vida = 3;
		addJugador(personaje);
	}
}
void initPuntajes()
{
	//inicia los puntajes en la grillla de clasificaciones con numeros random
	for (int i = 0; i < 10; i++)
		puntajes[i] = rand() % 100 + 1;
}
string crearNombres()
{
	//funcion para crear el nombre del jugador
	string nombre;
	cout << "\n\n (Puedes usar letras [del alfabeto ingles] numeros y simbolos, todo junto sin espacios)\n";
	cout << " Ingresa tu Alias: ";
	cin >> nombre;
	return nombre;
}

//Acciones
void disparar(APersonaje personaje, APersonaje& proyectil, char direccion) {
	//Crea un proyectil en la direccion que mira el jugador
	//se llamaria desde jugar
	proyectil.repVisual = '-';
	proyectil.bando = "bala";
	proyectil.direccion = direccion;
	proyectil.creado = true;
	switch (direccion) {
	case 'w'://hacia arriba
		proyectil.posX = personaje.posX;
		proyectil.posY = personaje.posY - 1;
		proyectil.repVisual = '|';
		break;
	case 's'://acia abajo
		proyectil.posX = personaje.posX;
		proyectil.posY = personaje.posY + 1;
		proyectil.repVisual = '|';
		break;
	case 'a'://hacia la izquierda
		proyectil.posX = personaje.posX - 1;
		proyectil.posY = personaje.posY;
		break;
	case 'd'://hacia la derecha
		proyectil.posX = personaje.posX + 1;
		proyectil.posY = personaje.posY;
		break;
	}
}
void mover(APersonaje& personaje, char dir) {
	//mueve el personaje jugable
	//se llamaria desde jugar
	int x = personaje.posX;//toma las posiciones iniciales del personaje
	int y = personaje.posY;
	switch (dir) {
	case 'W'://mover arriba
	case'w':
	case'8':
		if (mapa[personaje.posY - 1][personaje.posX] == ' ') {
			mapa[personaje.posY][personaje.posX] = ' ';
			personaje.posY--;
			mapa[personaje.posY][personaje.posX] = personaje.repVisual;
		}
		personaje.direccion = 'w';
		break;
	case 'S'://mover abajo
	case's':
	case'5':
		if (mapa[personaje.posY + 1][personaje.posX] == ' ') {
			mapa[personaje.posY][personaje.posX] = ' ';
			personaje.posY++;
			mapa[personaje.posY][personaje.posX] = personaje.repVisual;
		}
		personaje.direccion = 's';
		break;
	case 'A'://mover a la izquierda
	case'a':
	case'4':
		if (mapa[personaje.posY][personaje.posX - 1] == ' ') {
			mapa[personaje.posY][personaje.posX] = ' ';
			personaje.posX--;
			mapa[personaje.posY][personaje.posX] = personaje.repVisual;
		}
		personaje.direccion = 'a';
		break;
	case 'D'://mover a la derecha
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
		personaje.muertes++;
	}
}

//Sistema
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
void mostrarMapa() {
	//imprime el mapa en pantalla
	HANDLE console_color;
	console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int fil = 0; fil < filas; fil++) {
		for (int col = 0; col < columnas; col++) {
			if (mapa[fil][col] == 'C')
				SetConsoleTextAttribute(console_color, 9);
			else if (mapa[fil][col] == 'T')
				SetConsoleTextAttribute(console_color, 12);
			else
				SetConsoleTextAttribute(console_color, 7);
			cout << mapa[fil][col] << "  ";
		}
		cout << endl;
	}
}
void mostrarClasificaciones()
{
	//muestra las clasificaciones
	//setw es una funcion que permite ordenar la salida para que se vea como columnas ordenadas.
	//indica cuantos caracteres se van a usar para la siguiente salida (lo que está después del <<)
	HANDLE console_color;
	console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls");
	cout << " ----> CLASIFICACIONES <----\n\n";
	cout << setw(17) << " Posicion |" << setw(17) << " Jugador |" << setw(17) << " Puntaje " << endl;
	for (int i = 0; i < 10; i++) {
		if (jugadores[i] == jugador1.nombre || jugadores[i] == jugador2.nombre) {
			SetConsoleTextAttribute(console_color, 14);
		}
		else
			SetConsoleTextAttribute(console_color, 7);

		cout << setw(15) << i + 1 << " |" << setw(15) << jugadores[i] << " |" << setw(15) << puntajes[i] << endl;
	}
	cout << endl;
	cout << " <---Volver al menu\n";
	system("pause");
}
void OrdClasificaciones()
{
	//algoritmo para ordenar las clasificaciones
	//el criterio elegido son los puntajes de mayor a menor
	int auxPts = 0;
	string auxUser = "";
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 9; j++) {
			if (puntajes[j] < puntajes[i]) {
				//ordenar puntajes
				auxPts = puntajes[i];
				puntajes[i] = puntajes[j];
				puntajes[j] = auxPts;
				//ordenar jugadores
				auxUser = jugadores[i];
				jugadores[i] = jugadores[j];
				jugadores[j] = auxUser;
			}
		}
	}
}
void addClasificacion(APersonaje player)
{
	//añade una puntuacion a la grilla de puntajes
	//la añade en la posicion que corresponda segun el puntaje (comparando de mayor a menor)
	//desplaza los puntajes restantes una posicion hacia abajo, y descarta el sobrante.
	bool aniadida = false;
	int AuxPunt[10], auxPos;
	string AuxNomb[10];
	for (int i = 0; i < 10; i++) {
		if (aniadida) {
			//cuando ya se ha añadido una clasificacion desplaza las clasificaciones una posición hacia abajo.
			puntajes[i] = AuxPunt[i - auxPos];
			jugadores[i] = AuxNomb[i - auxPos];
		}
		if (!aniadida && player.puntaje > puntajes[i]) {
			//solo si no hay una clasificacion añadida se hace el backup de las posiciones restantes
			//y se añade la clasificacion en el lugar que corresponda segun la segunda clausula del if de arriba.
			//de mayor a menor
			for (int j = 0; j < 10 - i; j++) {
				AuxPunt[j] = puntajes[i + j];
				AuxNomb[j] = jugadores[i + j];
			}
			puntajes[i] = player.puntaje;
			jugadores[i] = player.nombre;
			aniadida = true;//avisa de que ya se añadió una clasificación
			auxPos = i + 1;
			//la posición donde se añade la clasificación +1
			//sirve para dar con los indices correctos del vector aux
		}
	}
}
void mostrarStats(APersonaje jugador, APersonaje enemigo) {
	//muestra las estadisticas del juego
	cout << "\n\n ------> HUD <------" << endl;
	cout << setw(15) << " JUGADOR | " << setw(15) << jugador.nombre << " | " << setw(15) << enemigo.nombre << endl;
	cout << setw(15) << " BANDO | " << setw(15) << jugador.bando << " | " << setw(15) << enemigo.bando << endl;
	cout << setw(15) << " VIDA | " << setw(15) << jugador.vida << " | " << setw(15) << enemigo.vida << endl;
	cout << setw(15) << " MUERTES | " << setw(15) << jugador.muertes << " | " << setw(15) << enemigo.muertes << endl;
	cout << setw(15) << " PUNTOS | " << setw(15) << jugador.puntaje << " | " << setw(15) << enemigo.puntaje << endl<<endl;
}

//Modo de juego
void jugar(APersonaje& jugador, APersonaje& enemigo) {
	//es el desarrollo del juego
	//inicia creando el mapa y añadiendo los jugadores al mapa
	crearMapa();
	addJugador(jugador);
	addJugador(enemigo);
	bool inGame = true;
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
			case '8'://llama al movimiento del enemigo
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
			case 'P'://toma la tecla P para poner en pausa
				//para esto menu pausa es bool, para cortar el juego si se desea salir de el en cualquier momento
				inGame = MenuPausa();
				continue;
				break;
			}
		}
		//actualiza lo que pasa en cada frame, para cada jugador
		accion(jugador, bala1);
		accion(enemigo, bala);
		sumarPuntos(jugador, enemigo);
		//chequea las muertes, si algun jugador tiene muertes mayores o iguales a 3 el juego se acaba
		if (jugador.muertes >= 3 || enemigo.muertes >= 3)
			inGame = false;
		//muestra el mapa en cada frame
		mostrarMapa();
		//muestra la ayuda para desplegar el menu pausa
		cout << "\n P: Pausar\n";
		//muestra las estadisticas en cada frame
		mostrarStats(jugador, enemigo);
		Sleep(65);
	} while (inGame);
	//maneja todo el juego
	//movimiento, tiros, ganador, todo sale de aca
}
void sumarPuntos(APersonaje& player, APersonaje& enemy)
{
	//suma puntos a un jugador si se cumple que uno esta vivo y el otro muerto
	if (player.vida < 1 && enemy.vida >= 1)
		enemy.puntaje += 32;
	if (player.vida >= 1 && enemy.vida < 1)
		player.puntaje += 32;
}
void recibirDmg(APersonaje& personaje, APersonaje& proyectil) {
	//recibe daño tiene problemas cuando la bala pasa por el costado, toma el daño igualmente.
	switch (proyectil.direccion) {
	case 'w'://direccion de la bala: caso vertical
	case 's':
		if (personaje.posX == proyectil.posX)//para la misma posicion X la bala solo puede pegar de arriba o abajo
			if (personaje.posY == proyectil.posY - 1 || personaje.posY == proyectil.posY + 1)
				personaje.vida--;
		break;
	case 'a'://direccion de la bala caso horizontal
	case 'd':
		if (personaje.posY == proyectil.posY)//para la misma posicion Y la bala solo puede pegar de la izquierda o la derecha
			if (personaje.posX == proyectil.posX - 1 || personaje.posX == proyectil.posX + 1 && bala.direccion)
				personaje.vida--;
		break;
	}
}
