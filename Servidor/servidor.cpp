#include <iostream>
#include <cstdlib>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <ctime>

#define PORT 8080
#define MAX_BUFFER 1024

using namespace std;

//Función para generar un nombre de usuario
string generarNombreUsuario(int longitud) {
    if (longitud < 5 || longitud > 15) {
        return "Error: La longitud del nombre de usuario debe estar entre 5 y 15 caracteres";
    }

    string vocales = "aeiou";
    string consonantes = "bcdfghjklmnpqrstvwxyz";
    string resultado = " ";

    bool esVocal = rand() % 2; //0 = empieza con consonante, 1 = empieza con vocal

    for (int i = 0; i < longitud; i++) {
        if (esVocal) {
            resultado += vocales[rand() % vocales.length()];
        } else {
            resultado += consonantes[rand() % consonantes.length()];
        }
        esVocal = !esVocal; //Alterna entre vocal y consonante
    }

    return resultado;
}

//Función para generar una contraseña
string generarContrasena(int longitud) {
    if (longitud < 8 || longitud > 50) {
        return "Error: La longitud de la contrasenia debe estar entre 8 y 50 caracteres";
    }

    string alfanumerico = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    string resultado = " ";

    for (int i = 0; i < longitud; i++) {
        resultado += alfanumerico[rand() % alfanumerico.length()];
    }

    return resultado;
}

int main() {
    WSADATA wsaData;
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX_BUFFER] = {0};
    char respuesta[MAX_BUFFER] = {0};

    //Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Error al inicializar Winsock" << endl;
        return 1;
    }

    //Crear el socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        cerr << "Error al crear el socket" << endl;
        WSACleanup();
        return 1;
    }

    //Configurar la estructura de la dirección del servidor
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    //Asociar el socket al puerto
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        cerr << "Error en bind" << endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    //Poner el socket en modo de escucha
    if (listen(server_fd, 3) < 0) {
        cerr << "Error en listen" << endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    cout << "Servidor escuchando en el puerto " << PORT << endl;

    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) == INVALID_SOCKET) {
            cerr << "Error en accept" << endl;
            closesocket(server_fd);
            WSACleanup();
            return 1;
        }

        memset(buffer, 0, MAX_BUFFER); // Limpiar el buffer de recepcion
        int valread = recv(new_socket, buffer, MAX_BUFFER, 0);
        if(valread == SOCKET_ERROR){
            cerr << "Error al recibir datos" << endl;
            closesocket(new_socket);
            continue;
        }

        //Procesar la solicitud
        char tipoSolicitud ;
        int longitud;

        string respuesta;
        sscanf(buffer, "%c %d", &tipoSolicitud, &longitud);

        if (tipoSolicitud == 'U') {
            respuesta = generarNombreUsuario(longitud);
        } else if (tipoSolicitud == 'P') {
            respuesta = generarContrasena(longitud);
        } else {
            respuesta = "Error: Opcion no valida";
        }

        //Enviar la respuesta al cliente
        send(new_socket, respuesta.c_str(), respuesta.length(), 0);

        closesocket(new_socket);
    }

    closesocket(server_fd);
    WSACleanup();

    return 0;
}
