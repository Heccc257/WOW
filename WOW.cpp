#include<bits/stdc++.h>

using namespace std;

string input_ord[5] = {"dragon", "ninja", "iceman", "lion", "wolf"};
string red_ord[5] = {"iceman", "lion", "wolf", "ninja", "dragon"};
string blue_ord[5] = {"lion", "dragon", "ninja", "iceman", "wolf"};
string side[2] = {"red", "blue"};
string weapon[3] = {"sword", "bomb", "arrow"};

map<string, int> ele, atk;

// red 0
// blue 1
class Clock {
public:
    int hour,min;
    int T;
    Clock() {}
    Clock(int _T):T(_T) {hour = min = 0;}
    bool time_flow(int t) {
        T -= t;
        min += t;
        if(min == 60) hour++, min = 0;
        return T < 0;
    }
    void Print() {
        cout<<setfill('0')<<setw(3)<<hour<<":"<<setw(2)<<min<<" ";
    }
};
Clock clk;

#define pr pair<int,int>
#define fr first
#define sc second

class Weapon {
private:
    int sword;
    int bomb;
    vector<int>arrow;

public:
    void debug() {
        cerr<<arrow[0]<<"\n";
    }
    Weapon() {
        sword = bomb = 0;
        arrow.clear();
    }

    void Add_weapon(int wid) {
        if(wid == 0) sword ++;
        else if(wid == 1) bomb ++;
        else  arrow.push_back(4 - wid);// 对于arrow,4 - wid代表耐久度
    }
    
    void Read_q(deque<int> &q) { // q中存武器格式与wid一致
        sword = bomb = 0;
        arrow.clear();
        for(int i=0;i<q.size();i++) Add_weapon(q[i]);
    }

    //对于arrow,4 - wid代表耐久度
    deque<int> Convert_to_q() {
        deque<int>q;
        q.clear();
        for(int i=1;i<=sword;i++) q.push_back(0);
        for(int i=1;i<=bomb;i++) q.push_back(1);
        for(int i=0;i<arrow.size();i++) q.push_back(4 - arrow[i]);
        
        // cerr<<"arrow: ";
        // for(int i=0;i<arrow.size();i++) cerr<<arrow[i]<<" ";
        // cerr<<"\n";
        // cerr<<"QAQ: ";
        // for(int i=0;i<q.size();i++) cerr<<q[i]<<" ";
        // cerr<<"\n";

        return q;

    }

    void Maintain() {sort(arrow.begin(), arrow.end());}
    int Total_wp() {return sword + bomb + arrow.size();}

    void Report() {cout<<"has "<<sword<<" sword "<<bomb<<" bomb "<<arrow.size()<<" arrow ";}

    int Get_Robbed() {
        if(sword) {
            sword --;
            return 0;
        } else if(bomb) {
            bomb -- ;
            return 1;
        } else if(arrow.size()) {
            int dur = 4 - arrow.back();
            arrow.pop_back();
            return dur;
        }
    }

};

class Warrior:public Weapon {
public:
    int id;
    int ele,atk;
    int side;
    string name;
    Warrior() {}
    Warrior(const Warrior &a):id(a.id), ele(a.ele), atk(a.atk), name(a.name), side(a.side) {}
    Warrior(int _id, int _ele, int _atk, string _name, int _side): id(_id), ele(_ele), atk(_atk), name(_name), side(_side) {}

    void print_id() {cout<<::side[side]<<" "<<name<<" "<<id<<" ";}

    string Num_to_string(int n) {
        string a;
        a.clear();
        while(n) a.push_back(char('0' + n%10)), n /= 10;
        a.reserve();
        return a;
    }
    string ID() {
        return ::side[side] + " " + name + " " + Num_to_string(id) + " ";
    }

    void Attr() {cout<<"with "<<ele<<" elements and force "<<atk<<"\n";}

    virtual void Get_Attacked(int patk) {ele -= patk;}

    virtual void Reatk(Warrior *p) {
        // 没有被杀死的时候才会反击
        p->Get_Attacked(atk / 2);
    }
    
    virtual void Fought_back(Warrior * enemy, int city_id) {
        clk.Print();
        print_id();
        cout<<"fought back against ";
        enemy->print_id();
        cout<<"in city "<<city_id<<"\n";
        enemy->Get_Attacked(atk / 2);
    }

    virtual void Deathrattle(Warrior *p,int ele, int pos) {
        clk.Print();
        print_id();
        cout<<"was killed in city "<<pos<<"\n";
    }

    virtual void Yelled(int city_id) {}
    virtual void End_march(int step) {}
    virtual void end_attack() {}
    virtual bool Esc(int K) {return 0;}
    inline bool dead() {return ele <= 0;}

    
    // void Sort_weapons() {sort(w.begin(), w.end());}
    // void Sort_weapons_cmp() {sort(w.begin(), w.end(), Weapon::cmp);}

    void Report_weapon() {
        clk.Print();
        print_id();
        Weapon::Report();
        cout<<"and "<<ele<<" elements\n";
        // cout<<"has "
    }
    virtual void Rob(Warrior *p) {}

    virtual void Bomb(Warrior *p) {
        // cerr<<"hour: " <<clk.hour<<"\n";
        // cerr<<atk<<"\n";
        cerr<<ID()<<p->ID()<<"\n";
        int dam = atk * 4 / 10;
        // cerr<<dam<<"\n";
        p->Get_Attacked(dam);
        Get_Attacked(dam / 2);
    }

    virtual void Sword(Warrior *p) {
        int dam = atk * 2 / 10;
        p->Get_Attacked(atk);
    }
    virtual void Arrow(Warrior *p) {
        int dam = atk * 3 / 10;
        p->Get_Attacked(dam);
    }
};


class Dragon:public Warrior {
public:
    Dragon(const Warrior &a): Warrior(a) {
        Add_weapon(id%3);
    }
    virtual void Yelled(int city_id) {
        clk.Print();
        print_id();
        printf("yelled in city %d\n",city_id);
    }
};

class Ninja:public Warrior {
public:
    Ninja(const Warrior &a): Warrior(a) {
        Add_weapon(id%3);
        Add_weapon((id + 1) % 3);
    }
    virtual void Fought_back(Warrior *p, int city_id) {}
    virtual void Bomb(Warrior *p) {
        int dam = atk * 4 / 10;
        p->Get_Attacked(dam);
    }
};

class Iceman:public Warrior {
public:
    Iceman(const Warrior &a): Warrior(a) {
        Add_weapon(id%3);
    }
    virtual void End_march(int step) {
        ele -= ele / 10;
    }
};

class Lion:public Warrior {
public:
    int Loyalty;
    Lion(const Warrior &a, int loy): Warrior(a), Loyalty(loy) {
        Add_weapon(id%3);
    }
    virtual bool Esc(int K) {
        if(this == NULL) return 0;
        Loyalty -= K;
        if(Loyalty <= 0) {
            clk.Print();
            print_id();
            cout<<"ran away\n";
            return 1;
        } else return 0;
    }
    
    virtual void Deathrattle(Warrior *p,int ele, int pos) {
        clk.Print();
        print_id();
        cout<<"was killed in city "<<pos<<"\n";
        p->ele += ele;
    }
};

class Wolf:public Warrior {
public:
    int kill_num;
    Wolf(const Warrior &a): Warrior(a), kill_num(0) {}

    virtual void Rob(Warrior *p) {
        if(p->name == "wolf") return ;
        while(Total_wp() < 10 && p->Total_wp()) {
            Add_weapon(p->Get_Robbed());
        }
    }

    virtual void end_attack() {
        kill_num ++;
        // 主动进攻结束后战斗还未完全结束的时候就需要发动
        if(kill_num > 0 && kill_num %2 == 0) {
            atk *= 2;
            ele *= 2;
        }
    }

};


vector< Warrior* >award[2];

class City {
public:
    Warrior *p[2];
    int id;
    int flag; // 初始为-1
    int lst_winner;// 上一场战斗的获胜者
    int city_ele;

    City() {}
    City(int _id):id(_id), flag(-1), lst_winner(-1), city_ele(0) {p[0] = p[1] = NULL;}
    void Set_flag(int color) {
        flag = color;
        clk.Print();
        cout<<side[color]<<" flag raised in city "<<id<<"\n";
        // cout<<
    }

    void Report(int Warrior_side) {
        clk.Print();
        p[Warrior_side]->print_id();
        printf("marched to city %d ",id);
        p[Warrior_side]->Attr();
    }

    void Reached(int side, int step) {
        if(p[side] != NULL) {
            p[side]->End_march(step);
            Report(side);
        }
    }

    void take_ele_report(Warrior *p) {
        clk.Print();
        p->print_id();
        cout<<"earned "<<city_ele<<" elements for his headquarter\n";
    }
    
    bool Tatakai_end(Warrior *a, Warrior *b, deque<int>&wa, deque<int>&wb) {
        if(a->dead() || b->dead()) return 1;
        if(wa.size() == 0 && wb.size() == 0) return 1;
        return 0;
    }

    void Attack(Warrior *a, Warrior *b, deque<int>&q) {
        if(!q.size()) return ;
        int now = q.front();
        q.pop_front();

        if(now == 0) q.push_back(0);
        else if(now == 2) q.push_back(3);

        if(now == 0) a->Sword(b);
        else if(now == 1) a->Bomb(b);
        else a->Arrow(b);
    }

    void Tatakai(Warrior *a, Warrior *b) {
        if(a == NULL || b == NULL) return ;


        // clk.Print();
        // a->print_id();
        // cout<<"attacked ";
        // b->print_id();
        // cout<<"in city "<<id<<" ";
        // a->Attr();

        // a主动进攻
        int aele = a->ele; // 处理狮子的情况
        int bele = b->ele; // 处理狮子的情况
        
        
        a->Maintain();
        b->Maintain();
        // 抢武器
        a->Rob(b);
        b->Rob(a);


    if(clk.hour == 2) {
        cerr<<"-------------\n";
    }
        deque<int>wa = a->Convert_to_q();
        deque<int>wb = b->Convert_to_q(); // a, b的武器序列
        
        // 战斗开始
        // cerr<<a->ID()<<" "<<b->ID()<<" "<<Tatakai_end(a, b, wa, wb)<<" "<<wa.size()<<" "<<wb.size() <<"\n";
        while(!Tatakai_end(a, b, wa, wb)) {
            // cerr<<"Tatakai\n";
            
            // if(clk.hour == 2 && b->id == 2 && a->id == 2) {

            //     cerr<<a->ID()<<" "<<b->ID()<<"\n";
            //     cerr<<"arrow\n";
            //     a->debug();
            //     b->debug();
            //     for(int i=0;i<wa.size();i++) cerr<<wa[i]<<" ";cerr<<"\n";
            //     for(int i=0;i<wb.size();i++) cerr<<wb[i]<<" ";cerr<<"\n";
            // }
            Attack(a, b, wa);
            swap(a, b);
            swap(wa, wb);
        }


        // b->Get_Attacked(a->atk);
        // if(b->dead()) {
        //     // b 死亡了
        //     a->end_attack();
        //     b->Deathrattle(a, bele, id);
        // } else {
        //     b->Fought_back(a, id);
        //     if(a->dead()) {
        //         a->Deathrattle(b, aele, id);
        //     }
        // }

        // 战斗结束

        

        // 更新武器数据
        a->Read_q(wa);
        b->Read_q(wb);

        // 抢武器
        if(a->dead()) b->Rob(a);
        else a->Rob(b);


        if(a->side == 1) swap(a, b);

        // 播报战斗结果
        if(!a->dead() && !b->dead()) {
            clk.Print();
            cout<<"both "<<a->ID()<<"and "<<b->ID()<<"were alive in city "<<id<<"\n";
        } else if( (a->dead() + b->dead()) == 1 ) {
            if(a->dead()) swap(a, b);
            clk.Print();
            cout<<a->ID()<<"killed "<<b->ID()<<"in city "<<id<<" remaining "<<a->ele << " elements\n";
        }

        if(!a->dead()) a->Yelled(id); // 武士欢呼
        if(!b->dead()) b->Yelled(id); // 武士欢呼

        // if(!a->dead() && !b->dead()) { // 没人死，结束
        //     lst_winner = -1;
        //     return ;
        // }


        // 奖励
        // Warrior *survivor = a->dead()? b : a;
        // award[survivor->side].push_back(survivor);

        // 销毁对方的指针
        // p[0]->dead() ? p[0] = NULL : p[1] = NULL;

        // // 取走城市生命值
        // take_ele_report(survivor);
        // // cout<<p[0]<<" "<<p[1]<<" ";
        // // cout<<"DEAD!!!!!!!!!!!!\n";
        // // 插旗
        // if(lst_winner == survivor->side && flag != survivor->side) Set_flag(lst_winner);

        // lst_winner = survivor->side;

    }

    void Round() {
        
        Warrior *a = p[0];
        Warrior *b = p[1];


        if(id %2 == 0) swap(a,b);

        Tatakai(a,b);

        if(p[0] && p[0]->dead()) delete p[0], p[0] = NULL;
        
        if(p[1] && p[1]->dead()) delete p[1], p[1] = NULL;

    }

    int only_one() {
        if((p[0] == NULL) + (p[1] == NULL) != 1) return -1;
        return p[1] != NULL;
    }

};

class Headquarter {
public:
    int ele;
    int now,tot;
    int side;
    string *ord;
    int taken;
    Headquarter() {}
    Headquarter(int _ele, string *_ord,int _side):ele(_ele), ord(_ord), now(0), taken(1), tot(0), side(_side) {}
    void Make_Warrior(City *city) {
        string Type = ord[now];

        if(ele < ::ele[Type]) {
            city->p[side] = NULL;
            return ;
        }

        ele -= ::ele[Type];

        Warrior war(++tot, ::ele[Type], ::atk[Type], Type, side); // 制造一个武士模板
        if(Type == "dragon") {
            city->p[side] = new Dragon(war);
        } else if(Type == "ninja") {
            city->p[side] = new Ninja(war);
        } else if(Type == "iceman") {
            city->p[side] = new Iceman(war);
        } else if(Type == "lion") {
            city->p[side] = new Lion(war, ele);
        } else if(Type == "wolf") {
            city->p[side] = new Wolf(war);
        }

        // 播报制造信息
        now = (now + 1) %5;
        clk.Print();
        city->p[side]->print_id();
        cout<<"born\n";
        if(Type == "lion") {
            cout<<"Its loyalty is "<<ele<<'\n';
        }
    }

    bool Taken(Warrior *enemy) {
        taken -- ;
        clk.Print();
        enemy->print_id();
        cout<<"reached "<<::side[side]<<" headquarter ";
        enemy->Attr();
        if(taken) return 0;
        else {
            clk.Print();
            cout<<::side[side]<<" headquarter was taken\n";
            return 1;
        }
    }

    void Earn(int &city_ele) {
        if(city_ele == 0) return ;
        // clk.Print();
        // fr->print_id();
        // cout<<"earned "<<city_ele<<" elements for his headquarter\n";
        ele += city_ele;
        city_ele = 0;
    }
    void Award() {
        for(int i=0;i<award[side].size();i++) {
            if(ele >= 8) {
                ele -= 8;
                award[side][i]->ele += 8;
            }
        }
        award[side].clear();
    }

    void Report() {
        clk.Print();
        cout<<ele<<" elements in "<<::side[side]<<" headquarter\n";
    }
};

City citys[25];
Warrior **temp = new Warrior* [25]; // 处理red的移动
int n,m,T,K;
int main() {
    // system("fc my.out std.out");
    // return 0;
    freopen("std.in","r",stdin);
    freopen("my.out","w",stdout);
    int Cas;
    scanf("%d",&Cas);
    Headquarter red, blue;

    for(int c=1;c<=Cas;c++) {
        award[0].clear();
        award[1].clear();
        printf("Case %d:\n",c);
        

        scanf("%d%d%d%d",&m, &n, &K, &T);

        //初始化clock
        clk = Clock(T);

        for(int i=0;i<5;i++) scanf("%d",&ele[input_ord[i]]);
        for(int i=0;i<5;i++) scanf("%d",&atk[input_ord[i]]);


        // 初始化Headquarter
        red = Headquarter(m, red_ord, 0);
        blue = Headquarter(m, blue_ord, 1);
        
        // 初始化citys
        for(int i=0;i<=n+1;i++) citys[i] = City(i);

        int taken = 0;
        while(1) {

            // 造武士
            red.Make_Warrior(&citys[0]);
            blue.Make_Warrior(&citys[n+1]);
            if(clk.time_flow(5)) break;

            //lion 逃跑
            if(citys[0].p[0] && citys[0].p[0]->Esc(0)) citys[0].p[0] = NULL;
            for(int i=1;i<=n;i++) {
                if(citys[i].p[0] && citys[i].p[0]->Esc(K)) citys[i].p[0] = NULL;
                if(citys[i].p[1] && citys[i].p[1]->Esc(K)) citys[i].p[1] = NULL;
            }
            if(citys[n + 1].p[1] && citys[n + 1].p[1]->Esc(0)) citys[n + 1].p[1] = NULL;


            if(clk.time_flow(5)) break;// 10分
            // 武士前进
            // 顺序按照到达城市1~n  
            if(citys[1].p[1] != NULL) { // 到达红色headqurter
                citys[1].p[1]->End_march(n+1);
                if(red.Taken(citys[1].p[1])) taken = 1;
            }

            for(int i=0;i<=n+1;i++) temp[i] = citys[i].p[0];
            for(int i=1;i<=n;i++) {

                citys[i].p[0] = temp[i - 1];
                citys[i].Reached(0, i);


                citys[i].p[1] = citys[i + 1].p[1];
                citys[i].Reached(1, n + 1 - i);

            }

            if(temp[n] != NULL) { // 到达蓝色headquarter
                temp[n]->End_march(n+1);
                if(blue.Taken(temp[n])) taken = 1;
            }

            if(taken) break;


            if(clk.time_flow(10)) break; // 20分

            // clk.Print();
            // 奖励


            if(clk.time_flow(10)) break; // 30分

            // clk.Print();

            if(clk.time_flow(10)) break; // 40分

            // clk.Print();
            // 战斗
            for(int i=1;i<=n;i++) citys[i].Round();

            if(clk.time_flow(10)) break; // 50分

            red.Report();
            blue.Report();
            
            if(clk.time_flow(5)) break; // 55分
            for(int i=1;i<=n;i++) {
                if(citys[i].p[0]) citys[i].p[0]->Report_weapon();
                if(citys[i].p[1]) citys[i].p[1]->Report_weapon();
            }

            if(clk.time_flow(5)) break;
            // exit(0);

        }
    }
    return 0;
}
/*
1
10 1 10 400
20 20 30 10 20
5 5 5 5 5


1
70 5 10 500
20 20 30 10 20
5 5 5 5 5

*/
