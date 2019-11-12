#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>
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
            x=a;
            y=b;
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
        void finddest(node*,bool*,stacks&,stacks&);
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
    s.push(recharge);
    answerlist=new node(recharge.x,recharge.y);
    listback=answerlist;


    while (!s.isempty()){
        node* second;
        node* tmptop;
  /*  if(!un.isempty()){
        cout<<"hrhhrrhhrhr";un.top->pos.show();
    }*/

        if(battery_now>battery/2){
            int coord=s.top->pos.x*cols+s.top->pos.y;
            node* current=pt[coord];
            if(!visited[coord]){
                visited[coord]=true;
                battery_now--;
                //cout<<"1eaf";current->pos.show();
                connect(current);
                if(battery_now==battery/2)
                    tmptop=current;
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
                if(!s.isempty()&&battery_now>battery/2){
                    s.pop();
                }
                if(s.top &&battery_now>battery/2){
                   // cout<<"tie:;";s.top->pos.show();

                        connect(s.top);
                        battery_now--;
                        if(battery_now==battery/2)
                            tmptop=pt[s.top->pos.x*cols+s.top->pos.y];
                        
                        if(dist[s.top->pos.x][s.top->pos.y]==1){
                            while(!s.isempty()){
                                if(!visited[s.top->pos.x*cols+s.top->pos.y]){                   
                                    //cout<<"gegegeg"; s.top->pos.show();
                                    un.push(s.top->pos);
                                }
                                s.pop();
                            }   
                           // cout<<"k";                           
                            while(!un.isempty()){
                                if(!visited[un.top->pos.x*cols+un.top->pos.y]){
                                    battery_now=battery; 
                                  //   cout<<"eih";
                                 //  cout<<"top"; un.top->pos.show();
                                    visited[un.top->pos.x*cols+un.top->pos.y]=true;
                                    battery_now=battery_now-dist[un.top->pos.x][un.top->pos.y];
                                    if(battery_now==battery/2){
                                        tmptop=pt[un.top->pos.x*cols+un.top->pos.y];    
                                    }
                                    node* tmp=un.top;
                                    un.pop();
                                    finddest(tmp,visited,s,un);
                                 //   cout<<"eih";
                                    break;
                                }
                                else un.pop();
                            }
                        }
                            
                        
                }
            }
            
        }
        else{
            if(battery_now==battery/2){
                //int coord=tmptop->pos.x*cols+tmptop->pos.y;
                node* current=tmptop;
             //   cout<<"now";
          //  cout<<"efefeEeefe";tmptop->pos.show();
                int now=findmin(current,visited);
                visited[now]=true;
                node* tmp=pt[now];
                while(tmp){
                    if(!visited[tmp->pos.x*cols+tmp->pos.y]){
                        un.push(tmp->pos);
                    }
                    tmp=tmp->next;
                }
                battery_now--;
            //pt[now]->pos.show();
                if(pt[now]->pos.x!=recharge.x||pt[now]->pos.y!=recharge.y)
                    connect(pt[now]);
                if(pt[now]->pos.x==recharge.x&&pt[now]->pos.y==recharge.y)
                    battery_now=battery;
                second=pt[now];
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
            //pt[now]->pos.show();
                if(pt[now]->pos.x!=recharge.x||pt[now]->pos.y!=recharge.y)
                    connect(pt[now]);
                if(pt[now]->pos.x==recharge.x&&pt[now]->pos.y==recharge.y)
                    battery_now=battery;
                second=pt[now];
            }
            if(second->pos.x==recharge.x &&second->pos.y==recharge.y){
                battery_now=battery;
                while(!s.isempty()){
                    if(!visited[s.top->pos.x*cols+s.top->pos.y]){                   //每個點都該進行深度 ..
                       // cout<<"gegegeg"; s.top->pos.show();
                        un.push(s.top->pos);
                    }
                        
                    s.pop();
                }
                while(!un.isempty()){
                    if(!visited[un.top->pos.x*cols+un.top->pos.y]){
                   //      cout<<"eih";
                  //     cout<<"top"; un.top->pos.show();
                        
                        visited[un.top->pos.x*cols+un.top->pos.y]=true;
                        battery_now=battery_now-dist[un.top->pos.x][un.top->pos.y];
                        if(battery_now==battery/2){
                            tmptop=pt[un.top->pos.x*cols+un.top->pos.y];    
                        }
                        node* tmp=un.top;
                        un.pop();
                        finddest(tmp,visited,s,un);
                     
                   //     cout<<"efea";un.top->pos.show();
        
                     //   cout<<"eih";
                        
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

    /*while(!s.isempty()){
        int coord=s.top->pos.x*cols+s.top->pos.y;

        node* current=pt[coord];
        s.pop();
        if(!visited[coord]){
            visited[coord]=true;
            current->pos.show();
        }
        int allvisit=0;
        while(current){
            if(!visited[current->pos.x*cols+current->pos.y]){
                s.push(current->pos);
                allvisit=1;
                //current->pos.show();
            }
            current=current->next;
        }

    }*/

}
void floor::outfile(){
    ofstream fout;
    fout.open("final.path",ios::out);
    node* tmp=answerlist->next;
    
    fout<<step-1<<endl;
    
    while(tmp){
        fout<<tmp->pos.x<<" "<<tmp->pos.y<<endl;
        tmp=tmp->next;
    }
    fout.close();
}
void floor::connect(node* x){
    node* tmp=new node(x->pos.x,x->pos.y);
    listback->next=tmp;
    listback=tmp;
    step++;
}
void floor::showlist(){
    cout<<step<<endl;
    node* tmp=answerlist->next;
    int s=0;
    while(tmp){
        tmp->pos.show();
        s++;
        tmp=tmp->next;
    }
    cout<<s<<endl;
}
void floor::finddest(node* dest,bool* visited,stacks& s,stacks& un){
    int small;
    node* current=pt[dest->pos.x*cols+dest->pos.y];
    node* tmp=new node(dest->pos.x,dest->pos.y);
    node* last=tmp;
    node* head;
    stacks k;
    //cout<<"efew";dest->pos.show();
    while(current->pos.x!=recharge.x||current->pos.y!=recharge.y){
        k.push(current->pos);
        small=findmin(current,visited);
        current=pt[small];
        visited[current->pos.x*cols+current->pos.y]=true;
      //  cout<<"e,.,.,";   current->pos.show();
        head=new node(current->pos.x,current->pos.y);
        head->next=tmp;
        tmp=head;
        step++;
    }
    while(!k.isempty()){
        s.push(k.top->pos);
        //cout<<"s.top";s.top->pos.show();
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
    node* tmp=current->next;

    int dis=dist[tmp->pos.x][tmp->pos.y];
    int small=tmp->pos.x*cols+tmp->pos.y;
    while(tmp){
        if(dis>dist[tmp->pos.x][tmp->pos.y]){

            dis=dist[tmp->pos.x][tmp->pos.y];
            small=tmp->pos.x*cols+tmp->pos.y;
            //cout<<"whilar";tmp->pos.show();
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
        //q.front->pos.show();
        int coord=q.front->pos.x*cols+q.front->pos.y;
        node* current=pt[coord];
        node* u=pt[coord];
        if(!visited[coord]){
            visited[coord]=true;
        }
        q.pop();
       // cout<<q.isempty();
        while(current){
            if(!visited[current->pos.x*cols+current->pos.y]){
                visited[current->pos.x*cols+current->pos.y]=true;
                q.push(current->pos);
                dist[current->pos.x][current->pos.y]=dist[u->pos.x][u->pos.y]+1;
            }
            current=current->next;
        }
    }

    ofstream fBFS;
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
    fBFS.close();
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
                    if(i==0){
                        node* tmp=new node(i+1,j);
                        current->next=tmp;
                        current=current->next;
                    }
                    else if(j==0){
                        node* tmp=new node(i,j+1);
                        current->next=tmp;
                        current=current->next;
                    }
                    else if(i==rows-1){
                        node* tmp=new node(i-1,j);
                        current->next=tmp;
                        current=current->next;
                    }
                    else if(j==cols-1){
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

  /* for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(pt[i*cols+j]){
                node* temp=pt[i*cols+j];
                while(temp){
                    cout<<temp->pos.x<<" "<<temp->pos.y<<",";
                    temp=temp->next;
                }
                cout<<endl;
            }
        }
    }
*/
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
    /*for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            cout<<map[i][j];
        }cout<<endl;
    }*/
    fin.close();
}
int main(){
    floor f;
    f.read_file();
    f.set_list();
    f.BFS();
    f.find_path();
//cout<<"lalalla"<<endl;
    //f.showlist();
    f.outfile();
//cout << (double)clock() / CLOCKS_PER_SEC << "S"<<endl;
//cout<<endl;
    return 0;
}
