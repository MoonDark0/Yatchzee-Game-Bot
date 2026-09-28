#include<bits/stdc++.h>
using namespace std;

#define endl "\n"

struct Transition{
    int destination;
    double chance;
};

map<std::vector<int>,int> CSpace;
vector<vector<int>> ICSpace;
vector<vector<int>> Points;
vector<vector<vector<int>>> Transition_Matrix;
vector<Transition> Adj_Transition_Matrix[252][32];

double ExProbability[(1<<15)][64];
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
            ICSpace.push_back(Temp);
        }
    }
}

//ONE - 0
//TWO
//THREE
//FOUR
//FIVE
//SIX - 5

//BONUS

//Pair - 6
//Two Pair - 7
//Full House -8

//Three - 9
//Four - 10
// Yatch - 11 




//S-Stair -12
//L-Stair -13

//Sum -14

vector<int> GenPoints(vector<int> Configuration){

    vector<int> PointsConfig(15);
    vector<int> Am(7);
    int sum=0;
    for(int i=0;i<Configuration.size();i++){
        sum+=Configuration[i];
        Am[Configuration[i]]++;
    }
    PointsConfig[14]=sum;
    for(int i=1;i<7;i++){
        int value=i*Am[i];
        PointsConfig[i-1]=value;

        if(Am[i]>1){
            PointsConfig[6]=2*i;
            PointsConfig[7]+=2*i;
        }
        if(Am[i]>2){
            PointsConfig[9]=3*i;
        }
        if(Am[i]>3){
            PointsConfig[10]=4*i;
        }
        if(Am[i]>4){
            PointsConfig[11]=50;
        }
    }

    if(PointsConfig[9]!=0 && (PointsConfig[7]-PointsConfig[9]/3*2)>0){
        PointsConfig[8]=PointsConfig[7]+PointsConfig[9]/3;
    }

    if(PointsConfig[6]==PointsConfig[7]){
        PointsConfig[7]=0;
    }
    
    int stairLen=0;
    for(int i=1;i<7;i++){
        if(Am[i]==0){
            stairLen=0;
        }
        else{
            stairLen++;
        }
        if(stairLen==4){
            PointsConfig[12]=15;
        }
        if(stairLen==5){
            PointsConfig[13]=20;
        }

    }
    return PointsConfig;

}

// 1 == Fixed 0 == Mobile

vector<vector<int>> GenTransitionMatrix(vector<int> Configuration){
    vector<vector<int>> Result(32,vector<int>(252));

    for(int mask=0;mask<32;mask++){
        for(int i=0;i<7776;i++){
        vector<int> Temp;
        int tv=i;
        for(int j=0;j<5;j++){
            if((mask>>j)%2){
                Temp.push_back(Configuration[j]);
            }
            else{
                Temp.push_back(tv%6+1);
            }
            tv/=6;
        }
        sort(Temp.begin(),Temp.end());
        Result[mask][CSpace[Temp]]++;
        }
    }

    return Result;


}

int main()
{
    cout<<fixed<<setprecision(8);
    //Gen Configuration Space
    GenConfiguration();
    
    //Build Scoring Matrix
    for(int i=0;i<ICSpace.size();i++){
        Points.push_back(GenPoints(ICSpace[i]));
    }

    // PRINT SCORING MATRIX

    for(int i=0;i<ICSpace.size();i++){
        for(int j=0;j<15;j++){
            cout<<Points[i][j]<<" ";
        }
        cout<<endl;
    }

    //Gen Transition Matrix
    for(int i=0;i<ICSpace.size();i++){
        Transition_Matrix.push_back(GenTransitionMatrix(ICSpace[i]));
    }

    for(int i=0;i<ICSpace.size();i++){
        for(int j=0;j<Transition_Matrix[i].size();j++){
            for(int k=0;k<Transition_Matrix[i][j].size();k++){
                if(Transition_Matrix[i][j][k]!=0){
                    Adj_Transition_Matrix[i][j].push_back({k,Transition_Matrix[i][j][k]/7776.});
                }
            }
        }
    }

    for(int i=0;i<ICSpace.size();i++){
        for(int j=0;j<32;j++){
            cout<<Adj_Transition_Matrix[i][j].size()<<" ";
            for(int k=0;k<Adj_Transition_Matrix[i][j].size();k++){
                cout<<Adj_Transition_Matrix[i][j][k].destination<<" "<<Adj_Transition_Matrix[i][j][k].chance<<" ";
            }
            cout<<endl;
        }
    }

    


    //Calculate Yatch PROBABILITY
    /*


        for(int rolls=1;rolls<20;rolls++){
            vector<vector<double>> DP(rolls,vector<double>(252));

            for(int i=0;i<252;i++){
                int maxi=0;
                for(int k=0;k<15;k++){
                    maxi=max(maxi,Points[i][k]);
                }
                DP[0][i]=maxi;
            }

            for(int roll=1;roll<rolls;roll++){

                for(int state=0;state<252;state++){
                    double BestExpectedValue=0;
                    for(int mask=0;mask<32;mask++){
                        double CurrentExpectedValue=0;
                        for(int i=0;i<252;i++){
                            CurrentExpectedValue+=Adj_Transition_Matrix[state][mask][i]*DP[roll-1][i];
                        }
                        BestExpectedValue=max(BestExpectedValue,CurrentExpectedValue);
                    }
                    DP[roll][state]=BestExpectedValue;
                }

            }

            double totalEV=0;
            for(int i=0;i<252;i++){
                totalEV+=Adj_Transition_Matrix[0][0][i]*DP[rolls-1][i];
            }
            cout<<"YATCHPROB: "<<totalEV<<"with n: "<<rolls<<endl;;
        }
    */



    
    for(int mask=(1<<15)-2;mask>-1;mask--){

        int maxsum=0;

        for(int i=0;i<6;i++){
            if((mask>>i)%2==1){
                maxsum+=(i+1)*5;
            }
        }

        #pragma omp parallel for
        for(int bonusAM=0;bonusAM<min(maxsum+1,64);bonusAM++){

            double DP[3][252];

            for(int i=0;i<252;i++){
                double maxi=0;
                for(int k=0;k<15;k++){
                    //Check if empty
                    if((mask>>k)%2==0){
                        //Check if bons applies
                        int deltaBonus=0;
                        if(k<6){
                            deltaBonus=Points[i][k];
                        } 

                        //Get value
                        double value=ExProbability[mask | 1<<k][min(bonusAM+deltaBonus,63)]+Points[i][k];

                        //Check if elegible for bonus
                        if(bonusAM<63 && (bonusAM+deltaBonus)>62){
                            value+=50.;
                        }

                        maxi=max(maxi,value);
                    }
                }
                DP[0][i]=maxi;
            }

            //Base DP Transition
            for(int roll=1;roll<3;roll++){

                for(int state=0;state<252;state++){
                    double BestExpectedValue=0;
                    for(int Rollmask=0;Rollmask<32;Rollmask++){
                        double CurrentExpectedValue=0;
                        for (auto [next, probability] : Adj_Transition_Matrix[state][Rollmask]) {
                            CurrentExpectedValue+=probability*DP[roll-1][next];
                        }
                        BestExpectedValue=max(BestExpectedValue,CurrentExpectedValue);
                    }
                    DP[roll][state]=BestExpectedValue;
                }

            }

           //Uniform DP
            double totalEV=0;
            for(int i=0;i<Adj_Transition_Matrix[0][0].size();i++){
                totalEV+=Adj_Transition_Matrix[0][0][i].chance*DP[3-1][Adj_Transition_Matrix[0][0][i].destination];
            }
            ExProbability[mask][bonusAM]=totalEV;



        }

        
    
    }

    for(int mask=0;mask<(1<<15);mask++){
        
            for(int bonusAM=0;bonusAM<64;bonusAM++){
                cout<<ExProbability[mask][bonusAM]<<" ";
            }
            cout<<endl;
        }


    return 0;
}