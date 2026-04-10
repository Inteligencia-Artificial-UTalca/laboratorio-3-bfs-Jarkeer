#include "Search.h"
#include <queue>
#include <chrono>
#include <unordered_map>
#include <iostream>
#include <limits>
#include <deque>

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

std::vector<std::pair<int,int>> Search::reconstruct(const std::unordered_map<std::pair<int,int>,std::pair<int,int>> &pathCache, const std::pair<int,int> &start){
	std::deque<std::pair<int,int>> nodes;
	auto node = start;//make copy

    //traverse path from goal to start

    //
	while(pathCache.find(node) != pathCache.end()){
        nodes.push_front(node);
        node = pathCache.at(node);
	}
    nodes.push_front(node);

    //revert path and return it
    std::vector<std::pair<int,int>> vec;
    for(auto p:nodes){
        vec.push_back(p);
    }
    return vec;
}

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
        //get node
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
			//copy the position
            std::pair<int,int> next = {pos.first + dir.first, pos.second + dir.second};
            //then move it
            //if illegal or visited, skip it
            if (next.first >= 0 && next.first < map.h && next.second >= 0 && next.second < map.w) {
                if (!visited[next.first][next.second] && map._map[next.first][next.second] != 1) {
           
            
            //add child to open list
                    visited[next.first][next.second] = true;
                    OPEN.push(next);
            //register path
            pathCache[next] = pos;
                }
            }
        
		}
	}
	std::cout<<"NOT FOUND!!!!\n";
    
    //let's just return start and goal to draw them
    std::vector<std::pair<int,int>> path;
    path.push_back(start);
    path.push_back(goal);
    return path;
}
