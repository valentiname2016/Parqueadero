import tkinter as tk
from tkinter import ttk
import ctypes
import os
import threading #librería para que salga la ventana emergente de la interfaz
 
class DatosPlaca(ctypes.Structure):
    _fields_ = [
        ("serie", ctypes.c_char_p),
        ("hora", ctypes.c_char_p),
        ("celda", ctypes.c_int),
        ("accion", ctypes.c_char_p)
    ]
 
class InterfazParqueadero:
    def __init__(self, ventana):
        self.ventana = ventana
        self.ventana.title("Monitor del Parqueadero - Librería Dinámica")
        self.ventana.geometry("550x420")
        self.ventana.configure(bg="#2c3e50")
 
        # conexión a la libreria dinámica
        ruta_libreria = os.path.abspath("./libparqueadero.so")
        self.libreria = ctypes.CDLL(ruta_libreria)
 
        self.libreria.recibirPlaca.argtypes = [ctypes.POINTER(DatosPlaca)]
        self.libreria.recibirPlaca.restype = ctypes.c_bool
        self.libreria.iniciarServidor.restype = ctypes.c_bool
 
        #interfaz
        tk.Label(ventana, text="PANEL DE CELDAS", font=("Arial", 14, "bold"), fg="white", bg="#2c3e50").pack(pady=10)
 
        self.frame_celdas = tk.Frame(ventana, bg="#2c3e50")
        self.frame_celdas.pack(pady=5)
 
        self.cuadritos = {}
        for i in range(1, 11):
            lbl = tk.Label(self.frame_celdas, text=f"Celda {i}\nLibre", bg="#2ecc71", fg="white", width=9, height=2, relief="solid")
            lbl.grid(row=(i-1)//5, column=(i-1)%5, padx=5, pady=5)
            self.cuadritos[i] = lbl
 
        #historial en la interfaz
        self.tabla = ttk.Treeview(ventana, columns=("Placa", "Hora", "Celda", "Accion"), show='headings', height=5)
        self.tabla.heading("Placa", text="Placa")
        self.tabla.heading("Hora", text="Hora")
        self.tabla.heading("Celda", text="Celda")
        self.tabla.heading("Accion", text="Acción")
        self.tabla.pack(pady=15, padx=15, fill=tk.X)
 
        hilo_servidor = threading.Thread(target=self.conectar_servidor_segundo_plano, daemon=True)
        hilo_servidor.start()
 
        self.servidor_listo = False
 
    def conectar_servidor_segundo_plano(self):
        #Abre el socket
        if self.libreria.iniciarServidor():
            self.servidor_listo = True
            self.ventana.after(300, self.actualizar_desde_cpp)
 
    def actualizar_desde_cpp(self):
        if not self.servidor_listo:
            return
 
        buf_serie = ctypes.create_string_buffer(10)
        buf_hora = ctypes.create_string_buffer(10)
        buf_accion = ctypes.create_string_buffer(10)
        
        datos_recibidos = DatosPlaca(
            serie=ctypes.cast(buf_serie, ctypes.c_char_p),
            hora=ctypes.cast(buf_hora, ctypes.c_char_p),
            celda=0,
            accion=ctypes.cast(buf_accion, ctypes.c_char_p)
        )
 
        if self.libreria.recibirPlaca(ctypes.byref(datos_recibidos)):
            placa = buf_serie.value.decode('utf-8')
            hora = buf_hora.value.decode('utf-8')
            accion = buf_accion.value.decode('utf-8', errors='ignore').strip().upper()
            celda = datos_recibidos.celda
 
            if accion == "INGRESO":
                self.cuadritos[celda].config(bg="#e74c3c", text=f"Celda {celda}\n{placa}")
                self.tabla.insert("", 0, values=(placa, hora, f"Celda {celda}", "INGRESA ⬇"))
            elif accion == "SALIDA":
                self.cuadritos[celda].config(bg="#2ecc71", text=f"Celda {celda}\nLibre")
                self.tabla.insert("", 0, values=(placa, hora, f"Celda {celda}", "SALE ⬆"))
 
        self.ventana.after(300, self.actualizar_desde_cpp)
 
 
if __name__ == "__main__":
    raiz = tk.Tk()
    app = InterfazParqueadero(raiz)
    raiz.mainloop()
 