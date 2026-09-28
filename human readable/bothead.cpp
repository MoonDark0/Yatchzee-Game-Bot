#include<bits/stdc++.h>
using namespace std;

struct Transition{
    int destination;
    double chance;
};

map<std::vector<int>,int> CSpace;
const vector<vector<int>> Points ;
const vector<Transition> Adj_Transition_Matrix[252][32] ;
const double ExProbability[(1<<15)][64] ;



void GenConfiguration(){
    
    int am=0;
    for(int i=0;i<7776;i++){
        vector<int> Temp;
        int tv=i;
        for(int j=0;j<5;j++){
            Temp.push_back(tv%6+1);
            tv/=6;
        }
        sort(Temp.begin(),Temp.end());
        if(0==CSpace.count(Temp)){
            CSpace[Temp]=am;
            am++;
        }
    }
}


int play(vector<int> Config,int round,int mask,int bonusAM){
    double DP[3][252];
    int Action[3][252];
    for(int i=0;i<252;i++){
        pair<double,int> maxi={0,0};
        for(int k=0;k<15;k++){

            if((mask>>k)%2==0){
     
                int deltaBonus=0;
                if(k<6){
                    deltaBonus=Points[i][k];
                } 
         
                double value=ExProbability[mask | 1<<k][min(bonusAM+deltaBonus,63)]+Points[i][k];
           
                if(bonusAM<63 && (bonusAM+deltaBonus)>62){
                    value+=50.;
                }
                maxi=max(maxi,{value,k+1});
                }
            }
        DP[0][i]=maxi.first;
        Action[0][i]=maxi.second;
    }

    for(int roll=1;roll<3;roll++){
        for(int state=0;state<252;state++){
            pair<double,int> BestExpectedValue={0,0};
                for(int Rollmask=0;Rollmask<32;Rollmask++){
                    double CurrentExpectedValue=0;
                        for (auto [next, probability] : Adj_Transition_Matrix[state][Rollmask]) {
                            CurrentExpectedValue+=probability*DP[roll-1][next];
                        }
                    BestExpectedValue=max(BestExpectedValue,{CurrentExpectedValue,Rollmask});
                }
            DP[roll][state]=BestExpectedValue.first;
            Action[roll][state]=-BestExpectedValue.second;
        }

    }
    sort(Config.begin(),Config.end());
    return Action[round][CSpace[Config]];

} 

int main()
{


    GenConfiguration();

    cout<<"BITMASK MEANING:"<<endl;
    cout<<"0-5 = 1 to 6"<<endl;
    cout<<"6,7,8 = pair, two pair, fullhouse"<<endl;
    cout<<"9,10,11 = three to yatch"<<endl;
    cout<<"12,13 = stairs , 14 = chance"<<endl;
    cout<<"0 is free, 1 is locked, dice are sorted"<<endl;
    cout<<"Input is all dice, then bitmask, remThrows, bonus"<<endl;

    cout<<"READY TO PLAY"<<endl;

    while (true) {
    int a, b, c, d, e;
    int bitmask, round, bonus;

    cerr << "WAITING FOR INPUT\n";

    if (!(cin >> a >> b >> c >> d >> e
              >> bitmask >> round >> bonus)) {
        cerr << "INPUT FAILED\n";
        break;
    }

    cerr << "GOT INPUT\n";

    int result = play({a,b,c,d,e}, round, bitmask, bonus);

    cerr << "PLAY FINISHED\n";

    cout << result << endl;
    }
    
    return 0;
}