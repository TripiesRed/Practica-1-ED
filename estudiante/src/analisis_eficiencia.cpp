//
// Created by gonzalomp on 16/10/22.
//

#include <iostream>
#include <chrono>
#include <image.h>
#include <vector>
#include <stdlib.h>

using namespace std;

void chrono_experiment(int n, int repetitions) {

    const int N = 1500; // Valor fijo

    // Inicializamos el experimento antes del bucle de conteo de tiempo para no afectar al análisis
    Image prueba (n,n);


    // Medimos el tiempo antes de iniciar el expermiento
    chrono::high_resolution_clock::time_point start_time = chrono::high_resolution_clock::now();

    // Realizamos el experimento tantas veces como indique repetitions, para tener cierta robustez
    // en el resultado (no todas las ejecuciones tardan lo mismo)
    for (int k = 0; k < repetitions; ++k){
        prueba.ShuffleRows();
    }

    // Medimos el tiempo al finalizar el experimento
    chrono::high_resolution_clock::time_point finish_time = chrono::high_resolution_clock::now();

    // Calculamos la diferencia entre el inicio y el final
    chrono::duration<double> total_duration = chrono::duration_cast<chrono::duration<double>>(finish_time - start_time);

    // Dividimos el tiempo total entre el número de repeticiones para obtener el tiempo medio por ejecución
    cout << n << "\t" << total_duration.count()/repetitions << endl;
}

int main () {

    for (int i = 100; i <= 3000; i += 100){
        chrono_experiment(i, 30);
    }

    cout << "OK" << endl;

    return 0;
}