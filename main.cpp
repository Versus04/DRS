#include <bits/stdc++.h>
#include "driver.h"
#include "race.h"
using namespace std;
void updateElo(double &RA, double &RB, double K = 20.0) {
    double EA = 1.0 / (1.0 + pow(10.0, (RB - RA) / 400.0));
    double EB = 1.0 - EA;

    double SA = 1.0; // A wins
    double SB = 0.0; // B loses

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
        d.rating = 1500;
        drivers[d.id]=d;
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
    for(auto& it: races)
    {
        cout<<it.second.id<<" -> "<<it.second.circuitid<<" -> "<<it.second.date<<" -> "<<it.second.name<<endl;
    }
    
}