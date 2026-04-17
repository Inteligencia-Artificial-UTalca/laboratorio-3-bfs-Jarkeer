// Javier Molina 
#include "Search.h"
#include <queue>
#include <chrono>
#include <unordered_map>
#include <iostream>
#include <limits>
#include <deque>
#include <cmath>

namespace std
{
    //this is needed to store a pair<int,int> in an associative container
    //such as unorered_set and unordered_map
    template<> struct hash<std::pair<int,int>>
    {
        std::size_t operator()( const std::pair<int,int> & p) const noexcept
        {
            hash<int> hasher;
            return hasher(p.first) ^ (hasher(p.second)<<1);
        }
    };
}

// ------------------------------------------------
//  RECONSTRUCT 
// ------------------------------------------------
std::vector<std::pair<int,int>> Search::reconstruct(const std::unordered_map<std::pair<int,int>,std::pair<int,int>> &pathCache, const std::pair<int,int> &start){
    std::deque<std::pair<int,int>> nodes;
    auto node = start;

    // Recorrer el camino de meta al inicio
    while(pathCache.find(node) != pathCache.end()){
        nodes.push_front(node);
        node = pathCache.at(node); 
    }
    nodes.push_front(node);

    std::vector<std::pair<int,int>> vec;
    for(auto p:nodes){
        vec.push_back(p);
    }
    return vec;
}

// ------------------------------------------------
//  BFS 
// ------------------------------------------------
std::vector<std::pair<int,int>> Search::BFS(const Map& map, std::pair<int,int> start, std::pair<int,int> goal){
    std::cout<<"===========================\nRunning BFS...\n";
    auto startTime = std::chrono::high_resolution_clock::now();

    //stores possible directions
    std::pair<int,int> dirs[]{{-1,0},{0,1},{1,0},{0,-1}};

    std::vector<std::vector<bool>> visited(map.h, std::vector<bool>(map.w, false));
    std::queue<std::pair<int,int>> OPEN;
    std::unordered_map<std::pair<int,int>,std::pair<int,int>> pathCache;

    //add firts node to open list
    OPEN.push(start);
    visited[start.first][start.second] = true;
    while(!OPEN.empty()){
        auto pos = OPEN.front();
        OPEN.pop();
        
        //check if node is goal
        if(pos==goal){
            auto endTime = std::chrono::high_resolution_clock::now();
            int count=0;
            for(int i=0;i<map.h;i++){
                for(int j=0;j<map.w;j++){
                    if(visited[i][j])count++;
                }
            }
            std::cout<<"VISITED: "<<count<<std::endl;
            std::cout<<"OPEN: "<<OPEN.size()<<std::endl;
            std::cout<<"FOUND in "<<std::chrono::duration<double, std::milli>(endTime-startTime).count()<<"ms\n";
            return reconstruct(pathCache,pos);
        }

        for(auto dir:dirs){
            std::pair<int,int> next = {pos.first + dir.first, pos.second + dir.second};
            
            if (next.first >= 0 && next.first < map.h && next.second >= 0 && next.second < map.w) {
                if (!visited[next.first][next.second] && map._map[next.first][next.second] != 1) {
                    visited[next.first][next.second] = true;
                    OPEN.push(next);
                    pathCache[next] = pos;
                }
            }
        }
    }
    std::cout<<"NOT FOUND!!!!\n";
    
    std::vector<std::pair<int,int>> path;
    path.push_back(start);
    path.push_back(goal);
    return path;
}

// ------------------------------------------------
//  Heuristica
// ------------------------------------------------

// Heuristica
float Search::Heuristic(std::pair<int, int> start, std::pair<int, int> goal) {
    return std::abs(start.first - goal.first) + std::abs(start.second - goal.second);
}

// Struct 
struct GreedyNode {
    std::pair<int, int> pos;
    float h;

    // Functor para la Priority Queue
    bool operator>(const GreedyNode& other) const {
        return h > other.h;
    }
};

// Algoritmo Greedy
std::vector<std::pair<int, int>> Search::Greedy(const Map& map, std::pair<int, int> start, std::pair<int, int> goal) {
    std::cout<<"===========================\nRunning Greedy BFS...\n";
    auto startTime = std::chrono::high_resolution_clock::now();

    std::vector<std::pair<int,int>> dirs{{-1,0}, {0,1}, {1,0}, {0,-1}};
    std::vector<std::vector<bool>> visited(map.h, std::vector<bool>(map.w, false));

    std::priority_queue<GreedyNode, std::vector<GreedyNode>, std::greater<GreedyNode>> OPEN;
    std::unordered_map<std::pair<int,int>, std::pair<int,int>> pathCache;

    // Agregar el nodo inicial
    OPEN.push({start, Heuristic(start, goal)});
    visited[start.first][start.second] = true;

   
    while(!OPEN.empty()) {
        // Extraer el nodo con la menor heuristica
        auto current = OPEN.top();
        OPEN.pop();
        auto pos = current.pos;

        // Verificar si es la meta
        if(pos == goal){
            auto endTime = std::chrono::high_resolution_clock::now();
            int count = 0;
            for(int i=0; i<map.h; i++) {
                for(int j=0; j<map.w; j++) {
                    if(visited[i][j]) count++;
                }
            }
            std::cout << "VISITED: " << count << std::endl;
            std::cout << "OPEN: " << OPEN.size() << std::endl;
            std::cout << "FOUND in " << std::chrono::duration<double, std::milli>(endTime-startTime).count() << "ms\n";
            return reconstruct(pathCache, pos);
        }

        for(auto dir : dirs) {
            
            std::pair<int,int> next = {pos.first + dir.first, pos.second + dir.second}; 

            // Validar limites
            if(next.first >= 0 && next.first < map.h && next.second >= 0 && next.second < map.w) {
                
                if(!visited[next.first][next.second] && map._map[next.first][next.second] != 1) {
                    visited[next.first][next.second] = true;
                    pathCache[next] = pos;

                    
                    OPEN.push({next, Heuristic(next, goal)});
                }
            }
        }
    }
    std::cout << "NOT FOUND!!!!\n";
    return {start, goal};
}

struct NodeAStart
{
    std:: pair<int, int> pos;
    float f;

    bool operator>(const NodeAStart& other) const{
        return f > other.f;
    }
};

std::vector<std::pair<int,int>> Search::AStart(const Map& map, std::pair<int,int> start, std::pair<int,int> goal)
{
    std::cout << "===========================\nRunning A*...\n";
    auto startTime = std::chrono::high_resolution_clock::now();

    std::vector<std::pair<int,int>> dirs{{-1,0}, {0,1}, {1,0}, {0,-1}};

    std::vector<std::vector<float>> gCost(map.h, std::vector<float>(map.w, std::numeric_limits<float>::infinity()));
    std::vector<std::vector<bool>> closed(map.h, std::vector<bool>(map.w, false));

    std::priority_queue<NodeAStart, std::vector<NodeAStart>, std::greater<NodeAStart>> OPEN;
    std::unordered_map<std::pair<int,int>, std::pair<int,int>> pathCache;

    // Iniciar nodo de inicio
    gCost[start.first][start.second] = 0;  
    float hStart = Heuristic(start, goal);
    OPEN.push({start, hStart});

    while(!OPEN.empty())
    {
        auto current = OPEN.top();
        OPEN.pop();
        auto pos = current.pos;

        // Si ya lo procesamos, lo saltamos
        if (closed[pos.first][pos.second]) continue;
        closed[pos.first][pos.second] = true;

        // Verificamos si llegamos a la meta
        if(pos == goal)
        {
            auto endTime = std::chrono::high_resolution_clock::now();
            int count = 0;
            for(int i = 0; i < map.h; i++)
                for(int j = 0; j < map.w; j++)
                    if (closed[i][j]) count++;

            std::cout << "VISITED: " << count << std::endl;
            std::cout << "OPEN: " << OPEN.size() << std::endl;
            std::cout << "FOUND in " << std::chrono::duration<double, std::milli>(endTime - startTime).count() << "ms\n";
            return reconstruct(pathCache, pos);
        }

        // Exploramos los 4 vecinos  
        for(auto dir : dirs)
        {
            std::pair<int,int> next = {pos.first + dir.first, pos.second + dir.second};

            if (next.first < 0 || next.first >= map.h || next.second < 0 || next.second >= map.w)
                continue;

            if (map._map[next.first][next.second] == 1 || closed[next.first][next.second])
                continue;

            float newG = gCost[pos.first][pos.second] + 1;

            if(newG < gCost[next.first][next.second])
            {
                gCost[next.first][next.second] = newG;
                float h = Heuristic(next, goal);
                float f = newG + h;

                pathCache[next] = pos;
                OPEN.push({next, f});
            }
        }
    }

    std::cout << "NOT FOUND!!!!\n";
    return {start, goal};

}
