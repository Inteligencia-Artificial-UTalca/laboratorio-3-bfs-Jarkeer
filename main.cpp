#include "Map.h"
#include "Search.h"
#include "ColorMap.h"
#include <cassert>
#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]){

    //Verify that the amount of arguments is correct
    //6 arguments: program name, map file, x1, y1, x2, y2

    if(argc != 6)
    {
        std::cerr << "Uso: " << argv[0] << " <map_file> <x1> <y1> <x2> <y2>" << std::endl;
        return 1;
    }

    int x1 = std::atoi(argv[2]);
    int y1 = std::atoi(argv[3]);
    int x2 = std::atoi(argv[4]);
    int y2 = std::atoi(argv[5]);

    // Primero cargamos el mapa para poder usar sus dimensiones (map.h y map.w)
    Map map(argv[1]);

    // Ahora sí podemos validar si las coordenadas están dentro de los límites
    if (x1 < 0 || x1 >= map.h || y1 < 0 || y1 >= map.w || 
        x2 < 0 || x2 >= map.h || y2 < 0 || y2 >= map.w) {
        std::cerr << "Error: Las coordenadas ingresadas estan fuera de los limites." << std::endl;
        return 1;
    }

    ColorMap colorMap(map);
    colorMap.print();

    auto path = Search::AStart(map,{x1, y1}, {x2, y2}); 
    colorMap.print(path);
    
    //Calculate path distance
    if (path.size() > 2 || (path.size() == 2 && path[0] != path[1])) {
        std::cout << "Dist: " << path.size() - 1 << std::endl;
    } else {
        std::cout << "Dist: 0" << std::endl;
    }
    //Print path distance
    
    return 0;
}