//
// Created by diegoos_03 on 14/10/22.
//

#include <iostream>
#include <cstring>
#include <cstdlib>

#include <image.h>

using namespace std;

int main (int argc, char *argv[]){

    char *origen, *destino; // nombres de los ficheros
    Image imagen;
    int e1, e2, s1, s2;  // valores para ajustar el contraste


    // Comprobar validez de la llamada
    if (argc != 7){
        cerr << "Error: Numero incorrecto de parametros.\n";
        cerr << "Uso: contraste <FichImagenOriginal> <FichImagenDestino> <e1> <e2> <s1> <s2>\n";
        exit (1);
    }

    // Obtener argumentos
    origen  = argv[1];
    destino = argv[2];
    e1 = stoi(argv[3]);
    e2 = stoi(argv[4]);
    s1 = stoi(argv[5]);
    s2 = stoi(argv[6]);


    // Mostramos argumentos
    cout << endl;
    cout << "Fichero origen: " << origen << endl;
    cout << "Fichero resultado: " << destino << endl;

    // Leer la imagen del fichero de entrada
    if (!imagen.Load(origen)){
        cerr << "Error: No pudo leerse la imagen." << endl;
        cerr << "Terminando la ejecucion del programa." << endl;
        return 1;
    }

    // Mostrar los parametros de la Imagen
    cout << endl;
    cout << "Dimensiones de " << origen << ":" << endl;
    cout << "   Imagen   = " << imagen.get_rows()  << " filas x " << imagen.get_cols() << " columnas " << endl;

    // Mostramos los parámetros para realizar el ajuste
    cout << endl;
    cout << "Se introdujeron los siguientes valores para realizar el ajuste de contraste:" << endl;
    cout << "\t Umbral inferior de la imagen de entrada: " << e1 << endl;
    cout << "\t Umbral superior de la imagen de entrada: " << e2 << endl;
    cout << "\t Umbral inferior de la imagen de salida: " << s1 << endl;
    cout << "\t Umbral superior de la imagen de salida: " << s2 << endl;

    Image newimagen(imagen);
    newimagen.AdjustContrast(e1, e2, s1, s2);

    // Mostrar los parametros de la Imagen Resultado
    cout << endl;
    cout << "Dimensiones de " << destino << ":" << endl;
    cout << "Imagen Generada  = " << newimagen.get_rows()  << " filas x " << newimagen.get_cols() << " columnas " << endl;

    // Guardar la imagen resultado en el fichero
    if (newimagen.Save(destino))
        cout  << "La imagen se guardo en " << destino << endl;
    else{
        cerr << "Error: No pudo guardarse la imagen." << endl;
        cerr << "Terminando la ejecucion del programa." << endl << endl;
        return 1;
    }

    return 0;
}