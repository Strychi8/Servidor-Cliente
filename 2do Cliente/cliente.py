import socket

def mostrar_menu():
    print("\nSeleccione una opcion:")
    print("1. Generar nombre de usuario")
    print("2. Generar contrasenia")
    print("3. Salir")
    return int(input("Opción: "))

def obtener_longitud(tipo):
    while True:
        try:
            longitud = int(input(f"Ingrese la longitud deseada para {tipo}: "))
            if (tipo == "nombre de usuario" and (longitud < 5 or longitud > 15)) or \
               (tipo == "contrasenia" and (longitud < 8 or longitud > 50)):
                print("Longitud no válida. Intente nuevamente.")
            else:
                return longitud
        except ValueError:
            print("Entrada no válida. Por favor ingrese un número.")

def main():
    server_address = ('127.0.0.1', 8080)

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect(server_address)

        while True:
            opcion = mostrar_menu()

            if opcion == 3:
                print("Saliendo...")
                break

            tipo = "nombre de usuario" if opcion == 1 else "contrasenia"
            longitud = obtener_longitud(tipo)

            # Enviar solicitud al servidor
            mensaje = f"{'U' if opcion == 1 else 'P'} {longitud}"
            sock.sendall(mensaje.encode('utf-8'))

            # Recibir respuesta del servidor
            respuesta = sock.recv(1024).decode('utf-8')
            print(f"Respuesta generada del servidor: {respuesta}")

if __name__ == "_main_":
    main()