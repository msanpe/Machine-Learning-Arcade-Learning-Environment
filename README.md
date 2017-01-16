# Robobit Machine Learning
Robobit team ML Project
**Fernando Verdejo - Miguel Sancho - Cristina Rivera**

# Instrucciones:
## Compilación para Debian/Ubuntu 64 bits:#
Requisitos previos:
Los requisitos previos caracteristicos del ALE mas la libreria ncurses. 

Para compilar:

Para realizar la compilación y ejecucion, será necesario añadir las siguientes variables de entorno:

LIBRARY_PATH="<Ruta hasta la carpeta>/Arcade-Learning-Enviorment/."
LD_LIBRARY_PATH="<Ruta hasta la carpeta>/Arcade-Learning-Enviorment/."

Clonamos el repositorio robobit-ml dentro de la carpeta Arcade-Learning-Enviorment.
Una vez hecho eso entramos en la carpeta robobit-ml y creamos el directorio obj.
A partir de ahi usamos el comando make para construir el objeto que deseemos.

	cd <Ruta hasta la carpeta>/Arcade-Learning-Enviorment
	git clone <robobithttp>
	cd robobit-ml
	mkdir obj
	make <target> (consultar el makefile para obtener mas información)


# -------------ALGORITMOS--------------
# Red Neuronal
## Implementación y Características:

**Estructura de codigo**

Con el diseño se ha hecho mucho énfasis en separar el algoritmo de la carga de datos y de la interacción con el usuario
con el fin de abstraer al máximo cada uno de los componentes para que la integración con el ALE fuese lo mas simple posible.

El diseño consta de tres partes:
* functions.cpp
 * Se encarga de la interacción con el usuario y de crear y llamar a las otras dos.
* data.cpp
 * Se encarga del manejo de los datos de entrenamiento.
* nn.cpp
 * El algoritmo en sí

**Uso de regularización:**

Se ha implementado la regularización L2 para evitar el overfitting. Para implementarla se debe penalizar la magnitud
cuadrada de todos los parámetros directamente en la función objetivo. Es decir, por cada peso W que tenga la red, debemos
añadir 1/2 * Lambda * W^2 a la función objetivo donde Lambda es la fuerza de la regularización. 
La regularización L2 ayuda a mitigar los picos en los vectores de pesos.

**Formato fichero de datos:**

El fichero de datos esta formateado de la siguiente manera:

    Numero de datos de entrenamiento
    Entrada11,Entrada12,Entrada1N,Salida11,Salida12,Salida1N
    Entrada21,Entrada22,Entrada2N,Salida21,Salida22,Salida2N
    
Ejemplo de fichero para puerta XOR:

    4
    0,0,1
    0,1,0
    1,0,0
    1,1,1 
**Ajuste de hiperparámetros:**

Los hiperparámetros de los que dispone la red son(Learning Rate, Momentum y Regularization Strength)
todos ellos pueden ser modificados desde el codigo de funciones.cpp

**Opciones de entrenamiento:**

Actualmente la red esta programada para que se ejecute hasta reducir su error a un valor específico.
Tambien se puede entrenar por número de epocas si se prefiere
  
**Uso:**
    
    Una vez ejecutada, se mostrará en pantalla un menu con diversas opciones
    1. Train new network
    2. Switch between continuos and discrete outputs
    3. Test network
    4. Load network
    5. Exit
    
    Para entrenar una nueva red, elegir la opción 1, ingresar el fichero de datos de entrenamiento
    y el nombre del fichero donde quieres guardar la arquitectura y pesos de la red.
    
    Cuando finalice el entrenamiento se podrá probar la red entrenada mediante la opción 3 
    donde habrá que ingresar las entradas de la red.
    
    Para cargar una red ya entrenada usaremos la opción 4.
    
    La opción 2 sirve para cambiar entre salidas discretas (1 o 0) y continuas (0,98764).

# Regresión Logistica
## Implementación y Características:

**Estructura de codigo**

La clase LogisticRegression provee todas las funcionalidades para la carga desde fichero, el entrenamiento del algoritmo, y el salvado a fichero de los pesos del algoritmo. Ademas el archivo hace uso de una clase de apoyo para todo el proyecto llamada Utils y donde se encuentran funcionalidades para calcular maximos, convertir string en vectores de float y en general todas las funciones de apoyo para el codigo. 

**Formato fichero de datos:**

Mismo formato que para la red neuronal

**Ajuste de hiperparámetros:**

El algoritmo solo dispone de Learning Rate y se puede ajustar en el fichero de LogisticRegression.cpp, al principio donde se inicializan las constantes. 

**Opciones de entrenamiento:**

Dispone de parada por época

# Perceptron Unicapa
## Implementación y Características:

**Estructura de codigo**
El perceptron implementado consta de un único archivo perceptron.cpp. Desde este fichero se cargan los datos y se realizan los cálculos según los parámetros introducidos para alpha y las épocas.

**Formato fichero de datos:**

Mismo formato que para la red neuronal

# Regresion Lineal
## Implementación y Características:

**Estructura de codigo**
La regresión lineal implementada consta de un único archivo linearregression.cpp. Desde este fichero se cargan los datos y se realizan los cálculos según los parámetros introducidos para alpha y las épocas.

**Formato fichero de datos:**

Mismo formato que para la red neuronal


# ----------------AGENTES---------------
## Implementación y Características:
Nuestro proyecto disponde de un agente para cada algoritmo implementado que actua como interfaz para realizar entrenamientos y crear ficheros con los parametros del entrenamiento. Los agentes reciben por parametro en tiempo de ejecucion el nombre del juego y adaptan los tamaños de tupla al juego deseado.  Ademas, disponemos de un agente minador de datos (data_agent).

#Data_Agent

data_agent.cpp se encarga de elaborar los conjuntos de datos para posteriores entrenamientos del algoritmo y tambien para realizar ingenieria inversa sobre la RAM y adivinar los puntos de interes de cada juego. Este programa hace uso de la libreria ncurses para poder ejecutar  El salvado de los conjuntos de datos se realiza dentro de la carpeta datos, y dentro de la carpeta de juego correspondiente.

**Compilacion**

Dentro de robobit-ml, ejecutar el comando make data para compilar el archivo. Se generará un ejecutable llamado data_agent

**Ejecucion**

./data_agent <romfile>

#Log_Agent

log_agent.cpp provee una interfaz amigable para la carga de ficheros, el entrenamiento desde fichero, y el testeo del algoritmo de regresion logistica con fichero o para instancias unicas. La carga de fichero para el entrenamiento la realiza desde la carpeta datos. No hace falta especificar la subcarpeta de juego, el propio agente busca en la que correspone al parametro pasado por shell. El salvado de los pesos se realiza en la misma ruta donde se ejecuta y podemos especificar el nombre del archivo.

**Compilacion**

Dentro de robobit-ml, ejecutar el comando make log para compilar el archivo. Se generará un ejecutable llamado log_agent

**Ejecucion**

./log_agent <romfile>


#nn_Agent

nn_agent.cpp provee una interfaz amigable para la carga de ficheros, el entrenamiento desde fichero, y el testeo del algoritmo de regresion logistica con fichero o para instancias unicas. La carga de fichero para el entrenamiento la realiza desde la carpeta datos. No hace falta especificar la subcarpeta de juego, el propio agente busca en la que correspone al parametro pasado por shell. El salvado de los pesos se realiza en la misma ruta donde se ejecuta y podemos especificar el nombre del archivo. Si deseamos utilizar la funcionalidad de cargar pesos, debemos previamente copiar el archivo con los datos donde se entrenaron los pesos a la ruta del archivo de pesos, y deberemos nombrarlo de la forma <nombre_archivo_pesos>_data. Ejemplo: si guardamos los pesos en el archivo weights, debemos copiar el archivo "datos_tennis.txt" a la ruta donde esta weights y renombrarlo a "weights_data".

**Compilacion**

Dentro de robobit-ml, ejecutar el comando make net para compilar el archivo. Se generará un ejecutable llamado net_agent

**Ejecucion**

./log_agent <romfile>

# ----------------BOTS---------------

## Implementación y Características:

Una vez que hayamos entrenado nuestros algoritmos de manera satisfactoria, podemos proceder a compilar nuestros bots para que juguen a los juegos. Existe un bot por algoritmo de entrenamiento. El bot no lee desde ningun fichero los parametros necesarios para jugar. Debemos ser nosotros quienes manualmente añadamos en el archivo de fuente <nombre_de_algoritmo>_bot.cpp los parametros caracterisiticos de cada algoritmo. Por ejemplo, para un bot de regresion logistica debemos añadir en el archivo el tamaño de tupla de entrada y salida, el maximo y el minimo del conjunto de datos, y los bias que deseamos. Todos estos datos estan encapsulados en los archivos de salvado de pesos de cada algoritmo respectivamente.

**Compilacion**

Dentro de robobit-ml, ejecutar el comando make <nombre_de_algoritmo>bot para compilar el archivo. Se generará un ejecutable llamado net_agent. Para mas info, consultar el makefile. 

**Ejecucion**

La ejecucion en este caso es igual que en los agentes, ya que en realidad el bot encapsula una de las funcoinalidades que ofrecen los agentes: 
./<nombre_de_algoritmo>_bot <romfile>











