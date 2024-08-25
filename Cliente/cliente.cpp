#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 8080
#define MAX_BUFFER 1024

using namespace std;

void mostrarMenu() {
    cout << "\nSeleccione una opcion: \n" << endl;
    cout << "1. Generar nombre de usuario" << endl;
    cout << "2. Generar contrasenia" << endl;
    cout << "3. Salir" << endl;
    cout << "\nOpcion: ";
}

int obtenerOpcion() {
    int opcion;
    cin >> opcion;
    while (cin.fail() || (opcion < 1 || opcion > 3)) {
        cin.clear(); // Limpia el estado de error de cin
        cin.ignore(INT_MAX, '\n'); // Descarta la entrada incorrecta
        cout << "\nOpción no valida, por favor ingrese un numero entre 1 y 3: ";
        cin >> opcion;
    }
    return opcion;
}

int obtenerLongitud(const string& tipo) {
    int longitud;
    while (true) {
        cout << "Ingrese la longitud deseada para " << tipo << ": ";
        cin >> longitud;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "\nEntrada no valida. Por favor ingrese un numero." << endl;
            continue;
        }
        if ((tipo == "nombre de usuario" && (longitud < 5 || longitud > 15)) ||
            (tipo == "contrasenia" && (longitud < 8 || longitud > 50))) {
            cout << "\nLongitud no valida. Intente nuevamente." << endl;
        } else {
            break;
        }
    }
    return longitud;
}

int main() {
    WSADATA wsaData;
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[MAX_BUFFER] = {0};
    char respuesta[MAX_BUFFER] = {0};

    // Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Error al inicializar Winsock" << endl;
        return 1;
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Error al crear el socket" << endl;
        WSACleanup();
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convertir direcciones IPv4 e IPv6 de texto a binario
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (serv_addr.sin_addr.s_addr == INADDR_NONE) {
        cerr << "Direccion IP no valida" << endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Conexion fallida" << endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

      int opcion = 0;

      do {
        mostrarMenu();
        opcion = obtenerOpcion();

        switch(opcion){
          case 1: // Generar nombre de usuario
           {
             string tipo = "nombre de usuario";
             int longitud = obtenerLongitud(tipo);

             snprintf(buffer, sizeof(buffer), "%c %d", 'U', longitud);
             send(sock, buffer, strlen(buffer), 0);
             memset(respuesta, 0 , MAX_BUFFER); //Limpiar el buffer de respuesta
             recv(sock, respuesta, MAX_BUFFER, 0);
             cout << "Respuesta generada del servidor: " << respuesta << endl;

           } break;

          case 2: // Generar contrasenia
             {
             string tipo = "contrasenia";
             int longitud = obtenerLongitud(tipo);
             snprintf(buffer, sizeof(buffer), "%c %d", 'P', longitud);

             send(sock, buffer, strlen(buffer), 0);
             memset(respuesta, 0 , MAX_BUFFER); //Limpiar el buffer de respuesta
             recv(sock, respuesta, MAX_BUFFER, 0);
             cout << "Respuesta generada del servidor: " << respuesta << endl;
             } break;

          case 3: // Salir
            cout << "Saliendo..." << endl; break;

          default: // Opcion no valida
            cout << "Opcion no valida, por favor seleccione una opcion entre 1 y 3." << endl; break;

        }

    } while (opcion != 3);

    closesocket(sock);
    WSACleanup();

    return 0;
}
