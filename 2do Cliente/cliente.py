import socket

PORT = 8080
MAX_BUFFER = 1024

def mostrar_menu():
    print("\nSeleccione una opcion:\n")
    print("1. Generar nombre de usuario")
    print("2. Generar contraseña")
    print("3. Salir")
    return int(input("\nOpcion: "))

def obtener_opcion():
    while True:
        try:
            opcion = int(input())
            if 1 <= opcion <= 3:
                return opcion
            else:
                print("\nOpción no válida, por favor ingrese un número entre 1 y 3: ")
        except ValueError:
            print("\nOpción no válida, por favor ingrese un número entre 1 y 3: ")

def obtener_longitud(tipo):
    while True:
        try:
            longitud = int(input(f"Ingrese la longitud deseada para {tipo}: "))
            if (tipo == "nombre de usuario" and 5 <= longitud <= 15) or (tipo == "contraseña" and 8 <= longitud <= 50):
                return longitud
            else:
                print("\nLongitud no válida. Intente nuevamente.")
        except ValueError:
            print("\nEntrada no válida. Por favor ingrese un número.")

def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        sock.connect(("127.0.0.1", PORT))
    except socket.error as e:
        print(f"Error al conectar con el servidor: {e}")
        return

    opcion = 0

    while opcion != 3:
        mostrar_menu()
        opcion = obtener_opcion()

        if opcion == 1:  # Generar nombre de usuario
            tipo = "nombre de usuario"
            longitud = obtener_longitud(tipo)
            mensaje = f'U {longitud}'.encode()
        elif opcion == 2:  # Generar contraseña
            tipo = "contraseña"
            longitud = obtener_longitud(tipo)
            mensaje = f'P {longitud}'.encode()
        else:  # Salir
            print("Saliendo...")
            break

        if opcion in [1, 2]:
            sock.sendall(mensaje)
            respuesta = sock.recv(MAX_BUFFER).decode()
            print(f"Respuesta generada del servidor: {respuesta}")

    sock.close()

if __name__ == "__main__":
    main()