#include <iostream>
#include <cstring>
#include <stack>
#include <windows.h>
#include <math.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

struct ListData
{
    char* Data;
    ListData* next;
};
void Push(ListData *&List, char* newStr)
{
    ListData *tmp = new ListData;
    tmp->Data = new char[strlen(newStr) + 1];
    strcpy(tmp->Data, newStr);
    tmp->next = nullptr;

    if (List == nullptr)
        List = tmp;
    else
    {
        tmp->next = List;
        List = tmp;
    }
}

void ClearList(ListData *&first)
{
    ListData * curr = first;
    while (curr)
    {
        ListData *temp = curr->next;
        delete[] curr->Data;
        delete curr;
        curr = temp;
    }
}

void* PrintList(ListData *first)
{
    ListData * curr = first;
    while (curr)
    {
        cout << curr->Data << ", ";
        curr = curr->next;
    }
}

struct el_tree
{
    el_tree* left;
    el_tree* right;

    int value;
    ListData* list;
};

class database
{
private:
    el_tree* root;
    int count;
public:
    database();
    ~database();

    int getcount();
    void add(int value, char* str);
    void del_el();
    void forward_wall();
    void* find(int value);
    int find(char* str);
    void save_base(char* path, int value);
    void load_base(char* path);
private:
    int find(char* str, ListData *first);
    void save_base(char* path, int value, ListData *first);
    void add(el_tree*&, int value, char* str);
    void del_el(el_tree* el);
};

database::database()
{
    root = nullptr;
    count = 0;
}

database::~database()
{
    del_el(root);
}

int database::getcount()
{
    return count;
}

void database::add(el_tree* &first, int value, char* str)
{
    el_tree* temp = new el_tree;
    temp->value = value;
    temp->list = nullptr;
    temp->left = temp->right = nullptr;

    if (!first)
    {
        first = temp;
        Push(first->list, str);
        count++;
    }
    else {
        el_tree* tmp1 = root;
        el_tree* tmp2 = nullptr;

        while (tmp1)
        {
            tmp2 = tmp1;
            if (value < tmp1->value){
                tmp1 = tmp1->left;
            }
            else if (value > tmp1->value){
                tmp1 = tmp1->right;
            }
            else{
                Push(tmp2->list, str);
                break;
            }
        }

        if (value < tmp2->value){
            tmp2->left = temp;
            Push(temp->list, str);
            count++;
        }
        else if (value > tmp2->value){
            tmp2->right = temp;
            Push(temp->list, str);
            count++;
        }
    }
}


void database::add(int value, char* str)
{
    add(root, value, str);
}

void database::del_el(el_tree* el)
{
    if(!el) return;
    if(!el->left && !el->right){
        ClearList(el->list);
        delete el;
        root = nullptr;
        return;
    }
    if(el->left){
        el_tree*cur = el->left;
        while(cur->left && cur->right->right){
            cur = cur->right;
            if(cur->right){
                el_tree*temp = cur->right->left;
                cur->right->left = el->left;
                cur->right->right = el->right;
                ClearList(el->list);
                delete el;
                el = cur->right;
                cur->right = temp;
            }
            else{
                cur->right = el->right;
                ClearList(el->list);
                delete el;
                el = cur;
            }
        }
    }
    else{
        el_tree*cur = el->right;
        while (cur->left && cur->left->left){
            cur = cur->left;
            if(cur->left){
                el_tree*temp = cur->left->right;
                cur->left->right = el->right;
                cur->left->left = el->left;
                ClearList(el->list);
                delete el;
                el = cur->left;
                cur->left = temp;
            }else{
                cur->left = el->left;
                ClearList(el->list);
                delete el;
                el = cur;
            }
        }
    }
    root = nullptr;
    count = 0;
}

void database::del_el()
{
    del_el(root);
}

void database::forward_wall()
{
    if(root == nullptr) {
        cout<<"\nСписок пуст."<<endl;
        return;
    }
    stack<el_tree*>s;
    s.push(root);
    cout<<"\n\t\tГруппа:\t\tСтуденты:";
    int i = 1;
    do{
        el_tree*cur = s.top();
        s.pop();
        cout<<"\n\t    №"<<i<<"  "<<cur->value; cout<<"\t\t"; PrintList(cur->list);
        if(cur->left)
            s.push(cur->left);
        if(cur->right)
            s.push(cur->right);
        i++;
    }while(!s.empty());
}

void* database::find(int value){
    if(root == nullptr) {
        cout<<"Список пуст."<<endl;
        return nullptr;
    }
//    stack<el_tree*>s;
//    s.push(root);
//    do{
//        el_tree*cur = s.top();
//        s.pop();
//        if(cur->value == value){
//            return PrintList(cur->list);
//        }
//        if(value > cur->value){
//            while(cur->right){
//                s.push(cur->right);
//                cur = cur->right;
//            }
//        }
//        if(value < cur->value){
//            while(cur->left){
//                s.push(cur->left);
//                cur = cur->left;
//            }
//        }
//    }while(!s.empty());
    el_tree *cur = root;
    while (cur) {
        if (value < cur->value) {
            cur = cur->left;
            continue;
        } else if (value > cur->value) {
            cur = cur->right;
            continue;
        } else {
            return PrintList(cur->list);
        }
    }
    return nullptr;
}

int database::find(char* str, ListData *first){
    if(root == nullptr) {
        cout<<"\nСписок пуст."<<endl;
        return 0;
    }
    ListData *temp = first;
    stack<el_tree*>s;
    s.push(root);
    do{
        el_tree*cur = s.top();
        temp = cur->list;
        s.pop();
        while(temp){
            if(strstr(temp->Data,str) != nullptr){
                return cur->value;
            }
            temp = temp->next;
        }
        if(cur->right)
            s.push(cur->right);
        if(cur->left)
            s.push(cur->left);
    }while(!s.empty());
    return -1;
}

int database::find(char* str){
    int d = find(str,nullptr);
    return d;
}

void database::save_base(char* path, int value, ListData *first){
    fstream file;
    if(root == nullptr) {
        cout<<"\nСписок пуст."<<endl;
        return;
    }
    if(value == 1){
        file.open(path,ios_base::out);
    }
    if(value == 0) {
        file.open(path,ios_base::out | ios_base::app);
    }
    ListData*curr = first;
    stack<el_tree*>s;
    s.push(root);
    if(!file.is_open()){
        throw "\nНе удается открыть файл!";
    }
    do {
        el_tree*cur = s.top();
        curr = cur->list;
        s.pop();
        file<<cur->value;
        while (curr)
        {
            file << curr->Data;
            curr = curr->next;
            if(curr){
                file << ", ";
            }
        }
        file<<endl;
        if(cur->right)
            s.push(cur->right);
        if(cur->left)
            s.push(cur->left);
    } while (!s.empty());
    file.close();
}

void database::save_base(char *path, int value){
    save_base(path,value,nullptr);
}

void database::load_base(char* path){
    fstream file;
    file.open(path,ios_base::in);
    if(!file.is_open()){
        throw "\nНе удается открыть файл!";
    }
    int el = 0;
    char str[255];
    while(file>>el && file.getline(str,sizeof(str),'\n')){
       add(el,str);
    }
    file.close();
}

int main(){
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    database db;
    char path[] = "C://database.txt";
    char str[255];
    int n, id, value;
    bool run = true;
    try{
        while(run){
            cout<<"\n\t\t\t  База данных учащихся: "<<endl;
            cout<<"\t\t\t1.Добавить группу и студента."<<endl;
            cout<<"\t\t\t2.Удалить базу."<<endl;
            cout<<"\t\t\t3.Показать базу."<<endl;
            cout<<"\t\t\t4.Всего групп."<<endl;
            cout<<"\t\t\t5.Найти."<<endl;
            cout<<"\t\t\t6.Сохранить базу."<<endl;
            cout<<"\t\t\t7.Загрузить базу."<<endl;
            cout<<"\t\t\t0.Выход."<<endl;
            cout<<">> "; cin>>n;
            if(!cin)
            {
                cout << "\nError: Неверный ввод!"<<endl;
                cin.clear();
                while (cin.get() != '\n');
                continue;
            }
            switch(n){
            case 1: {
                cout<<"\nВведите номер группы: "<<endl;
                cout<<">> "; cin>>id;
                cout<<"\nВведите имя студента этой группы: "<<endl;
                cout<<">> "; cin>>str;
                db.add(id,str);
                db.forward_wall();
                cout<<endl;
                break;
            }
            case 2: {
                db.del_el();
                db.forward_wall();
                cout<<endl;
                break;
            }
            case 3: {
                cout<<endl;
                db.forward_wall();
                cout<<endl;
                break;
            }
            case 4: {
                int count = db.getcount();
                cout<<"\n\tВсего групп - "<<count<<endl;
                cout<<endl;
                break;
            }
            case 5: {
                cout<<"\nПоказать группу или студента?(1/2)"<<endl;
                cout<<">> "; cin>>n;
                if(!cin)
                {
                    cout << "\nError: Неверный ввод!"<<endl;
                    cin.clear();
                    while (cin.get() != '\n');
                    continue;
                }
                switch(n){
                case 1:{
                    cout<<"\nВведите группу: "<<endl;
                    cout<<">> "; cin>>value;
                    cout<<"\nВ группе "<<value<<", числяться студенты: "; db.find(value);
                    cout<<endl;
                    break;
                }
                case 2:{
                    cout<<"\nВведите имя студента: "<<endl;
                    cout<<">> "; cin>>str;
                    int group = db.find(str);
                    if(group == 0 || group == -1){
                        cout<<"\nЭтого студента нет ни в одной из групп."<<endl;
                    }
                    else{
                        cout<<"\nСтудент "<<str<<" в "<<group<<" группе."<<endl;
                    }
                    cout<<endl;
                    break;
                }
                default: {
                    cout << "\nError: Неверный ввод!"<<endl;
                }
                }
                break;
            }
            case 6: {
                cout<<"\nПерезаписать базу или дописать?(1/0)."<<endl;
                cout<<">> "; cin>>n;
                db.save_base(path,n);
                cout<<"\nБаза данных успешно сохранена в '"<<path<<"'"<<endl;
                break;
            }
            case 7: {
                db.load_base(path);
                cout<<"\nБаза данных успешно загружена из '"<<path<<"'"<<endl;
                db.forward_wall();
                cout<<endl;
                break;
            }
            case 0: {
                run = false;
                break;
            }
            default:{
                cout << "\nError: Неверный ввод!"<<endl;
                break;
            }
            }
        }
    }catch(const char*){
        cout<<"\nНе удается открыть файл!"<<endl;
    }
    system("Pause");
    return 0;
}
