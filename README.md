# robobit-ml
Robobit team ML Project
**Fernando Verdejo - Miguel Sancho - Cristina Rivera**

## Instrucciones:
# Compilación para Debian/Ubuntu 64 bits:#
Para compilar:
LIBRARY_PATH="." g++ agent.cpp -o ./agent -lale -lncurses
Usamos LIBRARY_PATH="." para que se utilice la libreria *libale.so* que hay en el path. 
La libreria *libale.so* es una libreria dinámica que permite lanzar el emulador Stela.

# Ejecución:#
Para ejecutar un juego:
LD_LIBRARY_PATH="." ./agent roms/nombre_del_juego.bin





