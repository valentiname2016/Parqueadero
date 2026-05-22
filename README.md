# Parqueadero
Diagrama UML
<img width="781" height="365" alt="Captura de pantalla 2026-05-21 190835" src="https://github.com/user-attachments/assets/3a4cf9c8-2d79-4381-96a9-979e1cc608c0" />


RESUMEN DEL PROYECTO

1. Nombre del sistema
   Sistema de control y monitoreo de parqueadero con arquitectura cliente-servidor.

---

2. Descripción general
   El proyecto consiste en un sistema que simula el funcionamiento de un parqueadero en tiempo real. Está compuesto por tres módulos principales: un cliente desarrollado en C++, un servidor en C++ y una interfaz gráfica en Python. Estos componentes se comunican entre sí para registrar el ingreso y salida de vehículos, asignar celdas de parqueo y mostrar el estado del parqueadero.

---

3. Funcionamiento del sistema

3.1 Cliente (C++)
El cliente genera placas de vehículos de forma aleatoria junto con la hora y la celda asignada. Esta información se envía continuamente al servidor mediante un socket TCP en el formato:

PLACA,HORA,CELDA

---

3.2 Servidor (C++)
El servidor recibe la información enviada por el cliente. Su función principal es procesar los datos y determinar si un vehículo está ingresando o saliendo del parqueadero.

* Si la placa no existe en el registro, se interpreta como ingreso y se asigna una celda.
* Si la placa ya existe, se interpreta como salida y se libera la celda asignada.

El servidor utiliza una estructura tipo mapa para almacenar el estado de las placas y sus celdas asociadas.

---

3.3 Interfaz gráfica (Python)
La interfaz está desarrollada en Python utilizando Tkinter. Esta se encarga de visualizar en tiempo real el estado del parqueadero.

La interfaz:

* Carga una librería dinámica desarrollada en C++.
* Recibe la información procesada desde el servidor.
* Muestra el estado de las celdas (libre u ocupada).
* Registra un historial de movimientos con placa, hora y acción.

---

4. Comunicación entre componentes
   La comunicación entre el cliente y el servidor se realiza mediante sockets TCP/IP. El servidor procesa los datos y los expone a la interfaz a través de una librería dinámica (.so), que es consumida por Python mediante ctypes.

---

5. Lógica del sistema

* Ingreso: cuando una placa aparece por primera vez, se asigna una celda y se marca como ocupada.
* Salida: cuando una placa se repite, se libera la celda asociada y se actualiza el estado a libre.

---

6. Arquitectura del sistema

Cliente C++ → Socket TCP → Servidor C++ (.so) → ctypes → Interfaz Python

---

7. Tecnologías utilizadas

* C++ (cliente y servidor)
* Sockets TCP/IP
* Python
* Tkinter (interfaz gráfica)
* ctypes (integración con librería dinámica)
* Estructuras de datos (map)

---

8. Conclusión
   El sistema implementa una arquitectura cliente-servidor para simular el control de un parqueadero en tiempo real. Permite el registro de vehículos, la asignación y liberación de celdas y la visualización del estado del sistema a través de una interfaz gráfica, integrando comunicación entre lenguajes mediante sockets y librerías dinámicas.
