#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <cmath>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <limits>
#include <conio.h>
#include <cstdint>
#include <algorithm>
#include <atomic>
#include <future>
#include <sstream>

using namespace std;

void wait(int s= 1){
    const int CHECKS_PER_SECOND= 10;
    const int SLEEP_MS= 1000/ CHECKS_PER_SECOND;

    for (int i = 0; i < s* CHECKS_PER_SECOND; i++)
    {
        if (_kbhit())
        {
            char key= _getch();
            if (key== '\r'|| key== '\n')
            {
                while (_kbhit()) _getch();
                return;
            }
        }
        this_thread::sleep_for(chrono::milliseconds(SLEEP_MS));   
    }
}

void waitms(int ms= 10){
    this_thread::sleep_for(chrono::milliseconds(ms));
}

string usernameInput(){
    string username;

    while (true)
    {
        cout<< "Input your username: ";
        getline(cin, username);

        if (username.empty())
        {
            cout<< "Username can't be empty!\n";
            continue;
        }
        if (username.find(' ')!= string::npos)
        {
            cout<< "Username can't contain spaces!\n";
            continue;
        }
        if (username.size()< 4 || username.size()> 8)
        {
            cout<< "Username must be 4-8 characters!\n";
            continue;
        }
        break;
    }
    return username;
}

class Player{
    public:
    int x= 0;
    int y= 0;
    int distanceLastTraveled= 0;
    bool isStoryMidTriggered= false;

    struct Stats
    {
        string name;
        float maxHP;
        float HP;
        float baseMaxHP;
        float maxMP;
        float MP;
        float baseMaxMP;
        int XP;
        int maxXP;
        int level;
        float baseSTR;
        float STR;
        float baseDEF;
        float DEF;
        float baseCritDMG;
        float critDMG;
        int baseCritChance;
        int critChance;
        int critChanceBuffTurns= 0;
        int critBuffTurns= 0;
        int gold;
        int trust;
    };
    Stats createPlayerStats(){
        Stats p;
        p.name= usernameInput();
        p.baseMaxHP= 100.0f;
        p.maxHP= p.baseMaxHP;
        p.HP= p.maxHP;
        p.baseMaxMP= 100;
        p.maxMP= p.baseMaxMP;
        p.MP= p.maxMP;
        p.XP= 0;
        p.maxXP= 100;
        p.level= 1;
        p.baseSTR= 5.0f;
        p.STR= p.baseSTR;
        p.baseDEF= 5.0f;
        p.DEF= p.baseDEF;
        p.baseCritDMG= 1.5f;
        p.critDMG= p.baseCritDMG;
        p.baseCritChance= min(100, 5);
        p.critChance= p.baseCritChance;
        p.critBuffTurns= 0;
        p.critChanceBuffTurns= 0;
        p.gold= 100;
        p.trust= 0;
        return p;
    };

    Stats stats;

    struct Skill
    {
        float skill;
        float skill1;
        int skill2;
        int skillMPCost;
        int skill1MPCost;
        int skill2MPCost;
    };

    Skill createPlayerSkill(){
        Skill ps;
        ps.skill= stats.STR* 1.5f;
        ps.skill1= stats.critDMG* 1.2f;
        ps.skill2= stats.critChance* 2;
        ps.skillMPCost= 20;
        ps.skill1MPCost= 30;
        ps.skill2MPCost= 30;
        return ps;
    }

    Skill skill;

    struct Inventory{
        int weapon;
        int armor;
        int talisman;
        int talisman1;
        int potionHealth;
        int potionMana;
    };

    Inventory createPlayerInventory(){
        Inventory i;
        i.weapon= 0;
        i.armor= 0;
        i.talisman= 0;
        i.talisman1= 0;
        i.potionHealth= 0;
        i.potionMana= 0;
        return i;
    }

    Inventory inventory;

    struct Weapon{
        vector<string> weaponList;
        vector<int> weaponDMG;
        vector<int> weaponMaxAttackVariance;
        vector<int> weaponMinAttackVariance;
        vector<int> WEAPON_PRICE;
    };

    Weapon weapon;

    Weapon createWeapon(){
        Weapon w;
        w.weaponList= {"None", "Dagger", "Sword", "Mace", "Longsword", "GreatSword", "Enhanced Dagger", "Enhanced Sword", "Enhanced Mace", "Legendary Sword"};
        w.weaponDMG= {0, 5, 10, 12, 20, 30, 50, 100, 120, 200};
        w.weaponMaxAttackVariance= {5, 7, 15, 21, 15, 21, 31, 51, 71, 101};
        w.weaponMinAttackVariance= {2, 1, 4, 10, 4, 4, 10, 10, 20, 20};
        w.WEAPON_PRICE= {0, 180, 300, 320, 500, 540, 1000, 1500, 1620, 2000};
        return w;
    }

    string getWeaponName(const Inventory& i, const Weapon& w){
        if (i.weapon< 0 || i.weapon >= (int)w.weaponList.size())
        {
            return "Unknown Weapon";
        }
        return w.weaponList[i.weapon];
    }

    struct Armor{
        vector<string> armorList;
        vector<int> armorHP;
        vector<int> armorDefence;
        vector<int> ARMOR_PRICE;
    };

    Armor armor;

    Armor createArmor(){
        Armor a;
        a.armorList= {"None", "Leather Chestplate", "Leather Armor", "Iron Breastplate", "Iron Chestplate", "Chainmail Armor", "Chainmail Armor with Iron Chestplate", "Iron Armor"};
        a.armorDefence= {0, 10, 20, 30, 35, 40, 50, 90};
        a.armorHP= {0, 10, 20, 30, 40, 50, 70, 100};
        a.ARMOR_PRICE= {0, 160, 200, 300, 400, 500, 700, 900};
        return a;
    }

    string getArmorName(const Inventory& i, const Armor& a){
        if (i.armor< 0 || i.armor>= (int)a.armorList.size())
        {
            return "Unknown Armor";
        }
        return a.armorList[i.armor];
    }

    struct Talisman{
        vector<string> talismanList;
        vector<int> talismanHP;
        vector<int> talismanMP;
        vector<int> talismanSTR;
        vector<int> talismanDEF;
        vector<float> talismanCritDMG;
        vector<int> talismanCritChance;
        vector<int> talismanGold;
        vector<int> TALISMAN_PRICE;
    };

    Talisman talisman;

    Talisman createTalisman(){
        Talisman t;
        t.talismanList= {"None", "Fiery Silver Ring", "Bloody Mary's Gold Ring", "Oracle's Necklace", "Ares' Pendant", "Midas' Hand", "Odin's Ring"};
        t.talismanHP= {0, 0, 10, 5, 0, 0, 7};
        t.talismanMP= {0, 0, 0, 10, 5, 0, 8};
        t.talismanSTR= {0, 2, 3, 0, 15, 0, 12};
        t.talismanDEF= {0, 0, 2, 5, 7, 0, 6};
        t.talismanCritChance= {0, 10, 0, 0, 15, 0, 8};
        t.talismanCritDMG= {0, 0.10f, 0.01f, 0, 0.30f, 0, 0.10f};
        t.talismanGold= {0, 0, 0, 0, 0, 50, 8};
        t.TALISMAN_PRICE= {0, 200, 210, 300, 450, 500, 1000};
        return t;
    }

    string getTalismanName(const Inventory& i, const Talisman& t){
        if (i.talisman< 0 || i.talisman>= (int)t.talismanList.size())
        {
            return "Unknown Talisman";
        }
        return t.talismanList[i.talisman];
    }

    string getTalisman1Name(const Inventory& i, const Talisman& t){
        if (i.talisman1< 0 || i.talisman1>= (int)t.talismanList.size())
        {
            return "Unknown Talisman";
        }
        return t.talismanList[i.talisman1];
    }

    void checkStats(){
        cout<< fixed<< setprecision(1);
        
        cout<< "*************************************\n";
        cout<< "*************** STATS ***************\n";
        cout<< "*************************************\n\n";
        cout<< "Player: "<< stats.name<< "\n";
        cout<< "HP: "<< stats.HP<< "/"<< stats.maxHP<< "\n";
        cout<< "MP: "<< stats.MP<< "/"<< stats.maxMP<< "\n";
        cout<< "XP: "<< stats.XP<< "/"<< stats.maxXP<< "\n";
        cout<< "Level: "<< stats.level<< "\n";
        cout<< "Trust: "<< stats.trust<< "\n";
        int weaponIdx = inventory.weapon;
        int weaponDamage = 0;
        if (weaponIdx >= 0 && weaponIdx < static_cast<int>(weapon.weaponDMG.size())) {
            weaponDamage = weapon.weaponDMG[weaponIdx];
        }
        cout << "Weapon: " << getWeaponName(inventory, weapon) << " +" << weaponDamage << " DMG\n";
        cout<< "STR: "<< stats.STR<< "\n";
        cout<< "DEF: "<< stats.DEF<< "\n";
        float effectiveCritDMG= (stats.critBuffTurns> 0)? stats.critDMG* 1.2f: stats.critDMG;
        cout<< "Crit Damage: "<< effectiveCritDMG<< "\n";
        float effectiveCritChance= (stats.critChanceBuffTurns> 0)? stats.critChance* 2: stats.critChance;
        cout<< "Crit Chance: "<< effectiveCritChance<< "%\n";
        cout<< "Gold: "<< stats.gold<< "\n\n";
        cout<< "*************************************\n\n";
        wait(3);
    }

    void Inv(const Inventory& i, const Weapon& w, const Armor& a, const Talisman& t){
        cout<< "*************************************\n";
        cout<< "************* INVENTORY *************\n";
        cout<< "*************************************\n\n";
        cout<< "Weapon: "<< getWeaponName(i, w)<< "\n";
        cout<< "Armor: "<< getArmorName(i, a)<< "\n";
        cout<< "Talisman: "<< getTalismanName(i, t)<< "/"<< getTalisman1Name(i, t)<< "\n";
        cout<< "Health Potion: "<< i.potionHealth<< "\n";
        cout<< "Mana Potion: "<< i.potionMana<< "\n\n";
        cout<< "*************************************\n\n";
        wait(3);
    }

    void talismanAttributes(const Inventory& i, const Talisman& t){
        stats.maxHP = stats.baseMaxHP;
        stats.maxMP = stats.baseMaxMP;
        stats.STR = stats.baseSTR;
        stats.DEF = stats.baseDEF;
        stats.critDMG = stats.baseCritDMG;
        stats.critChance = stats.baseCritChance;

        if (i.armor>= 0&& i.armor< armor.armorList.size())
        {
            stats.maxHP+= armor.armorHP[i.armor];
            stats.DEF+= armor.armorDefence[i.armor];
        }
        
        auto applyTalisman = [&](int talismanIndex) {
            if (talismanIndex >= 0 && talismanIndex < static_cast<int>(t.talismanList.size()))
            {
                stats.maxHP += t.talismanHP[talismanIndex];
                stats.maxMP += t.talismanMP[talismanIndex];
                stats.STR += t.talismanSTR[talismanIndex];
                stats.DEF += t.talismanDEF[talismanIndex];
                stats.critDMG += t.talismanCritDMG[talismanIndex];
                stats.critChance += t.talismanCritChance[talismanIndex];
            }
    };

        applyTalisman(i.talisman);
        applyTalisman(i.talisman1);

        stats.HP = min(stats.HP, stats.maxHP);
        stats.MP = min(stats.MP, stats.maxMP);

        skill.skill = stats.STR * 2;
        skill.skill1 = stats.critDMG * 2;
    }
};

class Enemies{
    public: 
    struct Stats{
        string name;
        float HP;
        float maxHP;
        float STR;
        float DEF;
        float critDamage;
        int critChance;
        int XP;
        int gold;
        bool fled;
    };
    Stats createEnemyVillager(){
        Stats e;
        e.name= "Villager";
        e.HP= 50;
        e.maxHP= 50;
        e.STR= 5;
        e.DEF= 0;
        e.critDamage= 1.2f;
        e.critChance= 2;
        e.XP= 20;
        e.gold= 20;
        return e;
    }
    Stats createEnemyFarmer(){
        Stats e1;
        e1.name= "Farmer";
        e1.HP= 50;
        e1.maxHP= 50;
        e1.STR= 10;
        e1.DEF= 2;
        e1.critDamage= 1.2f;
        e1.critChance= 2;
        e1.XP= 20;
        e1.gold= 40;
        return e1;
    }
    Stats createEnemyGuard(){
        Stats e2;
        e2.name= "Guard";
        e2.HP= 70;
        e2.maxHP= 70;
        e2.STR= 15;
        e2.DEF= 10;
        e2.critDamage= 1.2f;
        e2.critChance= 2;
        e2.XP= 40;
        e2.gold= 20;
        return e2;
    }
    Stats createEnemyGuardDog(){
        Stats e3;
        e3.name= "Guard Dog";
        e3.HP= 20;
        e3.maxHP= 20;
        e3.STR= 10;
        e3.DEF= 0;
        e3.critDamage= 1.2f;
        e3.critChance= 2;
        e3.XP= 20;
        e3.gold= 0;
        return e3;
    }
    Stats createEnemySoldier(){
        Stats e4;
        e4.name= "Soldier";
        e4.HP= 100;
        e4.maxHP= 100;
        e4.STR= 15;
        e4.DEF= 15;
        e4.critDamage= 1.2f;
        e4.critChance= 2;
        e4.XP= 50;
        e4.gold= 50;
        return e4;
    }
    Stats createEnemyKnight(){
        Stats e5;
        e5.name= "Knight";
        e5.HP= 150;
        e5.maxHP=150;
        e5.STR= 20;
        e5.DEF= 20;
        e5.critDamage= 1.2f;
        e5.critChance= 2;
        e5.XP= 100;
        e5.gold= 75;
        return e5;
    }
    Stats createEnemyArcherKnight(){
        Stats e6;
        e6.name= "Archer Knight";
        e6.HP= 50;
        e6.maxHP=50;
        e6.STR= 40;
        e6.DEF= 50;
        e6.critDamage= 1.2f;
        e6.critChance= 2;
        e6.XP= 100;
        e6.gold= 75;
        return e6;
    }
    Stats createEnemyMage(){
        Stats e7;
        e7.name= "Mage";
        e7.HP= 50;
        e7.maxHP=50;
        e7.STR= 50;
        e7.DEF= 50;
        e7.critDamage= 1.2f;
        e7.critChance= 2;
        e7.XP= 120;
        e7.gold= 80;
        return e7;
    }
    Stats createEnemyGolem(){
        Stats e8;
        e8.name= "Golem";
        e8.HP= 300;
        e8.maxHP=300;
        e8.STR= 30;
        e8.DEF= 80;
        e8.critDamage= 1.2f;
        e8.critChance= 2;
        e8.XP= 200;
        e8.gold= 20;
        return e8;
    }
    Stats createEnemyPrince(){
        Stats e9;
        e9.name= "Prince";
        e9.HP= 200;
        e9.maxHP=200;
        e9.STR= 30;
        e9.DEF= 50;
        e9.critDamage= 1.2f;
        e9.critChance= 2;
        e9.XP= 300;
        e9.gold= 200;
        return e9;
    }
    Stats createEnemyThief(){
        Stats e10;
        e10.name= "Thief";
        e10.HP= 50;
        e10.maxHP=50;
        e10.STR= 10;
        e10.DEF= 5;
        e10.critDamage= 1.2f;
        e10.critChance= 2;
        e10.XP= 25;
        e10.gold= 20;
        e10.fled= false;
        return e10;
    }
    Stats createEnemyBandit(){
        Stats e11;
        e11.name= "Bandit";
        e11.HP= 30;
        e11.maxHP=30;
        e11.STR= 12;
        e11.DEF= 3;
        e11.critDamage= 1.2f;
        e11.critChance= 2;
        e11.XP= 20;
        e11.gold= 20;
        e11.fled= false;
        return e11;
    }

    Stats stats;

    vector<Stats> enemyTemplates;

    struct EnemyTracker{
        vector<string> enemyNames;
        vector<int> enemySlain;

        void init(){
            enemyNames= {"Villager", "Farmer", "Guard", "Guard Dog", "Soldier", "Knight", "Archer Knight", "Mage", "Golem", "Prince", "Thief", "Bandit"};
            enemySlain.assign(12, 0);
        }
        void slainedCount(const string& enemyName){
            for (size_t i = 0; i < enemyNames.size(); i++)
            {
                if (enemyNames[i]== enemyName)
                {
                    enemySlain[i]++;
                    return;
                }
            }
        }
        void slainDisplay(){
            cout<< "*******************************\n";
            cout<< "******** ENEMIES SLAIN ********\n";
            cout<< "*******************************\n\n";
            for (size_t i = 0; i < enemyNames.size(); i++)
            {
                cout<< enemyNames[i]<< ": "<< enemySlain[i]<< " slain.\n";
            }
            cout<< "*******************************\n\n";
            wait(4);
        }
    };

    EnemyTracker enemyTracker;
};

class Bosses{
    public:
    struct Stats{
        string name;
        string skillName;
        float baseMaxHP;
        float maxHP;
        float HP;
        float baseMaxMP;
        int maxMP;
        int MP;
        float baseSTR;
        float STR;
        float baseDEF;
        float DEF;
        float critDMG;
        int CritChance;
        int XP;
        int gold;
        int count;
        float skill;
        int skillCost;
    };

    Stats createBossDragon(){
        Stats b1;
        b1.name= "Dragon";
        b1.skillName= "Fire Breath";
        b1.baseMaxHP= 1000;
        b1.maxHP= b1.baseMaxHP;
        b1.HP= b1.maxHP;
        b1.baseMaxMP= 200;
        b1.maxMP= b1.baseMaxMP;
        b1.MP= b1.maxMP;
        b1.baseSTR= 150;
        b1.STR= b1.baseSTR;
        b1.baseDEF= 100;
        b1.DEF= b1.baseDEF;
        b1.critDMG= 2.0f;
        b1.CritChance= 10;
        b1.XP= 750;
        b1.gold= 2000;
        b1.count= 4;
        b1.skill= b1.baseSTR* 1.5f;
        b1.skillCost= 70;
        return b1;
    }

    Stats createBossKingGrayhaven(){
        Stats b2;
        b2.name= "King Hayvens";
        b2.skillName= "Piercing Thrust";
        b2.baseMaxHP= 500;
        b2.maxHP= b2.baseMaxHP;
        b2.HP= b2.maxHP;
        b2.baseMaxMP= 100;
        b2.maxMP= b2.baseMaxMP;
        b2.MP= b2.maxMP;
        b2.baseSTR= 100;
        b2.STR= b2.baseSTR;
        b2.baseDEF= 70;
        b2.DEF= b2.baseDEF;
        b2.critDMG= 1.5f;
        b2.CritChance= 10;
        b2.XP= 300;
        b2.gold= 500;
        b2.count= 1;
        b2.skill= b2.critDMG* 2.0f;
        b2.skillCost= 50;
        return b2;
    }

    Stats createBossKingStormwich(){
        Stats b3;
        b3.name= "King Emerlude";
        b3.skillName= "Double Slash";
        b3.baseMaxHP= 600;
        b3.maxHP= b3.baseMaxHP;
        b3.HP= b3.maxHP;
        b3.baseMaxMP= 100;
        b3.maxMP= b3.baseMaxMP;
        b3.MP= b3.maxMP;
        b3.baseSTR= 120;
        b3.STR= b3.baseSTR;
        b3.baseDEF= 90;
        b3.DEF= b3.baseDEF;
        b3.critDMG= 1.5f;
        b3.CritChance= 10;
        b3.XP= 300;
        b3.gold= 500;
        b3.count= 1;
        b3.skill= 2;
        b3.skillCost= 50;
        return b3;
    }

    Stats createBossKingHillsgate(){
        Stats b3;
        b3.name= "King Hilverstein";
        b3.skillName= "Poison Stab";
        b3.baseMaxHP= 700;
        b3.maxHP= b3.baseMaxHP;
        b3.HP= b3.maxHP;
        b3.baseMaxMP= 130;
        b3.maxMP= b3.baseMaxMP;
        b3.MP= b3.maxMP;
        b3.baseSTR= 70;
        b3.STR= b3.baseSTR;
        b3.baseDEF= 90;
        b3.DEF= b3.baseDEF;
        b3.critDMG= 1.5f;
        b3.CritChance= 10;
        b3.XP= 300;
        b3.gold= 500;
        b3.count= 1;
        b3.skill= 0;
        b3.skillCost= 60;
        return b3;
    }

    Stats createBossKingCelestial(){
        Stats b3;
        b3.name= "King Celestia";
        b3.skillName= "CRITICAL";
        b3.baseMaxHP= 800;
        b3.maxHP= b3.baseMaxHP;
        b3.HP= b3.maxHP;
        b3.baseMaxMP= 100;
        b3.maxMP= b3.baseMaxMP;
        b3.MP= b3.maxMP;
        b3.baseSTR= 140;
        b3.STR= b3.baseSTR;
        b3.baseDEF= 90;
        b3.DEF= b3.baseDEF;
        b3.critDMG= 1.5f;
        b3.CritChance= 10;
        b3.XP= 300;
        b3.gold= 500;
        b3.count= 1;
        b3.skill= 100;
        b3.skillCost= 50;
        return b3;
    }

    Stats stats;

    struct BossTracker{
        vector<string> bossNames;
        vector<int> bossSlain;

        void init(){
            bossNames= {"Dragon", "King Hayvens", "King Emerlude", "King Hilverstein", "King Celestia"};
            bossSlain.assign(bossNames.size(), 0);
        }
        void slainedCount(const string& bossName){
            for (size_t i = 0; i < bossNames.size(); i++)
            {
                if (bossNames[i]== bossName)
                {
                    bossSlain[i]++;
                    return;
                }
            }
        }
        void slainDisplay(){
            cout<< "*******************************\n";
            cout<< "********** BOSS SLAIN *********\n";
            cout<< "*******************************\n\n";
            for (size_t i = 0; i < bossNames.size(); i++)
            {
                cout<< bossNames[i]<< ": "<< bossSlain[i]<< " slain.\n";
            }
            cout<< "*******************************\n\n";
            wait(4);
        }
    };

    BossTracker bossTracker;
};

class WorldMap{
    public:
    struct Nation
    {
        vector<string> name;
        vector<int> positionx;
        vector<int> positionx1;
        vector<int> positiony;
        vector<int> positiony1;
        vector<int> populationCountMain;
    };

    Nation createNation(){
        Nation n;
        n.name= {"Grayhaven", "Stormwich", "Hillsgate", "Celestial"};
        n.positionx= {50, 600, -1200, -2000};
        n.positionx1= {100, 700, -1600, -2400};
        n.positiony= {20, -80, 500, -1000};
        n.positiony1= {100, -180, 700, -1400};
        return n;
    }

    struct Area{
        vector<string> name;
        vector<int> positionx;
        vector<int> positionx1;
        vector<int> positiony;
        vector<int> positiony1;
        int scaling;
    };

    Area createAreaGrayhaven(){
        Area ar;
        ar.name= {"Capital", "Town", "Market", "Farm"};
        ar.positionx= {76, 50, 76, 50};
        ar.positionx1= {100, 75, 100, 75};
        ar.positiony= {61, 61, 20, 20};
        ar.positiony1= {100, 100, 60, 60};
        ar.scaling= 0;
        return ar;
    }
    
    Area createAreaStormwich(){
        Area ar1;
        ar1.name= {"Capital", "Town", "Market", "Farm"};
        ar1.positionx= {651, 651, 600, 600};
        ar1.positionx1= {700, 700, 650, 650};
        ar1.positiony= {-180, -129, -180, -129};
        ar1.positiony1= {-130, -80, -130, -80};
        ar1.scaling= 1;
        return ar1;
    }
    
    Area createAreaHillsgate(){
        Area ar2;
        ar2.name= {"Capital", "Town", "Market", "Farm"};
        ar2.positionx= {-1600, -1600, -1401, -1401};
        ar2.positionx1= {-1400, -1400,-1200, -1200};
        ar2.positiony= {601, 500, 601, 500};
        ar2.positiony1= {700, 600, 700, 600};
        ar2.scaling= 2;
        return ar2;
    }
    
    Area createAreaCelestial(){
        Area ar3;
        ar3.name= {"Capital", "Town", "Market", "Farm"};
        ar3.positionx= {-2400, -2199, -2400, -2199};
        ar3.positionx1= {-2200, -2000,-2200, -2000};
        ar3.positiony= {-1400, -1400, -1199, -1199};
        ar3.positiony1= {-1200, -1200, -1000, -1000};
        ar3.scaling= 3;
        return ar3;
    }
};

Player player;
Enemies enemies;
Bosses bosses;
WorldMap worldMap;

void printTypewriter(const string& text, int ms_per_char= 30){
    for (char c : text)
    {
        cout<< c<< flush;
        waitms(ms_per_char);
    }
}

void storyIntro(){
    const string story= "You wake up stranded,"; printTypewriter(story); wait();
    const string story1= " your past a blank slate.\n"; printTypewriter(story1); wait(2);
    const string story2= "No memories,"; printTypewriter(story2); wait();
    const string story3= " no clues,"; printTypewriter(story3); wait();
    const string story4= " only a burning"; printTypewriter(story4); wait();
    const string story5= " inexplicable vengeance.\n"; printTypewriter(story5); wait();   
    const string story6= "Who you are."; printTypewriter(story6); wait(2);
    const string story7= " Is a mystery.\n"; printTypewriter(story7); wait(2);
    const string story8= "Why you seek revenge is unknown.\n"; printTypewriter(story8); wait(2);
    const string story9= "But you are determined to reclaim your identity...\n"; printTypewriter(story9); wait();
    const string story10= "and make someone pay.\n\n"; printTypewriter(story10); wait(2);
}

void storyMid(){
    const string story= "Suddenly,"; printTypewriter(story); wait();
    const string story1= " you felt an instinct,"; printTypewriter(story1); wait();
    const string story2= " a thought.\n"; printTypewriter(story2); wait(2);
    const string story3= "A mysterious voice in your head "; printTypewriter(story3); wait();
    const string story4= "that tells you.\n"; printTypewriter(story4); wait(2);
    const string story5= "'I await your presence,"; printTypewriter(story5); wait();
    const string story6= " far away of these lands'"; printTypewriter(story6); wait();
    const string storydot= "."; printTypewriter(storydot); wait(); 
    const string storydot1= "."; printTypewriter(storydot1); wait(); 
    const string storydot2= ".\n\n"; printTypewriter(storydot2); wait(); 
}

void cinClear(){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>:: max(), '\n');
}

void levelUp(){
    if (player.stats.XP>= player.stats.maxXP)
    {
        player.stats.XP-= player.stats.maxXP;
        player.stats.maxXP*= 1.1;
        player.stats.level++;
        player.stats.baseMaxHP+= 10;
        player.stats.baseMaxMP+= 10;
        player.stats.maxHP= player.stats.baseMaxHP;
        player.stats.maxMP= player.stats.baseMaxMP;
        player.stats.baseSTR+= 2;
        player.stats.baseDEF+= 2;
        player.stats.baseCritDMG+= 0.05f;
        player.stats.baseCritChance+= 2;
        player.stats.HP= player.stats.maxHP;
        player.stats.MP= player.stats.maxMP;

        player.skill = player.createPlayerSkill();
        
        player.talismanAttributes(player.inventory, player.talisman);

        cout<< "LEVEL UP!\n\n";
        waitms();
        cout<< "Max HP +10\n";
        waitms();
        cout<< "Max MP +10\n";
        waitms();
        cout<< "STR +2\n";
        waitms();
        cout<< "DEF +2\n";
        waitms();
        cout<< "Crit Damage +0.2\n";
        waitms();
        cout<< "Crit Chance +2%\n";
        waitms();
        cout<< "XP: "<< player.stats.XP<< "/"<< player.stats.maxXP<< "\n\n";
        wait(4);
    }
}

void scaleEnemyByLevel(int playerLevel){
    if (playerLevel<= 1) return;

    const float STAT_SCALING_PER_LEVEL= 0.12f;
    const float REWARD_SCALING_PER_LEVEL= 0.1f;

    float statMultiplier= 1.0f+ (playerLevel- 1)* STAT_SCALING_PER_LEVEL;

    enemies.stats.HP= enemies.stats.HP* statMultiplier;
    enemies.stats.maxHP= enemies.stats.maxHP* statMultiplier;
    enemies.stats.STR= enemies.stats.STR* statMultiplier;
    enemies.stats.DEF= enemies.stats.DEF* statMultiplier;

    float rewardMultiplier= 1.0f+ (playerLevel- 1)* REWARD_SCALING_PER_LEVEL;

    enemies.stats.XP= enemies.stats.XP* rewardMultiplier;
    enemies.stats.gold= enemies.stats.gold* rewardMultiplier;

    enemies.stats.HP= max(1.0f, enemies.stats.HP);
    enemies.stats.maxHP= max(1.0f, enemies.stats.maxHP);
    enemies.stats.STR= max(1.0f, enemies.stats.STR);
    enemies.stats.DEF= max(0.0f, enemies.stats.DEF);
}

void scaleBossByLevel(int playerLevel){
    if (playerLevel<= 1) return;

    const float BOSS_STAT_SCALING_PER_LEVEL= 0.05f;
    const float BOSS_REWARD_SCALING_PER_LEVEL= 0.05f;

    float statMultiplier= 1.0f+ (playerLevel- 1)* BOSS_STAT_SCALING_PER_LEVEL;

    bosses.stats.HP= bosses.stats.HP* statMultiplier;
    bosses.stats.maxHP= bosses.stats.maxHP* statMultiplier;
    bosses.stats.STR= bosses.stats.STR* statMultiplier;
    bosses.stats.DEF= bosses.stats.DEF* statMultiplier;

    float rewardMultiplier= 1.0f+ (playerLevel- 1)* BOSS_REWARD_SCALING_PER_LEVEL;

    bosses.stats.XP= bosses.stats.XP* rewardMultiplier;
    bosses.stats.gold= bosses.stats.gold* rewardMultiplier;

    bosses.stats.HP= max(1.0f, bosses.stats.HP);
    bosses.stats.maxHP= max(1.0f, bosses.stats.maxHP);
    bosses.stats.STR= max(1.0f, bosses.stats.STR);
    bosses.stats.DEF= max(1.0f, bosses.stats.DEF);
}

void scaleByNation(int nationScaliing){
    const float NATION_SCALING= 0.05f;
    const float NATION_REWARD_SCALING= 0.05f;

    float statMultiplier= 1.0f+ (nationScaliing)* NATION_SCALING;

    enemies.stats.HP= enemies.stats.HP* statMultiplier;
    enemies.stats.maxHP= enemies.stats.maxHP* statMultiplier;
    enemies.stats.STR= enemies.stats.STR* statMultiplier;
    enemies.stats.DEF= enemies.stats.DEF* statMultiplier;
    bosses.stats.HP= bosses.stats.HP* statMultiplier;
    bosses.stats.maxHP= bosses.stats.maxHP* statMultiplier;
    bosses.stats.STR= bosses.stats.STR* statMultiplier;
    bosses.stats.DEF= bosses.stats.DEF* statMultiplier;

    float rewardMultiplier= 1.0f+ (nationScaliing)* NATION_REWARD_SCALING;

    enemies.stats.XP= enemies.stats.XP* rewardMultiplier;
    enemies.stats.gold= enemies.stats.gold* rewardMultiplier;
    bosses.stats.XP= bosses.stats.XP* rewardMultiplier;
    bosses.stats.gold= bosses.stats.gold* rewardMultiplier;

    enemies.stats.HP= max(1.0f, enemies.stats.HP);
    enemies.stats.maxHP= max(1.0f, enemies.stats.maxHP);
    enemies.stats.STR= max(1.0f, enemies.stats.STR);
    enemies.stats.DEF= max(0.0f, enemies.stats.DEF);
    bosses.stats.HP= max(1.0f, bosses.stats.HP);
    bosses.stats.maxHP= max(1.0f, bosses.stats.maxHP);
    bosses.stats.STR= max(1.0f, bosses.stats.STR);
    bosses.stats.DEF= max(1.0f, bosses.stats.DEF);
}

void shop(){
    const int HEALTH_POTION_PRICE= 50;
    const int MANA_POTION_PRICE= 30;

    Player::Weapon w= player.createWeapon();
    Player::Armor a= player.createArmor();
    Player::Talisman t= player.createTalisman();

    auto discount= [&](int basePrice) -> int {
        if (player.stats.trust>= 500)
        {
            return basePrice/ 2;
        }
        return basePrice;
    };

    int shopInput;
    int confirmHealth;
    int confirmMana;
    int inputWeapon;
    int confirmWeapon;
    int inputArmor;
    int confirmArmor;
    int inputTalisman;
    int confirmTalisman;

    bool inShop= true;

    while (inShop)
    {
        cout<<"**************************\n";
        cout<<"********** SHOP **********\n";
        cout<<"**************************\n\n";
        cout<< "Gold: "<< player.stats.gold<< "\n\n";
        if (player.stats.trust>= 500)
        {
            cout<< "Trust Discount= 50%\n\n";
        }

        int discountHealthPotion= discount(HEALTH_POTION_PRICE);
        int discountManaPotion= discount(MANA_POTION_PRICE); 

        cout<< "1. Health Potion ("<< discountHealthPotion<< " gold)";
        if(player.stats.trust>= 500){
            cout<< " was ("<< HEALTH_POTION_PRICE<< ") [";
        }
        cout<< player.inventory.potionHealth<< "]\n";
        cout<< "2. Mana Potion ("<< discountManaPotion<< " gold)";
        if(player.stats.trust>= 500){
            cout<< " was ("<< MANA_POTION_PRICE<< ") [";
        }
        cout<< player.inventory.potionMana<< "]\n";
        cout<< "3. Weapons ["<< player.getWeaponName(player.inventory, w)<< "]\n";
        cout<< "4. Armor ["<< player.getArmorName(player.inventory, a)<< "]\n";
        cout<< "5. Talismans ["<< player.getTalismanName(player.inventory, t)<< "/"<< player.getTalisman1Name(player.inventory, t)<< "]\n";
        cout<< "0. Leave Shop\nChoice: ";

        if (!(cin>>shopInput))
        {
            cinClear();
            shopInput= -1;
        }
        cout<< '\n';

        if (shopInput== 0)
        {
            cout<< "Thanks for visiting!\n\n";
            wait();
            inShop= false;
        } else if (shopInput== 1)
        {
            if (player.stats.gold>= discountHealthPotion)
            {
                cout<< "Are you sure?\n1. Confirm\n0. Cancel\n";
                if (!(cin>> confirmHealth))
                {
                    cinClear();
                }
                cout<< "\n";
                if (confirmHealth== 1)
                {
                    player.stats.gold-= discountHealthPotion;
                    player.inventory.potionHealth++;
                    cout<< "Thanks for purchasing!\n";
                    wait();
                } else{
                    cout<< "Very well!\n";
                    continue;
                }
            }
        } else if (shopInput== 2)
        {
            if (player.stats.gold>= discountManaPotion)
            {
                cout<< "Are you sure?\n1. Confirm\n0. Cancel\n";
                if (!(cin>> confirmMana))
                {
                    cinClear();
                }
                cout<< "\n";
                if (confirmMana== 1)
                {
                    player.stats.gold-= discountManaPotion;
                    player.inventory.potionMana++;
                    cout<< "Thanks for purchasing!\n";
                    wait();
                } else{
                    cout<< "Very well!\n";
                    continue;
                }
            }
        } else if (shopInput== 3)
        {
            cout<< "******** WEAPONS ********\n\n";
            cout<< "Gold: "<< player.stats.gold<< "\n\n";

            for (size_t i = 1; i < w.weaponList.size(); i++)
            {
                int discountWeapon= discount(w.WEAPON_PRICE[i]);
                int menuNumber= i;
                bool isEquipped= (static_cast<size_t>(player.inventory.weapon)== i);

                cout<< menuNumber<< ". "<< w.weaponList[i]<< ", DMG: "<< w.weaponDMG[i]<< "\nPrice: "<< discountWeapon<< " gold.";
                if (player.stats.trust>= 500)
                {
                    cout<< " was ["<< w.WEAPON_PRICE[i]<< "]";
                }
                cout<< "\n\n";
                waitms(100);
                
                if (isEquipped) cout<< " [EQUIPPED]\n";
            }
            cout<< "0. Leave Menu\nChoice: ";
            
            if (!(cin>> inputWeapon))
            {
                cinClear();
            }
            cout<< "\n";
            
            if (inputWeapon>= 1&& static_cast<size_t>(inputWeapon)< w.weaponList.size())
            {
                int weaponIndex= inputWeapon;
                int discountWeapon= discount(w.WEAPON_PRICE[weaponIndex]);
                if (weaponIndex<= 0|| static_cast<size_t>(weaponIndex)>= w.weaponList.size())
                {
                    cout<< "Invalid weapon selection!\n\n";
                    wait();
                    continue;
                }
                if (player.inventory.weapon== weaponIndex)
                {
                    cout<< "You've equipped "<< w.weaponList[weaponIndex]<< "already!\n\n";
                    wait();
                } else if (player.stats.gold< discountWeapon)
                {
                    cout<< "Not enough gold!\n\n";
                    wait();
                } else{
                    cout<< "Are you sure?\n1. Confirm\n0. Cancel\n";
                    if (!(cin>> confirmWeapon))
                    {
                        cinClear();
                    }
                    cout<< "\n";
                    if (confirmWeapon== 1)
                    {
                        player.stats.gold-= discountWeapon;
                        player.inventory.weapon= weaponIndex;
                        cout<< "Thanks for purchasing!\n";
                        wait();
                    } else{
                        cout<< "Very well!\n";
                        continue;
                    }
                }
            }
        } else if (shopInput== 4)
        {
            cout<< "******** ARMOR ********\n\n";
            cout<< "Gold: "<< player.stats.gold<< "\n\n";

            for (size_t i = 1; i < a.armorList.size(); i++)
            {
                int discountArmor= discount(a.ARMOR_PRICE[i]);
                int menuNumber1= i;
                bool isEquipped1= (static_cast<size_t>(player.inventory.armor)== i);

                cout<< menuNumber1<< ". "<< a.armorList[i]<< "\nHP: "<< a.armorHP[i]<< ", DEF: "<< a.armorDefence[i]<< "\nPrice: "<< discountArmor<< " gold.";
                if (player.stats.trust>= 500)
                {
                    cout<< " was ["<< a.ARMOR_PRICE[i]<< "]";
                }
                cout<< "\n\n";
                waitms(100);
                
                if (isEquipped1) cout<< " [EQUIPPED]\n\n\n";
            }
            cout<< "0. Leave Menu\nChoice: ";
            
            if (!(cin>> inputArmor))
            {
                cinClear();
            }
            cout<< "\n";
            
            if (inputArmor>= 1&& static_cast<size_t>(inputArmor)< a.armorList.size())
            {
                int armorIndex= inputArmor;
                int discountArmor= discount(a.ARMOR_PRICE[armorIndex]);
                if (armorIndex<= 0|| static_cast<size_t>(armorIndex)>= a.armorList.size())
                {
                    cout<< "Invalid armmor selection!\n\n";
                    wait();
                    continue;
                }
                if (player.inventory.armor== armorIndex)
                {
                    cout<< "You've equipped "<< a.armorList[armorIndex]<< "already!\n\n";
                    wait();
                } else if (player.stats.gold< discountArmor)
                {
                    cout<< "Not enough gold!\n\n";
                    wait();
                } else{
                    cout<< "Are you sure?\n1. Confirm\n0. Cancel\n";
                    if (!(cin>> confirmArmor))
                    {
                        cinClear();
                    }
                    cout<< "\n";
                    if (confirmArmor== 1)
                    {
                        player.stats.gold-= discountArmor;
                        player.inventory.armor= armorIndex;
                        cout<< "Thanks for purchasing!\n";
                        wait();
                        player.talismanAttributes(player.inventory, player. talisman);
                    } else{
                        cout<< "Very well!\n";
                        continue;
                    }
                }
            }
        } else if (shopInput== 5)
        {
            cout<< "******** TALISMANS ********\n\n";
            cout<< "Gold: "<< player.stats.gold<< "\n\n";
            
            for (size_t j = 1; j < t.talismanList.size(); j++)
            {
                int menuNumber2= j;
                int discountTalisman= discount(t.TALISMAN_PRICE[j]);
                bool isEquipped2= (static_cast<size_t>(player.inventory.talisman)== j);
                bool isEquipped3= (static_cast<size_t>(player.inventory.talisman1)== j);

                cout<< menuNumber2<< ". "<< t.talismanList[j]<< "\n";

                bool hasStats= false;
                
                if (t.talismanHP[j]> 0)
                {
                    cout<< "HP+ "<< t.talismanHP[j]<< "   "; hasStats= true;
                }
                if (t.talismanMP[j]> 0)
                {
                    cout<< "MP+ "<< t.talismanMP[j]<< "   "; hasStats= true;
                }
                if (t.talismanSTR[j]> 0)
                {
                    cout<< "STR+ "<< t.talismanSTR[j]<< "   "; hasStats= true;
                }
                if (t.talismanDEF[j]> 0)
                {
                    cout<< "DEF+ "<< t.talismanDEF[j]<< "   "; hasStats= true;
                }
                if (t.talismanCritDMG[j]> 0)
                {
                    cout<< "Crit Damage+ "<< t.talismanCritDMG[j]<< "   "; hasStats= true;
                }
                if (t.talismanCritChance[j]> 0)
                {
                    cout<< "Crit Chance+ "<< t.talismanCritChance[j]<< "%   "; hasStats= true;
                }
                if (t.talismanGold[j]> 0)
                {
                    cout<< "Bonus Gold+ "<< t.talismanGold[j]<< "   "; hasStats= true;
                }
                if (!hasStats) cout<< "No stats";
                
                cout<< "\nPrice: "<< discountTalisman<< " gold.";
                cout<< " was ["<< t.TALISMAN_PRICE[j]<< "]";
                cout<< "\n\n";

                if (isEquipped2|| isEquipped3) cout<< " [EQUIPPED]\n";
                waitms(100);
            }
            cout<< "0. Leave Menu\nChoice: ";

            if (!(cin>>inputTalisman))
            {
                cinClear();
            }
            cout<< "\n";
            
            if (inputTalisman== 0)
            {
            } else if (inputTalisman>= 1&& static_cast<size_t>(inputTalisman)< t.talismanList.size())
            {
                int talismanIndex= inputTalisman;
                int discountTalisman= discount(t.TALISMAN_PRICE[talismanIndex]);
                
                if (player.inventory.talisman== talismanIndex|| player.inventory.talisman1== talismanIndex)
                {
                    cout<< "You've owned "<< t.talismanList[talismanIndex]<< " already!\n\n";
                    wait();
                } else if (player.stats.gold< discountTalisman)
                {
                    cout<< "Not enough gold!\n\n";
                } else{
                    cout<< "Are you sure?\n1. Confirm\n0. Cancel\n";
                    if (!(cin>> confirmTalisman))
                    {
                        cinClear();
                    }
                    cout<< "\n";
                    if (confirmTalisman== 1)
                    {
                        cout<< "Equip to slot (1 or 2): ";
                        int slotInput;
                        if (!(cin>> slotInput))
                        {
                            cinClear();
                            slotInput= 1;
                        }
                        cout<< "\n";
                        if (slotInput== 1)
                        {
                            player.inventory.talisman= talismanIndex;
                        } else if (slotInput== 2)
                        {
                            player.inventory.talisman1= talismanIndex;
                        } else{
                            cout<< "Invalid slot!\nDefaulting to slot 1.\n";
                            player.inventory.talisman= talismanIndex;
                            wait();
                        }
                        
                        player.stats.gold-= discountTalisman;
                        cout<< "Thanks for purchasing!\n\n";

                        player.talismanAttributes(player.inventory, t);
                    } else{
                        cout<< "Very well!\n\n";
                        continue;
                    }
                }

            } 
        } else{
            return;
        }
    }
}

inline void resetPlayerBuffs(){
    player.stats.critBuffTurns= 0;
    player.stats.critChanceBuffTurns= 0;
}

const int FLEE_HP_PERCENTAGE= 10;
const int FLEE_HP_PERCENTAGE_CIVILIAN= 30;
const int FLEE_CHANCE = 15;
const int FLEE_CHANCE_CIVILIAN= 40;
const float POTION_HEAL= 0.4f;
const int MANA_POTION_HEAL= 50;

void battle(){
    cout<< fixed<< setprecision(1);

    inline void resetPlayerBuffs();

    cout<< enemies.stats.name<< " interrupted!\n\n";

    for (int i = 0; i < 10; i++)
    {
        cout<< "***";
        waitms();
    }
    cout<< "***\n\n";
    waitms();

    while (player.stats.HP> 0&& enemies.stats.HP> 0)
    {
        player.stats.MP++;

        if (player.stats.MP> player.stats.maxMP) player.stats.MP= player.stats.maxMP;
        
        player.stats.critBuffTurns--;

        if (player.stats.critBuffTurns< 0) player.stats.critBuffTurns= 0;
        
        player.stats.critChanceBuffTurns--;

        if (player.stats.critChanceBuffTurns< 0) player.stats.critChanceBuffTurns= 0;

        cout<< "***** "<< enemies.stats.name<< " Stats *****\n";
        cout<< "HP: "<< enemies.stats.HP<< "/"<< enemies.stats.maxHP<< "\n";
        cout<< "STR: "<< enemies.stats.STR<< "\n";
        cout<< "DEF: "<< enemies.stats.DEF<< "\n";
        cout<< "Crit Damage: "<< enemies.stats.critDamage<< "\n";
        cout<< "Crit Chance: "<< enemies.stats.critChance<< "%\n";
        cout<< "************************\n\n";
        wait();

        bool extraAction= false;
        float eATK= 0.0f;
        float armorDefence= player.armor.armorDefence[player.inventory.armor];

        do
        {
            int battleInput1, skillInput1, potionInput1;

            // PLAYER DECLARATION

            float effectiveCritDMG= (player.stats.critBuffTurns> 0)? player.stats.critDMG* 2: player.stats.critDMG;
            float effectiveCritChance= (player.stats.critChanceBuffTurns> 0)? player.stats.critChance* 2: player.stats.critChance; 
            bool isCritical= (rand()% 100)< effectiveCritChance;
            float critMultiplier= isCritical? effectiveCritDMG: 1;
            float weaponDamage= player.weapon.weaponDMG[player.inventory.weapon];
            float baseDamagePlayer= (player.stats.STR+ weaponDamage)- (enemies.stats.DEF* 2/ 5);
            float attackVariance= rand()% player.weapon.weaponMaxAttackVariance[player.inventory.weapon]- player.weapon.weaponMinAttackVariance[player.inventory.weapon];
            float playerATK= max(1.0f, baseDamagePlayer+ attackVariance)* critMultiplier;
            
            // ENEMY DECLARATION
        
            int eAttackVariance= rand()% 5- 3;
            float eBaseDamage= enemies.stats.STR- ((player.stats.DEF+ armorDefence)* 2/ 5);
            bool eIsCritical= rand()% 100< enemies.stats.critChance;
            float eCritMultiplier= eIsCritical? enemies.stats.critDamage: 1;
            eATK= max(1.0f, eBaseDamage+ eAttackVariance)* eCritMultiplier;

            // PLAYER TURN

            cinClear();

            cout<< "1. Attack\n2. Skill\n3. Check Stats\n4. Inventory\n5. Potions\n0. Run\nChoice: ";
            if (!(cin>>battleInput1))
            {
                cinClear();
            }
            cout<< "\n";

            if (battleInput1< 0|| battleInput1> 5)
            {
                cout<< "Invalid choice!\n\n";
                wait(2);
                continue;
            }

            if (battleInput1== 0)
            {
                int chicken1= rand()% 100;
                if (chicken1< 20)
                {
                    cout<< "Failed!\n\n";
                    wait();
                    cout<< "The enemy catches up\n\n";
                    wait();
                    player.stats.HP-= eATK;
                    cout<< enemies.stats.name<< " attacks for "<< eATK<< " damage!\n";
                    wait(2);
                    cout<< player.stats.name<< "'s HP: "<< max(0.0f, player.stats.HP)<< "\n\n";
                    wait(2);

                    if (player.stats.HP<= 0)
                    {
                        cout<< "You";
                        waitms();
                        cout<< " ";
                        waitms();
                        cout<< "have";
                        waitms();
                        cout<< " ";
                        waitms();
                        cout<< "been";
                        waitms();
                        cout<< " ";
                        waitms();
                        cout<< "defeated!\n\n";
                        waitms();

                        cout<< "Exiting.";
                        wait();
                        cout<< ".";
                        wait();
                        cout<< ".";
                        return;
                    }
                    break;
                } else{
                    cout<< "You ran away\n\n";
                    wait();
                    return;
                }
            }
            if (battleInput1== 1)
            {
                enemies.stats.HP-= playerATK;
                cout<< "You attacked for "<< playerATK<< " damage!\n";
                if (isCritical== true)
                {
                    cout<< "CRITICAL HIT!!!\n\n";
                }
                wait(2);
                cout<< enemies.stats.name<< "'s HP "<< max(0.0f, enemies.stats.HP)<< "\n\n";
                wait(2);
                extraAction= false;
            } else if (battleInput1== 2)
            {
                cout<< "Mana: "<< player.stats.MP<< "\n";
                cout<< "Choose skill:\n1. Brute Strike: STR *1.5.   Mana required: "<< player.skill.skillMPCost<< ".\n";
                cout<< "2. Focus Strike: Crit Damage *1.2.   Mana required: "<< player.skill.skill1MPCost<< ".\n";
                cout<< "3. Enchanced Perception: Crit Chance *2.   Mana required: "<< player.skill.skill2MPCost<< ".\n0. Cancel\nChoice: ";
                
                if (!(cin>>skillInput1))
                {
                    cinClear();
                    skillInput1= 0;
                }
                cout<< "\n";
                if (skillInput1<=0 || skillInput1> 3)
                {
                    cout<< "You canceled cast\n\n";
                    wait(2);
                    extraAction= true;
                }
                if (skillInput1== 1)
                {
                    if (player.stats.MP< player.skill.skillMPCost)
                    {
                        cout<< "Not enough MP! ("<< player.skill.skillMPCost<< " MP required)\n";
                        wait();
                        extraAction= true;
                        continue;
                    }
                    cout<< "You used Brute Strike!\n";
                    wait(2);

                    float effectiveCritDMG= (player.stats.critBuffTurns> 0)? player.stats.critDMG* 2: player.stats.critDMG;
                    float effectiveCritChance= (player.stats.critChanceBuffTurns)> 0? player.stats.critChance* 2: player.stats.critChance;
                    bool isCritical= (rand()% 100)< effectiveCritChance;
                    float critMultiplier= isCritical? effectiveCritDMG: 1;
                    float weaponDamage= player.weapon.weaponDMG[player.inventory.weapon];
                    float baseDamagePlayer= (player.skill.skill+ weaponDamage)- (enemies.stats.DEF* 2/ 5);
                    float attackVariance= rand()% player.weapon.weaponMaxAttackVariance[player.inventory.weapon]- player.weapon.weaponMinAttackVariance[player.inventory.weapon];
                    float playerATK= max(1.0f, baseDamagePlayer+ attackVariance)* critMultiplier;
                    
                    player.stats.MP-= player.skill.skillMPCost;

                    enemies.stats.HP-= playerATK;
                    cout<< "You attacked for "<< playerATK<< " damage\n";
                    if (isCritical== true)
                    {
                        cout<< "CRITICAL HIT!!!\n\n";
                    }
                    wait(2);
                    cout<< enemies.stats.name<< "'s HP: "<< max(0.0f, enemies.stats.HP)<< "\n\n";
                    wait(2);
                    extraAction= false;
                } else if (skillInput1== 2)
                {
                    if (player.stats.MP< player.skill.skill1MPCost)
                    {
                        cout<< "Not enough MP! ("<< player.skill.skill1MPCost<< " MP required)\n";
                        wait();
                        extraAction= true;
                        continue;
                    }

                    player.stats.critBuffTurns+= 2;
                    player.stats.MP-= player.skill.skill1MPCost;

                    cout<< "You used Focus Strike! Critical Damage is buffed for 2 turns!\n\n";
                    wait(2);
                    extraAction= true;
                    continue;
                } else if (skillInput1== 3)
                {
                    if (player.stats.MP< player.skill.skill2MPCost)
                    {
                        cout<< "Not enough MP! ("<< player.skill.skill2MPCost<< " MP required)\n";
                        wait();
                        extraAction= true;
                        continue;
                    }

                    player.stats.critChanceBuffTurns+= 2;
                    player.stats.MP-= player.skill.skill2MPCost;

                    cout<< "You used Enhanced Perception! Critical Chance is doubled for 2 turns!\n\n";
                    wait(2);
                    extraAction= true;
                    continue;
                }
            } else if (battleInput1== 3)
            {
                player.checkStats();
                extraAction= true;
                continue;
            } else if (battleInput1== 4)
            {
                player.Inv(player.inventory, player.weapon, player.armor, player.talisman);
                extraAction= true;
                continue;
            } else if (battleInput1== 5)
            {
                cout<< "1. Health Potion ["<< player.inventory.potionHealth<< "]\n";
                cout<< "2. Mana Potion ["<< player.inventory.potionMana<< "]\n";
                cout<< "0. Cancel\nChoice: ";

                if (!(cin>> potionInput1))
                {
                    cinClear();
                    potionInput1= 0;
                }
                cout<< "\n";

                if (potionInput1== 1)
                {
                    if (player.inventory.potionHealth> 0)
                    {
                        int heal= player.stats.maxHP* POTION_HEAL;
                        player.stats.HP= min(player.stats.maxHP, player.stats.HP+ (player.stats.maxHP* POTION_HEAL));
                        player.inventory.potionHealth--;
                        cout<< "Restored "<< heal<< " HP!\n\n";
                        wait(2);
                    } else{
                        cout<< "No health potion!\n\n";
                    }
                    
                } else if (potionInput1== 2)
                {
                    if (player.inventory.potionMana> 0)
                    {
                        player.stats.MP= min(player.stats.maxMP, player.stats.MP+ MANA_POTION_HEAL);
                        player.inventory.potionMana--;
                        cout<< "Restored "<< MANA_POTION_HEAL<< " MP!\n\n";
                        wait(2);
                    } else{
                        cout<< "No mana potion!\n\n";
                    }
                } 
                extraAction= false;
            }
            if (enemies.stats.HP<= 0)
            {
                cout<< "You have defeated the "<< enemies.stats.name<< "!\n";
                wait();
                cout<< "XP gained: "<< enemies.stats.XP<< "\n";
                wait();
                cout<< "Gold gained: "<< enemies.stats.gold<< "\n";
                wait(2);

                enemies.enemyTracker.slainedCount(enemies.stats.name);

                player.talismanAttributes(player.inventory, player.talisman);

                player.stats.XP+= enemies.stats.XP;
                player.stats.gold+= enemies.stats.gold;

                levelUp();
                
                break;
            }
        } while (extraAction);

        if (enemies.stats.HP<= 0)
        {
            break;
        }

        if (enemies.stats.HP> 0&& enemies.stats.HP<= enemies.stats.maxHP* FLEE_HP_PERCENTAGE/ 100)
        {
            int roll= rand()% 100;
            if (roll< FLEE_CHANCE)
            {
                cout<< enemies.stats.name<< " fled.\n";
                wait(2);
                return;
            }
        }
        
        // ENEMY TURN

        if (enemies.stats.HP> 0)
        {
            player.stats.HP-= eATK;
            cout<< enemies.stats.name<< " attacks for "<< eATK<< " damage!\n";
            wait(2);
            cout<< player.stats.name<< "'s HP: "<< max(0.0f, player.stats.HP)<< "\n\n";
            wait(2);
        }
        
        if (player.stats.HP<= 0)
        {
            cout<< "You";
            waitms();
            cout<< " ";
            waitms();
            cout<< "have";
            waitms();
            cout<< " ";
            waitms();
            cout<< "been";
            waitms();
            cout<< " ";
            waitms();
            cout<< "defeated!\n\n";
            waitms();

            cout<< "Exiting.";
            wait();
            cout<< ".";
            wait();
            cout<< ".\n";
            return;
        }
    }
}

void wildernessBattle(){
    cout<< fixed<< setprecision(1);

    inline void resetPlayerBuffs();

    cout<< "A wild "<< enemies.stats.name<< " has appeared!\n\n";

    for (int i = 0; i < 10; i++)
    {
        cout<< "***";
        waitms();
    }
    cout<< "***\n\n";
    waitms();

    while (player.stats.HP> 0&& enemies.stats.HP> 0)
    {
        player.stats.MP++;
        
        if (player.stats.MP> player.stats.maxMP) player.stats.MP= player.stats.maxMP;
        
        player.stats.critBuffTurns--;

        if (player.stats.critBuffTurns< 0) player.stats.critBuffTurns= 0;
        
        player.stats.critChanceBuffTurns--;

        if (player.stats.critChanceBuffTurns< 0) player.stats.critChanceBuffTurns= 0;
        
        cout<< "***** "<< enemies.stats.name<< " Stats *****\n";
        cout<< "HP: "<< enemies.stats.HP<< "/"<< enemies.stats.maxHP<< "\n";
        cout<< "STR: "<< enemies.stats.STR<< "\n";
        cout<< "DEF: "<< enemies.stats.DEF<< "\n";
        cout<< "Crit Damage: "<< enemies.stats.critDamage<< "\n";
        cout<< "Crit Chance: "<< enemies.stats.critChance<< "%\n";
        cout<< "************************\n\n";
        wait();

        bool extraAction= false;
        float eATK= 0.0f;
        float armorDefence= player.armor.armorDefence[player.inventory.armor];

        do
        {
            int battleInput1, skillInput1, potionInput1;

            float effectiveCritDMG= (player.stats.critBuffTurns> 0)? player.stats.critDMG* 2: player.stats.critDMG;
            float effectiveCritChance= (player.stats.critChanceBuffTurns> 0)? player.stats.critChance* 2: player.stats.critChance; 
            bool isCritical= (rand()% 100)< effectiveCritChance;
            float critMultiplier= isCritical? effectiveCritDMG: 1;
            float weaponDamage= player.weapon.weaponDMG[player.inventory.weapon];
            float baseDamagePlayer= (player.stats.STR+ weaponDamage)- (enemies.stats.DEF* 2/ 5);
            float attackVariance= rand()% player.weapon.weaponMaxAttackVariance[player.inventory.weapon]- player.weapon.weaponMinAttackVariance[player.inventory.weapon];
            float playerATK= max(1.0f, baseDamagePlayer+ attackVariance)* critMultiplier;
        
            int eAttackVariance= rand()% 5- 3;
            float eBaseDamage= enemies.stats.STR- ((player.stats.DEF+ armorDefence)* 2/ 5);
            bool eIsCritical= rand()% 100< enemies.stats.critChance;
            float eCritMultiplier= eIsCritical? enemies.stats.critDamage: 1;
            eATK= max(1.0f, eBaseDamage+ eAttackVariance)* eCritMultiplier;

            cinClear();

            cout<< "1. Attack\n2. Skill\n3. Check Stats\n4. Inventory\n5. Potions\n0. Run\nChoice: ";
            if (!(cin>>battleInput1))
            {
                cinClear();
                battleInput1= -1;
            }
            cout<< "\n";

            if (battleInput1< 0|| battleInput1> 5)
            {
                cout<< "Invalid choice!\n\n";
                wait(2);
                continue;
            }

            if (battleInput1== 0)
            {
                int chicken1= rand()% 100;
                if (chicken1< 20)
                {
                    cout<< "Failed!\n\n";
                    wait();
                    cout<< "The enemy catches up\n\n";
                    wait();
                    player.stats.HP-= eATK;
                    cout<< enemies.stats.name<< " attacks for "<< eATK<< " damage!\n";
                    wait(2);
                    cout<< player.stats.name<< "'s HP: "<< max(0.0f, player.stats.HP)<< "\n\n";
                    wait(2);

                    if (player.stats.HP<= 0)
                    {
                        cout<< "You";
                        waitms();
                        cout<< " ";
                        waitms();
                        cout<< "have";
                        waitms();
                        cout<< " ";
                        waitms();
                        cout<< "been";
                        waitms();
                        cout<< " ";
                        waitms();
                        cout<< "defeated!\n\n";
                        waitms();

                        cout<< "Exiting.";
                        wait();
                        cout<< ".";
                        wait();
                        cout<< ".\n";
                        return;
                    }
                    break;
                } else{
                    cout<< "You ran away\n\n";
                    wait();
                    return;
                }
            }
            if (battleInput1== 1)
            {
                enemies.stats.HP-= playerATK;
                cout<< "You attacked for "<< playerATK<< " damage!\n";
                if (isCritical== true)
                {
                    cout<< "CRITICAL HIT!!!\n\n";
                }
                wait(2);
                cout<< enemies.stats.name<< "'s HP "<< max(0.0f, enemies.stats.HP)<< "\n\n";
                wait(2);
                extraAction= false;
            } else if (battleInput1== 2)
            {
                cout<< "Mana: "<< player.stats.MP<< "\n";
                cout<< "Choose skill:\n1. Brute Strike: STR *1.5.   Mana required: "<< player.skill.skillMPCost<< ".\n";
                cout<< "2. Focus Strike: Crit Damage *1.2.   Mana required: "<< player.skill.skill1MPCost<< ".\n";
                cout<< "3. Enchanced Perception: Crit Chance *2.   Mana required: "<< player.skill.skill2MPCost<< ".\n0. Cancel\nChoice: ";
                
                if (!(cin>>skillInput1))
                {
                    cinClear();
                    skillInput1= -1;
                    extraAction= true;
                }
                cout<< "\n";
                if (skillInput1<=0 || skillInput1> 3)
                {
                    cout<< "You canceled cast\n\n";
                    wait(2);
                    extraAction= true;
                }
                if (skillInput1== 1)
                {
                    if (player.stats.MP< player.skill.skillMPCost)
                    {
                        cout<< "Not enough MP! ("<< player.skill.skillMPCost<< " MP required)\n";
                        wait();
                        extraAction= true;
                        continue;
                    }
                    cout<< "You used Brute Strike!\n";
                    wait(2);

                    float effectiveCritDMG= (player.stats.critBuffTurns> 0)? player.stats.critDMG* 2: player.stats.critDMG;
                    float effectiveCritChance= (player.stats.critChanceBuffTurns)> 0? player.stats.critChance* 2: player.stats.critChance;
                    bool isCritical= (rand()% 100)< effectiveCritChance;
                    float critMultiplier= isCritical? effectiveCritDMG: 1;
                    float weaponDamage= player.weapon.weaponDMG[player.inventory.weapon];
                    float baseDamagePlayer= (player.skill.skill+ weaponDamage)- (enemies.stats.DEF* 2/ 5);
                    float attackVariance= rand()% player.weapon.weaponMaxAttackVariance[player.inventory.weapon]- player.weapon.weaponMinAttackVariance[player.inventory.weapon];
                    float playerATK= max(1.0f, baseDamagePlayer+ attackVariance)* critMultiplier;
                    
                    player.stats.MP-= player.skill.skillMPCost;

                    enemies.stats.HP-= playerATK;
                    cout<< "You attacked for "<< playerATK<< " damage\n";
                    if (isCritical== true)
                    {
                        cout<< "CRITICAL HIT!!!\n\n";
                    }
                    wait(2);
                    cout<< enemies.stats.name<< "'s HP: "<< max(0.0f, enemies.stats.HP)<< "\n\n";
                    wait(2);
                    extraAction= false;
                } else if (skillInput1== 2)
                {
                    if (player.stats.MP< player.skill.skill1MPCost)
                    {
                        cout<< "Not enough MP! ("<< player.skill.skill1MPCost<< " MP required)\n";
                        wait();
                        extraAction= true;
                        continue;
                    }

                    player.stats.critBuffTurns+= 2;
                    player.stats.MP-= player.skill.skill1MPCost;

                    cout<< "You used Focus Strike! Critical Damage is buffed for 2 turns!\n\n";
                    wait(2);
                    extraAction= true;
                    continue;
                } else if (skillInput1== 3)
                {
                    if (player.stats.MP< player.skill.skill2MPCost)
                    {
                        cout<< "Not enough MP! ("<< player.skill.skill2MPCost<< " MP required)\n";
                        wait();
                        extraAction= true;
                        continue;
                    }

                    player.stats.critChanceBuffTurns+= 2;
                    player.stats.MP-= player.skill.skill2MPCost;

                    cout<< "You used Enhanced Perception! Critical Chance is doubled for 2 turns!\n\n";
                    wait(2);
                    extraAction= true;
                    continue;
                }
            } else if (battleInput1== 3)
            {
                player.checkStats();
                extraAction= true;
                continue;
            } else if (battleInput1== 4)
            {
                player.Inv(player.inventory, player.weapon, player.armor, player.talisman);
                extraAction= true;
                continue;
            } else if (battleInput1== 5)
            {
                cout<< "1. Health Potion ["<< player.inventory.potionHealth<< "]\n";
                cout<< "2. Mana Potion ["<< player.inventory.potionMana<< "]\n";
                cout<< "0. Cancel\nChoice: ";

                if (!(cin>> potionInput1))
                {
                    cinClear();
                    potionInput1= 0;
                }
                cout<< "\n";

                if (potionInput1== 1)
                {
                    if (player.inventory.potionHealth> 0)
                    {
                        int heal= player.stats.maxHP* POTION_HEAL;
                        player.stats.HP= min(player.stats.maxHP, player.stats.HP+ (player.stats.maxHP* POTION_HEAL));
                        player.inventory.potionHealth--;
                        cout<< "Restored "<< heal<< " HP!\n\n";
                        wait(2);
                    } else{
                        cout<< "No health potion!\n\n";
                        wait();
                        extraAction= true;
                        continue;
                    }
                    
                } else if (potionInput1== 2)
                {
                    if (player.inventory.potionMana> 0)
                    {
                        player.stats.MP= min(player.stats.maxMP, player.stats.MP+ MANA_POTION_HEAL);
                        player.inventory.potionMana--;
                        cout<< "Restored "<< MANA_POTION_HEAL<< " MP!\n\n";
                        wait(2);
                    } else{
                        cout<< "No mana potion!\n\n";
                        wait();
                        extraAction= true;
                        continue;
                    }
                } 
                extraAction= false;
            }
            if (enemies.stats.HP<= 0)
            {
                cout<< "You have defeated the "<< enemies.stats.name<< "!\n";
                wait();
                cout<< "XP gained: "<< enemies.stats.XP<< "\n";
                wait();
                cout<< "Gold gained: "<< enemies.stats.gold<< "\n";
                wait(2);

                enemies.enemyTracker.slainedCount(enemies.stats.name);

                player.talismanAttributes(player.inventory, player.talisman);

                player.stats.XP+= enemies.stats.XP;
                player.stats.gold+= enemies.stats.gold;

                levelUp();

                break;
            }
        } while (extraAction);

        if (enemies.stats.HP<= 0)
        {
            break;
        }

        if (enemies.stats.HP> 0&& enemies.stats.HP<= enemies.stats.maxHP* FLEE_HP_PERCENTAGE/ 100)
        {
            int roll= rand()% 100;
            if (roll< FLEE_CHANCE)
            {
                cout<< enemies.stats.name<< " fled.\n";
                enemies.stats.fled= true;
                wait(2);
                return;
            }
        }

        if (enemies.stats.HP> 0)
        {
            player.stats.HP-= eATK;
            cout<< enemies.stats.name<< " attacks for "<< eATK<< " damage!\n";
            wait(2);
            cout<< player.stats.name<< "'s HP: "<< max(0.0f, player.stats.HP)<< "\n\n";
            wait(2);
        }
        
        if (player.stats.HP<= 0)
        {
            cout<< "You";
            waitms();
            cout<< " ";
            waitms();
            cout<< "have";
            waitms();
            cout<< " ";
            waitms();
            cout<< "been";
            waitms();
            cout<< " ";
            waitms();
            cout<< "defeated!\n\n";
            waitms();

            cout<< "Exiting.";
            wait();
            cout<< ".";
            wait();
            cout<< ".\n";
            return;
        }
    } 
}

void battleVillager(int nationScale){
    enemies.stats= enemies.createEnemyVillager();
    scaleEnemyByLevel(player.stats.level);
    scaleByNation(nationScale);
    battle();
    if (enemies.stats.HP<= 0){
        player.stats.trust-= 10;
        cout<< "Trust gained: -10\n\n";
    }
    wait(2);
}

void battleFarmer(int nationScale){
    enemies.stats= enemies.createEnemyFarmer();
    scaleEnemyByLevel(player.stats.level);
    scaleByNation(nationScale);
    battle();
    if (enemies.stats.HP<= 0){
        player.stats.trust-= 10;
        cout<< "Trust gained: -10\n\n";
    }
    wait(2);
}

void battleGuard(int nationScale){
    enemies.stats= enemies.createEnemyGuard();
    scaleEnemyByLevel(player.stats.level);
    scaleByNation(nationScale);
    battle();
    if (enemies.stats.HP<= 0){
        player.stats.trust-= 10;
        cout<< "Trust gained: -10\n\n";
    }
    wait(2);
}

void battleGuardDog(int nationScale){
    enemies.stats= enemies.createEnemyGuardDog();
    scaleEnemyByLevel(player.stats.level);
    scaleByNation(nationScale);
    battle();
    if (enemies.stats.HP<= 0){
        player.stats.trust-= 20;
        cout<< "Trust gained: -20\n\n";
    }
    wait(2);
}

void battleSoldier(int nationScale){
    enemies.stats= enemies.createEnemySoldier();
    scaleEnemyByLevel(player.stats.level);
    scaleByNation(nationScale);
    battle();
    if (enemies.stats.HP<= 0){
        player.stats.trust-= 20;
        cout<< "Trust gained: -20\n\n";
    }
    wait(2);
}

void battleKnight(int nationScale){
    enemies.stats= enemies.createEnemyKnight();
    scaleEnemyByLevel(player.stats.level);
    scaleByNation(nationScale);
    battle();
    if (enemies.stats.HP<= 0){
        player.stats.trust-= 30;
        cout<< "Trust gained: -30\n\n";
    }
    wait(2);
}

void battleArcherKnight(int nationScale){
    enemies.stats= enemies.createEnemyArcherKnight();
    scaleEnemyByLevel(player.stats.level);
    scaleByNation(nationScale);
    battle();
    if (enemies.stats.HP<= 0){
        player.stats.trust-= 30;
        cout<< "Trust gained: -30\n\n";
    }
    wait(2);
}

void battleMage(int nationScale){
    enemies.stats= enemies.createEnemyMage();
    scaleEnemyByLevel(player.stats.level);
    scaleByNation(nationScale);
    battle();
    if (enemies.stats.HP<= 0){
        player.stats.trust-= 40;
        cout<< "Trust gained: -40\n\n";
    }
    wait(2);
}

void battleGolem(int nationScale){
    enemies.stats= enemies.createEnemyGolem();
    scaleEnemyByLevel(player.stats.level);
    scaleByNation(nationScale);
    battle();
    if (enemies.stats.HP<= 0){
        player.stats.trust-= 50;
        cout<< "Trust gained: -50\n\n";
    }
    wait(2);
}

void battlePrince(int nationScale){
    enemies.stats= enemies.createEnemyPrince();
    scaleEnemyByLevel(player.stats.level);
    scaleByNation(nationScale);
    battle();
    if (enemies.stats.HP<= 0){
        player.stats.trust-= 100;
        cout<< "Trust gained: -100\n\n";
    }
    wait(2);
}

void battleThief(){
    enemies.stats= enemies.createEnemyThief();
    scaleEnemyByLevel(player.stats.level);
    wildernessBattle();
    if (enemies.stats.HP<= 0){
        player.stats.trust+= 10;
        cout<< "Trust gained: +10\n\n";
    }
    wait(2);
}

void battleBandit(){
    enemies.stats= enemies.createEnemyBandit();
    scaleEnemyByLevel(player.stats.level);
    wildernessBattle();
    if (enemies.stats.HP<= 0){
        player.stats.trust+= 10;
        cout<< "Trust gained: +10\n\n";
    }
    wait(2);
}

void bossBattleDragon(){
    cout<< fixed<< setprecision(1);

    inline void resetPlayerBuffs();

    player.stats.critBuffTurns= 0;
    player.stats.critChanceBuffTurns= 0;

    if (bosses.stats.count< 1)
    {
        return;
    }

    int turn= 0;

    for (int i = 0; i < 11; i++)
    {
        cout<< "***";
        waitms();
    }
    cout<< "***\n\n";
    waitms();

    cout<< "    "<< bosses.stats.name<< " stands their ground!\n\n";

    for (int i = 0; i < 11; i++)
    {
        cout<< "***";
        waitms();
    }
    cout<< "***\n\n";
    waitms();
    wait(2);

    while(player.stats.HP> 0&& bosses.stats.HP> 0)
    {   
        turn++;
        
        player.stats.MP++;
        if(player.stats.MP> player.stats.maxMP) player.stats.MP= player.stats.maxMP;
        bosses.stats.MP++;
        if(bosses.stats.MP> bosses.stats.maxMP) bosses.stats.MP= bosses.stats.maxMP;
        player.stats.critBuffTurns--;
        if (player.stats.critBuffTurns< 0) player.stats.critBuffTurns= 0;
        player.stats.critChanceBuffTurns--;
        if (player.stats.critChanceBuffTurns< 0) player.stats.critChanceBuffTurns= 0;


        cout<< "************************************\n";
        cout<< "            DRAGON STATS            \n";
        cout<< "************************************\n\n";
        cout<< "HP: "<<bosses.stats.HP<< "/"<< bosses.stats.maxHP<< "\n";
        cout<< "MP: "<<bosses.stats.MP<< "/"<< bosses.stats.maxMP<< "\n";
        cout<< "STR: "<< bosses.stats.STR<< "\n";
        cout<< "DEF: "<< bosses.stats.DEF<< "\n";
        cout<< "Crit Damage: "<< bosses.stats.critDMG<< "\n";
        cout<< "Crit Chance: "<< bosses.stats.CritChance<< "%\n\n";
        cout<< "************************************\n\n";
        wait(2);

        if (turn== 1)
        {
            cout<< "Turn: "<< turn<< "\n\n";
        } else{
            cout<< "Turns: "<< turn<< "\n\n";
        }
        
        float bATK= 0.0f;
        float armorDefence= player.armor.armorDefence[player.inventory.armor];
        bool extraAction= false;

        do
        {
            int battleInput, skillInput, potionInput;

            float effectiveCritDMG= (player.stats.critBuffTurns> 0)? player.stats.critDMG* 2: player.stats.critDMG;
            float effectiveCritChance= (player.stats.critChanceBuffTurns> 0)? player.stats.critChance* 2: player.stats.critChance; 
            bool isCritical= (rand()% 100)< effectiveCritChance;
            float critMultiplier= isCritical? effectiveCritDMG: 1;
            float weaponDamage= player.weapon.weaponDMG[player.inventory.weapon];
            float baseDamagePlayer= (player.stats.STR+ weaponDamage)- (bosses.stats.DEF* 2/ 5);
            float attackVariance= rand()% player.weapon.weaponMaxAttackVariance[player.inventory.weapon]- player.weapon.weaponMinAttackVariance[player.inventory.weapon];
            float playerATK= max(1.0f, baseDamagePlayer+ attackVariance)* critMultiplier;

            cinClear();

            cout<< "1. Attack\n";
            cout<< "2. Skill\n";
            cout<< "3. Check Stats\n";
            cout<< "4. Inventory\n";
            cout<< "5. Potion\n";
            cout<< "0. Run\nChoice: ";

            if (!(cin>>battleInput))
            {
                cinClear();
                battleInput= -1;
            }
            cout<< "\n";

            if (battleInput< 0|| battleInput> 5)
            {
                cout<< "There is no running away!\n\n";
                wait(2);
                extraAction= true;
            }

            if (battleInput== 0)
            {
                cout<< "There is no running away!\n\n";
                wait(2);
                extraAction= true;
            }

            if (battleInput== 1)
            {
                bosses.stats.HP-= playerATK;
                cout<< "You attacked for "<< playerATK<< " damage!\n";
                if (isCritical== true)
                {
                    cout<< "CRITICAL HIT!!!\n\n";
                }
                wait(2);
                cout<< bosses.stats.name<< "'s HP "<<max(0.0f, bosses.stats.HP)<< "\n\n";
                wait(2);
                extraAction= false;
            }
            else if (battleInput== 2)
            {
                cout<< "Mana: "<< player.stats.MP<< "\n";
                cout<< "Choose skill:\n1. Brute Strike: STR *1.5.   Mana required: "<< player.skill.skillMPCost<< ".\n";
                cout<< "2. Focus Strike: Crit Damage *1.2.   Mana required: "<< player.skill.skill1MPCost<< ".\n";
                cout<< "3. Enchanced Perception: Crit Chance *2.   Mana required: "<< player.skill.skill2MPCost<< ".\n0. Cancel\nChoice: ";
                
                if (!(cin>> skillInput))
                {
                    cinClear();
                    skillInput= -1;
                    extraAction= true;
                }
                cout<< "\n";

                if (skillInput<= 0|| skillInput> 3)
                {
                    cout<< "You canceled cast\n\n";
                    wait(2);
                    extraAction= true;
                }
                
                else if (skillInput== 1)
                {
                    if (player.stats.MP< player.skill.skillMPCost)
                    {
                        cout<< "Not enough MP! ("<< player.skill.skillMPCost<< " MP required)\n";
                        wait();
                        extraAction= true;
                        continue;
                    }
                    
                    cout<< "You used Brute Strike!\n";
                    wait(2);

                    float effectiveCritDMG= (player.stats.critBuffTurns> 0)? player.stats.critDMG* 2: player.stats.critDMG;
                    float effectiveCritChance= (player.stats.critChanceBuffTurns)> 0? player.stats.critChance* 2: player.stats.critChance;
                    bool isCritical= (rand()% 100)< effectiveCritChance;
                    float critMultiplier= isCritical? effectiveCritDMG: 1;
                    float weaponDamage= player.weapon.weaponDMG[player.inventory.weapon];
                    float baseDamagePlayer= (player.skill.skill+ weaponDamage)- (bosses.stats.DEF* 2/ 5);
                    float attackVariance= rand()% player.weapon.weaponMaxAttackVariance[player.inventory.weapon]- player.weapon.weaponMinAttackVariance[player.inventory.weapon];
                    float playerATK= max(1.0f, baseDamagePlayer+ attackVariance)* critMultiplier;
                    
                    player.stats.MP-= player.skill.skillMPCost;

                    bosses.stats.HP-= playerATK;
                    cout<< "You attacked for "<< playerATK<< " damage!\n";
                    if (isCritical== true)
                    {
                        cout<< "CRITICAL HIT!!!\n\n";
                    }
                    wait(2);
                    cout<< bosses.stats.name<< "'s HP "<<max(0.0f, bosses.stats.HP)<< "\n\n";
                    wait(2);
                    extraAction= false;
                }

                else if (skillInput== 2)
                {
                    if (player.stats.MP< player.skill.skillMPCost)
                    {
                        cout<< "Not enough MP! ("<< player.skill.skillMPCost<< " MP required)\n";
                        wait();
                        extraAction= true;
                        continue;
                    }

                    player.stats.critBuffTurns+= 2;
                    player.stats.MP-= player.skill.skill1MPCost;

                    cout<< "You used Focus Strike! Critical Damage is buffed for 2 turns!\n\n";
                    wait(2);
                    extraAction= true;
                    continue;
                }
                
                else if (skillInput== 3)
                {
                    if (player.stats.MP< player.skill.skill2MPCost)
                    {
                        cout<< "Not enough MP! ("<< player.skill.skill2MPCost<< " MP required)\n";
                        wait();
                        extraAction= true;
                        continue;
                    }

                    player.stats.critChanceBuffTurns+= 2;
                    player.stats.MP-= player.skill.skill2MPCost;

                    cout<< "You used Enhanced Perception! Critical Chance is doubled for 2 turns!\n\n";
                    wait(2);
                    extraAction= true;
                    continue;
                }
                
            }

            else if (battleInput== 3)
            {
                player.checkStats();
                extraAction= true;
                continue;
            }
            
            else if (battleInput== 4)
            {
                player.Inv(player.inventory, player.weapon, player.armor, player.talisman);
                extraAction= true;
                continue;
            }
            
            else if (battleInput== 5)
            {
                cout<< "1. Health Potion ["<< player.inventory.potionHealth<< "]\n";
                cout<< "2. Mana Potion ["<< player.inventory.potionMana<< "]\n";
                cout<< "0. Cancel\nChoice: ";

                if (!(cin>> potionInput))
                {
                    cinClear();
                    potionInput= -1;
                }
                cout<< "\n";

                if (potionInput== 1)
                {
                    if (player.inventory.potionHealth> 0)
                    {
                        int heal= player.stats.maxHP* POTION_HEAL;
                        player.stats.HP= min(player.stats.maxHP, player.stats.HP+ (player.stats.maxHP* POTION_HEAL));
                        player.inventory.potionHealth--;
                        cout<< "Restored "<< heal<< " HP!\n\n";
                        wait(2);
                    }
                    else{
                        cout<< "No health potion!\n\n";
                        wait();
                        extraAction= true;
                        continue;
                    }
                }

                else if (potionInput== 2)
                {
                    if (player.inventory.potionMana> 0)
                    {
                        player.stats.MP= min(player.stats.maxMP, player.stats.MP+ MANA_POTION_HEAL);
                        player.inventory.potionMana--;
                        cout<< "Restored "<< MANA_POTION_HEAL<< " MP!\n\n";
                        wait(2);
                    }
                    else{
                        cout<< "No mana potion!\n\n";
                        wait();
                        extraAction= true;
                        continue;
                    }
                }
                extraAction= true;
            }
            
            if (bosses.stats.HP<= 0)
            {
                cout<< "You have defeated the "<< bosses.stats.name<< "!\n";
                wait();
                cout<< "XP gained: "<< bosses.stats.XP<< "\n";
                wait();
                cout<< "Gold gained: "<< bosses.stats.gold<< "\n";
                wait();

                bosses.bossTracker.slainedCount(bosses.stats.name);

                player.talismanAttributes(player.inventory, player.talisman);

                player.stats.XP+= bosses.stats.XP;
                player.stats.gold+= bosses.stats.gold;
                bosses.stats.count--;

                levelUp();

                break;
            }
        } while (extraAction);

        if (bosses.stats.HP> 0)
        {
            if (turn% 5!= 0)
            {
                int bAttackVariance= rand()% 41- 20;
                float bBaseDamage= bosses.stats.STR- ((player.stats.DEF+ armorDefence)* 2/ 5);
                bool bIsCritical= rand()% 100< bosses.stats.CritChance;
                float bCritMultiplier= bIsCritical? bosses.stats.critDMG: 1;
                bATK= max(1.0f, bBaseDamage+ bAttackVariance)* bCritMultiplier;

                player.stats.HP-= bATK;
                cout<< bosses.stats.name<< " attacks for "<< bATK<< " damage!\n";
                wait();
                if (bIsCritical== true)
                {
                    cout<< "CRITICAL HIT!!!\n\n";
                }
                wait();
                cout<< player.stats.name<< "'s HP: "<< max(0.0f, player.stats.HP)<< "\n\n";
                wait();
            }
            else if (turn% 5== 0&& bosses.stats.MP>= bosses.stats.skillCost)
            {
                int bAttackVariance= rand()% 41- 20;
                float bBaseDamage= bosses.stats.skill- ((player.stats.DEF+ armorDefence)* 2/ 5);
                bool bIsCritical= rand()% 100< bosses.stats.CritChance;
                float bCritMultiplier= bIsCritical? bosses.stats.critDMG: 1;
                bATK= max(1.0f, bBaseDamage+ bAttackVariance)* bCritMultiplier;

                player.stats.HP-= bATK;
                cout<< bosses.stats.name<< " uses a skill! \n";
                wait();
                cout<< bosses.stats.skillName<< "!!!\n";
                wait();
                cout<< bosses.stats.name<< " attacks you for: "<< bATK<< " damage!\n";
                if (bIsCritical== true)
                {
                    cout<< "CRITICAL HIT!!!\n\n";
                }
                wait();
                cout<< player.stats.name<< "'s HP: "<< max(0.0f, player.stats.HP)<< "\n\n";
                wait();
            }
            else if (turn% 5== 0 && bosses.stats.MP< bosses.stats.skillCost)
            {
                cout<< bosses.stats.name<< " is exhausted1!!\n\n";
                continue;
            }
            
        }
        if (player.stats.HP<= 0)
        {
            cout<< "You";
            waitms();
            cout<< " ";
            waitms();
            cout<< "have";
            waitms();
            cout<< " ";
            waitms();
            cout<< "been";
            waitms();
            cout<< " ";
            waitms();
            cout<< "defeated!\n\n";
            waitms();

            cout<< "Exiting.";
            wait();
            cout<< ".";
            wait();
            cout<< ".\n";
            return;
        }   
    }
    
}

void bossBattleKingGrayhaven(){
    cout<< fixed<< setprecision(1);

    inline void resetPlayerBuffs();

    player.stats.critBuffTurns= 0;
    player.stats.critChanceBuffTurns= 0;

    if (bosses.stats.count< 1)
    {
        return;
    }

    int turn= 0;

    for (int i = 0; i < 11; i++)
    {
        cout<< "***";
        waitms();
    }
    cout<< "***\n\n";
    waitms();

    int roll= rand()% 4;
    if (roll< 1) cout<< bosses.stats.name<<" has had enough!\n\n";
    else if (roll< 2) cout<< bosses.stats.name<< " is angered!\n\n";
    else if (roll< 3) cout<< bosses.stats.name<< " is furious!\n\n"; 
    else cout<< "You've angered "<< bosses.stats.name<< "\n\n";

    for (int i = 0; i < 11; i++)
    {
        cout<< "***";
        waitms();
    }
    cout<< "***\n\n";
    waitms();
    wait(2);

    while (player.stats.HP> 0&& bosses.stats.HP> 0)
    {
        turn++;
        
        player.stats.MP++;
        if(player.stats.MP> player.stats.maxMP) player.stats.MP= player.stats.maxMP;
        bosses.stats.MP++;
        if(bosses.stats.MP> bosses.stats.maxMP) bosses.stats.MP= bosses.stats.maxMP;
        player.stats.critBuffTurns--;
        if (player.stats.critBuffTurns< 0) player.stats.critBuffTurns= 0;
        player.stats.critChanceBuffTurns--;
        if (player.stats.critChanceBuffTurns< 0) player.stats.critChanceBuffTurns= 0;

        cout<< "************************************\n";
        cout<< "        "<< bosses.stats.name<< " Stats        \n";
        cout<< "************************************\n";
        cout<< "HP: "<<bosses.stats.HP<< "/"<< bosses.stats.maxHP<< "\n";
        cout<< "MP: "<<bosses.stats.MP<< "/"<< bosses.stats.maxMP<< "\n";
        cout<< "STR: "<< bosses.stats.STR<< "\n";
        cout<< "DEF: "<< bosses.stats.DEF<< "\n";
        cout<< "Crit Damage: "<< bosses.stats.critDMG<< "\n";
        cout<< "Crit Chance: "<< bosses.stats.CritChance<< "\n\n";
        cout<< "************************************\n\n";
        wait(2);

        if (turn== 1)
        {
            cout<< "Turn: "<< turn<< "\n\n";
        } else{
            cout<< "Turns: "<< turn<< "\n\n";
        }

        float bATK= 0.0f;
        float armorDefence= player.armor.armorDefence[player.inventory.armor];
        bool extraAction= false;

        do
        {
            int battleInput, skillInput, potionInput;

            float effectiveCritDMG= (player.stats.critBuffTurns> 0)? player.stats.critDMG* 2: player.stats.critDMG;
            float effectiveCritChance= (player.stats.critChanceBuffTurns> 0)? player.stats.critChance* 2: player.stats.critChance; 
            bool isCritical= (rand()% 100)< effectiveCritChance;
            float critMultiplier= isCritical? effectiveCritDMG: 1;
            float weaponDamage= player.weapon.weaponDMG[player.inventory.weapon];
            float baseDamagePlayer= (player.stats.STR+ weaponDamage)- (bosses.stats.DEF* 2/ 5);
            float attackVariance= rand()% player.weapon.weaponMaxAttackVariance[player.inventory.weapon]- player.weapon.weaponMinAttackVariance[player.inventory.weapon];
            float playerATK= max(1.0f, baseDamagePlayer+ attackVariance)* critMultiplier;

            cinClear();

            cout<< "1. Attack\n";
            cout<< "2. Skill\n";
            cout<< "3. Check Stats\n";
            cout<< "4. Inventory\n";
            cout<< "5. Potion\n";
            cout<< "0. Run\nChoice: ";

            if (!(cin>>battleInput))
            {
                cinClear();
                battleInput= -1;
            }
            cout<< "\n";

            if (battleInput< 0|| battleInput> 5)
            {
                cout<< "There is no running away!\n\n";
                wait(2);
                extraAction= true;
            }

            if (battleInput== 0)
            {
                cout<< "There is no running away!\n\n";
                wait(2);
                extraAction= true;
            }

            if (battleInput== 1)
            {
                bosses.stats.HP-= playerATK;
                cout<< "You attacked for "<< playerATK<< " damage!\n";
                if (isCritical== true)
                {
                    cout<< "CRITICAL HIT!!!\n\n";
                }
                wait(2);
                cout<< bosses.stats.name<< "'s HP "<<max(0.0f, bosses.stats.HP)<< "\n\n";
                wait(2);
                extraAction= false;
            }
            else if (battleInput== 2)
            {
                cout<< "Mana: "<< player.stats.MP<< "\n";
                cout<< "Choose skill:\n1. Brute Strike: STR *1.5.   Mana required: "<< player.skill.skillMPCost<< ".\n";
                cout<< "2. Focus Strike: Crit Damage *1.2.   Mana required: "<< player.skill.skill1MPCost<< ".\n";
                cout<< "3. Enchanced Perception: Crit Chance *2.   Mana required: "<< player.skill.skill2MPCost<< ".\n0. Cancel\nChoice: ";
                
                if (!(cin>> skillInput))
                {
                    cinClear();
                    skillInput= -1;
                    extraAction= true;
                }
                cout<< "\n";

                if (skillInput<= 0|| skillInput> 3)
                {
                    cout<< "You canceled cast\n\n";
                    wait(2);
                    extraAction= true;
                }
                
                else if (skillInput== 1)
                {
                    if (player.stats.MP< player.skill.skillMPCost)
                    {
                        cout<< "Not enough MP! ("<< player.skill.skillMPCost<< " MP required)\n";
                        wait();
                        extraAction= true;
                        continue;
                    }
                    
                    cout<< "You used Brute Strike!\n";
                    wait(2);

                    float effectiveCritDMG= (player.stats.critBuffTurns> 0)? player.stats.critDMG* 2: player.stats.critDMG;
                    float effectiveCritChance= (player.stats.critChanceBuffTurns)> 0? player.stats.critChance* 2: player.stats.critChance;
                    bool isCritical= (rand()% 100)< effectiveCritChance;
                    float critMultiplier= isCritical? effectiveCritDMG: 1;
                    float weaponDamage= player.weapon.weaponDMG[player.inventory.weapon];
                    float baseDamagePlayer= (player.skill.skill+ weaponDamage)- (bosses.stats.DEF* 2/ 5);
                    float attackVariance= rand()% player.weapon.weaponMaxAttackVariance[player.inventory.weapon]- player.weapon.weaponMinAttackVariance[player.inventory.weapon];
                    float playerATK= max(1.0f, baseDamagePlayer+ attackVariance)* critMultiplier;
                    
                    player.stats.MP-= player.skill.skillMPCost;

                    bosses.stats.HP-= playerATK;
                    cout<< "You attacked for "<< playerATK<< " damage!\n";
                    if (isCritical== true)
                    {
                        cout<< "CRITICAL HIT!!!\n\n";
                    }
                    wait(2);
                    cout<< bosses.stats.name<< "'s HP "<<max(0.0f, bosses.stats.HP)<< "\n\n";
                    wait(2);
                    extraAction= false;
                }

                else if (skillInput== 2)
                {
                    if (player.stats.MP< player.skill.skillMPCost)
                    {
                        cout<< "Not enough MP! ("<< player.skill.skillMPCost<< " MP required)\n";
                        wait();
                        extraAction= true;
                        continue;
                    }

                    player.stats.critBuffTurns+= 2;
                    player.stats.MP-= player.skill.skill1MPCost;

                    cout<< "You used Focus Strike! Critical Damage is buffed for 2 turns!\n\n";
                    wait(2);
                    extraAction= true;
                    continue;
                }
                
                else if (skillInput== 3)
                {
                    if (player.stats.MP< player.skill.skill2MPCost)
                    {
                        cout<< "Not enough MP! ("<< player.skill.skill2MPCost<< " MP required)\n";
                        wait();
                        extraAction= true;
                        continue;
                    }

                    player.stats.critChanceBuffTurns+= 2;
                    player.stats.MP-= player.skill.skill2MPCost;

                    cout<< "You used Enhanced Perception! Critical Chance is doubled for 2 turns!\n\n";
                    wait(2);
                    extraAction= true;
                    continue;
                }
                
            }

            else if (battleInput== 3)
            {
                player.checkStats();
                extraAction= true;
                continue;
            }
            
            else if (battleInput== 4)
            {
                player.Inv(player.inventory, player.weapon, player.armor, player.talisman);
                extraAction= true;
                continue;
            }
            
            else if (battleInput== 5)
            {
                cout<< "1. Health Potion ["<< player.inventory.potionHealth<< "]\n";
                cout<< "2. Mana Potion ["<< player.inventory.potionMana<< "]\n";
                cout<< "0. Cancel\nChoice: ";

                if (!(cin>> potionInput))
                {
                    cinClear();
                    potionInput= -1;
                }
                cout<< "\n";

                if (potionInput== 1)
                {
                    if (player.inventory.potionHealth> 0)
                    {
                        int heal= player.stats.maxHP* POTION_HEAL;
                        player.stats.HP= min(player.stats.maxHP, player.stats.HP+ (player.stats.maxHP* POTION_HEAL));
                        player.inventory.potionHealth--;
                        cout<< "Restored "<< heal<< " HP!\n\n";
                        wait(2);
                    }
                    else{
                        cout<< "No health potion!\n\n";
                        wait();
                        extraAction= true;
                        continue;
                    }
                }

                else if (potionInput== 2)
                {
                    if (player.inventory.potionMana> 0)
                    {
                        player.stats.MP= min(player.stats.maxMP, player.stats.MP+ MANA_POTION_HEAL);
                        player.inventory.potionMana--;
                        cout<< "Restored "<< MANA_POTION_HEAL<< " MP!\n\n";
                        wait(2);
                    }
                    else{
                        cout<< "No mana potion!\n\n";
                        wait();
                        extraAction= true;
                        continue;
                    }
                }
                extraAction= true;
            }
            
            if (bosses.stats.HP<= 0)
            {
                cout<< "You have defeated the "<< bosses.stats.name<< "!\n";
                wait();
                cout<< "XP gained: "<< bosses.stats.XP<< "\n";
                wait();
                cout<< "Gold gained: "<< bosses.stats.gold<< "\n";
                wait();

                bosses.bossTracker.slainedCount(bosses.stats.name);

                player.talismanAttributes(player.inventory, player.talisman);

                player.stats.XP+= bosses.stats.XP;
                player.stats.gold+= bosses.stats.gold;
                bosses.stats.count--;

                levelUp();

                break;
            }
        } while (extraAction);

        if (bosses.stats.HP> 0)
        {
            if (turn% 5!= 0)
            {
                int bAttackVariance= rand()% 41- 20;
                float bBaseDamage= bosses.stats.STR- ((player.stats.DEF+ armorDefence)* 2/ 5);
                bool bIsCritical= rand()% 100< bosses.stats.CritChance;
                float bCritMultiplier= bIsCritical? bosses.stats.critDMG: 1;
                bATK= max(1.0f, bBaseDamage+ bAttackVariance)* bCritMultiplier;

                player.stats.HP-= bATK;
                cout<< bosses.stats.name<< " attacks for "<< bATK<< " damage!\n";
                wait();
                if (bIsCritical== true)
                {
                    cout<< "CRITICAL HIT!!!\n\n";
                }
                wait();
                cout<< player.stats.name<< "'s HP: "<< max(0.0f, player.stats.HP)<< "\n\n";
                wait();
            }
            else if(turn% 5== 0&& bosses.stats.MP>= bosses.stats.skillCost)
            {
                int bAttackVariance= rand()% 41- 20;
                float bBaseDamage= bosses.stats.STR- ((player.stats.DEF+ armorDefence)* 2/ 5);
                bool bIsCritical= rand()% 100< bosses.stats.CritChance;
                float bCritMultiplier= bIsCritical? bosses.stats.critDMG* bosses.stats.skill: 1;
                bATK= max(1.0f, bBaseDamage+ bAttackVariance)* bCritMultiplier;

                player.stats.HP-= bATK;
                cout<< bosses.stats.name<< " uses a skill! \n";
                wait();
                cout<< bosses.stats.skillName<< "!!!\n";
                wait();
                cout<< bosses.stats.name<< " attacks you for: "<< bATK<< " damage!\n";
                if (bIsCritical== true)
                {
                    cout<< "CRITICAL HIT!!!\n\n";
                }
                wait();
                cout<< player.stats.name<< "'s HP: "<< max(0.0f, player.stats.HP)<< "\n\n";
                wait();
            }
            else if (turn% 5== 0&& bosses.stats.MP< bosses.stats.skillCost)
            {
                cout<< bosses.stats.name<< " is exhausted1!!\n\n";
                continue;
            }
        }
        if (player.stats.HP<= 0)
        {
            cout<< "You";
            waitms();
            cout<< " ";
            waitms();
            cout<< "have";
            waitms();
            cout<< " ";
            waitms();
            cout<< "been";
            waitms();
            cout<< " ";
            waitms();
            cout<< "defeated!\n\n";
            waitms();

            cout<< "Exiting.";
            wait();
            cout<< ".";
            wait();
            cout<< ".\n";
            return;
        } 
    }
}

void bossBattleKingStormwich(){
    cout<< fixed<< setprecision(1);

    inline void resetPlayerBuffs();

    player.stats.critBuffTurns= 0;
    player.stats.critChanceBuffTurns= 0;

    if (bosses.stats.count< 1)
    {
        return;
    }

    int turn= 0;

    for (int i = 0; i < 11; i++)
    {
        cout<< "***";
        waitms();
    }
    cout<< "***\n\n";
    waitms();

    int roll= rand()% 4;
    if (roll< 1) cout<< bosses.stats.name<<" has had enough!\n\n";
    else if (roll< 2) cout<< bosses.stats.name<< " is angered!\n\n";
    else if (roll< 3) cout<< bosses.stats.name<< " is furious!\n\n"; 
    else cout<< "You've angered "<< bosses.stats.name<< "\n\n";

    for (int i = 0; i < 11; i++)
    {
        cout<< "***";
        waitms();
    }
    cout<< "***\n\n";
    waitms();
    wait(2);

    while (player.stats.HP> 0&& bosses.stats.HP> 0)
    {
        turn++;
        
        player.stats.MP++;
        if(player.stats.MP> player.stats.maxMP) player.stats.MP= player.stats.maxMP;
        bosses.stats.MP++;
        if(bosses.stats.MP> bosses.stats.maxMP) bosses.stats.MP= bosses.stats.maxMP;
        player.stats.critBuffTurns--;
        if (player.stats.critBuffTurns< 0) player.stats.critBuffTurns= 0;
        player.stats.critChanceBuffTurns--;
        if (player.stats.critChanceBuffTurns< 0) player.stats.critChanceBuffTurns= 0;

        cout<< "************************************\n";
        cout<< "        "<< bosses.stats.name<< " Stats        \n";
        cout<< "************************************\n";
        cout<< "HP: "<<bosses.stats.HP<< "/"<< bosses.stats.maxHP<< "\n";
        cout<< "MP: "<<bosses.stats.MP<< "/"<< bosses.stats.maxMP<< "\n";
        cout<< "STR: "<< bosses.stats.STR<< "\n";
        cout<< "DEF: "<< bosses.stats.DEF<< "\n";
        cout<< "Crit Damage: "<< bosses.stats.critDMG<< "\n";
        cout<< "Crit Chance: "<< bosses.stats.CritChance<< "\n\n";
        cout<< "************************************\n\n";
        wait(2);

        if (turn== 1)
        {
            cout<< "Turn: "<< turn<< "\n\n";
        } else{
            cout<< "Turns: "<< turn<< "\n\n";
        }

        float bATK= 0.0f;
        float armorDefence= player.armor.armorDefence[player.inventory.armor];
        bool extraAction= false;

        do
        {
            int battleInput, skillInput, potionInput;

            float effectiveCritDMG= (player.stats.critBuffTurns> 0)? player.stats.critDMG* 2: player.stats.critDMG;
            float effectiveCritChance= (player.stats.critChanceBuffTurns> 0)? player.stats.critChance* 2: player.stats.critChance; 
            bool isCritical= (rand()% 100)< effectiveCritChance;
            float critMultiplier= isCritical? effectiveCritDMG: 1;
            float weaponDamage= player.weapon.weaponDMG[player.inventory.weapon];
            float baseDamagePlayer= (player.stats.STR+ weaponDamage)- (bosses.stats.DEF* 2/ 5);
            float attackVariance= rand()% player.weapon.weaponMaxAttackVariance[player.inventory.weapon]- player.weapon.weaponMinAttackVariance[player.inventory.weapon];
            float playerATK= max(1.0f, baseDamagePlayer+ attackVariance)* critMultiplier;

            cinClear();

            cout<< "1. Attack\n";
            cout<< "2. Skill\n";
            cout<< "3. Check Stats\n";
            cout<< "4. Inventory\n";
            cout<< "5. Potion\n";
            cout<< "0. Run\nChoice: ";

            if (!(cin>>battleInput))
            {
                cinClear();
                battleInput= -1;
            }
            cout<< "\n";

            if (battleInput< 0|| battleInput> 5)
            {
                cout<< "There is no running away!\n\n";
                wait(2);
                extraAction= true;
            }

            if (battleInput== 0)
            {
                cout<< "There is no running away!\n\n";
                wait(2);
                extraAction= true;
            }

            if (battleInput== 1)
            {
                bosses.stats.HP-= playerATK;
                cout<< "You attacked for "<< playerATK<< " damage!\n";
                if (isCritical== true)
                {
                    cout<< "CRITICAL HIT!!!\n\n";
                }
                wait(2);
                cout<< bosses.stats.name<< "'s HP "<<max(0.0f, bosses.stats.HP)<< "\n\n";
                wait(2);
                extraAction= false;
            }
            else if (battleInput== 2)
            {
                cout<< "Mana: "<< player.stats.MP<< "\n";
                cout<< "Choose skill:\n1. Brute Strike: STR *1.5.   Mana required: "<< player.skill.skillMPCost<< ".\n";
                cout<< "2. Focus Strike: Crit Damage *1.2.   Mana required: "<< player.skill.skill1MPCost<< ".\n";
                cout<< "3. Enchanced Perception: Crit Chance *2.   Mana required: "<< player.skill.skill2MPCost<< ".\n0. Cancel\nChoice: ";
                
                if (!(cin>> skillInput))
                {
                    cinClear();
                    skillInput= -1;
                    extraAction= true;
                }
                cout<< "\n";

                if (skillInput<= 0|| skillInput> 3)
                {
                    cout<< "You canceled cast\n\n";
                    wait(2);
                    extraAction= true;
                }
                
                else if (skillInput== 1)
                {
                    if (player.stats.MP< player.skill.skillMPCost)
                    {
                        cout<< "Not enough MP! ("<< player.skill.skillMPCost<< " MP required)\n";
                        wait();
                        extraAction= true;
                        continue;
                    }
                    
                    cout<< "You used Brute Strike!\n";
                    wait(2);

                    float effectiveCritDMG= (player.stats.critBuffTurns> 0)? player.stats.critDMG* 2: player.stats.critDMG;
                    float effectiveCritChance= (player.stats.critChanceBuffTurns)> 0? player.stats.critChance* 2: player.stats.critChance;
                    bool isCritical= (rand()% 100)< effectiveCritChance;
                    float critMultiplier= isCritical? effectiveCritDMG: 1;
                    float weaponDamage= player.weapon.weaponDMG[player.inventory.weapon];
                    float baseDamagePlayer= (player.skill.skill+ weaponDamage)- (bosses.stats.DEF* 2/ 5);
                    float attackVariance= rand()% player.weapon.weaponMaxAttackVariance[player.inventory.weapon]- player.weapon.weaponMinAttackVariance[player.inventory.weapon];
                    float playerATK= max(1.0f, baseDamagePlayer+ attackVariance)* critMultiplier;
                    
                    player.stats.MP-= player.skill.skillMPCost;

                    bosses.stats.HP-= playerATK;
                    cout<< "You attacked for "<< playerATK<< " damage!\n";
                    if (isCritical== true)
                    {
                        cout<< "CRITICAL HIT!!!\n\n";
                    }
                    wait(2);
                    cout<< bosses.stats.name<< "'s HP "<<max(0.0f, bosses.stats.HP)<< "\n\n";
                    wait(2);
                    extraAction= false;
                }

                else if (skillInput== 2)
                {
                    if (player.stats.MP< player.skill.skillMPCost)
                    {
                        cout<< "Not enough MP! ("<< player.skill.skillMPCost<< " MP required)\n";
                        wait();
                        extraAction= true;
                        continue;
                    }

                    player.stats.critBuffTurns+= 2;
                    player.stats.MP-= player.skill.skill1MPCost;

                    cout<< "You used Focus Strike! Critical Damage is doubled for 2 turns!\n\n";
                    wait(2);
                    extraAction= true;
                    continue;
                }
                
                else if (skillInput== 3)
                {
                    if (player.stats.MP< player.skill.skill2MPCost)
                    {
                        cout<< "Not enough MP! ("<< player.skill.skill2MPCost<< " MP required)\n";
                        wait();
                        extraAction= true;
                        continue;
                    }

                    player.stats.critChanceBuffTurns+= 2;
                    player.stats.MP-= player.skill.skill2MPCost;

                    cout<< "You used Enhanced Perception! Critical Chance is doubled for 2 turns!\n\n";
                    wait(2);
                    extraAction= true;
                    continue;
                }
                
            }

            else if (battleInput== 3)
            {
                player.checkStats();
                extraAction= true;
                continue;
            }
            
            else if (battleInput== 4)
            {
                player.Inv(player.inventory, player.weapon, player.armor, player.talisman);
                extraAction= true;
                continue;
            }
            
            else if (battleInput== 5)
            {
                cout<< "1. Health Potion ["<< player.inventory.potionHealth<< "]\n";
                cout<< "2. Mana Potion ["<< player.inventory.potionMana<< "]\n";
                cout<< "0. Cancel\nChoice: ";

                if (!(cin>> potionInput))
                {
                    cinClear();
                    potionInput= -1;
                }
                cout<< "\n";

                if (potionInput== 1)
                {
                    if (player.inventory.potionHealth> 0)
                    {
                        int heal= player.stats.maxHP* POTION_HEAL;
                        player.stats.HP= min(player.stats.maxHP, player.stats.HP+ (player.stats.maxHP* POTION_HEAL));
                        player.inventory.potionHealth--;
                        cout<< "Restored "<< heal<< " HP!\n\n";
                        wait(2);
                    }
                    else{
                        cout<< "No health potion!\n\n";
                        wait();
                        extraAction= true;
                        continue;
                    }
                }

                else if (potionInput== 2)
                {
                    if (player.inventory.potionMana> 0)
                    {
                        player.stats.MP= min(player.stats.maxMP, player.stats.MP+ MANA_POTION_HEAL);
                        player.inventory.potionMana--;
                        cout<< "Restored "<< MANA_POTION_HEAL<< " MP!\n\n";
                        wait(2);
                    }
                    else{
                        cout<< "No mana potion!\n\n";
                        wait();
                        extraAction= true;
                        continue;
                    }
                }
                extraAction= true;
            }
            
            if (bosses.stats.HP<= 0)
            {
                cout<< "You have defeated the "<< bosses.stats.name<< "!\n";
                wait();
                cout<< "XP gained: "<< bosses.stats.XP<< "\n";
                wait();
                cout<< "Gold gained: "<< bosses.stats.gold<< "\n";
                wait();

                bosses.bossTracker.slainedCount(bosses.stats.name);

                player.talismanAttributes(player.inventory, player.talisman);

                player.stats.XP+= bosses.stats.XP;
                player.stats.gold+= bosses.stats.gold;
                bosses.stats.count--;

                levelUp();

                break;
            }
        } while (extraAction);

        if (bosses.stats.HP> 0)
        {
            if (turn% 5!= 0&& bosses.stats.MP>= bosses.stats.skillCost)
            {
                int bAttackVariance= rand()% 41- 20;
                float bBaseDamage= bosses.stats.STR- ((player.stats.DEF+ armorDefence)* 2/ 5);
                bool bIsCritical= rand()% 100< bosses.stats.CritChance;
                float bCritMultiplier= bIsCritical? bosses.stats.critDMG: 1;
                bATK= max(1.0f, bBaseDamage+ bAttackVariance)* bCritMultiplier;

                player.stats.HP-= bATK;
                cout<< bosses.stats.name<< " attacks for "<< bATK<< " damage!\n";
                wait();
                if (bIsCritical== true)
                {
                    cout<< "CRITICAL HIT!!!\n\n";
                }
                wait();
                cout<< player.stats.name<< "'s HP: "<< max(0.0f, player.stats.HP)<< "\n\n";
                wait();
            }
            else if(turn% 5== 0&& bosses.stats.MP>= bosses.stats.skillCost)
            {
                bosses.stats.MP-= bosses.stats.skillCost;
                int bAttackVariance= rand()% 41- 20;
                float bBaseDamage= bosses.stats.STR- ((player.stats.DEF+ armorDefence)* 2/ 5);
                bool bIsCritical= rand()% 100< bosses.stats.CritChance;
                float bCritMultiplier= bIsCritical? bosses.stats.critDMG: 1;
                bATK= max(1.0f, bBaseDamage+ bAttackVariance)* bCritMultiplier;

                player.stats.HP-= bATK;
                cout<< bosses.stats.name<< " uses a skill! \n";
                wait();
                cout<< bosses.stats.skillName<< "!!!\n";
                wait();
                cout<< bosses.stats.name<< " attacks you for: "<< bATK<< " damage!\n";
                if (bIsCritical== true)
                {
                    cout<< "CRITICAL HIT!!!\n\n";
                }
                wait();
                cout<< player.stats.name<< "'s HP: "<< max(0.0f, player.stats.HP)<< "\n\n";
                wait();

                if (player.stats.HP<= 0){
                    cout<< "You";
                    waitms();
                    cout<< " ";
                    waitms();
                    cout<< "have";
                    waitms();
                    cout<< " ";
                    waitms();
                    cout<< "been";
                    waitms();
                    cout<< " ";
                    waitms();
                    cout<< "defeated!\n\n";
                    waitms();

                    cout<< "Exiting.";
                    wait();
                    cout<< ".";
                    wait();
                    cout<< ".";
                    break;
                }          
                
                player.stats.HP-= bATK;
                cout<< bosses.stats.name<< " attacks you again for: "<< bATK<< " damage!\n";
                if (bIsCritical== true)
                {
                    cout<< "CRITICAL HIT!!!\n\n";
                }
                wait();
                cout<< player.stats.name<< "'s HP: "<< max(0.0f, player.stats.HP)<< "\n\n";
                wait(); 
            }
            else if (turn% 5== 0&& bosses.stats.MP< bosses.stats.skillCost)
            {
                cout<< bosses.stats.name<< " is exhausted1!!\n\n";
                continue;
            }
        }
        if (player.stats.HP<= 0)
        {
            cout<< "You";
            waitms();
            cout<< " ";
            waitms();
            cout<< "have";
            waitms();
            cout<< " ";
            waitms();
            cout<< "been";
            waitms();
            cout<< " ";
            waitms();
            cout<< "defeated!\n\n";
            waitms();

            cout<< "Exiting.";
            wait();
            cout<< ".";
            wait();
            cout<< ".\n";
            return;
        } 
    }
}

void bossBattleKingHillsgate(){
    cout<< fixed<< setprecision(1);

    inline void resetPlayerBuffs();

    if (bosses.stats.count< 1)
    {
        return;
    }

    int turn= 0;

    for (int i = 0; i < 11; i++)
    {
        cout<< "***";
        waitms();
    }
    cout<< "***\n\n";
    waitms();

    int roll= rand()% 4;
    if (roll< 1) cout<< bosses.stats.name<<" has had enough!\n\n";
    else if (roll< 2) cout<< bosses.stats.name<< " is angered!\n\n";
    else if (roll< 3) cout<< bosses.stats.name<< " is furious!\n\n"; 
    else cout<< "You've angered "<< bosses.stats.name<< "\n\n";

    for (int i = 0; i < 11; i++)
    {
        cout<< "***";
        waitms();
    }
    cout<< "***\n\n";
    waitms();
    wait(2);

    while (player.stats.HP> 0&& bosses.stats.HP> 0)
    {
        turn++;
        
        player.stats.MP++;
        if(player.stats.MP> player.stats.maxMP) player.stats.MP= player.stats.maxMP;
        bosses.stats.MP++;
        if(bosses.stats.MP> bosses.stats.maxMP) bosses.stats.MP= bosses.stats.maxMP;
        player.stats.critBuffTurns--;
        if (player.stats.critBuffTurns< 0) player.stats.critBuffTurns= 0;
        player.stats.critChanceBuffTurns--;
        if (player.stats.critChanceBuffTurns< 0) player.stats.critChanceBuffTurns= 0;
        bosses.stats.skill--;
        if (bosses.stats.skill< 0) bosses.stats.skill= 0;
        if (bosses.stats.skill> 0) 
        {
            player.stats.HP-= 10;
            cout<< "HP -10\n\n";
            wait();
        };

        cout<< "************************************\n";
        cout<< "        "<< bosses.stats.name<< " Stats        \n";
        cout<< "************************************\n";
        cout<< "HP: "<<bosses.stats.HP<< "/"<< bosses.stats.maxHP<< "\n";
        cout<< "MP: "<<bosses.stats.MP<< "/"<< bosses.stats.maxMP<< "\n";
        cout<< "STR: "<< bosses.stats.STR<< "\n";
        cout<< "DEF: "<< bosses.stats.DEF<< "\n";
        cout<< "Crit Damage: "<< bosses.stats.critDMG<< "\n";
        cout<< "Crit Chance: "<< bosses.stats.CritChance<< "\n\n";
        cout<< "************************************\n\n";
        wait(2);

        if (turn== 1)
        {
            cout<< "Turn: "<< turn<< "\n\n";
        } else{
            cout<< "Turns: "<< turn<< "\n\n";
        }

        float bATK= 0.0f;
        float armorDefence= player.armor.armorDefence[player.inventory.armor];
        bool extraAction= false;

        do
        {
            int battleInput, skillInput, potionInput;

            float effectiveCritDMG= (player.stats.critBuffTurns> 0)? player.stats.critDMG* 2: player.stats.critDMG;
            float effectiveCritChance= (player.stats.critChanceBuffTurns> 0)? player.stats.critChance* 2: player.stats.critChance; 
            bool isCritical= (rand()% 100)< effectiveCritChance;
            float critMultiplier= isCritical? effectiveCritDMG: 1;
            float weaponDamage= player.weapon.weaponDMG[player.inventory.weapon];
            float baseDamagePlayer= (player.stats.STR+ weaponDamage)- (bosses.stats.DEF* 2/ 5);
            float attackVariance= rand()% player.weapon.weaponMaxAttackVariance[player.inventory.weapon]- player.weapon.weaponMinAttackVariance[player.inventory.weapon];
            float playerATK= max(1.0f, baseDamagePlayer+ attackVariance)* critMultiplier;

            cinClear();

            cout<< "1. Attack\n";
            cout<< "2. Skill\n";
            cout<< "3. Check Stats\n";
            cout<< "4. Inventory\n";
            cout<< "5. Potion\n";
            cout<< "0. Run\nChoice: ";

            if (!(cin>>battleInput))
            {
                cinClear();
                battleInput= -1;
            }
            cout<< "\n";

            if (battleInput< 0|| battleInput> 5)
            {
                cout<< "There is no running away!\n\n";
                wait(2);
                extraAction= true;
            }

            if (battleInput== 0)
            {
                cout<< "There is no running away!\n\n";
                wait(2);
                extraAction= true;
            }

            if (battleInput== 1)
            {
                bosses.stats.HP-= playerATK;
                cout<< "You attacked for "<< playerATK<< " damage!\n";
                if (isCritical== true)
                {
                    cout<< "CRITICAL HIT!!!\n\n";
                }
                wait(2);
                cout<< bosses.stats.name<< "'s HP "<<max(0.0f, bosses.stats.HP)<< "\n\n";
                wait(2);
                extraAction= false;
            }
            else if (battleInput== 2)
            {
                cout<< "Mana: "<< player.stats.MP<< "\n";
                cout<< "Choose skill:\n1. Brute Strike: STR *1.5.   Mana required: "<< player.skill.skillMPCost<< ".\n";
                cout<< "2. Focus Strike: Crit Damage *1.2.   Mana required: "<< player.skill.skill1MPCost<< ".\n";
                cout<< "3. Enchanced Perception: Crit Chance *2.   Mana required: "<< player.skill.skill2MPCost<< ".\n0. Cancel\nChoice: ";
                
                if (!(cin>> skillInput))
                {
                    cinClear();
                    skillInput= -1;
                    extraAction= true;
                }
                cout<< "\n";

                if (skillInput<= 0|| skillInput> 3)
                {
                    cout<< "You canceled cast\n\n";
                    wait(2);
                    extraAction= true;
                }
                
                else if (skillInput== 1)
                {
                    if (player.stats.MP< player.skill.skillMPCost)
                    {
                        cout<< "Not enough MP! ("<< player.skill.skillMPCost<< " MP required)\n";
                        wait();
                        extraAction= true;
                        continue;
                    }
                    
                    cout<< "You used Brute Strike!\n";
                    wait(2);

                    float effectiveCritDMG= (player.stats.critBuffTurns> 0)? player.stats.critDMG* 2: player.stats.critDMG;
                    float effectiveCritChance= (player.stats.critChanceBuffTurns)> 0? player.stats.critChance* 2: player.stats.critChance;
                    bool isCritical= (rand()% 100)< effectiveCritChance;
                    float critMultiplier= isCritical? effectiveCritDMG: 1;
                    float weaponDamage= player.weapon.weaponDMG[player.inventory.weapon];
                    float baseDamagePlayer= (player.skill.skill+ weaponDamage)- (bosses.stats.DEF* 2/ 5);
                    float attackVariance= rand()% player.weapon.weaponMaxAttackVariance[player.inventory.weapon]- player.weapon.weaponMinAttackVariance[player.inventory.weapon];
                    float playerATK= max(1.0f, baseDamagePlayer+ attackVariance)* critMultiplier;
                    
                    player.stats.MP-= player.skill.skillMPCost;

                    bosses.stats.HP-= playerATK;
                    cout<< "You attacked for "<< playerATK<< " damage!\n";
                    if (isCritical== true)
                    {
                        cout<< "CRITICAL HIT!!!\n\n";
                    }
                    wait(2);
                    cout<< bosses.stats.name<< "'s HP "<<max(0.0f, bosses.stats.HP)<< "\n\n";
                    wait(2);
                    extraAction= false;
                }

                else if (skillInput== 2)
                {
                    if (player.stats.MP< player.skill.skillMPCost)
                    {
                        cout<< "Not enough MP! ("<< player.skill.skillMPCost<< " MP required)\n";
                        wait();
                        extraAction= true;
                        continue;
                    }

                    player.stats.critBuffTurns+= 2;
                    player.stats.MP-= player.skill.skill1MPCost;

                    cout<< "You used Focus Strike! Critical Damage is doubled for 2 turns!\n\n";
                    wait(2);
                    extraAction= true;
                    continue;
                }
                
                else if (skillInput== 3)
                {
                    if (player.stats.MP< player.skill.skill2MPCost)
                    {
                        cout<< "Not enough MP! ("<< player.skill.skill2MPCost<< " MP required)\n";
                        wait();
                        extraAction= true;
                        continue;
                    }

                    player.stats.critChanceBuffTurns+= 2;
                    player.stats.MP-= player.skill.skill2MPCost;

                    cout<< "You used Enhanced Perception! Critical Chance is doubled for 2 turns!\n\n";
                    wait(2);
                    extraAction= true;
                    continue;
                }
                
            }

            else if (battleInput== 3)
            {
                player.checkStats();
                extraAction= true;
                continue;
            }
            
            else if (battleInput== 4)
            {
                player.Inv(player.inventory, player.weapon, player.armor, player.talisman);
                extraAction= true;
                continue;
            }
            
            else if (battleInput== 5)
            {
                cout<< "1. Health Potion ["<< player.inventory.potionHealth<< "]\n";
                cout<< "2. Mana Potion ["<< player.inventory.potionMana<< "]\n";
                cout<< "0. Cancel\nChoice: ";

                if (!(cin>> potionInput))
                {
                    cinClear();
                    potionInput= -1;
                }
                cout<< "\n";

                if (potionInput== 1)
                {
                    if (player.inventory.potionHealth> 0)
                    {
                        int heal= player.stats.maxHP* POTION_HEAL;
                        player.stats.HP= min(player.stats.maxHP, player.stats.HP+ (player.stats.maxHP* POTION_HEAL));
                        player.inventory.potionHealth--;
                        cout<< "Restored "<< heal<< " HP!\n\n";
                        wait(2);
                    }
                    else{
                        cout<< "No health potion!\n\n";
                        wait();
                        extraAction= true;
                        continue;
                    }
                }

                else if (potionInput== 2)
                {
                    if (player.inventory.potionMana> 0)
                    {
                        player.stats.MP= min(player.stats.maxMP, player.stats.MP+ MANA_POTION_HEAL);
                        player.inventory.potionMana--;
                        cout<< "Restored "<< MANA_POTION_HEAL<< " MP!\n\n";
                        wait(2);
                    }
                    else{
                        cout<< "No mana potion!\n\n";
                        wait();
                        extraAction= true;
                        continue;
                    }
                }
                extraAction= true;
            }
            
            if (bosses.stats.HP<= 0)
            {
                cout<< "You have defeated the "<< bosses.stats.name<< "!\n";
                wait();
                cout<< "XP gained: "<< bosses.stats.XP<< "\n";
                wait();
                cout<< "Gold gained: "<< bosses.stats.gold<< "\n";
                wait();

                bosses.bossTracker.slainedCount(bosses.stats.name);

                player.talismanAttributes(player.inventory, player.talisman);

                player.stats.XP+= bosses.stats.XP;
                player.stats.gold+= bosses.stats.gold;
                bosses.stats.count--;

                levelUp();

                break;
            }
        } while (extraAction);

        if (bosses.stats.HP> 0)
        {
            if (turn% 5!= 0&& bosses.stats.MP>= bosses.stats.skillCost)
            {
                int bAttackVariance= rand()% 41- 20;
                float bBaseDamage= bosses.stats.STR- ((player.stats.DEF+ armorDefence)* 2/ 5);
                bool bIsCritical= rand()% 100< bosses.stats.CritChance;
                float bCritMultiplier= bIsCritical? bosses.stats.critDMG: 1;
                bATK= max(1.0f, bBaseDamage+ bAttackVariance)* bCritMultiplier;

                player.stats.HP-= bATK;
                cout<< bosses.stats.name<< " attacks for "<< bATK<< " damage!\n";
                wait();
                if (bIsCritical== true)
                {
                    cout<< "CRITICAL HIT!!!\n\n";
                }
                wait();
                cout<< player.stats.name<< "'s HP: "<< max(0.0f, player.stats.HP)<< "\n\n";
                wait();
            }
            else if(turn% 5== 0&& bosses.stats.MP>= bosses.stats.skillCost)
            {
                bosses.stats.MP-= bosses.stats.skillCost;
                int bAttackVariance= rand()% 41- 20;
                float bBaseDamage= bosses.stats.STR- ((player.stats.DEF+ armorDefence)* 2/ 5);
                bool bIsCritical= rand()% 100< bosses.stats.CritChance;
                float bCritMultiplier= bIsCritical? bosses.stats.critDMG: 1;
                bATK= max(1.0f, bBaseDamage+ bAttackVariance)* bCritMultiplier;

                player.stats.HP-= bATK;
                cout<< bosses.stats.name<< " uses a skill! \n";
                wait();
                cout<< bosses.stats.skillName<< "!!!\n";
                wait();
                cout<< bosses.stats.name<< " attacks you for: "<< bATK<< " damage!\n";
                if (bIsCritical== true)
                {
                    cout<< "CRITICAL HIT!!!\n\n";
                }
                wait();
                cout<< player.stats.name<< "'s HP: "<< max(0.0f, player.stats.HP)<< "\n\n";
                wait();
                
                cout<< "Effect Applied: "<< bosses.stats.skillName<< "\n";
                cout<< "Your HP would drain by 10 on every turn\n\n";
                wait(2);

                bosses.stats.skill+= 5;
            }
            else if (turn% 5== 0&& bosses.stats.MP< bosses.stats.skillCost)
            {
                cout<< bosses.stats.name<< " is exhausted1!!\n\n";
                continue;
                if (bosses.stats.skill> 0) {
                    player.stats.HP-= 10;
                    cout<< "HP -10\n\n";
                };
            }
        }
        if (player.stats.HP<= 0)
        {
            cout<< "You";
            waitms();
            cout<< " ";
            waitms();
            cout<< "have";
            waitms();
            cout<< " ";
            waitms();
            cout<< "been";
            waitms();
            cout<< " ";
            waitms();
            cout<< "defeated!\n\n";
            waitms();

            cout<< "Exiting.";
            wait();
            cout<< ".";
            wait();
            cout<< ".\n";
            return;
        } 
    }
}

void bossBattleKingCelestial(){
    cout<< fixed<< setprecision(1);

    inline void resetPlayerBuffs();

    player.stats.critBuffTurns= 0;
    player.stats.critChanceBuffTurns= 0;

    if (bosses.stats.count< 1)
    {
        return;
    }

    int turn= 0;

    for (int i = 0; i < 11; i++)
    {
        cout<< "***";
        waitms();
    }
    cout<< "***\n\n";
    waitms();

    int roll= rand()% 4;
    if (roll< 1) cout<< bosses.stats.name<<" has had enough!\n\n";
    else if (roll< 2) cout<< bosses.stats.name<< " is angered!\n\n";
    else if (roll< 3) cout<< bosses.stats.name<< " is furious!\n\n"; 
    else cout<< "You've angered "<< bosses.stats.name<< "\n\n";

    for (int i = 0; i < 11; i++)
    {
        cout<< "***";
        waitms();
    }
    cout<< "***\n\n";
    waitms();
    wait(2);

    while (player.stats.HP> 0&& bosses.stats.HP> 0)
    {
        turn++;
        
        player.stats.MP++;
        if(player.stats.MP> player.stats.maxMP) player.stats.MP= player.stats.maxMP;
        bosses.stats.MP++;
        if(bosses.stats.MP> bosses.stats.maxMP) bosses.stats.MP= bosses.stats.maxMP;
        player.stats.critBuffTurns--;
        if (player.stats.critBuffTurns< 0) player.stats.critBuffTurns= 0;
        player.stats.critChanceBuffTurns--;
        if (player.stats.critChanceBuffTurns< 0) player.stats.critChanceBuffTurns= 0;

        cout<< "************************************\n";
        cout<< "        "<< bosses.stats.name<< " Stats        \n";
        cout<< "************************************\n";
        cout<< "HP: "<<bosses.stats.HP<< "/"<< bosses.stats.maxHP<< "\n";
        cout<< "MP: "<<bosses.stats.MP<< "/"<< bosses.stats.maxMP<< "\n";
        cout<< "STR: "<< bosses.stats.STR<< "\n";
        cout<< "DEF: "<< bosses.stats.DEF<< "\n";
        cout<< "Crit Damage: "<< bosses.stats.critDMG<< "\n";
        cout<< "Crit Chance: "<< bosses.stats.CritChance<< "\n\n";
        cout<< "************************************\n\n";
        wait(2);

        if (turn== 1)
        {
            cout<< "Turn: "<< turn<< "\n\n";
        } else{
            cout<< "Turns: "<< turn<< "\n\n";
        }

        float bATK= 0.0f;
        float armorDefence= player.armor.armorDefence[player.inventory.armor];
        bool extraAction= false;

        do
        {
            int battleInput, skillInput, potionInput;

            float effectiveCritDMG= (player.stats.critBuffTurns> 0)? player.stats.critDMG* 2: player.stats.critDMG;
            float effectiveCritChance= (player.stats.critChanceBuffTurns> 0)? player.stats.critChance* 2: player.stats.critChance; 
            bool isCritical= (rand()% 100)< effectiveCritChance;
            float critMultiplier= isCritical? effectiveCritDMG: 1;
            float weaponDamage= player.weapon.weaponDMG[player.inventory.weapon];
            float baseDamagePlayer= (player.stats.STR+ weaponDamage)- (bosses.stats.DEF* 2/ 5);
            float attackVariance= rand()% player.weapon.weaponMaxAttackVariance[player.inventory.weapon]- player.weapon.weaponMinAttackVariance[player.inventory.weapon];
            float playerATK= max(1.0f, baseDamagePlayer+ attackVariance)* critMultiplier;

            cinClear();

            cout<< "1. Attack\n";
            cout<< "2. Skill\n";
            cout<< "3. Check Stats\n";
            cout<< "4. Inventory\n";
            cout<< "5. Potion\n";
            cout<< "0. Run\nChoice: ";

            if (!(cin>>battleInput))
            {
                cinClear();
                battleInput= -1;
            }
            cout<< "\n";

            if (battleInput< 0|| battleInput> 5)
            {
                cout<< "There is no running away!\n\n";
                wait(2);
                extraAction= true;
            }

            if (battleInput== 0)
            {
                cout<< "There is no running away!\n\n";
                wait(2);
                extraAction= true;
            }

            if (battleInput== 1)
            {
                bosses.stats.HP-= playerATK;
                cout<< "You attacked for "<< playerATK<< " damage!\n";
                if (isCritical== true)
                {
                    cout<< "CRITICAL HIT!!!\n\n";
                }
                wait(2);
                cout<< bosses.stats.name<< "'s HP "<<max(0.0f, bosses.stats.HP)<< "\n\n";
                wait(2);
                extraAction= false;
            }
            else if (battleInput== 2)
            {
                cout<< "Mana: "<< player.stats.MP<< "\n";
                cout<< "Choose skill:\n1. Brute Strike: STR *1.5.   Mana required: "<< player.skill.skillMPCost<< ".\n";
                cout<< "2. Focus Strike: Crit Damage *1.2.   Mana required: "<< player.skill.skill1MPCost<< ".\n";
                cout<< "3. Enchanced Perception: Crit Chance *2.   Mana required: "<< player.skill.skill2MPCost<< ".\n0. Cancel\nChoice: ";
                
                if (!(cin>> skillInput))
                {
                    cinClear();
                    skillInput= -1;
                    extraAction= true;
                }
                cout<< "\n";

                if (skillInput<= 0|| skillInput> 3)
                {
                    cout<< "You canceled cast\n\n";
                    wait(2);
                    extraAction= true;
                }
                
                else if (skillInput== 1)
                {
                    if (player.stats.MP< player.skill.skillMPCost)
                    {
                        cout<< "Not enough MP! ("<< player.skill.skillMPCost<< " MP required)\n";
                        wait();
                        extraAction= true;
                        continue;
                    }
                    
                    cout<< "You used Brute Strike!\n";
                    wait(2);

                    float effectiveCritDMG= (player.stats.critBuffTurns> 0)? player.stats.critDMG* 2: player.stats.critDMG;
                    float effectiveCritChance= (player.stats.critChanceBuffTurns)> 0? player.stats.critChance* 2: player.stats.critChance;
                    bool isCritical= (rand()% 100)< effectiveCritChance;
                    float critMultiplier= isCritical? effectiveCritDMG: 1;
                    float weaponDamage= player.weapon.weaponDMG[player.inventory.weapon];
                    float baseDamagePlayer= (player.skill.skill+ weaponDamage)- (bosses.stats.DEF* 2/ 5);
                    float attackVariance= rand()% player.weapon.weaponMaxAttackVariance[player.inventory.weapon]- player.weapon.weaponMinAttackVariance[player.inventory.weapon];
                    float playerATK= max(1.0f, baseDamagePlayer+ attackVariance)* critMultiplier;
                    
                    player.stats.MP-= player.skill.skillMPCost;

                    bosses.stats.HP-= playerATK;
                    cout<< "You attacked for "<< playerATK<< " damage!\n";
                    if (isCritical== true)
                    {
                        cout<< "CRITICAL HIT!!!\n\n";
                    }
                    wait(2);
                    cout<< bosses.stats.name<< "'s HP "<<max(0.0f, bosses.stats.HP)<< "\n\n";
                    wait(2);
                    extraAction= false;
                }

                else if (skillInput== 2)
                {
                    if (player.stats.MP< player.skill.skillMPCost)
                    {
                        cout<< "Not enough MP! ("<< player.skill.skillMPCost<< " MP required)\n";
                        wait();
                        extraAction= true;
                        continue;
                    }

                    player.stats.critBuffTurns+= 2;
                    player.stats.MP-= player.skill.skill1MPCost;

                    cout<< "You used Focus Strike! Critical Damage is doubled for 2 turns!\n\n";
                    wait(2);
                    extraAction= true;
                    continue;
                }
                
                else if (skillInput== 3)
                {
                    if (player.stats.MP< player.skill.skill2MPCost)
                    {
                        cout<< "Not enough MP! ("<< player.skill.skill2MPCost<< " MP required)\n";
                        wait();
                        extraAction= true;
                        continue;
                    }

                    player.stats.critChanceBuffTurns+= 2;
                    player.stats.MP-= player.skill.skill2MPCost;

                    cout<< "You used Enhanced Perception! Critical Chance is doubled for 2 turns!\n\n";
                    wait(2);
                    extraAction= true;
                    continue;
                }
                
            }

            else if (battleInput== 3)
            {
                player.checkStats();
                extraAction= true;
                continue;
            }
            
            else if (battleInput== 4)
            {
                player.Inv(player.inventory, player.weapon, player.armor, player.talisman);
                extraAction= true;
                continue;
            }
            
            else if (battleInput== 5)
            {
                cout<< "1. Health Potion ["<< player.inventory.potionHealth<< "]\n";
                cout<< "2. Mana Potion ["<< player.inventory.potionMana<< "]\n";
                cout<< "0. Cancel\nChoice: ";

                if (!(cin>> potionInput))
                {
                    cinClear();
                    potionInput= -1;
                }
                cout<< "\n";

                if (potionInput== 1)
                {
                    if (player.inventory.potionHealth> 0)
                    {
                        int heal= player.stats.maxHP* POTION_HEAL;
                        player.stats.HP= min(player.stats.maxHP, player.stats.HP+ (player.stats.maxHP* POTION_HEAL));
                        player.inventory.potionHealth--;
                        cout<< "Restored "<< heal<< " HP!\n\n";
                        wait(2);
                    }
                    else{
                        cout<< "No health potion!\n\n";
                        wait();
                        extraAction= true;
                        continue;
                    }
                }

                else if (potionInput== 2)
                {
                    if (player.inventory.potionMana> 0)
                    {
                        player.stats.MP= min(player.stats.maxMP, player.stats.MP+ MANA_POTION_HEAL);
                        player.inventory.potionMana--;
                        cout<< "Restored "<< MANA_POTION_HEAL<< " MP!\n\n";
                        wait(2);
                    }
                    else{
                        cout<< "No mana potion!\n\n";
                        wait();
                        extraAction= true;
                        continue;
                    }
                }
                extraAction= true;
            }
            
            if (bosses.stats.HP<= 0)
            {
                cout<< "You have defeated the "<< bosses.stats.name<< "!\n";
                wait();
                cout<< "XP gained: "<< bosses.stats.XP<< "\n";
                wait();
                cout<< "Gold gained: "<< bosses.stats.gold<< "\n";
                wait();

                bosses.bossTracker.slainedCount(bosses.stats.name);

                player.talismanAttributes(player.inventory, player.talisman);

                player.stats.XP+= bosses.stats.XP;
                player.stats.gold+= bosses.stats.gold;
                bosses.stats.count--;

                levelUp();

                break;
            }
        } while (extraAction);

        if (bosses.stats.HP> 0)
        {
            if (turn% 5!= 0)
            {
                int bAttackVariance= rand()% 41- 20;
                float bBaseDamage= bosses.stats.STR- ((player.stats.DEF+ armorDefence)* 2/ 5);
                bool bIsCritical= rand()% 100< bosses.stats.CritChance;
                float bCritMultiplier= bIsCritical? bosses.stats.critDMG: 1;
                bATK= max(1.0f, bBaseDamage+ bAttackVariance)* bCritMultiplier;

                player.stats.HP-= bATK;
                cout<< bosses.stats.name<< " attacks for "<< bATK<< " damage!\n";
                wait();
                if (bIsCritical== true)
                {
                    cout<< "CRITICAL HIT!!!\n\n";
                }
                wait();
                cout<< player.stats.name<< "'s HP: "<< max(0.0f, player.stats.HP)<< "\n\n";
                wait();
            }
            else if(turn% 5== 0&& bosses.stats.MP>= bosses.stats.skillCost)
            {
                int bAttackVariance= rand()% 41- 20;
                float bBaseDamage= bosses.stats.STR- ((player.stats.DEF+ armorDefence)* 2/ 5);
                bool bIsCritical= rand()% 100< bosses.stats.CritChance+ bosses.stats.skill;
                float bCritMultiplier= bIsCritical? bosses.stats.critDMG: 1;
                bATK= max(1.0f, bBaseDamage+ bAttackVariance)* bCritMultiplier;

                player.stats.HP-= bATK;
                cout<< bosses.stats.name<< " uses a skill! \n";
                wait();
                cout<< bosses.stats.skillName<< "!!!\n";
                wait();
                cout<< bosses.stats.name<< " attacks you for: "<< bATK<< " damage!\n";
                if (bIsCritical== true)
                {
                    cout<< "CRITICAL HIT!!!\n\n";
                }
                wait();
                cout<< player.stats.name<< "'s HP: "<< max(0.0f, player.stats.HP)<< "\n\n";
                wait();
            }
            else if (turn% 5== 0&& bosses.stats.MP< bosses.stats.skillCost)
            {
                cout<< bosses.stats.name<< " is exhausted1!!\n\n";
                continue;
            }
        }
        if (player.stats.HP<= 0)
        {
            cout<< "You";
            waitms();
            cout<< " ";
            waitms();
            cout<< "have";
            waitms();
            cout<< " ";
            waitms();
            cout<< "been";
            waitms();
            cout<< " ";
            waitms();
            cout<< "defeated!\n\n";
            waitms();

            cout<< "Exiting.";
            wait();
            cout<< ".";
            wait();
            cout<< ".\n";
            return;
        } 
    }
}

void travel(){

    if (player.stats.HP<= 0) return;

    WorldMap::Nation wM= worldMap.createNation();
    WorldMap::Area nAreaGrayhaven= worldMap.createAreaGrayhaven();
    WorldMap::Area nAreaStormwich= worldMap.createAreaStormwich();
    WorldMap::Area nAreaHillsgate= worldMap.createAreaHillsgate();
    WorldMap::Area nAreaCelestial= worldMap.createAreaCelestial();
    
    int travelInput;
    int travelDistance;
    bool isTravelling= true;
    bool inNation= false;
    bool inFarm= false;
    bool inMarket= false;
    bool inTown= false;
    bool inCapital= false;

    int inFarmInput;
    int inMarketInput;
    int inTownInput;
    int inCapitalInput;

    if (!player.isStoryMidTriggered&& player.stats.trust>= 300)
    {
        player.isStoryMidTriggered= true;
        storyMid();   
    }

    while (isTravelling)
    {
        if (player.stats.HP<= 0) return;

        levelUp();

        inFarm= false;
        inMarket= false;
        inTown= false;
        inCapital= false;

        string currentLocation= "Wilderness";
        string currentArea= "Wilderness";
        for (size_t i = 0; i < wM.name.size(); i++)
        {
            int minX= min(wM.positionx[i], wM.positionx1[i]);
            int maxX= max(wM.positionx[i], wM.positionx1[i]);
            int minY= min(wM.positiony[i], wM.positiony1[i]);
            int maxY= max(wM.positiony[i], wM.positiony1[i]);
            if (player.x>= minX&& player.x<= maxX&& player.y>= minY&& player.y<= maxY){
                currentLocation= wM.name[i];
                inNation= true;
                break;
            }
        }

        if (currentLocation== "Wilderness"&& abs(player.x)> 2000|| abs(player.y)> 2000)
        {
            bosses.stats= bosses.createBossDragon();
            scaleBossByLevel(player.stats.level);

            if (bosses.stats.count== 0){
                return;
            } else bossBattleDragon();
        }

        if (inNation)
        {
            if (player.stats.HP<= 0) return;
            if (currentLocation== "Grayhaven")
            {
                for (size_t i = 0; i < nAreaGrayhaven.name.size(); i++)
                {
                    int minX= min(nAreaGrayhaven.positionx[i], nAreaGrayhaven.positionx1[i]);
                    int maxX= max(nAreaGrayhaven.positionx[i], nAreaGrayhaven.positionx1[i]);
                    int minY= min(nAreaGrayhaven.positiony[i], nAreaGrayhaven.positiony1[i]);
                    int maxY= max(nAreaGrayhaven.positiony[i], nAreaGrayhaven.positiony1[i]);

                    if (player.x>= minX&& player.x<= maxX&& player.y>= minY&& player.y<= maxY)
                    {
                        currentArea= nAreaGrayhaven.name[i];
                        if (i== 0) inCapital= true;
                        else if (i== 1) inTown= true;
                        else if (i== 2) inMarket= true;
                        else if (i== 3) inFarm= true;
                    }
                }
            }

            if (currentLocation== "Stormwich")
            {
                for (size_t i = 0; i < nAreaStormwich.name.size(); i++)
                {
                    int minX= min(nAreaStormwich.positionx[i], nAreaStormwich.positionx1[i]);
                    int maxX= max(nAreaStormwich.positionx[i], nAreaStormwich.positionx1[i]);
                    int minY= min(nAreaStormwich.positiony[i], nAreaStormwich.positiony1[i]);
                    int maxY= max(nAreaStormwich.positiony[i], nAreaStormwich.positiony1[i]);

                    if (player.x>= minX&& player.x<= maxX&& player.y>= minY&& player.y<= maxY)
                    {
                        currentArea= nAreaStormwich.name[i];
                        if (i== 0) inCapital= true;
                        else if (i== 1) inTown= true;
                        else if (i== 2) inMarket= true;
                        else if (i== 3) inFarm= true;
                    }
                }
            }

            if (currentLocation== "Hillsgate")
            {
                for (size_t i = 0; i < nAreaHillsgate.name.size(); i++)
                {
                    int minX= min(nAreaHillsgate.positionx[i], nAreaHillsgate.positionx1[i]);
                    int maxX= max(nAreaHillsgate.positionx[i], nAreaHillsgate.positionx1[i]);
                    int minY= min(nAreaHillsgate.positiony[i], nAreaHillsgate.positiony1[i]);
                    int maxY= max(nAreaHillsgate.positiony[i], nAreaHillsgate.positiony1[i]);

                    if (player.x>= minX&& player.x<= maxX&& player.y>= minY&& player.y<= maxY)
                    {
                        currentArea= nAreaHillsgate.name[i];
                        if (i== 0) inCapital= true;
                        else if (i== 1) inTown= true;
                        else if (i== 2) inMarket= true;
                        else if (i== 3) inFarm= true;
                    }
                }
            }

            if (currentLocation== "Celestial")
            {
                for (size_t i = 0; i < nAreaCelestial.name.size(); i++)
                {
                    int minX= min(nAreaCelestial.positionx[i], nAreaCelestial.positionx1[i]);
                    int maxX= max(nAreaCelestial.positionx[i], nAreaCelestial.positionx1[i]);
                    int minY= min(nAreaCelestial.positiony[i], nAreaCelestial.positiony1[i]);
                    int maxY= max(nAreaCelestial.positiony[i], nAreaCelestial.positiony1[i]);

                    if (player.x>= minX&& player.x<= maxX&& player.y>= minY&& player.y<= maxY)
                    {
                        currentArea= nAreaCelestial.name[i];
                        if (i== 0) inCapital= true;
                        else if (i== 1) inTown= true;
                        else if (i== 2) inMarket= true;
                        else if (i== 3) inFarm= true;
                    }
                }
            }
        }
        
        if (currentLocation== "Grayhaven"&& player.stats.trust<= -100)
        {
            bosses.stats= bosses.createBossKingGrayhaven();
            scaleBossByLevel(player.stats.level);

            bossBattleKingGrayhaven();
            if (player.stats.HP<= 0) return;
        }

        if (currentLocation== "Stormwich"&& player.stats.trust<= -100)
        {
            bosses.stats= bosses.createBossKingStormwich();
            scaleBossByLevel(player.stats.level);

            bossBattleKingStormwich();
            if (player.stats.HP<= 0) return;
        }

        if (currentLocation== "Hillsgate"&& player.stats.trust<= -100)
        {
            bosses.stats= bosses.createBossKingHillsgate();
            scaleBossByLevel(player.stats.level);

            bossBattleKingHillsgate();
            if (player.stats.HP<= 0) return;
        }

        if (currentLocation== "Celestial"&& player.stats.trust<= -100)
        {
            bosses.stats= bosses.createBossKingCelestial();
            scaleBossByLevel(player.stats.level);

            bossBattleKingCelestial();
            if (player.stats.HP<= 0) return;
        }

        cout<< "**************************\n";
        cout<< "******* TRAVELLING *******\n";
        cout<< "**************************\n\n";
        cout<< "Location: "<< currentLocation<< '\n';
        cout<< "Area: "<< currentArea<< "\n";
        cout<< "Coordinates: x= "<< player.x<< "   y= "<< player.y<< "\n\n";
        cout<< "1. Travel x, East/West\n";
        cout<< "2. Travel y, North/South\n";
        cout<< "3. Check Stats\n";
        cout<< "4. Map\n";
        cout<< "5. Rest\n";

        if (currentLocation== "Wilderness") inNation= false;

        if (inNation== true) cout<< "6. Nation Coordinates\n";
        
        if (inFarm== true) cout<< "7. Act\n";

        if (inMarket== true)
        {
            cout<< "7. Act\n";
            cout<< "8. Shop\n";
        }
        if (inTown== true) cout<< "7. Act\n";
        
        if (inCapital== true)
        {
            cout<< "7. Act\n";
            cout<< "8. Shop\n";
        }
        
        cout<< "0. Cancel\nChoice: ";

        // CIN TRAVEL INPUT

        if (!(cin>> travelInput))
        {
            cinClear();
            travelInput= -1;
        }
        cout<< '\n';

        if (travelInput== 2147)
        {
            cout<< "CHEAT ACTIVATED!\n\n";
            
            for(int i= 0; i< 10; i++){
                player.stats.XP= player.stats.maxXP;
                levelUp();
            }
            player.stats.gold+= 99999;
            player.stats.trust+= 999;
            player.x= -2200;
            player.y= -1200;
            continue;
        }
        if (travelInput== 7412)
        {
            cout<< "CHEAT ACTIVATED!\n";
            
            for(int i= 0; i< 10; i++){
                player.stats.XP= player.stats.maxXP;
                levelUp();
            }
            player.stats.gold+= 99999;
            player.stats.trust+= -999;
            player.inventory.weapon= 9;
            player.inventory.armor= 7;
            player.inventory.talisman= 6;
            player.inventory.talisman1= 4;
            player.x= -2200;
            player.y= -1200;
            continue;
        }
        
        // TRAVEL INPUT 0 4 1 2 5 3

        if (travelInput== 0)
        {
            return;
        } 

        // TRAVEL INPUT 4 1 2 5 3

        else if (travelInput== 4)
        {
            cout<< "*****************************************\n";
            cout<< "************** WORLD MAP ****************\n";
            cout<< "*****************************************\n";
            cout<< "*                   *                   *\n";
            cout<< "*                   *                   *\n";
            cout<< "*                   *                   *\n";
            cout<< "*     Hillsgate     *                   *\n";
            cout<< "*    (-1400, 600)   *                   *\n";
            cout<< "*                   *    Grayhaven      *\n";
            cout<< "*                   *    (75, 60)       *\n";
            cout<< "*                   *                   *\n";
            cout<< "* * * * * * * * * * * * * * * * * * * * *\n";
            cout<< "*                   *                   *\n";
            cout<< "*                   *                   *\n";
            cout<< "*                   *     Stormwich     *\n";
            cout<< "*                   *    (650, -130)    *\n";
            cout<< "*                   *                   *\n";
            cout<< "*                   *                   *\n";
            cout<< "* Celestial         *                   *\n";
            cout<< "*(-2200, -1200)     *                   *\n";
            cout<< "*****************************************\n\n";
            cout<< "Each nation are splitted into 4 different area!\n\n";
            wait(5);
        } 
        
        // TRAVEL INPUT 1 2 5 3
        
        else if (travelInput== 1)
        {
            cout<< "How far? (negative= West, positive= East, -100 to 100): ";
            if (!(cin>> travelDistance))
            {
                cinClear();
                travelDistance= 0;
            }
            cout<< "\n";
            travelDistance= max(-100, min(100, travelDistance));

            player.x+= travelDistance;
            player.distanceLastTraveled += abs(travelDistance);
            
            cout<< "Travelling";
            wait();
            cout<< ".";
            wait();
            cout<< ".";
            wait();
            cout<< ".\n\n";

            int roll= rand()% 2;
            
            if (inNation== false)
            {
                if (roll== 1)
                {
                    if (rand()% 2== 1)
                    {
                        battleThief();
                        if (player.stats.HP<= 0) return;
                    } else{
                        battleBandit();
                        if (player.stats.HP<= 0) return;
                    }
                }
            }

            continue;
        } 
        
        // TRAVEL INPUT 2 5 3

        else if (travelInput== 2)
        {
            cout<< "How far? (negative= South, positive= North, -100 to 100): ";
            if (!(cin>> travelDistance))
            {
                cinClear();
                travelDistance= 0;
            }
            cout<< "\n";
            travelDistance= max(-100, min(100, travelDistance));

            player.y+= travelDistance;
            player.distanceLastTraveled += abs(travelDistance);

            cout<< "Travelling";
            wait();
            cout<< ".";
            wait();
            cout<< ".";
            wait();
            cout<< ".\n\n";

            int roll= rand()% 2;
            
            if (inNation== false)
            {
                if (roll== 1)
                {
                    if (rand()% 2== 1)
                    {
                        battleThief();
                        if (player.stats.HP<= 0) return;
                    } else{
                        battleBandit();
                        if (player.stats.HP<= 0) return;
                    }
                }
            }
            continue;
        } 
        
        // TRAVEL INPUT 5 3
        
        else if (travelInput== 5)
        {
            if(player.distanceLastTraveled>= 100){
                cout<< fixed<< setprecision(1);

                float HPRestored= player.stats.maxHP* 0.2f;
                float MPRestored= player.stats.maxMP* 0.2f;

                player.distanceLastTraveled= 0;
                player.stats.HP= min(player.stats.maxHP, player.stats.HP+ HPRestored);
                player.stats.MP= min(player.stats.maxMP, player.stats.MP+ MPRestored);

                cout<< "You decided to rest.\nHP restored: "<< HPRestored<< "\nMP restored: "<< MPRestored<< "\n\n";
                wait(2);
                
                if (inNation== false)
                {
                    if (rand()% 100< 30)
                    {
                        cout<< "You were ambushed while resting\n\n";
                        wait(2);
                        if (rand()% 2== 1)
                        {
                            battleThief();
                            if (player.stats.HP<= 0) return;
                        } else{
                            battleBandit();
                            if (player.stats.HP<= 0) return;
                        }
                    }
                    wait(2);
                }
            } else{
                int remaining= 100- player.distanceLastTraveled;
                cout<< "You're not tired yet\n\n";
                wait(2);
            }
        }
        
        // TRAVEL INPUT 3
        
        else if (travelInput== 3){
            player.checkStats();
        }

        // TRAVEL INPUT 6
        
        else if (travelInput== 6&& inNation)
        {
            cout<< "Areas in "<< currentLocation<< ": \n\n";

            if (currentLocation== "Grayhaven")
            {
                for (size_t i = 0; i < nAreaGrayhaven.name.size(); i++)
                {
                    cout<< nAreaGrayhaven.name[i]<< ":\n";
                    cout<< "   x: "<< nAreaGrayhaven.positionx[i]<< " to "<< nAreaGrayhaven.positionx1[i]<< "\n";
                    cout<< "   y: "<< nAreaGrayhaven.positiony[i]<< " to "<< nAreaGrayhaven.positiony1[i]<< "\n\n";
                    wait();
                }
            } else if (currentLocation== "Stormwich")
            {
                for (size_t i = 0; i < nAreaStormwich.name.size(); i++)
                {
                    cout<< nAreaStormwich.name[i]<< ":\n";
                    cout<< "   x: "<< nAreaStormwich.positionx[i]<< " to "<< nAreaStormwich.positionx1[i]<< "\n";
                    cout<< "   y: "<< nAreaStormwich.positiony[i]<< " to "<< nAreaStormwich.positiony1[i]<< "\n\n";
                    wait();
                }
            } else if (currentLocation== "Hillsgate")
            {
                for (size_t i = 0; i < nAreaHillsgate.name.size(); i++)
                {
                    cout<< nAreaHillsgate.name[i]<< ":\n";
                    cout<< "   x: "<< nAreaHillsgate.positionx[i]<< " to "<< nAreaHillsgate.positionx1[i]<< "\n";
                    cout<< "   y: "<< nAreaHillsgate.positiony[i]<< " to "<< nAreaHillsgate.positiony1[i]<< "\n\n";
                    wait();
                }
            } else if (currentLocation== "Celestial")
            {
                for (size_t i = 0; i < nAreaCelestial.name.size(); i++)
                {
                    cout<< nAreaCelestial.name[i]<< ":\n";
                    cout<< "   x: "<< nAreaCelestial.positionx[i]<< " to "<< nAreaCelestial.positionx1[i]<< "\n";
                    cout<< "   y: "<< nAreaCelestial.positiony[i]<< " to "<< nAreaCelestial.positiony1[i]<< "\n\n";
                    wait();
                }
            }
        }      

        // TRAVEL INPUT 7
        
        // FARM
        else if (inFarm== true&& travelInput== 7)
        {
            cout<< "************ ACT ************\n";
            cout<< "1. Protect the farm\n";
            cout<< "2. Burn the farm\n0. Do nothing\nChoice: ";

            if (!(cin>> inFarmInput))
            {
                cinClear();
                inFarmInput= -1;            
            }

            if (inFarmInput== 1)
            {
                cout<< "Protecting";
                cout<< ".";
                wait();
                cout<< ".";
                wait();
                cout<< ".\n\n";
                wait();

                bool isProtected= false;
                int roll= rand()% 100;
                
                if (roll< 30)
                {
                    if (rand()% 2== 1)
                    {
                        battleThief();
                        if (player.stats.HP<= 0) return;
                        if (enemies.stats.HP> 0)
                        {
                            cout<< "You abandoned the farm!\n\n";
                            wait();
                            player.stats.trust-= 10;
                            cout<< "Trust -10\n\n";
                        } else if (enemies.stats.HP<= 0) isProtected= true;
                        
                    } else{
                        battleBandit();
                        if (player.stats.HP<= 0) return;
                        if (enemies.stats.HP> 0)
                        {
                            cout<< "You abandoned the farm!\n\n";
                            wait();
                            player.stats.trust-= 10;
                            cout<< "Trust -10\n\n";
                        } else if (enemies.stats.HP<= 0) isProtected= true;
                    }
                    wait(2);
                } else{
                    isProtected= true;
                }

                if (player.stats.HP<= 0) return;
                if(isProtected== true){
                    cout<< "You've successfully protected the farm for an hour!\n\n";
                    wait();
                    player.stats.XP+= 20;
                    player.stats.trust+= 20;
                    cout<< "XP +20\n";
                    cout<< "Trust gained: +20\n\n";
                    wait();
                }
                if (rand()% 2== 1)
                {
                    int roll= max(1, rand()% 31);
                    cout<< "You've received "<< roll<< " amount of gold!\n\n";
                    player.stats.gold+= roll;
                    wait();
                }
            } else if (inFarmInput== 2)
            {
                cout<< "You burned down some part of the farm, causing chaos!\n\n";
                wait();
                int roll= rand()% 100;
                if (roll< 50)
                {
                    if (currentLocation== "Grayhaven") battleFarmer(nAreaGrayhaven.scaling);
                    if (currentLocation== "Stormwich") battleFarmer(nAreaStormwich.scaling);
                    if (currentLocation== "Hillsgate") battleFarmer(nAreaHillsgate.scaling);
                    if (currentLocation== "Celestial") battleFarmer(nAreaCelestial.scaling);
                    if (player.stats.HP<= 0) return;
                } else if (roll< 80)
                {  
                    if (currentLocation== "Grayhaven") battleVillager(nAreaGrayhaven.scaling);
                    if (currentLocation== "Stormwich") battleVillager(nAreaStormwich.scaling);
                    if (currentLocation== "Hillsgate") battleVillager(nAreaHillsgate.scaling);
                    if (currentLocation== "Celestial") battleVillager(nAreaCelestial.scaling);
                    if (player.stats.HP<= 0) return;
                } else{
                    if (currentLocation== "Grayhaven") battleGuard(nAreaGrayhaven.scaling);
                    if (currentLocation== "Stormwich") battleGuard(nAreaStormwich.scaling);
                    if (currentLocation== "Hillsgate") battleGuard(nAreaHillsgate.scaling);
                    if (currentLocation== "Celestial") battleGuard(nAreaCelestial.scaling);
                    if (player.stats.HP<= 0) return;
                }
            } else{
                continue;
            }  
        }
        
        // MARKET
        else if (inMarket== true&& travelInput== 7)
        {
            cout<< "************ ACT ************\n";
            cout<< "1. Shop\n";
            cout<< "2. Help a shop owner\n3. Steal\n4. Start a fight\n0. Do nothing\nChoice: ";

            if (!(cin>> inMarketInput))
            {
                cinClear();
                inMarketInput= -1;
            }
            cout<< "\n";
            
            if (inMarketInput== 1)
            {
                shop();
            } else if (inMarketInput== 2)
            {
                int helpShopInput;

                cout<< "***** HELP A SHOP OWNER *****\n";
                cout<< "1. Clean the shop\n2. Guard the shop\n0. Cancel\nChoice: ";

                if (!(cin>> helpShopInput))
                {
                    cinClear();
                    helpShopInput= -1;
                }
                
                if (helpShopInput== 1)
                {
                    cout<< "Cleaning the shop";
                    for (int i = 0; i < 3; i++)
                    {
                        cout<< ".";
                        wait();
                    }
                    cout<< "\n\nYou've cleaned the shop!\n\n";

                    player.stats.XP+= 10;
                    player.stats.trust+= 10;

                    cout<< "XP +10\n";

                    if (rand()%2== 1)
                    {
                        player.stats.gold+= 10;
                        cout<< "The shop owner pays you 10 gold in return!\n";
                    }
                    cout<< "Trust gained: +10\n\n";
                    wait(2);                 
                } else if (helpShopInput== 2)
                {
                    
                    cout<< "Guarding the shop";
                    for (int i = 0; i < 3; i++)
                    {
                        cout<< ".";
                        wait();
                    }
                    cout<< "\n\n";
                    
                    int roll= rand()%100;
                    bool isGuarded= false;

                    if (roll< 30)
                    {
                        if (rand()%2== 1)
                        {
                            battleThief();
                            if (player.stats.HP<= 0) return;
                            if (enemies.stats.HP> 0)
                            {
                                cout<< "You abandoned the shop owner!\n\n";
                                wait();
                                player.stats.trust-= 10;
                                cout<< "Trust -10\n\n";
                            } else if (enemies.stats.HP<= 0) isGuarded= true;
                        } else{
                            battleBandit();
                            if (player.stats.HP<= 0) return;
                            if (enemies.stats.HP> 0&& enemies.stats.fled== false)
                            {
                                cout<< "You abandoned the shop owner!\n\n";
                                wait();
                                player.stats.trust-= 10;
                                cout<< "Trust -10\n\n";
                            } else if (enemies.stats.HP> 0&& enemies.stats.fled== true)
                            {
                                cout<< "You managed to kick out the thief\n\n";
                                wait();
                                player.stats.trust+= 20;
                                cout<< "Trust +20\n\n";
                                isGuarded= true;
                            }
                             
                            else if (enemies.stats.HP<= 0) isGuarded= true;
                        }
                        wait(2);
                    } else{
                        isGuarded= true;
                    }
                    
                    if (player.stats.HP<= 0) return;

                    if(isGuarded== true){
                    cout<< "You've successfully guarded the shop for an hour!\n\n";
                    wait();
                    player.stats.XP+= 20;
                    player.stats.trust+= 20;
                    cout<< "XP +20\n";
                    cout<< "Trust gained: +20\n\n";
                    wait();
                        if (rand()% 2== 1)
                        {
                            int roll= max(1, rand()% 51);
                            cout<< "You've received "<< roll<< " amount of gold from the shop owner in return!\n\n";
                            wait();
                            player.stats.gold+= roll;
                        }
                    }          
                } else{
                    return;
                }
            } else if (inMarketInput== 3)
            {
                cout<< "Stealing";
                for (int i = 0; i < 3; i++)
                {
                    cout<< ".";
                    wait();
                }
                cout<< "\n\n";

                if (rand()%2== 1)
                {
                    int roll= rand()% 30+ 1;

                    cout<< "You've successfully stole "<< roll<< " amount of gold from the shop owner!\n\n";
                    
                    player.stats.gold+= roll;
                    player.stats.trust-= 10;
                    cout<< "Trust -10\n\n";
                } else{
                    if (rand()%3 < 2)
                    {
                        if (currentLocation== "Grayhaven") battleVillager(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleVillager(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleVillager(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleVillager(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return;
                    } else{
                        if (currentLocation== "Grayhaven") battleGuard(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleGuard(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleGuard(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleGuard(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return;
                    }
                }
            } else if (inMarketInput== 4)
            {
                int startAFight;

                cout<< "You see: \n1. Villager\n2. Guard\n3. Guard Dog\n0. Keep it to yourself\nChoice: \n";

                if (!(cin>> startAFight))
                {
                    cinClear();
                    startAFight= -1;
                }
                cout<< "\n";

                if (startAFight== 1)
                {
                    cout<< "You decided to do some chaos to provoke a fight!\n\n";
                    wait();

                    if (currentLocation== "Grayhaven") battleVillager(nAreaGrayhaven.scaling);
                    if (currentLocation== "Stormwich") battleVillager(nAreaStormwich.scaling);
                    if (currentLocation== "Hillsgate") battleVillager(nAreaHillsgate.scaling);
                    if (currentLocation== "Celestial") battleVillager(nAreaCelestial.scaling);
                    if (player.stats.HP<= 0) return;
                } else if (startAFight== 2)
                {
                    cout<< "You decided to do some chaos to provoke a fight!\n\n";
                    wait();

                    if (currentLocation== "Grayhaven") battleGuard(nAreaGrayhaven.scaling);
                    if (currentLocation== "Stormwich") battleGuard(nAreaStormwich.scaling);
                    if (currentLocation== "Hillsgate") battleGuard(nAreaHillsgate.scaling);
                    if (currentLocation== "Celestial") battleGuard(nAreaCelestial.scaling);
                    if (player.stats.HP<= 0) return;
                } else if (startAFight== 3)
                {
                    cout<< "You decided to do some chaos to provoke a fight!\n\n";
                    wait();

                    if (currentLocation== "Grayhaven") battleGuardDog(nAreaGrayhaven.scaling);
                    if (currentLocation== "Stormwich") battleGuardDog(nAreaStormwich.scaling);
                    if (currentLocation== "Hillsgate") battleGuardDog(nAreaHillsgate.scaling);
                    if (currentLocation== "Celestial") battleGuardDog(nAreaCelestial.scaling);
                    if (player.stats.HP<= 0) return;
                    if (currentLocation== "Grayhaven") battleGuard(nAreaGrayhaven.scaling);
                    if (currentLocation== "Stormwich") battleGuard(nAreaStormwich.scaling);
                    if (currentLocation== "Hillsgate") battleGuard(nAreaHillsgate.scaling);
                    if (currentLocation== "Celestial") battleGuard(nAreaCelestial.scaling);
                    if (player.stats.HP<= 0) return;
                } else if (startAFight== 0)
                {
                    cout<< "You decided to keep it to yourself.\n\n";
                    return;
                } else{
                    return;
                } 
            } else{
                continue;
            }
        }
        else if (inMarket== true&& travelInput== 8) shop();        
        
        // TOWN
        else if (inTown== true&& travelInput== 7)
        {
            cout<< "************ ACT ************\n";
            cout<< "1. Steal\n";
            cout<< "2. Start a fight\n";
            cout<< "3. Wreak havoc\n0. Do nothing\nChoice: ";

            if (!(cin>> inTownInput))
            {
                cinClear();
                inTownInput= -1;
            }
            cout<< "\n";

            if (inTownInput== 0)
            {
                return;
            } else if (inTownInput== 1)
            {
                cout<< "Stealing";
                for (int i = 0; i < 3; i++)
                {
                    cout<< ".";
                    wait();
                }
                cout<< "\n\n";

                if (rand()%2== 1)
                {
                    int roll= rand()% 30+ 1;

                    cout<< "You've successfully stole "<< roll<< " amount of gold from a civilian!\n\n";

                    player.stats.gold+= roll;
                    player.stats.trust--;
                    cout<< "Trust -1\n\n";
                } else {
                    int roll= rand()% 100;

                    if (roll< 60)
                    {
                        if (currentLocation== "Grayhaven") battleVillager(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleVillager(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleVillager(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleVillager(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return;
                    } else if (roll< 85)
                    {
                        if (currentLocation== "Grayhaven") battleGuard(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleGuard(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleGuard(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleGuard(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return;
                    } else if (roll< 95)
                    {
                        if (currentLocation== "Grayhaven") battleGuardDog(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleGuardDog(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleGuardDog(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleGuardDog(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return;
                        if (currentLocation== "Grayhaven") battleGuard(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleGuard(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleGuard(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleGuard(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return;
                    } else if (roll< 99)
                    {
                        if (currentLocation== "Grayhaven") battleSoldier(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleSoldier(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleSoldier(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleSoldier(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return;
                    } else if (roll< 100)
                    {
                        if (currentLocation== "Grayhaven") battleKnight(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleKnight(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleKnight(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleKnight(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return;
                    }
                }
            } else if (inTownInput== 2){

                int startAFight;

                cout<< "You see: \n1. Villager\n2. Guard\n3. Guard Dog\n4. Soldier\n5. Knight\n0. Keep it to yourself\nChoice: \n";

                if (!(cin>>startAFight))
                {
                    cinClear();
                    startAFight= -1;
                }
                cout<< "\n";

                if (startAFight== 1)
                {
                    cout<< "You've provoked a fight!\n\n";
                    if (currentLocation== "Grayhaven") battleVillager(nAreaGrayhaven.scaling);
                    if (currentLocation== "Stormwich") battleVillager(nAreaStormwich.scaling);
                    if (currentLocation== "Hillsgate") battleVillager(nAreaHillsgate.scaling);
                    if (currentLocation== "Celestial") battleVillager(nAreaCelestial.scaling);
                    if (player.stats.HP<= 0) return;
                }
                
                else if (startAFight== 2)
                {
                    cout<< "You've provoked a fight!\n\n";
                    if (currentLocation== "Grayhaven") battleGuard(nAreaGrayhaven.scaling);
                    if (currentLocation== "Stormwich") battleGuard(nAreaStormwich.scaling);
                    if (currentLocation== "Hillsgate") battleGuard(nAreaHillsgate.scaling);
                    if (currentLocation== "Celestial") battleGuard(nAreaCelestial.scaling);
                    if (player.stats.HP<= 0) return;
                }
                
                else if (startAFight== 3)
                {
                    cout<< "You've provoked a fight!\n\n";
                    if (currentLocation== "Grayhaven") battleGuardDog(nAreaGrayhaven.scaling);
                    if (currentLocation== "Stormwich") battleGuardDog(nAreaStormwich.scaling);
                    if (currentLocation== "Hillsgate") battleGuardDog(nAreaHillsgate.scaling);
                    if (currentLocation== "Celestial") battleGuardDog(nAreaCelestial.scaling);
                    if (player.stats.HP<= 0) return;
                }
                
                else if (startAFight== 4)
                {
                    cout<< "You've provoked a fight!\n\n";
                    if (currentLocation== "Grayhaven") battleSoldier(nAreaGrayhaven.scaling);
                    if (currentLocation== "Stormwich") battleSoldier(nAreaStormwich.scaling);
                    if (currentLocation== "Hillsgate") battleSoldier(nAreaHillsgate.scaling);
                    if (currentLocation== "Celestial") battleSoldier(nAreaCelestial.scaling);
                    if (player.stats.HP<= 0) return;
                }
                
                else if (startAFight== 5)
                {
                    cout<< "You've provoked a fight!\n\n";
                    if (currentLocation== "Grayhaven") battleKnight(nAreaGrayhaven.scaling);
                    if (currentLocation== "Stormwich") battleKnight(nAreaStormwich.scaling);
                    if (currentLocation== "Hillsgate") battleKnight(nAreaHillsgate.scaling);
                    if (currentLocation== "Celestial") battleKnight(nAreaCelestial.scaling);
                    if (player.stats.HP<= 0) return;
                } 
                else if(startAFight== 0){
                    cout<< "You decided to keep it to yourself.\n\n";
                    return;
                }
                else{
                    return;
                }
            } else if (inTownInput== 3)
            {
                int wreakHavocInput;

                cout<< "******** WREAK HAVOC ********\n";
                cout<< "1. Provoke\n2. Burn a stack of clothes\n3. Burn a building\n0. Cancel\nChoice: ";

                if (!(cin>> wreakHavocInput))
                {
                    cinClear();
                    wreakHavocInput= -1;
                }
                cout<< "\n";
                
                if (wreakHavocInput== 0)
                {
                    return;
                } else if (wreakHavocInput== 1)
                {
                    int roll= rand()% 2;

                    if (roll== 1)
                    {
                        int roll1= rand()% 5;

                            if (roll1== 0)
                            {
                                enemies.stats= enemies.createEnemyVillager();
                                cout<< "You've successfully provoke a "<< enemies.stats.name<< "!\n\n";
                                wait();
                                player.stats.XP+= 5;
                                cout<< "XP +5\n\n";
                                wait();
                                if (currentLocation== "Grayhaven") battleVillager(nAreaGrayhaven.scaling);
                                if (currentLocation== "Stormwich") battleVillager(nAreaStormwich.scaling);
                                if (currentLocation== "Hillsgate") battleVillager(nAreaHillsgate.scaling);
                                if (currentLocation== "Celestial") battleVillager(nAreaCelestial.scaling);
                                if (player.stats.HP<= 0) return;
                            }
                            if (roll1== 1)
                            {
                                enemies.stats= enemies.createEnemyGuard();
                                cout<< "You've successfully provoke a "<< enemies.stats.name<< "!\n\n";
                                wait();
                                player.stats.XP+= 10;
                                cout<< "XP +10\n\n";
                                if (currentLocation== "Grayhaven") battleGuard(nAreaGrayhaven.scaling);
                                if (currentLocation== "Stormwich") battleGuard(nAreaStormwich.scaling);
                                if (currentLocation== "Hillsgate") battleGuard(nAreaHillsgate.scaling);
                                if (currentLocation== "Celestial") battleGuard(nAreaCelestial.scaling);
                                if (player.stats.HP<= 0) return;
                            }
                            if (roll1== 2)
                            {
                                enemies.stats= enemies.createEnemyGuardDog();
                                cout<< "You've successfully provoke a "<< enemies.stats.name<< "!\n\n";
                                wait();
                                player.stats.XP+= 10;
                                cout<< "XP +10\n\n";
                                if (currentLocation== "Grayhaven") battleGuardDog(nAreaGrayhaven.scaling);
                                if (currentLocation== "Stormwich") battleGuardDog(nAreaStormwich.scaling);
                                if (currentLocation== "Hillsgate") battleGuardDog(nAreaHillsgate.scaling);
                                if (currentLocation== "Celestial") battleGuardDog(nAreaCelestial.scaling);
                                if (player.stats.HP<= 0) return;
                            }
                            if (roll1== 3)
                            {
                                enemies.stats= enemies.createEnemySoldier();
                                cout<< "You've successfully provoke a "<< enemies.stats.name<< "!\n\n";
                                wait();
                                player.stats.XP+= 10;
                                cout<< "XP +10\n\n";
                                if (currentLocation== "Grayhaven") battleSoldier(nAreaGrayhaven.scaling);
                                if (currentLocation== "Stormwich") battleSoldier(nAreaStormwich.scaling);
                                if (currentLocation== "Hillsgate") battleSoldier(nAreaHillsgate.scaling);
                                if (currentLocation== "Celestial") battleSoldier(nAreaCelestial.scaling);
                                if (player.stats.HP<= 0) return;
                            }
                            if (roll1== 4)
                            {
                                enemies.stats= enemies.createEnemyKnight();
                                cout<< "You've successfully provoke a "<< enemies.stats.name<< "!\n\n";
                                wait();
                                player.stats.XP+= 10;
                                cout<< "XP +10\n\n";
                                if (currentLocation== "Grayhaven") battleKnight(nAreaGrayhaven.scaling);
                                if (currentLocation== "Stormwich") battleKnight(nAreaStormwich.scaling);
                                if (currentLocation== "Hillsgate") battleKnight(nAreaHillsgate.scaling);
                                if (currentLocation== "Celestial") battleKnight(nAreaCelestial.scaling);
                                if (player.stats.HP<= 0) return;
                            }
                            
                    }
                    
                }
                else if (wreakHavocInput== 2)
                {
                    cout<< "You burned a stack of clothes, causing a mild chaos in town\n\n";
                    wait();
                    player.stats.XP+= 30;
                    cout<< "XP +30\n\n";
                    wait();
                    player.stats.trust-= 30;
                    cout<< "Trust gained: -30\n\n";
                    wait();
                    
                    int roll= rand()% 3;

                    if (roll== 0)
                    {
                        if (currentLocation== "Grayhaven") battleGuard(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleGuard(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleGuard(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleGuard(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return;
                    } else if (roll== 1)
                    {
                        if (currentLocation== "Grayhaven") battleSoldier(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleSoldier(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleSoldier(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleSoldier(nAreaCelestial.scaling); 
                        if (player.stats.HP<= 0) return;                   
                    }
                    else{
                        if (currentLocation== "Grayhaven") battleKnight(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleKnight(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleKnight(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleKnight(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return;
                    }
                }
                else if (wreakHavocInput== 3){
                    cout<< "You burned down a building, causing a massive chaos across the towb!\n\n";
                    wait();
                    player.stats.XP+= 50;
                    cout<< "XP +50\n\n";
                    wait();
                    player.stats.trust-= 50;
                    cout<< "Trust gained: -50\n\n";
                    wait();

                    int roll= rand()%100;
                    
                    if (roll< 40)
                    {
                        if (currentLocation== "Grayhaven") battleGuard(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleGuard(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleGuard(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleGuard(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return;
                    }
                    else if (roll< 80)
                    {
                        if (currentLocation== "Grayhaven") battleSoldier(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleSoldier(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleSoldier(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleSoldier(nAreaCelestial.scaling); 
                        if (player.stats.HP<= 0) return;
                    }
                    else if (roll< 87)
                    {
                        if (currentLocation== "Grayhaven") battleKnight(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleKnight(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleKnight(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleKnight(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return;
                    }
                    else if (roll< 94)
                    {
                        if (currentLocation== "Grayhaven") battleArcherKnight(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleArcherKnight(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleArcherKnight(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleArcherKnight(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return;
                    }
                    else{
                        if (currentLocation== "Grayhaven") battleMage(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleMage(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleMage(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleMage(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return;
                    }
                }
            } else if (inTownInput< 0 || inTownInput> 3)
            {
                continue;
            } else{
                continue;
            }       
        } 
        
        //CAPITAL
        else if (inCapital== true&& travelInput== 7)
        {
            cout<< "************ ACT ************\n";
            cout<< "1. Shop\n";
            cout<< "2. Steal\n";
            cout<< "3. Start a fight\n";
            cout<< "4. Wreak havoc\n";
            cout<< "5. Burn the city\n";
            if (player.stats.trust< -100)
            {
                cout<< "7. ???\n";
            }
            cout<< "0. Do nothing\nChoice: ";

            if (!(cin>> inCapitalInput))
            {
                cinClear();
                inCapitalInput= -1;
            }
            cout<< "\n";

            if (inCapitalInput== 0)
            {
                return;
            } else if (inCapitalInput< 0 || inCapitalInput> 5)
            {
                continue;
            } else if (inCapitalInput== 1)
            {
                shop();
            } else if (inCapitalInput== 2)
            {
                cout<< "Stealing";
                for (int i = 0; i < 3; i++)
                {
                    cout<< ".";
                    wait();
                }
                cout<< "\n\n";

                if (rand()%2== 1)
                {
                    int roll= rand()% 50+ 1;

                    cout<< "You've successfully stole "<< roll<< " amount of gold from the shop owner!\n\n";

                    player.stats.gold+= roll;
                    player.stats.trust-= 10;
                    cout<< "Trust -10\n\n";
                } else{
                    int roll= rand()% 9;;
                    if (roll< 1)
                    {
                        if (currentLocation== "Grayhaven") battleVillager(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleVillager(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleVillager(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleVillager(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return;
                    }
                    else if (roll< 2)
                    {
                        if (currentLocation== "Grayhaven") battleGuard(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleGuard(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleGuard(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleGuard(nAreaCelestial.scaling); 
                        if (player.stats.HP<= 0) return;
                    }
                    else if (roll< 3)
                    {
                        if (currentLocation== "Grayhaven") battleGuardDog(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleGuardDog(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleGuardDog(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleGuardDog(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return; 
                    }
                    else if (roll< 4)
                    {
                        if (currentLocation== "Grayhaven") battleSoldier(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleSoldier(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleSoldier(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleSoldier(nAreaCelestial.scaling); 
                        if (player.stats.HP<= 0) return;
                    }
                    else if (roll< 5)
                    {
                        if (currentLocation== "Grayhaven") battleKnight(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleKnight(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleKnight(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleKnight(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return;
                    }
                    else if (roll< 6)
                    {
                        if (currentLocation== "Grayhaven") battleArcherKnight(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleArcherKnight(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleArcherKnight(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleArcherKnight(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return;
                    }
                }
            } else if (inCapitalInput== 3)
            {
                int startAFight;
                cout<< "You see: \n1. Villager\n2. Guard\n3. Guard Dog\n4. Soldier\n5. Knight\n6. Archer Knight\n7. Mage\n";
                int roll= rand()% 100;
                bool inPrince= false;

                if (roll< 20){
                    cout<< "8. Prince\n";
                    inPrince= true;
                }

                cout<< "0. Keep it to yourself\nChoice: \n";

                if (!(cin>> startAFight))
                {
                    cinClear();
                    startAFight= -1;
                }
                cout<< "\n";

                if (startAFight== 1)
                {
                    cout<< "You've created some chaos provoking a fight!\n\n";
                    wait();
                    if (currentLocation== "Grayhaven") battleVillager(nAreaGrayhaven.scaling);
                    if (currentLocation== "Stormwich") battleVillager(nAreaStormwich.scaling);
                    if (currentLocation== "Hillsgate") battleVillager(nAreaHillsgate.scaling);
                    if (currentLocation== "Celestial") battleVillager(nAreaCelestial.scaling);
                    if (player.stats.HP<= 0) return;
                }
                else if (startAFight== 2)
                {
                    cout<< "You've created some chaos provoking a fight!\n\n";
                    wait();
                    if (currentLocation== "Grayhaven") battleGuard(nAreaGrayhaven.scaling);
                    if (currentLocation== "Stormwich") battleGuard(nAreaStormwich.scaling);
                    if (currentLocation== "Hillsgate") battleGuard(nAreaHillsgate.scaling);
                    if (currentLocation== "Celestial") battleGuard(nAreaCelestial.scaling);
                    if (player.stats.HP<= 0) return;
                }
                else if (startAFight== 3)
                {
                    cout<< "You've created some chaos provoking a fight!\n\n";
                    wait();
                    if (currentLocation== "Grayhaven") battleGuardDog(nAreaGrayhaven.scaling);
                    if (currentLocation== "Stormwich") battleGuardDog(nAreaStormwich.scaling);
                    if (currentLocation== "Hillsgate") battleGuardDog(nAreaHillsgate.scaling);
                    if (currentLocation== "Celestial") battleGuardDog(nAreaCelestial.scaling);
                    if (player.stats.HP<= 0) return;
                }
                else if (startAFight== 4)
                {
                    cout<< "You've created some chaos provoking a fight!\n\n";
                    wait();
                    if (currentLocation== "Grayhaven") battleSoldier(nAreaGrayhaven.scaling);
                    if (currentLocation== "Stormwich") battleSoldier(nAreaStormwich.scaling);
                    if (currentLocation== "Hillsgate") battleSoldier(nAreaHillsgate.scaling);
                    if (currentLocation== "Celestial") battleSoldier(nAreaCelestial.scaling);
                    if (player.stats.HP<= 0) return;
                }
                else if (startAFight== 5)
                {
                    cout<< "You've created some chaos provoking a fight!\n\n";
                    wait();
                    if (currentLocation== "Grayhaven") battleKnight(nAreaGrayhaven.scaling);
                    if (currentLocation== "Stormwich") battleKnight(nAreaStormwich.scaling);
                    if (currentLocation== "Hillsgate") battleKnight(nAreaHillsgate.scaling);
                    if (currentLocation== "Celestial") battleKnight(nAreaCelestial.scaling);
                    if (player.stats.HP<= 0) return;
                }
                else if (startAFight== 6)
                {
                    cout<< "You've created some chaos provoking a fight!\n\n";
                    wait();
                    if (currentLocation== "Grayhaven") battleArcherKnight(nAreaGrayhaven.scaling);
                    if (currentLocation== "Stormwich") battleArcherKnight(nAreaStormwich.scaling);
                    if (currentLocation== "Hillsgate") battleArcherKnight(nAreaHillsgate.scaling);
                    if (currentLocation== "Celestial") battleArcherKnight(nAreaCelestial.scaling);
                    if (player.stats.HP<= 0) return;
                }
                else if (startAFight== 7)
                {
                    cout<< "You've created some chaos provoking a fight!\n\n";
                    wait();
                    if (currentLocation== "Grayhaven") battleMage(nAreaGrayhaven.scaling);
                    if (currentLocation== "Stormwich") battleMage(nAreaStormwich.scaling);
                    if (currentLocation== "Hillsgate") battleMage(nAreaHillsgate.scaling);
                    if (currentLocation== "Celestial") battleMage(nAreaCelestial.scaling);
                    if (player.stats.HP<= 0) return;

                    if (rand()%10< 2)
                    {
                        if (currentLocation== "Grayhaven") battleGolem(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleGolem(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleGolem(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleGolem(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return;
                    }
                }
                else if (inPrince== true && startAFight== 8)
                {
                    cout<< "You charged towards the Prince!\n\n";
                    wait();
                    if (currentLocation== "Grayhaven") battlePrince(nAreaGrayhaven.scaling);
                    if (currentLocation== "Stormwich") battlePrince(nAreaStormwich.scaling);
                    if (currentLocation== "Hillsgate") battlePrince(nAreaHillsgate.scaling);
                    if (currentLocation== "Celestial") battlePrince(nAreaCelestial.scaling);
                    if (player.stats.HP<= 0) return;
                }
            } else if (inCapitalInput== 4)
            {
                int wreakHavocInput;

                cout<< "******** WREAK HAVOC ********\n";
                cout<< "1. Provoke\n2. Burn a stack of clothes\n3. Burn a building\n0. Cancel\nChoice: ";

                if (!(cin>> wreakHavocInput))
                {
                    cinClear();
                    wreakHavocInput= -1;
                }
                cout<< "\n";
                
                if (wreakHavocInput== 0)
                {
                    return;
                } else if (wreakHavocInput== 1)
                {
                    int roll= rand()% 2;

                    if (roll== 1)
                    {
                        int roll1= rand()% 5;

                            if (roll1== 0)
                            {
                                enemies.stats= enemies.createEnemyVillager();
                                cout<< "You've successfully provoke a "<< enemies.stats.name<< "!\n\n";
                                wait();
                                player.stats.XP+= 5;
                                cout<< "XP +5\n\n";
                                wait();
                                if (currentLocation== "Grayhaven") battleVillager(nAreaGrayhaven.scaling);
                                if (currentLocation== "Stormwich") battleVillager(nAreaStormwich.scaling);
                                if (currentLocation== "Hillsgate") battleVillager(nAreaHillsgate.scaling);
                                if (currentLocation== "Celestial") battleVillager(nAreaCelestial.scaling);
                                if (player.stats.HP<= 0) return;
                            }
                            if (roll1== 1)
                            {
                                enemies.stats= enemies.createEnemyGuard();
                                cout<< "You've successfully provoke a "<< enemies.stats.name<< "!\n\n";
                                wait();
                                player.stats.XP+= 10;
                                cout<< "XP +10\n\n";
                                if (currentLocation== "Grayhaven") battleGuard(nAreaGrayhaven.scaling);
                                if (currentLocation== "Stormwich") battleGuard(nAreaStormwich.scaling);
                                if (currentLocation== "Hillsgate") battleGuard(nAreaHillsgate.scaling);
                                if (currentLocation== "Celestial") battleGuard(nAreaCelestial.scaling);
                                if (player.stats.HP<= 0) return;
                            }
                            if (roll1== 2)
                            {
                                enemies.stats= enemies.createEnemyGuardDog();
                                cout<< "You've successfully provoke a "<< enemies.stats.name<< "!\n\n";
                                wait();
                                player.stats.XP+= 10;
                                cout<< "XP +10\n\n";
                                if (currentLocation== "Grayhaven") battleGuardDog(nAreaGrayhaven.scaling);
                                if (currentLocation== "Stormwich") battleGuardDog(nAreaStormwich.scaling);
                                if (currentLocation== "Hillsgate") battleGuardDog(nAreaHillsgate.scaling);
                                if (currentLocation== "Celestial") battleGuardDog(nAreaCelestial.scaling);
                                if (player.stats.HP<= 0) return;
                            }
                            if (roll1== 3)
                            {
                                enemies.stats= enemies.createEnemySoldier();
                                cout<< "You've successfully provoke a "<< enemies.stats.name<< "!\n\n";
                                wait();
                                player.stats.XP+= 10;
                                cout<< "XP +10\n\n";
                                if (currentLocation== "Grayhaven") battleSoldier(nAreaGrayhaven.scaling);
                                if (currentLocation== "Stormwich") battleSoldier(nAreaStormwich.scaling);
                                if (currentLocation== "Hillsgate") battleSoldier(nAreaHillsgate.scaling);
                                if (currentLocation== "Celestial") battleSoldier(nAreaCelestial.scaling);
                                if (player.stats.HP<= 0) return;
                            }
                            if (roll1== 4)
                            {
                                enemies.stats= enemies.createEnemyKnight();
                                cout<< "You've successfully provoke a "<< enemies.stats.name<< "!\n\n";
                                wait();
                                player.stats.XP+= 10;
                                cout<< "XP +10\n\n";
                                if (currentLocation== "Grayhaven") battleKnight(nAreaGrayhaven.scaling);
                                if (currentLocation== "Stormwich") battleKnight(nAreaStormwich.scaling);
                                if (currentLocation== "Hillsgate") battleKnight(nAreaHillsgate.scaling);
                                if (currentLocation== "Celestial") battleKnight(nAreaCelestial.scaling);
                                if (player.stats.HP<= 0) return;
                            }
                            
                    }
                    
                }
                else if (wreakHavocInput== 2)
                {
                    cout<< "You burned a stack of clothes, causing a mild chaos in town\n\n";
                    wait();
                    player.stats.XP+= 30;
                    cout<< "XP +30\n\n";
                    wait();
                    player.stats.trust-= 30;
                    cout<< "Trust gained: -30\n\n";
                    wait();
                    
                    int roll= rand()% 3;

                    if (roll== 0)
                    {
                        if (currentLocation== "Grayhaven") battleGuard(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleGuard(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleGuard(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleGuard(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return;
                    } else if (roll== 1)
                    {
                        if (currentLocation== "Grayhaven") battleSoldier(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleSoldier(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleSoldier(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleSoldier(nAreaCelestial.scaling); 
                        if (player.stats.HP<= 0) return;                  
                    }
                    else{
                        if (currentLocation== "Grayhaven") battleKnight(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleKnight(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleKnight(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleKnight(nAreaCelestial.scaling); 
                        if (player.stats.HP<= 0) return;
                    }
                }
                else if (wreakHavocInput== 3){
                    cout<< "You burned down a building, causing a massive chaos across the towb!\n\n";
                    wait();
                    player.stats.XP+= 50;
                    cout<< "XP +50\n\n";
                    wait();
                    player.stats.trust-= 50;
                    cout<< "Trust gained: -50\n\n";
                    wait();

                    int roll= rand()%100;
                    
                    if (roll< 40)
                    {
                        if (currentLocation== "Grayhaven") battleGuard(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleGuard(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleGuard(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleGuard(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return;
                    }
                    else if (roll< 80)
                    {
                        if (currentLocation== "Grayhaven") battleSoldier(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleSoldier(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleSoldier(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleSoldier(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return; 
                    }
                    else if (roll< 87)
                    {
                        if (currentLocation== "Grayhaven") battleKnight(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleKnight(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleKnight(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleKnight(nAreaCelestial.scaling);
                        if (player.stats.HP<= 0) return; 
                    }
                    else if (roll< 94)
                    {
                        if (currentLocation== "Grayhaven") battleArcherKnight(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleArcherKnight(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleArcherKnight(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleArcherKnight(nAreaCelestial.scaling); 
                        if (player.stats.HP<= 0) return;
                    }
                    else{
                        if (currentLocation== "Grayhaven") battleMage(nAreaGrayhaven.scaling);
                        if (currentLocation== "Stormwich") battleMage(nAreaStormwich.scaling);
                        if (currentLocation== "Hillsgate") battleMage(nAreaHillsgate.scaling);
                        if (currentLocation== "Celestial") battleMage(nAreaCelestial.scaling); 
                        if (player.stats.HP<= 0) return;
                    }
                }
            } else if (inCapitalInput== 5)
            {
                cout<< "You'threw torches througout the city, burning it down!!!\n";
                wait(2);
                player.stats.trust-= 500;
                cout<< "Trust gained: -500\n\n";
                wait();
            } else if(inCapitalInput== 6){
                cout<< "x> |2000|   y> |2000|???\n\n";

            } else if (player.stats.trust< -100&& inCapitalInput== 7)
            {
                if (currentLocation== "Grayhaven"&& player.stats.trust<= -100)
                {
                    bosses.stats= bosses.createBossKingGrayhaven();
                    scaleBossByLevel(player.stats.level);

                    if (bosses.stats.count== 0){
                        return;
                    } else bossBattleKingGrayhaven();
                    if (player.stats.HP<= 0) return;
                }

                if (currentLocation== "Stormwich"&& player.stats.trust<= -100)
                {
                    bosses.stats= bosses.createBossKingStormwich();
                    scaleBossByLevel(player.stats.level);

                    if (bosses.stats.count== 0){
                        return;
                    } else bossBattleKingStormwich();
                    if (player.stats.HP<= 0) return;
                }

                if (currentLocation== "Hillsgate"&& player.stats.trust<= -100)
                {
                    bosses.stats= bosses.createBossKingHillsgate();
                    scaleBossByLevel(player.stats.level);

                    if (bosses.stats.count== 0){
                        return;
                    } else bossBattleKingHillsgate();
                    if (player.stats.HP<= 0) return;
                }

                if (currentLocation== "Celestial"&& player.stats.trust<= -100)
                {
                    bosses.stats= bosses.createBossKingCelestial();
                    scaleBossByLevel(player.stats.level);

                    if (bosses.stats.count== 0){
                        return;
                    } else bossBattleKingCelestial();
                    if (player.stats.HP<= 0) return;
                }
            }
        }
        else if (inCapital== true&& travelInput== 8) shop();
    } 
}

void checkCoordinates(){
    cout<< "************************\nx= "<< player.x<< "                y= "<< player.y<< "\n************************\n\n"; wait(2);
}

void listSaveSlots(){
    cout<< "**************************\n";
    cout<< "******* SAVE SLOTS *******\n";
    cout<< "**************************\n\n";

    for (int slot = 1; slot <= 3; slot++)
    {
        string filename = "save"+ to_string(slot)+ ".dat";
        ifstream inFile(filename);

        if (inFile)
        {
            string header, name;
            int level;
            getline(inFile, header);
            getline(inFile, name);
            inFile>> level;
            cout<< "Slot "<< slot<< ": "<< name<< " (level "<< level<< ")\n";
            inFile.close();
        } else{
            cout<< "Slot "<< slot<< ": Empty\n";
        }
        cout<< "\n";
    }   
}

void saveGame(int slot){
    if (slot< 1|| slot> 3)
    {   
        cout<< "Invalid save slot! Must be 1-3.\n\n";
        wait();
        return;
    }
    
    string filename= "save"+ to_string(slot)+ ".dat";
    ofstream outFile(filename);

    if (!outFile)
    {
        cout<< "ERROR!\n\n";
        wait();
        return;
    }
    
    outFile<< "RPG_SAVE_v1\n";

    outFile<< player.stats.name<< "\n";
    outFile<< player.stats.level<< "\n";
    outFile<< player.stats.XP<<" "<< player.stats.maxXP<<  "\n";
    outFile<< player.stats.HP<<" "<< player.stats.maxHP<<  "\n";
    outFile<< player.stats.MP<<" "<< player.stats.maxMP<<  "\n";
    outFile<< player.stats.gold<< "\n";
    outFile<< player.stats.trust<< "\n";

    outFile<< player.stats.baseSTR<< " "<< player.stats.baseDEF<< "\n";
    outFile<< player.stats.baseCritDMG<< " "<< player.stats.baseCritChance<< "\n";

    outFile<< player.stats.critBuffTurns<< " "<< player.stats.critChanceBuffTurns<< "\n";

    outFile<< player.x<< " "<< player.y<< "\n";
    outFile<< player.distanceLastTraveled<< "\n";
    outFile<< player.isStoryMidTriggered<< "\n";

    outFile<< player.inventory.weapon<< " "<< player.inventory.armor<< " "<< player.inventory.talisman<< " "<< player.inventory.talisman1<< "\n";
    outFile<< player.inventory.potionHealth<< " "<< player.inventory.potionMana<< "\n";

    for (int count : enemies.enemyTracker.enemySlain) {
        outFile<< count<< " ";
    }
    outFile<< "\n";

    for (int count : bosses.bossTracker.bossSlain)
    {
        outFile<< count<< " ";
    }
    outFile<< "\n";
    
    outFile.close();
    cout<< "Game Saved!\n\n";
    wait();
}

bool loadGame(int slot){
    if (slot< 1|| slot> 3)
    {
        cout<< "Invalid save slot! Must be 1-3.\n\n";
        wait();
        return false;
    }
    
    string filename= "save"+ to_string(slot)+ ".dat";
    ifstream inFile(filename);

    if (!inFile)
    {
        cout<< "No save file found in slot "<< slot<< "!\n\n";
        wait(2);
        return false;
    }

    string header;
    getline(inFile, header);
    if (header != "RPG_SAVE_v1")
    {
        cout<< "ERROR!\n\n";
        inFile.close();
        wait(2);
        return false;
    }
    
    getline(inFile, player.stats.name);
    inFile >> player.stats.level >> player.stats.XP >> player.stats.maxXP;
    inFile >> player.stats.HP >> player.stats.baseMaxHP;
    inFile >> player.stats.MP >> player.stats.baseMaxMP;
    inFile >> player.stats.gold >> player.stats.trust;

    inFile >> player.stats.baseSTR >> player.stats.baseDEF;
    inFile >> player.stats.baseCritDMG >> player.stats.baseCritChance;

    inFile >> player.stats.critBuffTurns >> player.stats.critChanceBuffTurns;

    inFile >> player.x >> player.y;
    inFile>> player.distanceLastTraveled;
    inFile>> player.isStoryMidTriggered;

    inFile >> player.inventory.weapon >> player.inventory.armor >> player.inventory.talisman >> player.inventory.talisman1;
    inFile >> player.inventory.potionHealth >> player.inventory.potionMana;

    player.weapon= player.createWeapon();
    player.armor= player.createArmor();
    player.talisman= player.createTalisman();

    for (size_t i = 0; i < enemies.enemyTracker.enemySlain.size(); i++) {
        inFile >> enemies.enemyTracker.enemySlain[i];
    }

    for (size_t i = 0; i < bosses.bossTracker.bossSlain.size(); i++) {
        inFile >> bosses.bossTracker.bossSlain[i];
    }
    
    inFile.close();

    player.talismanAttributes(player.inventory, player.talisman);
    player.skill = player.createPlayerSkill();
    
    cout << "Game loaded successfully from slot " << slot << "!\n\n";
    wait(2);
    return true;
}

int main(){
    srand(time(0));

    enemies.enemyTracker.init();
    bosses.bossTracker.init();

    cout<< "****************************************\n";
    cout<< "        Welcome to RPG Levelling!       \n";
    cout<< "****************************************\n";
    wait(2);

    int startInput;
    bool gameInitialized= false;

    while (!gameInitialized)
    {
        cout<< "1. New Game\n2. Load Game\n0. Exit\nChoice: ";

        if (!(cin>> startInput))
        {
            cinClear();
            startInput= -1;
        }
        cout<< "\n";
        
        if (startInput== 0)
        {
            cout<< "Exiting game."; wait(); cout<< "."; wait(); cout<< "."; wait();
            return 0;
        }
        else if (startInput== 1)
        {
            player.stats= player.createPlayerStats();
            player.inventory= player.createPlayerInventory();
            player.skill= player.createPlayerSkill();
            player.weapon= player.createWeapon();
            player.armor= player.createArmor();
            player.talisman = player.createTalisman();

            player.inventory.potionHealth= 3;
            player.inventory.potionMana= 3;

            player.talismanAttributes(player.inventory, player.talisman);

            cout<< "\n";

            storyIntro();

            gameInitialized= true;
        }
        else if (startInput== 2)
        {
            int loadSlot;
            listSaveSlots();
            cout<< "Select slot to load (1-3)\nChoice: ";

            if (!(cin>> loadSlot)) {
                cinClear();
                loadSlot = -1;
            }
            cout<< "\n";

            if (loadSlot>= 1&& loadSlot<= 3)
            {
                string filename = "save" + to_string(loadSlot) + ".dat";
                ifstream testFile(filename);

                if (testFile)
                {
                    testFile.close();
                    loadGame(loadSlot);

                    player.talismanAttributes(player.inventory, player.talisman);
                    player.skill = player.createPlayerSkill();

                    gameInitialized = true;
                } else{
                    cout << "No save file found in slot " << loadSlot << "!\n\n";
                    wait(2);
                }
            }
            else {
                cout << "Invalid selection!\n\n";
                wait(2);
            }
        } else {
            cout << "Invalid choice! Please try again.\n\n";
            wait();
        }
    }

    cout<< "Initializing";
    wait();
    for (int i = 0; i < 3; i++)
    {
        cout<< ".";
        wait();
    }
    cout<< "\n\n";
    wait();

    player.checkStats();

    int playerInput;
    do
    {
        inline void resetPlayerBuffs();

        cout<< "*************************\n";
        cout<< "***** RPG LEVELLING *****\n";
        cout<< "*************************\n";
        cout<< "1. Travel\n2. Inventory\n3. Check Stats\n4. Check Coordinates\n5. Save Game\n6. Load Game\n7. Slained Count\n0. Exit\nChoice: ";

        if (!(cin>> playerInput))
        {
            cinClear();
            playerInput= -1;
        }
        cout<< "\n";

        switch (playerInput)
        {
        case 0:
            cout<< "Exiting game."; wait(); cout<< "."; wait(); cout<< "."; wait();
            break;
        case 1:
            travel();
            break;
        case 2:
            player.Inv(player.inventory, player.weapon, player.armor, player.talisman);
            break;
        case 3:
            player.checkStats();
            break;
        case 4:
            checkCoordinates();
            break;
        case 5:
            int saveSlot;
            listSaveSlots();
            cout << "Select slot (1-3): ";
            if (!(cin >> saveSlot)) {
                cinClear();
                saveSlot = -1;
            }
            cout << "\n";
            if (saveSlot >= 1 && saveSlot <= 3) {
                saveGame(saveSlot);
            } else {
                cout << "Invalid selection!\n\n";
                wait(2);
            }
            break;
        case 6:
            int loadSlot;
            listSaveSlots();
            cout << "Select slot to load (1-3): ";
            if (!(cin >> loadSlot)) {
                cinClear();
                loadSlot = -1;
            }
            cout << "\n";
            if (loadSlot >= 1 && loadSlot <= 3) {
                loadGame(loadSlot);
            } else {
                cout << "Invalid selection!\n\n";
                wait(2);
            }
            break;
        case 7:
            enemies.enemyTracker.slainDisplay();
            bosses.bossTracker.slainDisplay();
            break;
        case 8:
            bosses.stats= bosses.createBossDragon();
            break;
        default:
            break;
        }

        if (player.stats.HP<= 0) return 0;
        
    } while (playerInput!= 0);
    return 0;
}
