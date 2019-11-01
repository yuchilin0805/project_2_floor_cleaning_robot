#include <iostream>
#include <fstream>

using namespace std;

class floor{
    public:
        floor(){
            rows=0;
            cols=0;
            battery=0;
        }
        void read_file();
    private:
        int rows;
        int cols;
        int battery;
        char** map;
};

void floor::read_file(){
    ifstream fin("floor.data");
    fin>>rows>>cols>>battery;

    map=new char*[rows];
    for(int i=0;i<rows;i++){
        map[i]=new char [cols];
    }
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            fin>>map[i][j];
        }
    }
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            cout<<map[i][j];
        }cout<<endl;
    }

}
int main(){
    floor f;
    f.read_file();


    return 0;
}