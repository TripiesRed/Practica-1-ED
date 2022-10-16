//
// Created by gonzalomp on 16/10/22.
//

#include <iostream>
#include <chrono>
#include <image.h>
#include <vector>
#include <stdlib.h>

using namespace std;

void bubbleSort(vector<int> arr){
    int tmp;
    for (int i = 0; i < arr.size() - 1; i++){
        for (int j = 0; j < arr.size() - i - 1; j++){
            if (arr[j] > arr[j + 1]) {
                tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
}

void chrono_experiment(int vector_size, int repetitions) {
    // Inicializamos el experimento antes del bucle de conteo de tiempo para no afectar al análisis
    vector<int> collection;
    for (int i = 0; i < vector_size; ++i){
        collection.push_back(rand() % 1000);
    }

    // Medimos el tiempo antes de iniciar el expermiento
    chrono::high_resolution_clock::time_point start_time = chrono::high_resolution_clock::now();

    // Realizamos el experimento tantas veces como indique repetitions, para tener cierta robustez
    // en el resultado (no todas las ejecuciones tardan lo mismo)
    for (int k = 0; k < repetitions; ++k){
        bubbleSort(collection);
    }

    // Medimos el tiempo al finalizar el experimento
    chrono::high_resolution_clock::time_point finish_time = chrono::high_resolution_clock::now();

    // Calculamos la diferencia entre el inicio y el final
    chrono::duration<double> total_duration = chrono::duration_cast<chrono::duration<double>>(finish_time - start_time);

    // Dividimos el tiempo total entre el número de repeticiones para obtener el tiempo medio por ejecución
    cout << vector_size << "\t" << total_duration.count()/repetitions << endl;
}
int main() {

    for (int i = 100; i <= 5000; i += 100){
        chrono_experiment(i, 30);
    }
    return 0;
}

