#include <map>
#include <set>
#include <algorithm>
#include <vector>
#include <math.h>
#include <iostream>
#include <utility>
#include "json/single_include/nlohmann/json.hpp"
#include <fstream>

using namespace std;
using json = nlohmann::json;

struct karta
{
    int type;
    int value;
};


map<set<pair<int, int>>, int> Hierarhy5;//typ, wartoœæ
map<set<pair<int, int>>, int> Hierarhy7;

int amount[4][13];

bool next_combination(vector<int>& elements,int k, int n) {
    vector<int> base;
    for (int i = n - k; i < n; i++) base.push_back(i);

    if (elements == base) return false;

    while (elements.back() == n - (k - elements.size()) - 1) {
        elements.pop_back();
    }
    int el = elements.back();
    elements.pop_back();

    elements.push_back(++el);

    while (elements.size() != k) {
        elements.push_back(++el);
    }

    return true;

}

void update_amount(set<pair<int, int>> new_unit) {
    for (auto it_karta : new_unit) {
        auto [x, y] = it_karta;
        amount[x][y]++;
    }
}

pair<int, int> convert_togrid(int index) {
    index = 52 - index;
    int val = (index % 4 == 0) ? -1 : 0;
    val += index / 4;

    int type = (index + 3) % 4;

    //type = index - val * (type + 1) - 1;

    return { type, val };
}

void licz_POKER(int& unit_value) {

    for (int i = 0; i <= 8; i++) {
        for (int j = 0; j < 4; j++) {
            set<pair<int, int>> new_unit = { {j, 12 - i}, {j, 11 - i}, {j, 10 - i}, {j, 9 - i}, {j, 8 - i} };
            Hierarhy5[new_unit] = unit_value;
            update_amount(new_unit);
        }
        unit_value++;
    }

    for (int j = 0; j <= 3; j++) { // dla pokera z asem jako 1
        set<pair<int, int>> new_unit = { {j, 12}, {j, 0}, {j, 1}, {j, 2}, {j, 3} };
        Hierarhy5[new_unit] = unit_value;
        update_amount(new_unit);
    }
    unit_value++;
}

void licz_KARETA(int& unit_value) {
    for (int i = 12; i >= 0; i--) {
        set<pair<int, int>> new_unit = { {0, i}, {1,i}, {2, i}, {3, i} }; // 4 kart bazowe(kareta)
        for (int j = 12; j >= 0; j--) {
            if (j == i) continue;

            for (int k = 0; k <= 3; k++) {
                new_unit.insert({ k, j });
                Hierarhy5[new_unit] = unit_value;
                update_amount(new_unit);

                new_unit.erase({ k, j });
            }
            unit_value++;
        }
    }
}

void licz_FULLHOUSE(int& unit_value) {
    for (int i = 12; i >= 0; i--) {
        vector<int> combination1 = { 0, 1, 2 };
        int saved_val = unit_value;
        do {
            set<pair<int, int>> new_unit = { {combination1[0], i}, {combination1[1], i}, {combination1[2], i} }; // 3 kart bazowe(trójka)
            unit_value = saved_val;
            for (int j = 12; j >= 0; j--) {
                if (i == j) continue;

                vector<int> combination2 = { 0, 1 };

                do {
                    new_unit.insert({ combination2[0], j });
                    new_unit.insert({ combination2[1], j }); // dodatkowa para
                    Hierarhy5[new_unit] = unit_value;

                    update_amount(new_unit);

                    new_unit.erase({ combination2[0], j });
                    new_unit.erase({ combination2[1], j });

                } while (next_combination(combination2, 2, 4));

                unit_value++;
            }

        } while (next_combination(combination1, 3, 4));
    }
}

void licz_KOLOR(int& unit_value) {
    int saved_val = unit_value;
    for (int i = 0; i <= 3; i++) {
        unit_value = saved_val;

        vector<int> combination = { 0, 1, 2, 3, 4 };

        do {
            set<pair<int, int>> new_unit;
            for (int v : combination) new_unit.insert({ i, 12 - v }); //ca³y kolor

            if (Hierarhy5[new_unit] != 0) continue; //je¿eli formacja to poker

            Hierarhy5[new_unit] = unit_value;
            unit_value++;

            update_amount(new_unit);

        } while (next_combination(combination, 5, 13));
    }
}

void licz_STRIT(int& unit_value) {

    for (int i = 12; i >= 4; i--) {
        unit_value++;
        for (int t1 = 0; t1 <= 3; t1++)
            for (int t2 = 0; t2 <= 3; t2++)
                for (int t3 = 0; t3 <= 3; t3++)
                    for (int t4 = 0; t4 <= 3; t4++)
                        for (int t5 = 0; t5 <= 3; t5++) {
                            set<pair<int, int>> new_unit = { {t1, i}, {t2, i - 1}, {t3, i - 2}, {t4, i - 3}, {t5, i - 4} };

                            if (Hierarhy5[new_unit] != 0) continue; //je¿eli formacja to poker

                            Hierarhy5[new_unit] = unit_value;
                            update_amount(new_unit);
                        }
    }
    unit_value++;
    for (int t1 = 0; t1 <= 3; t1++) // as zamias jeden
        for (int t2 = 0; t2 <= 3; t2++)
            for (int t3 = 0; t3 <= 3; t3++)
                for (int t4 = 0; t4 <= 3; t4++)
                    for (int t5 = 0; t5 <= 3; t5++) {
                        set<pair<int, int>> new_unit = { {t1, 12}, {t2, 0}, {t3, 1}, {t4, 2}, {t5, 3} };

                        if (Hierarhy5[new_unit] != 0) continue; //je¿eli formacja to poker

                        Hierarhy5[new_unit] = unit_value;

                        update_amount(new_unit);
                    }
}

void licz_TROJKA(int& unit_value) {
    for (int i = 12; i >= 0; i--) {
        vector<int> combination1 = { 0, 1, 2 };
        int saved_val = unit_value;
        do {
            set<pair<int, int>> new_unit = { {combination1[0], i}, {combination1[1], i}, {combination1[2], i} }; // 3 kart bazowe(trójka)
            unit_value = saved_val;

            vector<int> combination2 = { 0,1 };
            int actval1 = 12, actval2 = 12;
            do {               
                pair<int, int> k[2];
                int d = 0;
             
                k[0].second = 12 - combination2[0];
                k[1].second = 12 - combination2[1];

                if (k[0].second == i || k[1].second == i) continue;

                for (int t1 = 0; t1 < 4; t1++)
                    for (int t2 = 0; t2 < 4; t2++) {
                        k[0].first = t1;
                        k[1].first = t2;

                        new_unit.insert(k[0]);
                        new_unit.insert(k[1]); // 2 karty dodatkowe

                        if (Hierarhy5[new_unit] != 0) {
                            new_unit.erase(k[0]);
                            new_unit.erase(k[1]);
                            continue; //je¿eli formacja to FULLHOUSE
                        }

                        Hierarhy5[new_unit] = unit_value;

                        update_amount(new_unit);

                        new_unit.erase(k[0]);
                        new_unit.erase(k[1]);
                    }

                unit_value++;

            } while (next_combination(combination2, 2, 13));

        } while (next_combination(combination1, 3, 4));
    }
}

void licz_2PARY(int& unit_value) {
    vector<int> value_combination = { 0,1 };

    do { // ustalanie wartoœci 2 par

        set<pair<int, int>> new_unit;
        int saved_unitval = unit_value;
        vector<int> type_combination = { 0, 1 };

        do {//ustalanie typu lepszej pary

            new_unit.insert({ type_combination[0], 12 - value_combination[0] });
            new_unit.insert({ type_combination[1], 12 - value_combination[0] }); //2 karty bazowe(lepsza para)

            vector<int> sectype_combination = { 0, 1 };

            do { //ustalanie typu gorszej pary

                new_unit.insert({ sectype_combination[0], 12 - value_combination[1] });
                new_unit.insert({ sectype_combination[1], 12 - value_combination[1] }); //2 karty dodatkowe(gorsza para)

                unit_value = saved_unitval;
                int actval = 12;
                for (int index = 0; index < 52; index++) { //1 karta dodatkowa

                    auto [type, val] = convert_togrid(index);

                    if (val == 12 - value_combination[0] || val == 12 - value_combination[1]) continue; //je¿eli ma tak¹ sam¹ wartoœc co jedna z par wywala

                    new_unit.insert({ type, val });                   

                    Hierarhy5[new_unit] = unit_value;

                    if (actval != val) {
                        unit_value++;
                        actval = val;
                    }
   
                    update_amount(new_unit);
                    new_unit.erase({ type, val });
                }
                new_unit.erase({ sectype_combination[0], 12 - value_combination[1] });
                new_unit.erase({ sectype_combination[1], 12 - value_combination[1] });

            } while (next_combination(sectype_combination, 2, 4));

            new_unit.erase({ type_combination[0], 12 - value_combination[0] });
            new_unit.erase({ type_combination[1], 12 - value_combination[0] });

        } while (next_combination(type_combination, 2, 4));

    } while (next_combination(value_combination, 2, 13));

}

void licz_PARA(int& unit_value) {
    for (int i = 12; i >= 0; i--) {
        vector<int> type_combination = { 0,1 };
        int saved_val = unit_value;
        do {
            set<pair<int, int>> new_unit = { {type_combination[0], i}, {type_combination[1], i} }; // 2 karty bazowe(para)
            vector<int> value_combination2 = { 0, 1, 2 };
            unit_value = saved_val;
            int actval1 = 12, actval2 = 12, actval3 = 12;
            do {
                pair<int, int> k[3];
                int d = 0;

                for (int v = 0; v < 3; v++) k[v].second = 12 - value_combination2[v];

                if (k[0].second == i || k[1].second == i || k[2].second == i) continue;
                if (k[0].second == k[1].second || k[0].second == k[2].second || k[1].second == k[2].second) {
                    continue;

                }
                
                for (int t1 = 0; t1 < 4; t1++)
                    for (int t2 = 0; t2 < 4; t2++)
                        for (int t3 = 0; t3 < 4; t3++) {
                            k[0].first = t1;
                            k[1].first = t2;
                            k[2].first = t3;

                            for (int j = 0; j < 3; j++) new_unit.insert(k[j]);

                            Hierarhy5[new_unit] = unit_value;

                            update_amount(new_unit);

                            for (int j = 0; j < 3; j++) new_unit.erase(k[j]);
                        }

                unit_value++;
            } while (next_combination(value_combination2, 3, 13));

        } while (next_combination(type_combination, 2, 4));
    }
}

void licz_WYS_KARTA(int& unit_value) {
    vector<int> v_comb = { 0, 1, 2, 3, 4 };

    do {

        bool isStreet = true; // sprawdzamy czy formacja daje strita
        for (int i = 0; i < 4; i++) if (v_comb[i] != v_comb[i + 1] - 1) isStreet = false;
        if (isStreet) {
            continue;
        }
            
        isStreet = true; // sprawdzamy czy formacja daje strita z asem na pocz¹tku (12, 0, 1, 2, 3) > (0, 12, 11, 10, 9)
        if (v_comb[0] != 0) isStreet = false;
        for (int i = 1; i <= 4; i++) if (v_comb[i] != 8 + i) isStreet = false;
        if (isStreet) {
            continue;
        } 

        unit_value++;

        for (int t1 = 0; t1 <= 3; t1++)
            for (int t2 = 0; t2 <= 3; t2++)
                for (int t3 = 0; t3 <= 3; t3++)
                    for (int t4 = 0; t4 <= 3; t4++)
                        for (int t5 = 0; t5 <= 3; t5++) {

                            bool isColor = true; // czy formacja daje kolor
                            if (t1 == t2 &&
                                t1 == t3 &&
                                t1 == t4 &&
                                t1 == t5) continue;

                            set<pair<int, int>> new_unit = {
                                {t1, 12 - v_comb[0]},
                                {t2, 12 - v_comb[1]},
                                {t3, 12 - v_comb[2]},
                                {t4, 12 - v_comb[3]},
                                {t5, 12 - v_comb[4]} };

                            Hierarhy5[new_unit] = unit_value;
                            update_amount(new_unit);
                        }

    } while (next_combination(v_comb, 5, 13));
}

json mapToJson(const map<set<pair<int, int>>, int>& hierarchy) {
    json j;
    for (const auto& entry : hierarchy) {
        //json pairs_json = json::array();
        string formation;
        for (const auto& p : entry.first) {
            //pairs_json.push_back({p.first, p.second});

            if (p.first == 0) formation += "h";
            else if (p.first == 1) formation += "d";
            else if (p.first == 2) formation += "c";
            else if (p.first == 3) formation += "s";

            formation += to_string(p.second + 2);
        }
        
        j[formation] = entry.second;
    }
    return j;
}

void saveJsonToFile(const json& j, const string& filename) {
    std::ofstream outFile;
    outFile.open(filename);

    if (!outFile.is_open()) {
        cerr << "Unable to open file for writing: " << filename << endl;
        return;
    }
    outFile << j.dump(0); // 4 to liczba spacji do wciêcia
    outFile.close();
}

int FindBestValue(set<pair<int, int>> formation, pair<int, int> p1, pair<int, int> p2) {

    vector<pair<int, int>> vec_for(formation.begin(), formation.end());
    int BestVal = Hierarhy5[formation];

    for (int i = 0; i < 5; i++)
    {
        vector<pair<int, int>> actFormation = vec_for;
        actFormation[i] = p1;
        sort(actFormation.begin(), actFormation.end());
        set<pair<int, int>> f2(actFormation.begin(), actFormation.end());
      
        int actVal = Hierarhy5[f2];

        if (actVal < BestVal) BestVal = actVal;
    }

    for (int i = 0; i < 5; i++)
    {
        vector<pair<int, int>> actFormation = vec_for;
        actFormation[i] = p2;
        sort(actFormation.begin(), actFormation.end());
        set<pair<int, int>> f2(actFormation.begin(), actFormation.end());

        int actVal = Hierarhy5[f2];

        if (actVal < BestVal) BestVal = actVal;
    }

    return BestVal;
}

bool checkHand(set<pair<int, int>> formation, pair<int, int> p1, pair<int, int> p2) {

    bool result = true;

    if (formation.count(p1) != 0 || formation.count(p2) != 0) result = false;
    
    return result;
}

int main()
{   
    
    //kier, karo, trefl, pick

    int unit_value = 1;

    //POKER
    
    licz_POKER(unit_value); //DZIA£A


    //KARETA
    
    licz_KARETA(unit_value); //DZIA£A


    //FULL-HOUSE

    licz_FULLHOUSE(unit_value); //DZIA£A


    //KOLOR
    
    licz_KOLOR(unit_value); //DZIA£A


    //STRIT

    licz_STRIT(unit_value); //DZIA£A

    
    // TROJKA

    licz_TROJKA(unit_value); //DZIA£A


    //DWIE PARY
    
    licz_2PARY(unit_value); //DZIA£A


    //JEDNA PARA
    
    licz_PARA(unit_value); //DZIA£A


    //WYSOKA KARTA

    licz_WYS_KARTA(unit_value);

    /*for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 4; j++) {
            cout << amount[j][i] << " ";
        }
        cout << "\n";
    }*/

    /*for (auto& formation : hierarhy) {
        for (auto& karta : formation.first) {
            cout << karta.first << "," << karta.second << "; ";
        }
        cout << formation.second << "\n";
    }*/

    int counet = 0;

    /*for (int i = 0; i < 51; i++) {
        pair<int, int> k1 = convert_togrid(i);

        for (int j = i + 1; j < 52; j++) {
            pair<int, int> k2 = convert_togrid(j);

            for (auto[formation, value] : Hierarhy5) {
                

                if (formation.count(k1) != 0 || formation.count(k2) != 0) continue;

                set<pair<int, int>> formation7 = formation;
                formation7.insert(k1);
                formation7.insert(k2);

                if (Hierarhy7[formation7] != 0) continue;

                Hierarhy7[formation7] = FindBestValue(formation, k1, k2);

                cout << counet++ << "\n";
            }
        }
    }*/

    //json j = mapToJson(Hierarhy5);
    //saveJsonToFile(j, "full_hierarhy5.json");

    json j = mapToJson(Hierarhy5);
    saveJsonToFile(j, "C:\\Users\\pc\\source\\repos\\PokerCalculatorWPF\\bin\\Debug\\full_hierarhy5.json");

    return 0;
}
