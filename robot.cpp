#include <iostream>
#include <fstream>
//#include <cstring>
//#include <time.h>
using namespace std;
class floor;
class node;
class stacks;
class queues;
class point{
    public:
        friend floor;
        friend node;
        friend stacks;
        friend queues;
        void set(int a,int b){
            x=a;y=b;
        }
        void show(){
            cout<<x<<" "<<y<<endl;
        }
    private:
        int x;
        int y;
};
class node{
    public:
        friend floor;
        friend stacks;
        friend queues;
        node(){};
        ~node(){};
        node(int a,int b){
            next=NULL;
            pos.set(a,b);
        }
    private:
        node* next;
        point pos;
};
class queues{
    public:
        friend floor;
        queues(){
            size=0;
            front=NULL;
            back=NULL;
        }
        ~queues(){}
        void push(point);
        void pop();
        bool isempty();
    private:
        node* front;
        node* back;
        int size;
};
bool queues::isempty(){
    return ((front&&back)==0);
}
void queues::push(point x){
    size++;
    if(isempty()){
        front=new node(x.x,x.y);
        back=front;
        return;
    }
    node* tmp=new node(x.x,x.y);
    back->next=tmp;
    back=tmp;
}
void queues::pop(){
    if(isempty()) return;
    node* tmp=front;
    front=front->next;
    delete tmp;
    tmp=NULL;
    size--;
}
class stacks{
    public:
        friend floor;
        stacks(){
           size=0;
           top=NULL;
        }
        ~stacks(){};
        bool isempty();
        point Top();
        void push(point);
        void pop();
    private:
        node* top;
        int size;
};
bool stacks::isempty(){
    return (size==0);
}
point stacks::Top(){
    return top->pos;
}
void stacks::push(point x){
    size++;
    if(!top){
        top=new node(x.x,x.y);
        top->next=NULL;
    }
    else{
        node* tmp=new node(x.x,x.y);
        tmp->next=top;
        top=tmp;
    }
}
void stacks::pop(){
    if(isempty())return;
    node* tmp=top;
    top=top->next;
    delete tmp;
    tmp=NULL;
    size--;
}
class floor{
    public:
        floor(){
            rows=0; cols=0; battery=0;step=0;
        }
        ~floor(){}
        void read_file();
        void set_list();
        void find_path();     //DFS
        void BFS();
        int findmin(node*,bool*);
        void finddest(int,int ,bool*,stacks&,stacks&);
        void connect(node*);
        void showlist();
        void outfile();
    private:
        int rows;
        int cols;
        int battery;
        char** map;
        point recharge;
        node** pt;
        int* shortest;
        int** dist;
        node* answerlist;
        node* listback;
        int step;
};
void floor::find_path(){
    stacks s;
    stacks un;
    bool* visited=new bool[rows*cols];
    int battery_now=battery;
    for(int i=0;i<rows*cols;i++){
        visited[i]=false;
    }
    answerlist=new node(recharge.x,recharge.y);
    listback=answerlist;
    if(pt[recharge.x*cols+recharge.y]->next)
        s.push(recharge);
    else
        connect(pt[recharge.x*cols+recharge.y]);
    
    while (!s.isempty()){
        node* second;
        if(battery_now>dist[s.top->pos.x][s.top->pos.y]){
            int coord=s.top->pos.x*cols+s.top->pos.y;
            node* current=pt[coord];
            second=current;
            if(!visited[coord]){
                visited[coord]=true;
                battery_now--;
                connect(current);
                if(pt[coord]->pos.x==recharge.x&&pt[coord]->pos.y==recharge.y)
                    battery_now=battery;
            }
            int allvisit=1;
            int flag=0;
            while(current){
                if(!visited[current->pos.x*cols+current->pos.y]){
                    allvisit=0;
                    if(!flag){
                        s.push(current->pos);
                        flag=1;
                    }
                    if(flag)
                        un.push(current->pos);
                }
                current=current->next;
            }
            if(allvisit){          //走回去
                if(!s.isempty()&&battery_now>dist[s.top->pos.x][s.top->pos.y]){
                    s.pop();
                }
                if(s.top &&battery_now>dist[s.top->pos.x][s.top->pos.y]){
                    connect(s.top);
                    battery_now--;
                    second=pt[s.top->pos.x*cols+s.top->pos.y];
                    if(dist[s.top->pos.x][s.top->pos.y]==1){
                        while(!s.isempty()){
                            if(!visited[s.top->pos.x*cols+s.top->pos.y]){
                                un.push(s.top->pos);
                            }
                            s.pop();
                        }
                        while(!un.isempty()){
                            if(!visited[un.top->pos.x*cols+un.top->pos.y]){
                                battery_now=battery;
                                visited[un.top->pos.x*cols+un.top->pos.y]=true;
                                battery_now=battery_now-dist[un.top->pos.x][un.top->pos.y];
                                second=pt[un.top->pos.x*cols+un.top->pos.y];
                                int a=un.top->pos.x;
                                int b=un.top->pos.y;
                                un.pop();
                                finddest(a,b,visited,s,un);
                                break;
                            }
                            else un.pop();
                        }
                    }
                }
            }
        }
        else{
            int now=findmin(second,visited);
            visited[now]=true;
            node* tmp=pt[now];
            while(tmp){
                if(!visited[tmp->pos.x*cols+tmp->pos.y]){
                    un.push(tmp->pos);
                }
                tmp=tmp->next;
            }
            battery_now--;
            second=pt[now];
            if(pt[now]->pos.x!=recharge.x||pt[now]->pos.y!=recharge.y)
                connect(pt[now]);
            if(pt[now]->pos.x==recharge.x&&pt[now]->pos.y==recharge.y){
                battery_now=battery;
                while(!s.isempty()){
                    if(!visited[s.top->pos.x*cols+s.top->pos.y]){
                        un.push(s.top->pos);
                    }
                    s.pop();
                }
                while(!un.isempty()){
                    if(!visited[un.top->pos.x*cols+un.top->pos.y]){
                        visited[un.top->pos.x*cols+un.top->pos.y]=true;
                        battery_now=battery_now-dist[un.top->pos.x][un.top->pos.y];
                        second=pt[un.top->pos.x*cols+un.top->pos.y];
                        int a=un.top->pos.x;
                        int b=un.top->pos.y;
                        un.pop();
                        finddest(a,b,visited,s,un);
                        break;
                    }
                    else{
                        un.pop();
                    }
                }
            }
        }
    }
    if(listback->pos.x!=recharge.x||listback->pos.y!=recharge.y){
        node* current=new node(recharge.x,recharge.y);
        connect(current);
    }
}
void floor::outfile(){
    ofstream fout;
    fout.open("final.path",ios::out);
    node* tmp=answerlist->next;
    fout<<step-1<<endl;
    while(tmp){
        node* todel=tmp;
        fout<<tmp->pos.x<<" "<<tmp->pos.y<<endl;
        tmp=tmp->next;
        delete todel;
        todel=NULL;
    }
    fout.close();
}
void floor::connect(node* x){
    node* tmp=new node(x->pos.x,x->pos.y);
    listback->next=tmp;
    listback=tmp;
    step++;
}
void floor::finddest(int a,int b,bool* visited,stacks& s,stacks& un){
    int small;
    node* current=pt[a*cols+b];
    node* tmp=new node(a,b);
    node* last=tmp;
    node* head;
    stacks k;
    while(current->pos.x!=recharge.x||current->pos.y!=recharge.y){
        k.push(current->pos);
        small=findmin(current,visited);
        current=pt[small];
        visited[current->pos.x*cols+current->pos.y]=true;
        head=new node(current->pos.x,current->pos.y);
        head->next=tmp;
        tmp=head;
        step++;
    }
    while(!k.isempty()){
        s.push(k.top->pos);
        node* tmp=pt[k.top->pos.x*cols+k.top->pos.y];
        while(tmp){
            if(!visited[tmp->pos.x*cols+tmp->pos.y]){
                un.push(tmp->pos);
            }
            tmp=tmp->next;
        }
        k.pop();
    }
    listback->next=head;
    listback=last;
    step++;
}
int floor::findmin(node* current,bool* visited){
    node* tmp=pt[current->pos.x*cols+current->pos.y]->next;
    int dis=dist[tmp->pos.x][tmp->pos.y];
    int small=tmp->pos.x*cols+tmp->pos.y;
    while(tmp){
        if(dis>dist[tmp->pos.x][tmp->pos.y]){
            dis=dist[tmp->pos.x][tmp->pos.y];
            small=tmp->pos.x*cols+tmp->pos.y;
            if(!visited[tmp->pos.x*cols+tmp->pos.y]){
                break;
            }
        }
        tmp=tmp->next;
    }
    return small;
}
void floor::BFS(){
    queues q;
    bool* visited=new bool[rows*cols];
    for(int i=0;i<rows*cols;i++){
        visited[i]=false;
    }
    dist=new int*[rows];
    for(int i=0;i<rows;i++)
        dist[i]=new int[cols];
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            dist[i][j]=-1;
        }
    }
    dist[recharge.x][recharge.y]=0;
    q.push(recharge);
    while(!q.isempty()){
        int coord=q.front->pos.x*cols+q.front->pos.y;
        node* current=pt[coord];
        node* u=pt[coord];
        if(!visited[coord]){
            visited[coord]=true;
        }
        q.pop();
        while(current){
            if(!visited[current->pos.x*cols+current->pos.y]){
                visited[current->pos.x*cols+current->pos.y]=true;
                q.push(current->pos);
                dist[current->pos.x][current->pos.y]=dist[u->pos.x][u->pos.y]+1;
            }
            current=current->next;
        }
    }
   /*ofstream fBFS;
    fBFS.open("bfs.txt",ios::out);

    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(dist[i][j]==-1)
                fBFS<<"   ";
            else
                fBFS<<dist[i][j]<<"  ";
        }
        fBFS<<endl;
    }
    fBFS.close();*/
}
void floor::set_list(){
    pt=new node*[rows*cols];
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++)
            if(map[i][j]!='1')
                pt[i*cols+j]=new node(i,j);
    }
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(map[i][j]=='0'||map[i][j]=='R'){
                if(map[i][j]=='R')
                    recharge.set(i,j);
                node* current=pt[i*cols+j];
                 if(i>0 && j<cols-1 && i<rows-1 && j>0){
                    if(map[i-1][j]=='0'||map[i-1][j]=='R'){
                        node* tmp=new node(i-1,j);
                        current->next=tmp;
                        current=current->next;
                    }
                    if(map[i][j-1]=='0'||map[i][j-1]=='R'){
                        node* tmp=new node(i,j-1);
                        current->next=tmp;
                        current=current->next;
                    }
                    if(map[i+1][j]=='0'||map[i+1][j]=='R'){
                        node* tmp=new node(i+1,j);
                        current->next=tmp;
                        current=current->next;
                    }
                    if(map[i][j+1]=='0'||map[i][j+1]=='R'){
                        node* tmp=new node(i,j+1);
                        current->next=tmp;
                        current=current->next;
                    }
                 }
                else{
                    if(i==0&&map[i+1][j]=='0'){
                        node* tmp=new node(i+1,j);
                        current->next=tmp;
                        current=current->next;
                    }
                    else if(j==0&&map[i][j+1]=='0'){
                        node* tmp=new node(i,j+1);
                        current->next=tmp;
                        current=current->next;
                    }
                    else if(i==rows-1&&map[i-1][j]=='0'){
                        node* tmp=new node(i-1,j);
                        current->next=tmp;
                        current=current->next;
                    }
                    else if(j==cols-1&&map[i][j-1]=='0'){
                        node* tmp=new node(i,j-1);
                        current->next=tmp;
                        current=current->next;
                    }
                }
            }
            else if(map[i][j]=='1'){
                pt[i*cols+j]=NULL;
            }
        }
    }
}
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
    fin.close();
}
int main(){
    floor f;
    f.read_file();
    f.set_list();
    f.BFS();
    f.find_path();
    f.outfile();
    //cout <<(double)clock()/CLOCKS_PER_SEC << " S" ;
    return 0;
}
