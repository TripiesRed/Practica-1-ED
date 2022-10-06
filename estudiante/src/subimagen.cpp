//
// Created by gonzalomp on 6/10/22.
//

#include <iostream>
#include <cstring>
#include <cstdlib>

#include <image.h>

using namespace std;

int main (int argc, char *argv[]) {

    char *origen, *destino; // nombres de los ficheros
    Image image;
    int coordx, coordy, subfils, subcols; // datos de las para realizar el zoom de una zona concreta

    // Comprobar validez de la llamada
    if (argc != 7) {
        cerr << "Error: Numero incorrecto de parametros.\n";
        cerr << "Uso: negativo <FichImagenOriginal> <FichImagenDestino>\n";
        exit(1);
    }

    // Obtener argumentos
    origen = argv[1];
    destino = argv[2];
    coordx = stoi(argv[3]);
    coordy = stoi(argv[4]);
    subfils = stoi(argv[5]);
    subcols = stoi(argv[6]);

    // Mostramos argumentos
    cout << endl;
    cout << "Fichero origen: " << origen << endl;
    cout << "Fichero resultado: " << destino << endl;

    // Leer la imagen del fichero de entrada
    if (!image.Load(origen)) {
        cerr << "Error: No pudo leerse la imagen." << endl;
        cerr << "Terminando la ejecucion del programa." << endl;
        return 1;
    }

    // Mostrar los parametros de la Imagen
    cout << endl;
    cout << "Dimensiones de " << origen << ":" << endl;
    cout << "   Imagen   = " << image.get_rows() << " filas x " << image.get_cols() << " columnas " << endl;

    // Mostramos los parámetros para generar la subimagen
    cout << endl;
    cout << "El valor del coordenada x es: " << coordx << endl;
    cout << "El valor de la coordenada y es: " << coordy << endl;

    Image newimage = image.Crop(coordx,coordy,subfils, subcols);

    // Mostrar los parametros de la Imagen Resultado
    cout << endl;
    cout << "Dimensiones de " << destino << ":" << endl;
    cout << "Imagen Generada  = " << newimage.get_rows()  << " filas x " << newimage.get_cols() << " columnas " << endl;

    // Guardar la imagen resultado en el fichero
    if (newimage.Save(destino))
        cout  << "La imagen se guardo en " << destino << endl;
    else{
        cerr << "Error: No pudo guardarse la imagen." << endl;
        cerr << "Terminando la ejecucion del programa." << endl;
        return 1;
    }


    return 0;
}
