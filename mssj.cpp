#include<bits/stdc++.h>

using namespace std;

string input_ord[5] = {"dragon", "ninja", "iceman", "lion", "wolf"};
string red_ord[5] = {"iceman", "lion", "wolf", "ninja", "dragon"};
string blue_ord[5] = {"lion", "dragon", "ninja", "iceman", "wolf"};

// red 0
// blue 1

class Warrior {
public:
    int id;
    int ele,atk;
    string name;
    Warrior() {}
    Warrior(const Warrior &a):id(a.id), ele(a.ele), atk(a.atk), name(a.name) {}
    Warrior(int _id, int _ele, int _atk, string _name): id(_id), ele(_ele), atk(_atk), name(_name) {}

    virtual void Get_Attacked(int patk) {
        ele -= patk;
    }

    virtual void Reatk(Warrior *p) {
        // 没有被杀死的时候才会反击
        p->Get_Attacked(atk / 2);
    }

    virtual void Deathrattle(Warrior *p,int ele) {}
    virtual void Yelled() {}
    virtual void End() {}
    virtual void end_attack() {}

    inline bool dead() {return ele <= 0;}
};

class Dragon:public Warrior {
public:
    virtual void Yelled() {
        printf("dragon %d yelled ",id);
    }
};

class Ninja:public Warrior {
public:
    virtual void Reatk(Warrior *p) {}
};

class Iceman:public Warrior {
public:
    virtual void End() {
        ele = min(1, ele - 9);
        atk += 20;
    }
};

class Lion:public Warrior {
public:
    virtual void Deathrattle(Warrior *p,int ele) {
        p->ele += ele;
    }
};

class Wolf:public Warrior {
public:

    int kill_num;

    Wolf(const Warrior a): Warrior(a), kill_num(0) {}
    
    virtual void end_attack() {
        // 主动进攻结束后战斗还未完全结束的时候就需要发动
        atk *= 2;
        ele *= 2;

    }
};

class City {
public:
    
    Warrior *p[2];
    int id;
    int flag; // 初始为-1
    int lst_winner;// 上一场战斗的获胜者
    int city_ele;
    
    void Fight(Warrior *a, Warrior *b) {
        // a主动进攻
        int aele = a->ele; // 处理狮子的情况
        int bele = b->ele; // 处理狮子的情况
        
        // 战斗开始
        b->Get_Attacked(a->atk);
        if(b->dead()) {
            // b 死亡了
            a->end_attack();
            b->Deathrattle(a,bele);
        } else {
            // b 反击
            a->Get_Attacked(b->atk);
            if(a->dead()) {
                a->Deathrattle(b,aele);
            }
        }

        // 战斗结束
        if(!a->dead()) a->Yelled(); // 武士欢呼
        if(!a->dead() && b->dead()) return ;// 没人死，结束
        Warrior *survivor = a->dead()? b : a;
    }

};

class Make_Warrior {
public:
    string *ord;
    int now, tot;

    void Make() {

    }
};

class Headquater {

};

int main() {


    return 0;
}