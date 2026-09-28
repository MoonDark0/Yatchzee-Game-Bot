#include<bits/stdc++.h>
using namespace std;

int main(){

    string element;

    ifstream myfile;
    myfile.open ("table.txt");

    int app=0;
    while(cin>>element){
        cout<<element<<" ";
        if(element=="}")cout<<endl;
        if(element=="#include<bits/stdc++.h>"){cout<<endl;}
        if(app<3){
            if(element=="Points"){
                app++;
                cout<<"={";
                for(int i=0;i<252;i++){
                    if(i!=0)cout<<",";
                    cout<<"{";
                    for(int j=0;j<15;j++){
                        if(j!=0){
                            cout<<",";
                        }
                        double d;
                        myfile>>d;
                        cout<<d;
                    }
                    cout<<"}";
                }
                cout<<"};\n";
            }


            if(element=="Adj_Transition_Matrix[252][32]"){
                app++;
                
                cout<<"={";
                for(int i=0;i<252;i++){
                    if(i!=0)cout<<",";
                    cout<<"{";
                    for(int j=0;j<32;j++){
                        if(j!=0){
                            cout<<",";
                        }
                        int am;myfile>>am;
                        cout<<"vector<Transition>({";
                        for(int k=0;k<am;k++){
                            if(k!=0){
                                cout<<",";
                            }
                            int t; double d;
                            myfile>>t>>d;
                            cout<<"Transition({"<<t<<","<<d<<"})";
                        }
                        cout<<"})";
                        
                    }
                    cout<<"}";
                }
                cout<<"};\n";
                

            }


            if(element=="ExProbability[(1<<15)][64]"){
                app++;
                
                
                cout<<"={";
                for(int i=0;i<(1<<15);i++){
                    if(i!=0)cout<<",";
                    cout<<"{";
                    for(int j=0;j<64;j++){
                        if(j!=0){
                            cout<<",";
                        }
                        double d;
                        myfile>>d;
                        cout<<d;
                        
                    }
                    cout<<"}";
                }
                cout<<"};\n";
                

            }


        }
        

    }



}