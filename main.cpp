#include <bits/stdc++.h>
#include "driver.h"
#include "race.h"
#include "raceresults.h"
using namespace std;
static bool cmp(RaceResult& A , RaceResult& B)
{
    return A.positionOrder< B.positionOrder;
}
static bool cmp1(pair<string,pair<double,double>>& d1 , pair<string,pair<double,double>>& d2){
    return d1.second.first>d2.second.first;
}
static bool cmp2(pair<string,double>& a, pair<string,double>& b) {
    return a.second > b.second;
}
int getBonus(int pos) {
    if (pos == 1) return 25;
    else if (pos == 2) return 18;
    else if (pos == 3) return 15;
    else if (pos == 4) return 12;
    else if (pos == 5) return 10;
    else if (pos == 6) return 8;
    else if (pos == 7) return 6;
    else if (pos == 8) return 4;
    else if (pos == 9) return 2;
    else if (pos == 10) return 1;
    return 0;
}
void updateElo(double &RA, double &RB, int posA, int posB, double K = 10.0) {
    double EA = 1.0 / (1.0 + pow(10.0, (RB - RA) / 400.0));
    double EB = 1.0 - EA;

    int diff = posB - posA;
    double weight = 0.5 + 0.05 * diff; 

    double SA = min(1.0, max(0.0, weight));
    double SB = 1.0 - SA;

    RA = RA + K * (SA - EA);
    RB = RB + K * (SB - EB);
}


int main()
{
    fstream fin;
    fin.open("dataset/drivers.csv");
    unordered_map<int, driver> drivers;
    //unordered_map<int,>
    unordered_map<int, race > races;
    string line;
    getline(fin, line);
    while(getline(fin, line))
    {
        stringstream ss(line);
        string field;
        vector<string>fields;
        while(getline(ss,field,','))
        {
            if(field.front()=='"' && field.back()=='"')
            {
                field=field.substr(1,field.length()-2);
            }
            fields.push_back(field);
        }
        driver d;
        d.id=stoi(fields[0]);
        d.name = fields[4]+" "+fields[5];
        d.rating = 1000;
        drivers[d.id]=d;
        d.peakrating=1000;
    }
    fin.close();
    fin.clear();
    fin.open("dataset/races.csv");
    getline(fin,line);
    while(getline(fin, line))
    {
        stringstream ss(line);
        string field;
        vector<string>fields;
        while(getline(ss,field,','))
        {
            if(field.front()=='"' && field.back()=='"')
            {
                field=field.substr(1,field.length()-2);
            }
            fields.push_back(field);
        }
        race r;
        r.id=stoi(fields[0]);
        r.year=stoi(fields[1]);
        r.round=stoi(fields[2]);
        r.circuitid=stoi(fields[3]);
        r.name=fields[4];
        r.date=fields[5];
        races[r.id]=r;
    }
    
     fin.close();
    fin.clear();
    fin.open("dataset/results.csv");
    getline(fin,line);
    unordered_map<int ,vector<RaceResult>> raceResults;
    while(getline(fin, line))
    {
        stringstream ss(line);
        string field;
        vector<string>fields;
        while(getline(ss,field,','))
        {
            if(field.front()=='"' && field.back()=='"')
            {
                field=field.substr(1,field.length()-2);
            }
            fields.push_back(field);
        }
        RaceResult r;
        r.raceid= stoi(fields[1]);
        r.driverId =stoi( fields[2]);
        r.positionOrder = stoi(fields[8]);
        r.statusId = stoi(fields[17]);
        raceResults[r.raceid].push_back(r);
    }
    vector<race> raceList;
    for (auto &kv : races) raceList.push_back(kv.second);
    sort(raceList.begin(), raceList.end(), [](const race &a, const race &b) {
    if (a.year != b.year) return a.year < b.year;
    return a.round < b.round;
});

    int currentyear = raceList[0].year;

    for (auto &rc : raceList) {
    auto &results = raceResults[rc.id];
    sort(results.begin(), results.end(), cmp);
    for (int i = 0; i < results.size(); i++) {
        for (int j = i+1; j < results.size(); j++) {
            double dynamicK = (results[i].positionOrder == 1 ? 20 : 10);
            updateElo(drivers[results[i].driverId].rating,
                      drivers[results[j].driverId].rating ,results[i].positionOrder,results[j].positionOrder,dynamicK);
        }
    }
   /* if(!raceResults.empty())
    {
        int winnerid = results[0].driverId;
        drivers[winnerid].rating+=5;
    } */
    for (int k = 0; k < results.size(); k++) {
        int pos = results[k].positionOrder;
        int dId = results[k].driverId;
        int bonus = getBonus(pos) / 5;
        drivers[dId].rating+=bonus;
        drivers[dId].peakrating = max(drivers[dId].peakrating, drivers[dId].rating);
    }
    if(rc.year!=currentyear)
    {
        unordered_set<int> activeDrivers;
        for (auto &race : raceList) {
            if (race.year == currentyear) {
                for (auto &rr : raceResults[race.id]) {
                    activeDrivers.insert(rr.driverId);
                }
            }
        }

        vector<pair<string,double>> yearlyBoard;
        for (int dId : activeDrivers) {
            yearlyBoard.push_back({drivers[dId].name, drivers[dId].rating});
        }
        sort(yearlyBoard.begin(), yearlyBoard.end(), cmp2);
        /*cout<<"Season  "<<currentyear<<" Ratings"<<endl;

        for (int i = 0; i < min(10, (int)yearlyBoard.size()); i++) {
            cout << i+1 << ". " << yearlyBoard[i].first
                 << " -> " << yearlyBoard[i].second << "\n";
        }*/

        for (auto &it : drivers) {
            if (!activeDrivers.count(it.first)) {
                it.second.rating -= 20;
                if (it.second.rating < 1000) it.second.rating = 1000; 
                it.second.peakrating = max(it.second.peakrating,it.second.rating);
            }
        }

        currentyear = rc.year;
    }

}

     vector<pair<string,pair<double,double>>>leaderboard;
    for (auto& it : drivers)
    {
        leaderboard.push_back({it.second.name,{it.second.rating,it.second.peakrating}});
    }
    sort(leaderboard.begin(),leaderboard.end(),cmp1);
   cout << " Top 20 Elo Ratings (1950-2024)\n";

    for(int i=0 ;i<20;i++)
    {
        cout<<leaderboard[i].first<<" -> "<<leaderboard[i].second.first<<" -> "<<leaderboard[i].second.second<<endl;
    }
   
    
    
    
    
}