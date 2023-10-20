#include <bits/stdc++.h>

using namespace std;


struct Cell {
    pair<int, int> coord;
    vector <int> neighbours;
    int status;
    int index;
    Cell(pair<int, int> coord, int index) {
        coord = coord;
        index = index;
        status = 0; // -2 - стена, 0 - неизвестно, 1 - известно не всё, 2 - известно всё
        neighbours = {-1, -1, -1, -1}; //-1 не проверели,  -2 - стена
    }
    Cell() {}
};

vector<Cell> our_labyrinth;
map <pair<int, int>, int> correspondence;


int AskAboutNeighbour(){
    cout << 1 << '\n';
    int answer;
    std::cin >> answer;
    return answer;
}

pair<int, int> NewCoord(pair<int, int> coord, int direction){
    if (direction == 0){
        return {coord.first, coord.second + 1}; //вверх
    }
    if (direction == 1){
        return {coord.first + 1, coord.second}; //вправо
    }
    if (direction == 2){
        return {coord.first, coord.second - 1}; //вниз
    }
    return {coord.first - 1, coord.second}; //влево
}

int UpdateStatus(Cell my_cell){
    for (int i = 0; i < 4; ++i){
        if (my_cell.neighbours[i] == -1){
            return 0;
        }
    }
    return 1;
}

struct ForBFS{
    Cell cell;
    vector<pair<int, int>> path;
    ForBFS(Cell cell, vector<pair<int, int>> path){
        cell = Cell(cell.coord, cell.index);
        path = path;
    }
};

ForBFS BFS(Cell now_cell, int A, int B){
    queue<pair<Cell, vector<pair<int, int>>>> Q;
    vector<bool> visit(our_labyrinth.size(), false);
    visit[now_cell.index] = true;
    Q.push({now_cell, vector<pair<int, int>>()});
    while (!Q.empty()) {
        auto now = Q.front();
        Q.pop();
        now.first.status = UpdateStatus(now.first);
        now.second.push_back(now.first.coord);
        if (now.first.status == 1) {
            return ForBFS(now.first, now.second);
        }
        visit[now.first.index] = true;
        if (now.first.status == 2) {
            for (int i = 0; i < 4; ++i){
                int ind = now.first.neighbours[i];
                if (!visit[ind]){
                    Q.push({our_labyrinth[ind], now.second});
                }
            }
        }
    }
    return ForBFS(now_cell, vector<pair<int, int>>());
    
}

long long GoOnPath(vector<pair<int, int>> path, int direstion, int A, int B){
    long long time = 0;
    int size = path.size();
    for(int i = 1; i < size; i++){
        int new_direction = 0;
        if (path[i].first - path[i - 1].first == 1) {
            new_direction = 1;
        }
        if (path[i].first - path[i - 1].first == -1) {
            new_direction = 3;
        }
        if (path[i].second - path[i - 1].second == -1) {
            new_direction = 2;
        }
        time += (A + B * abs(direstion - new_direction));
        for (int k = 0; k < abs(direstion - new_direction); ++k) {
            cout << 2 << '\n';
            int a;
            std::cin >> a;
        }
        cout << 1 << '\n';
        int a;
        std::cin >> a;
        direstion = new_direction;
    }
    return time;
}

int Bypass (Cell now_cell, long long time, int direction, int A, int B) {
    now_cell.status = UpdateStatus(now_cell);
    if (now_cell.status == 2){
        auto res = BFS(now_cell, A, B);
        if (res.path.size() == 0){
            return time;
        }
        time += GoOnPath(res.path, direction, A, B);
        return Bypass(res.cell, time, direction, A, B);
    }
    for (int i = 0; i < 4; ++i){
        if(now_cell.neighbours[direction + i] == -1){
            if (AskAboutNeighbour() == 1) { // идем вперед в новую вершину;
                pair<int, int> my_coord = NewCoord(now_cell.coord, direction);
                int my_index = our_labyrinth.size();
                Cell new_cell(my_coord, my_index);
                correspondence[my_coord] = my_index;
                for(int j = 0; j < 4; ++j){
                    if(correspondence.count(NewCoord(my_coord, j))) {
                        int ind = correspondence[NewCoord(my_coord, j)];
                        new_cell.neighbours[j] = ind;
                        our_labyrinth[ind].neighbours[(j + 2) % 4] = my_index;
                    }
                }
                our_labyrinth.push_back(new_cell);
                time += (A + B * i);
                return Bypass(new_cell, time, direction, A, B);
            }
        } else {
            now_cell.neighbours[direction] = -2;
        }
        cout << 2 << '\n';
        int a;
        std::cin >> a;
        direction = (direction + 1) % 4;
    }
}



int main()
{
    int x_start, y_start, x_next, y_next, A, B, C, K;
    std::cin >> x_start >> y_start >> x_next >> y_next >> A >> B >> C >> K;
    int direction = 0;
    if (x_start - x_next == -1){
        direction = 1;
    }
    if (y_start - y_next == 1){
        direction = 2;
    }
    if (x_start - x_next == 1){
        direction = 3;
    }
    Cell start({0, 0}, 0);
    our_labyrinth.push_back(start);
    correspondence[{0, 0}] = 0;
    long long time = Bypass(start, 0, direction, A, B);
    cout << 4 << " " << time << '\n';
    return 0;
}
