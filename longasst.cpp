#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
using namespace std;

struct TreeNode{ 
    // nodes of the tree , ie versions of the file
    public:
    int version_id;
    string content;
    string message;
    time_t created_timestamp;
    time_t snapshot_timestamp;
    TreeNode* parent;
    vector<TreeNode*> children;

    TreeNode(){
        version_id=0;
        content="";
        message="";
        parent=NULL;
        snapshot_timestamp=0;
        created_timestamp= std::time(0);
        // put current time
    }

};


class Tree{ 
    // for each file 
    public:
    string name;
    TreeNode* root;
    TreeNode* active_version;
    time_t updated_timestamp;
    vector<TreeNode*> version_map; 
    // ^ to store the versions by unique ID (index)

    int biggest_tree_index;
    int recent_files_index;

    // ^ to keep track of the file's positions in the heaps 

    int total_versions;

    Tree(string name){
        this-> biggest_tree_index=-1;
        this->recent_files_index=-1;
        this->name=name;
        updated_timestamp=std::time(0);

        TreeNode* cur= new TreeNode;
        cur->message="initial snapshot :)";
        cur->content= "";
        cur->version_id=0;
        version_map.push_back(cur); 
        cur->snapshot_timestamp=std::time(0);
        total_versions=version_map.size();
        active_version=cur;
        root=cur;
    }

    void read(){
        cout<< active_version->content<<endl;
    }

    void insert(string n){
        string c;

        if (active_version->content.empty()){
            c=n;
        }
        else {
            c=" " + n;
        }

        this->updated_timestamp=std::time(0);
        
        if (active_version->snapshot_timestamp==0){
            active_version->content+=c;
        }
        else{
            TreeNode* cur= new TreeNode;
            active_version->children.push_back(cur);
            cur->parent= active_version;
            cur->content=active_version->content+ c;
            version_map.push_back(cur);
            cur->version_id=version_map.size()-1;
            active_version=cur;
        }
        total_versions=version_map.size();
        
    }

    void update(string c){
        this->updated_timestamp=std::time(0);

        if (active_version->snapshot_timestamp==0){
            active_version->content=c;
        }
        else{
            TreeNode* cur= new TreeNode;
            active_version->children.push_back(cur);
            cur->parent= active_version;
            cur->content=c;
            version_map.push_back(cur);
            cur->version_id=version_map.size()-1;
            active_version=cur;
        }
        total_versions=version_map.size();
        
    }


    void snapshot(string m){

        if (active_version->snapshot_timestamp==0){
            active_version->snapshot_timestamp =std::time(0); 
            active_version->message=m;
            cout<<"Snapshotted!"<<endl;
        }
        else{
            cout<<"Already a snapshot!"<<endl;
        }
        total_versions=version_map.size();
    }

    void rollback(int u){
        if (u==-1){
            if (active_version!=root){
                active_version= active_version->parent;
                cout<<"Rolled back to the parent!"<<endl;
            }
            else{
                std::cout<< "Rollback not possible, already at root!"<<endl;
            }
        }
        else{
            if (u>=version_map.size()|| u<0){
                std::cout<< "Index out of range"<< endl; 
                return;
            }
            active_version=version_map[u];
            cout<< "Rolled back to "<< u << " successfully"<<endl;
        }
        total_versions=version_map.size();
    }

    void history(){

        TreeNode* cur=active_version;
        vector<TreeNode*> f;
        while(cur!=NULL){
            f.push_back(cur);
            cur=cur->parent;
        }

        // from root to active version

        while(!f.empty()){
            if (f[f.size()-1]->snapshot_timestamp!=0){
                std::cout<< "The version ID is : "<< f[f.size()-1]->version_id<< " "
                << " | Snapshot timestamp is : "<< ctime(&f[f.size()-1]->snapshot_timestamp)
                << " | Creation timestamp is : "<< ctime(&f[f.size()-1]->created_timestamp)
            
                << " | Message is : "<< f[f.size()-1]->message<<endl;
            }
            f.pop_back();
        }
        total_versions=version_map.size();
    }
    
};

// two heaps : one each for biggest trees and recent files

class biggest_trees{

    vector <Tree*> v;

    public:
    

    void insert(Tree* g){
        v.push_back(g);
        g->biggest_tree_index=v.size()-1;
        heapifyup(v.size()-1);
    }

    void update(int n){
        heapifyup(n); 
        // we are only increasing the no. of versions so it can go only up
    }

    void heapifyup(int n){
        if ((n-1)>-1 && v[(n-1)/2]->total_versions<v[n]->total_versions||
            ((n-1)>-1 && v[(n-1)/2]->total_versions==v[n]->total_versions && v[(n-1)/2]->name>v[n]->name)){
            
            swap(v[(n-1)/2],v[n]);
            v[(n-1)/2]->biggest_tree_index=(n-1)/2;
            v[n]->biggest_tree_index=n;
            heapifyup((n-1)/2);
        }
    }

    void heapifydown(int n){

        int parent =n;
        int left=2*n+1;
        int right=2*n+2;

        if (left<v.size()&& (( v[left]->total_versions>v[parent]->total_versions)
            || ((v[left]->total_versions == v[parent]->total_versions) && (v[left]->name < v[parent]->name)))){
            parent=left;
        }
        if (right<v.size()&& ((v[right]->total_versions>v[parent]->total_versions)
            || ((v[right]->total_versions == v[parent]->total_versions) && (v[right]->name < v[parent]->name)))){
            parent=right;
        }
        if (parent!=n){
            swap(v[parent],v[n]);
            v[parent]->biggest_tree_index=parent;
            v[n]->biggest_tree_index=n;
            
            heapifydown(parent);
        }
    }

    void popfirst(){
        if (v.size()!=0){
            v[0]=v[v.size()-1];
            v[0]->biggest_tree_index = 0;
            v.pop_back();
            if (v.size()!=0){
                heapifydown(0);
            }
        }
    }

    void BIGGEST_TREES(int num){
        
        if (num<=0){
            cout<<"Invalid number"<<endl;
            return;
        }
        if (num>v.size()){
            std::cout<< "Number is greater than the number of files, here are all the files though!"<<endl;
        } 

        int h=min(int(v.size()),num);
        vector<Tree*> x;
        int i=0;
      
        while (i<h){
            x.push_back(v[0]);
            std::cout<< v[0]->name<< " ";
            popfirst();
            i++;
        }

        while (!x.empty()){
            v.push_back(x[x.size()-1]);
            v[v.size()-1]->biggest_tree_index=v.size()-1;
            heapifyup(v.size()-1);
            x.pop_back();
        }

        std::cout<<endl;
    }
};

class recent_files{

    vector <Tree*> v;

    public:
    

    void insert(Tree* g){
        v.push_back(g);
        g->recent_files_index=v.size()-1;
        heapifyup(v.size()-1);
    }

    void update(int n){
        heapifyup(n);
    }

    void heapifyup(int n){
        
        if ((n-1)>-1 && v[(n-1)/2]->updated_timestamp<v[n]->updated_timestamp||
        ((n-1)>-1 && v[(n-1)/2]->updated_timestamp==v[n]->updated_timestamp && v[(n-1)/2]->name>v[n]->name)){
            swap(v[(n-1)/2],v[n]);
            v[(n-1)/2]->recent_files_index=(n-1)/2;
            v[n]->recent_files_index=n;
            heapifyup((n-1)/2);
        }
    }

    void heapifydown(int n){

        int parent =n;
        int left=2*n+1;
        int right=2*n+2;

        if (left<v.size()&& ((v[left]->updated_timestamp>v[parent]->updated_timestamp)
            || ((v[left]->updated_timestamp == v[parent]->updated_timestamp) && (v[left]->name < v[parent]->name)))){
            parent=left;
        }
        if (right<v.size()&& ((v[right]->updated_timestamp>v[parent]->updated_timestamp)
            || ((v[right]->updated_timestamp== v[parent]->updated_timestamp) && (v[right]->name < v[parent]->name)))){
            parent=right;
        }
       

        if (parent!=n){
            swap(v[parent],v[n]);
            v[parent]->recent_files_index=parent;
            v[n]->recent_files_index=n;
            
            heapifydown(parent);
        }
    }

    
    void popfirst(){
        if (v.size()!=0){
            v[0]=v[v.size()-1];
            v[0]->recent_files_index = 0;
            v.pop_back();
            if (v.size()!=0){
                heapifydown(0);
            }
        }
    }

    void RECENT_FILES(int num){

        if (num<=0){
            cout<<"Invalid number"<<endl;
            return;
        }

        if (num>v.size()){
            std::cout<< "Number is greater than the number of files, here are all the files though!"<<endl;
        }

        int h=min(int(v.size()),num);
        vector<Tree*> x;
        int i=0;

        while (i<h){
            x.push_back(v[0]);
            std::cout<< v[0]->name<< " ";
            popfirst();
            i++;
        }
        while (!x.empty()){
            v.push_back(x[x.size()-1]);
            v[v.size()-1]->recent_files_index=v.size()-1;
            heapifyup(v.size()-1);
            x.pop_back();
        }
        std::cout<<endl;
    }
};


// for storing files along with their pointers

class hashmap{

    public:
    vector<vector<pair<string,Tree*> > > mymap;

    hashmap(){
        mymap.resize(1000);
    }

    void insert(string filename, Tree* root){

        int index=0;
        int fc=filename.size();
        for (int i=0;i<fc;i++){
            index*=23;
            index+=(filename[i]-0);
            index%=1000;
        }
        mymap[index].push_back(make_pair(filename,root));
    }

    Tree* find(string filename){

        int index=0;
        int fc=filename.size();
        for (int i=0;i<fc;i++){
            index*=23;
            index+=(filename[i]-0);
            index%=1000;
        }

        int fg= mymap[index].size();
        for (int j=0;j<fg;j++){
            pair<string,Tree*> y = mymap[index][j];
            if (y.first==filename){
                return y.second;
            }          
        }
        return NULL;
    }

};

int main() {
    biggest_trees BT;
    recent_files RF;
    hashmap mymap;
    string line;

    while (getline(cin, line)) {

        if (line.empty()) {continue;}

        istringstream iss(line);
        string command;
        iss >> command;

        if (command == "EXIT"){
            cout<< "byeee!"<<endl;
            break;
        }

        if (command == "CREATE") {
            string fname;
            iss >> fname;
            if (!mymap.find(fname)) {
                Tree* cur= new Tree(fname);
                mymap.insert(fname,cur);
                BT.insert(cur);
                RF.insert(cur);
                cout << "File " << fname << " is created"<<endl;
            } 
            else {
                cout << "File " << fname << " already exists!"<<endl;
            }
        }

        else if (command == "INSERT"){
            string fname;
            iss >> fname;
            string content;
            getline(iss, content);

            if (mymap.find(fname)){

                if (!content.empty() && content[0] == ' ')
                    content.erase(0, 1);

                Tree* t = mymap.find(fname);
                t->insert(content);
                BT.update(t->biggest_tree_index);
                RF.update(t->recent_files_index);
                cout<< "Inserted " << content<< " into "<< fname<<endl;
            }

            else{
                cout << "File doesn't exist!" << endl;
            }
        }

        else if (command == "UPDATE"){
            string fname;
            iss >> fname;
            string content;
            getline(iss, content);

            if (mymap.find(fname)){

                if (!content.empty() && content[0] == ' ')
                    {content.erase(0, 1);}

                Tree* t = mymap.find(fname);
                t->update(content);
                BT.update(t->biggest_tree_index);
                RF.update(t->recent_files_index);
                cout<< "Updated "<< fname<<endl;
            }

            else{
                cout << "File doesn't exist!" << endl;
            }
        }

        else if (command == "SNAPSHOT"){

            string fname;
            iss >> fname;
            string content;
            getline(iss, content);

            if (mymap.find(fname)){

                if (!content.empty() && content[0] == ' ')
                    {content.erase(0, 1);}

                Tree* t = mymap.find(fname);
                t->snapshot(content);
                BT.update(t->biggest_tree_index);
                RF.update(t->recent_files_index);
                
            }

            else {
                cout << "File doesn't exist!" << endl;
            }
        }
        
        else if (command == "ROLLBACK") {

            string fname; 
            iss >> fname;
            int id;

            if (mymap.find(fname)){

                Tree* t = mymap.find(fname);
                if (iss >> id) { 
                    // rollback with id
                    t->rollback(id);
                }
                
                else{
                    t->rollback(-1); 
                    // else rollback to parent
                }
            }

            else{
                cout<< "File doesn't exist!"<<endl;
            }
        }

        else if (command == "HISTORY") {

            string fname;
            iss >> fname;
            if (mymap.find(fname)) {
                mymap.find(fname)->history();
            }
            else{
                cout<< "File doesn't exist!"<<endl;
            }
        }

        else if (command == "READ") {

            string fname;
            iss >> fname;

            if (mymap.find(fname)) {
                mymap.find(fname)->read();
            }
            else{
                cout<< "File doesnt exist"<<endl;
            }
        }

        else if (command == "BIGGEST_TREES") {

            string num;
            if (!(iss >> num)){
                cout<< "Sorry, positive integer argument is required!"<<endl;
            }
            else{
                bool flag = true;
                for (int i = 0; i < num.size(); i++) {
                    if (!isdigit(num[i])) {
                        flag = false;
                        break;
                    }
                }
                
                if (!flag) {
                    cout << "Sorry, only positive integer arguments allowed!" << endl;
                }
                else{
                int k = stoi(num);
                BT.BIGGEST_TREES(k);}
            }
        }

        else if (command == "RECENT_FILES") {

            string num;
            if (!(iss >> num)){
                cout<< "Sorry, positive integer argument is required!"<<endl;
            }
            else{
                bool flag = true;
                for (int i = 0; i < num.size(); i++) {
                    if (!isdigit(num[i])){
                        flag = false;
                        break;
                    }
                }
                
                if (!flag) {
                    cout << "Sorry, only positive integer arguments allowed!" << endl;
                }
                else{
                    int k = stoi(num);
                    RF.RECENT_FILES(k);
                }
            }
        }


        else {
            cout << "Unknown command: " << command << endl;
        }
    }
}

// g++ -std=c++11 longasst.cpp -o longasst

// ./longasst
