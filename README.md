# Robobit Machine Learning
Robobit team ML Project
**Fernando Verdejo - Miguel Sancho - Cristina Rivera**

# Instrucciones:
## Compilación para Debian/Ubuntu 64 bits:#
Para compilar:
LIBRARY_PATH="." g++ agent.cpp -o ./agent -lale -lncurses
Usamos LIBRARY_PATH="." para que se utilice la libreria *libale.so* que hay en el path. 
La libreria *libale.so* es una libreria dinámica que permite lanzar el emulador Stela.

## Ejecución:
Para ejecutar un juego:
LD_LIBRARY_PATH="." ./agent roms/nombre_del_juego.bin


# Red Neuronal
## Para usarla de forma standalone sin ALE
**Compilación:**

    g++ main.cpp functions.cpp data.cpp nn.cpp -o net
**Ejecución:**
    
    ./net   
**Uso:**

    Una vez ejecutada, se mostrara en pantalla un menu con diversas opciones
    1. Train new network
    2. Switch between continuos and discrete outputs
    3. Test network
    4. Load network
    5. Exit
    
    Para entrenar una nueva red, elegir la opcion 1, ingresar el fichero de datos de entrenamiento
    y el nombre del fichero donde quieres guardar la arquitectura y pesos de la red.
    
    Cuando finalice el entrenamiento se podrá probar la red entrenada mediante la opcion 3 
    donde habrá que ingresar las entradas de la red.
   
    Para cargar una red ya entrenada usaremos la opcion 4.
    
    La opcion 2 sirve para cambiar entre salidad discretas (1 o 0) y continuas (0,98764).
**Formato fichero de datos:**
El fichero de datos esta formateado de la siguiente manera:

    Numero de datos de entrenamiento
    Entrada11 Entrada12 Entrada1N
    Salida11 Salida12 Salida1N
    Entrada21 Entrada22 Entrada2N
    Salida21 Salida22 Salida2N
    
Ejemplo de fichero para puerta XOR:

    4
    0 0
    1
    0 1
    0
    1 0
    0
    1 1
    1 
**Ajuste de hiperparámetros:**

Los hiperparámetros de los que dispone la red son(Learning Rate, Momentum y Regularization Strength)
todos ellos pueden ser modificados desde el codigo de funciones.cpp

**Opciones de entrenamiento:**

Actualmente la red esta programada para que se ejecute hasta reducir su error a un valor especifico.
Tambien se puede entrenar por número de epocas si se prefiere

**Estructura de codigo**

Con el diseño se ha hecho mucho enfasis en separar el algoritmo de la carga de datos y de la interaccion con el usuario
con el fin de abstraer al máximo cada uno de los componentes para que la integración con el ALE fuese lo mas simple posible.

El diseño consta de tres partes:
* functions.cpp
  * Se encarga de la interaccion con el usuario y de crear y llamar a las otras dos.
* data.cpp
  * Se encarga del manejo de los datos de entrenamiento.
* nn.cpp
  * El algoritmo en si
