/*CS880 Assignment 8
  File: Qlearning.h
  author: Tianyi Gu
  Q-learning algorithm for MDP without model
  A model-free approach
*/  
#ifndef Rlearning_H
#define Rlearning_H
 
#include <string>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
#include "gnuplot_i.hpp"
#include <fstream>
#include <string>
#include <sys/time.h>
#include <vector>
//#include <sstream>
//using namespace std;

#include <time.h>
//class StateNode
template<class ActionNode>
class StateNode{
public:
  // default constructor
StateNode(int _x, int _y)
    : x(_x), y(_y),reward(-0.1),actionNum(0),isterminal(false),actiontaken(-1){}  
  int x;
  int y;   
  float reward;
  ActionNode **actions;
  int actionNum;
  bool isterminal;
  int actiontaken;

  void clear(){
    if(actionNum>0){
      for(int i=0;i<actionNum;i++){
        actions[i]->clear();
        delete actions[i];
      }
      delete [] actions;
    }
  };
};
class ActionNode{
public:
  // default constructor
ActionNode(int _action, float *_transitionProbility,int _stateNum,StateNode<ActionNode> ** _states)
    :myaction(_action), states(_states), stateNum(_stateNum),transitionProbility(_transitionProbility),
      Qvalue(0),nsa(0){}
  int myaction;//action of state 0 up 1 down 2 letf 3 right
  StateNode<ActionNode> ** states;  // uctStateNodes
  int stateNum;
  float *transitionProbility;//transitionfunction   
  float Qvalue;//Q    
  float nsa;//visited count

  void clear(){
    if(stateNum>0){
      delete [] transitionProbility;
      delete [] states;
    }
  };
};

class Rlearning{   
private:
  StateNode<ActionNode> *** gridworld; // our world is a 2Darray of state node 
  std::string alg;
  int size;
  float gamma;
  float epsilon;
  int exps;
  int eps;
  float alpha;
  float lambda;
  int isfig;
  std::string timenow;
  std::string para;

  void myplot(std::string datafile,int coln,std::string pname,std::string* lname,
              std::string _xlabel,std::string _ylabel){
    Gnuplot::set_terminal_std("png");
  
    Gnuplot gp;    
    gp << "set xlabel \""+_xlabel+"\"";
    gp << "set ylabel \""+_ylabel+"\"";
    gp << "set output \"./fig/" +pname+".png\"";
    std::string pcmd = "plot [0:500][]";    
    for(int i=1;i<coln;i++){
      std::stringstream ss;
      ss<<(i+1);
      //std::to_string(i+1);
      pcmd+=" \""+datafile+"\" u 1:"+ss.str()+ " w l t \""+lname[i-1]+"\"";
      if((i+1)!=coln){
        pcmd+=",";
      }
    }
    gp<<pcmd;
  }
  //calc transiton probability
  float getT( StateNode<ActionNode>* statep,StateNode<ActionNode>* state, int a){
    float t=0;				
    //4 corner
    if((statep->x==0&&statep->y==0&&state->x==0&&state->y==0&&(a==1||a==3))//leftdown
       ||(statep->x==0&&statep->y==(size-1)&&state->x==0&&state->y==(size-1)&&(a==0||a==3))//leftup
       ||(statep->x==(size-1)&&statep->y==(size-1)&&state->x==(size-1)&&state->y==(size-1)&&(a==0||a==2))//leftup
       ||(statep->x==(size-1)&&statep->y==0&&state->x==(size-1)&&state->y==0&&(a==1||a==2))//rightdown			
       ){
      t=0.9;
      return t;
    }		
    if((statep->x==0&&statep->y==0&&state->x==0&&state->y==0)//leftdown corner
       ||(statep->x==0&&statep->y==(size-1)&&state->x==0&&state->y==(size-1))//leftup corner
       ||(statep->x==(size-1)&&statep->y==(size-1)&&state->x==(size-1)&&state->y==(size-1))//leftup corner
       ||(statep->x==(size-1)&&statep->y==0&&state->x==(size-1)&&state->y==0)//rightdown corner	
       ){
      t=0.1;
      return t;
    }			
    //4 edge except corner
    if((statep->x==state->x&&statep->y==0&&state->y==0&&a==1)//down edge
       ||(statep->x==state->x&&statep->y==(size-1)&&state->y==(size-1)&&a==0)//up edge
       ||(statep->y==state->y&&statep->x==(size-1)&&state->x==(size-1)&&a==2)//right edge
       ||(statep->y==state->y&&statep->x==0&&state->x==0&&a==3)//left edge
       ){
      t=0.85;
      return t;
    }
    else if((statep->x==state->x&&statep->y==0&&state->y==0)//down edge
            ||(statep->x==state->x&&statep->y==(size-1)&&state->y==(size-1))//up edge
            ||(statep->y==state->y&&statep->x==(size-1)&&state->x==(size-1))//right edge
            ||(statep->y==state->y&&statep->x==0&&state->x==0)//left edge
            ){
      t=0.05;
      return t;
    }
    //center grid			
    if(((statep->x-state->x)==1&&(statep->y-state->y)==0&&a==2)//right
       ||((statep->x-state->x)==-1&&(statep->y-state->y)==0&&a==3)//left
       ||((statep->x-state->x)==0&&(statep->y-state->y)==1&&a==0)//up
       ||((statep->x-state->x)==0&&(statep->y-state->y)==-1&&a==1)//down
       ){
      t=0.85;	
      return t;		
    }
    else if(((statep->x-state->x)==1&&(statep->y-state->y==0))//right
            ||((statep->x-state->x)==-1&&(statep->y-state->y)==0)//left
            ||((statep->x-state->x)==0&&(statep->y-state->y)==1)//up
            ||((statep->x-state->x)==0&&(statep->y-state->y)==-1)//down		
            ){
      t=0.05;
      return t;
    }				
    return t;
  }
 // generate actions for state
  void getActions( StateNode<ActionNode>* state){
    if(state->isterminal) return;
    state->actionNum=4;
    state->actions = new ActionNode *[4];
    if(!state->actions){
      std::cout<<"memory out!"<<"\n";
    }
    state->actions[0] = GenerateAction(state,0);
    state->actions[1] = GenerateAction(state,1);
    state->actions[2] = GenerateAction(state,2);
    state->actions[3] = GenerateAction(state,3);
  }
	
  // generate one action for state
  ActionNode* GenerateAction( StateNode<ActionNode>* state,int a){		
    ActionNode* _action;
    //4corner
    if(state->x==0&&state->y==0){			
      StateNode<ActionNode>** astate = new StateNode<ActionNode>*[3];
      float *_transitionProbility = new float[3];
      astate[0] = gridworld[0][0];
      astate[1] = gridworld[0][1];
      astate[2] = gridworld[1][0];
      
      for(int t=0;t<3;t++){
        _transitionProbility[t] = getT(astate[t],state,a);
      }
      _action = new  ActionNode(a,_transitionProbility,3,astate);
      return _action;
    }
    if(state->x==0&&state->y==size-1){			
     StateNode<ActionNode>** astate = new StateNode<ActionNode>*[3];
      float *_transitionProbility = new float[3];
      astate[0] = gridworld[0][size-1];
      astate[1] = gridworld[1][size-1];
      astate[2] = gridworld[0][size-2];
      
      for(int t=0;t<3;t++){
        _transitionProbility[t] = getT(astate[t],state,a);
      }
      _action = new  ActionNode(a,_transitionProbility,3,astate);
      return _action;
    }
    if(state->x==size-1&&state->y==0){			
     StateNode<ActionNode>** astate = new StateNode<ActionNode>*[3];
      float *_transitionProbility = new float[3];
      astate[0] = gridworld[size-1][0];
      astate[1] = gridworld[size-2][0];
      astate[2] = gridworld[size-1][1];
      for(int t=0;t<3;t++){
        _transitionProbility[t] = getT(astate[t],state,a);
      }
      _action = new  ActionNode(a,_transitionProbility,3,astate);
      return _action;
    }
    //4 edge
    if(state->x==0&&state->y!=0&&state->y!=size-1){	//left edge		
     StateNode<ActionNode>** astate = new StateNode<ActionNode>*[4];
      float *_transitionProbility = new float[4];
      astate[0] = gridworld[state->x][state->y];
      astate[1] = gridworld[state->x+1][state->y];
      astate[2] = gridworld[state->x][state->y-1];
      astate[3] = gridworld[state->x][state->y+1];
      for(int t=0;t<4;t++){
        _transitionProbility[t] = getT(astate[t],state,a);
      }
      _action = new  ActionNode(a,_transitionProbility,4,astate);
      return _action;
    }
    if(state->x==size-1&&state->y!=0&&state->y!=size-1){	//right edge		
     StateNode<ActionNode>** astate = new StateNode<ActionNode>*[4];
      float *_transitionProbility = new float[4];
      astate[0] = gridworld[state->x][state->y];
      astate[1] = gridworld[state->x-1][state->y];
      astate[2] = gridworld[state->x][state->y-1];
      astate[3] = gridworld[state->x][state->y+1];			
      for(int t=0;t<4;t++){
        _transitionProbility[t] = getT(astate[t],state,a);
      }
      _action = new  ActionNode(a,_transitionProbility,4,astate);
      return _action;
    }
    if(state->x!=0&&state->x!=size-1&&state->y==0){	//bottom edge		
     StateNode<ActionNode>** astate = new StateNode<ActionNode>*[4];
      float *_transitionProbility = new float[4];
      astate[0] = gridworld[state->x][state->y];
      astate[1] = gridworld[state->x-1][state->y];
      astate[2] = gridworld[state->x+1][state->y];
      astate[3] = gridworld[state->x][state->y+1];
      for(int t=0;t<4;t++){
        _transitionProbility[t] = getT(astate[t],state,a);
      }
      _action = new  ActionNode(a,_transitionProbility,4,astate);
      return _action;
    }
    if(state->x!=0&&state->x!=size-1&&state->y==size-1){	//top edge		
     StateNode<ActionNode>** astate = new StateNode<ActionNode>*[4];
      float *_transitionProbility = new float[4];
      astate[0] = gridworld[state->x][state->y];
      astate[1] = gridworld[state->x-1][state->y];
      astate[2] = gridworld[state->x+1][state->y];
      astate[3] = gridworld[state->x][state->y-1];				
      for(int t=0;t<4;t++){
        _transitionProbility[t] = getT(astate[t],state,a);
      }
      _action = new  ActionNode(a,_transitionProbility,4,astate);
      return _action;
    }
		
    //center grid
   StateNode<ActionNode>** astate = new StateNode<ActionNode>*[4];
    float *_transitionProbility = new float[4];
    astate[0] = gridworld[state->x-1][state->y];
    astate[1] = gridworld[state->x+1][state->y];
    astate[2] = gridworld[state->x][state->y+1];
    astate[3] = gridworld[state->x][state->y-1];
    for(int t=0;t<4;t++){
      _transitionProbility[t] = getT(astate[t],state,a);
    }
    _action = new  ActionNode(a,_transitionProbility,4,astate);
    return _action;
  }

  //GetActionofMaxQ
  int GetActionofMaxQ(StateNode<ActionNode> * _state,float & _qvalue){
    if(_state->isterminal){
      _qvalue=0;
      return -1;
    }
    float tempQ=-999;
    bool isfirst=true;
    int a=-1;
    int maxCount=0;
    std::vector<int> acs;
    for(int i=3;i>=0;i--){
      if(isfirst||tempQ<_state->actions[i]->Qvalue){
        tempQ=_state->actions[i]->Qvalue;
        acs.clear();
        acs.push_back(i);
        maxCount=1;
        isfirst=false;
      }
      else if(tempQ==_state->actions[i]->Qvalue){
        maxCount++;
        acs.push_back(i);
      }     
    }
    _qvalue=tempQ;
    a=acs[0];
    if(maxCount>1){
      struct timeval tv;    
      gettimeofday(&tv,NULL);    
      int seed= (tv.tv_sec * 1000 + tv.tv_usec / 1000)%91;   
      srand(seed);
      int aindex=rand()%maxCount;
      a = acs[aindex];
    }
    return a;
  }
  //ChooseAction Function
  int ChooseAction(StateNode<ActionNode> * _state){
    int a=-1;
    struct timeval tv;    
    gettimeofday(&tv,NULL);    
    int seed= (tv.tv_sec * 1000 + tv.tv_usec / 1000)%91; 
    srand(seed);
    //e-greedy
    float curdie=(float)(rand()%100)/100.0;
    if(curdie<epsilon){       
      gettimeofday(&tv,NULL);    
      seed= (tv.tv_sec * 1000 + tv.tv_usec / 1000)%173;   
      srand(seed);
      a=rand()%4;
      return a;
    }
    //max_A
    float tempQ=-999;
    a = GetActionofMaxQ(_state,tempQ);   	
    return a;
  }
  
  //G(s,a)
  void GnerateStateByTran(ActionNode *_action, StateNode<ActionNode>** s){
    struct timeval tv;    
    gettimeofday(&tv,NULL);    
    int seed= (tv.tv_sec * 1000 + tv.tv_usec / 1000)%91;   
    srand(seed);
    float curdie=(float)(rand()%100)/100.0;		
    //uctStateNode<ActionNode> * nextstate;
    //int cc=0;
    float acumulateT=0;
    float preacumulateT=0;
    for(int i=0;i<_action->stateNum;i++){
      preacumulateT = acumulateT;
      acumulateT += _action->transitionProbility[i];
      if(curdie<=acumulateT&&curdie>=preacumulateT){
        //nextstate = _action->states[i];		
        *s=	_action->states[i];	
        //cc++;
      }
    }		
    //return nextstate;
    return;
  }
    
  //Qlearning Algorithm
  void updateQ(){
    std::ofstream myfile;
    std::ofstream myfile2;    
    std::string _fname1= "./data/ql_mq_"+para;        
    std::string _fname2= "./data/ql_at_"+para;       
    const char* fname1 = _fname1.c_str();
    const char* fname2 = _fname2.c_str();
    myfile.open (fname1);
    myfile2.open (fname2);
    /* while(e<=eps){ */
    /*   e++; */
    float avgMaxQ[eps];
    float avgTimeStep[eps];
    float avgMaxQ_last[4];
    for(int i=0;i<eps;i++){
      avgMaxQ[i]=0;
      avgTimeStep[i]=0;
    }
    int exp=exps;
    while(exp>=0){
      exp--;
      //int cure = e;
      //reset qvalue
      for(int i=0;i<size;i++){     
        for(int j=0;j<size;j++){
          if(i==size-1&&j==size-1) continue;
          gridworld[i][j]->actions[0]->Qvalue=0;
          gridworld[i][j]->actions[1]->Qvalue=0;
          gridworld[i][j]->actions[2]->Qvalue=0;
          gridworld[i][j]->actions[3]->Qvalue=0;
        }
      }
      int curTimeStep=0;
      //for all apsode
      int e=0;
      while(e<eps){
        e++;
        //set start state
        StateNode<ActionNode>* currentState=gridworld[0][0];
        //learning loop
        curTimeStep=0;
        while(!currentState->isterminal){            
          int a = ChooseAction(currentState);
          StateNode<ActionNode>* sp = NULL;
          GnerateStateByTran(currentState->actions[a], &sp);
          float Qofsp;
          GetActionofMaxQ(sp,Qofsp);
          currentState->actions[a]->Qvalue=
              currentState->actions[a]->Qvalue+
              alpha*(sp->reward+gamma*Qofsp-
                     currentState->actions[a]->Qvalue);
          currentState = sp;
          curTimeStep++;          
          //std::cout<<currentState->x<<","<<currentState->y<<"\n";
          //std::cout<<curTimeStep<<"\n";
          //std::cout <<currentState->actions[a]->Qvalue<<"\n";
        }
        //The average amount of time steps necessary to reach the goal        
        avgTimeStep[e] = avgTimeStep[e]+((float)curTimeStep-avgTimeStep[e])/(float)(exps-exp);

        //The average amount of max qvalue of the start state
        float cmq = 0;
        GetActionofMaxQ(gridworld[0][0],cmq);
        avgMaxQ[e] = avgMaxQ[e]+(cmq-avgMaxQ[e])/(float)(exps-exp);
        if(e==eps-1){
          avgMaxQ_last[0] = avgMaxQ_last[0]+(gridworld[0][0]->actions[0]->Qvalue-
                                             avgMaxQ_last[0])/(float)(exps-exp);
          avgMaxQ_last[1] = avgMaxQ_last[1]+(gridworld[0][0]->actions[1]->Qvalue-
                                             avgMaxQ_last[1])/(float)(exps-exp);
          avgMaxQ_last[2] = avgMaxQ_last[2]+(gridworld[0][0]->actions[2]->Qvalue-
                                             avgMaxQ_last[2])/(float)(exps-exp);
          avgMaxQ_last[3] = avgMaxQ_last[3]+(gridworld[0][0]->actions[3]->Qvalue-
                                             avgMaxQ_last[3])/(float)(exps-exp);
        }
      }
      //std::cout<<exp<<"\n";
      //cout <<gridworld[0][0]->actions[0]->Qvalue<<endl;
      
      /* cout<<"best policy: exp"<<exp<<endl; */
      /* for(int j=size-1;j>=0;j--){      */
      /*   for(int i=0;i<size;i++){ */
      /*     float f; */
      /*     int a = GetActionofMaxQ(gridworld[i][j],f); */
      /*     cout<<setprecision(3)<<f<<"("<<a<<")"<<","; */
      /*   } */
      /*   cout<<endl; */
      /* } */
      /* } */
      //std::cout  <<"eps:"<<e<<"\n";
      //std::cout <<"avgQ"<<avgMaxQ<<"\n";
      //std::cout<<"avgT"<<avgTimeStep<<"\n";
    }
    std::cout  <<"====================================================\n";
    std::cout  <<"Average Q-value for start state over all "<<exps<<" expriments\n";
    std::cout  <<"------------------\n";
    std::cout  <<"|    up | "<<std::setprecision(3)<<avgMaxQ_last[0]<<"\n";
    std::cout  <<"|  down | "<<std::setprecision(3)<<avgMaxQ_last[1]<<"\n";
    std::cout  <<"| right | "<<std::setprecision(3)<<avgMaxQ_last[2]<<"\n";
    std::cout  <<"|  left | "<<std::setprecision(3)<<avgMaxQ_last[3]<<"\n";
    std::cout  <<"------------------\n";
    for(int i=0;i<eps;i++){
      myfile<<i<<" "<<avgMaxQ[i]<<"\n";
      myfile2<<i<<" "<<avgTimeStep[i]<<"\n";
    }
      
    myfile.close();
    myfile2.close();
    std::string lnamesT[1];
    lnamesT[0]="maxT";
    std::string lnamesQ[1];
    lnamesQ[0]="maxQ";
    if(isfig){
      myplot(fname1,2,"maxQ"+para+timenow,lnamesQ,"episode numbers","avg max Qvalue for start state");
      myplot(fname2,2,"maxT"+para+timenow,lnamesT,"episode numbers","avg Time Step");
    }
  }

  //Sarsa(lambda) Algorithm
  void updateSarsa(){
    std::ofstream myfile;
    std::ofstream myfile2;   
    std::string _fname1= "./data/sa_mq_"+para;
    std::string _fname2= "./data/sa_at_"+para;
    const char* fname1 = _fname1.c_str();
    const char* fname2 = _fname2.c_str();
    myfile.open (fname1);
    myfile2.open (fname2);
    float avgMaxQ[eps];
    float avgTimeStep[eps];
    float avgMaxQ_last[4];
    for(int i=0;i<eps;i++){
      avgMaxQ[i]=0;
      avgTimeStep[i]=0;
    }
    int exp=exps;
    while(exp>=0){
      exp--;
      int e = 0;
      //reset qvalue and N
      for(int i=0;i<size;i++){     
        for(int j=0;j<size;j++){
          if(i==size-1&&j==size-1) continue;
          for(int ac=0;ac<4;ac++){
            gridworld[i][j]->actions[ac]->Qvalue=0;            
            gridworld[i][j]->actions[ac]->nsa=0;
          }           
        }
      }       
      //learning loop
      while(e<eps){
        e++;
        //set start state
        StateNode<ActionNode>* currentState=gridworld[0][0];
        int currentaction=0;
        int curTimeStep=0;
        //rest N
        for(int i=0;i<size;i++){     
          for(int j=0;j<size;j++){
            if(i==size-1&&j==size-1) continue;
            for(int ac=0;ac<4;ac++){                          
              gridworld[i][j]->actions[ac]->nsa=0;
            }           
          }
        }     
        while(!currentState->isterminal){
          StateNode<ActionNode>* sp = NULL;
          GnerateStateByTran(currentState->actions[currentaction], &sp);
          int a=-1 ;
          float Qofsp=0;
          if(!sp->isterminal){
            a = ChooseAction(sp);
            Qofsp=sp->actions[a]->Qvalue;
          }
          currentState->actions[currentaction]->nsa=currentState->actions[currentaction]->nsa+1;
          float delta = sp->reward+
              gamma*Qofsp-currentState->actions[currentaction]->Qvalue;
          for(int i=0;i<size;i++){     
            for(int j=0;j<size;j++){
              if(i==size-1&&j==size-1) continue;
              for(int ac=0;ac<4;ac++){
                gridworld[i][j]->actions[ac]->Qvalue=gridworld[i][j]->actions[ac]->Qvalue+
                    alpha*delta*gridworld[i][j]->actions[ac]->nsa;
                gridworld[i][j]->actions[ac]->nsa=gamma*lambda*gridworld[i][j]->actions[ac]->nsa;
              }
            }
          }
          currentState=sp;
          currentaction=a;
          curTimeStep++;
          //std::cout<<currentState->x<<","<<currentState->y<<"\n";
          //std::cout <<currentState->actions[a]->Qvalue<<"\n";        
        }      
        
        /* cout<<"best policy: exp"<<exp<<endl; */
        /* for(int j=size-1;j>=0;j--){      */
        /*   for(int i=0;i<size;i++){ */
        /*     float f; */
        /*     int a = GetActionofMaxQ(gridworld[i][j],f); */
        /*     cout<<setprecision(3)<<f<<"("<<a<<")"<<","; */
        /*   } */
        /*   cout<<endl; */
        /* } */
        //The average amount of time steps necessary to reach the goal        
        avgTimeStep[e] = avgTimeStep[e]+((float)curTimeStep-avgTimeStep[e])/(float)(exps-exp);

        //The average amount of max qvalue of the start state
        float cmq = 0;
        GetActionofMaxQ(gridworld[0][0],cmq);
        avgMaxQ[e] = avgMaxQ[e]+(cmq-avgMaxQ[e])/(float)(exps-exp);
        if(e==eps-1){
          avgMaxQ_last[0] = avgMaxQ_last[0]+(gridworld[0][0]->actions[0]->Qvalue-
                                             avgMaxQ_last[0])/(float)(exps-exp);
          avgMaxQ_last[1] = avgMaxQ_last[1]+(gridworld[0][0]->actions[1]->Qvalue-
                                             avgMaxQ_last[1])/(float)(exps-exp);
          avgMaxQ_last[2] = avgMaxQ_last[2]+(gridworld[0][0]->actions[2]->Qvalue-
                                             avgMaxQ_last[2])/(float)(exps-exp);
          avgMaxQ_last[3] = avgMaxQ_last[3]+(gridworld[0][0]->actions[3]->Qvalue-
                                             avgMaxQ_last[3])/(float)(exps-exp);
        }
      }
    }
    std::cout  <<"====================================================\n";
    std::cout  <<"Average Q-value for start state over all "<<exps<<" expriments\n";
    std::cout  <<"------------------\n";
    std::cout  <<"|    up | "<<std::setprecision(3)<<avgMaxQ_last[0]<<"\n";
    std::cout  <<"|  down | "<<std::setprecision(3)<<avgMaxQ_last[1]<<"\n";
    std::cout  <<"| right | "<<std::setprecision(3)<<avgMaxQ_last[2]<<"\n";
    std::cout  <<"|  left | "<<std::setprecision(3)<<avgMaxQ_last[3]<<"\n";
    std::cout  <<"------------------\n";
    for(int i=0;i<eps;i++){
      myfile<<i<<" "<<avgMaxQ[i]<<"\n";
      myfile2<<i<<" "<<avgTimeStep[i]<<"\n";
    }
      
    myfile.close();
    myfile2.close();
    std::string lnamesT[1];
    lnamesT[0]="maxT";
    std::string lnamesQ[1];
    lnamesQ[0]="maxQ";
    if(isfig){
      myplot(fname1,2,"SarsamaxQ"+para+"_"+timenow,lnamesQ,
             "episode numbers","avg max Qvalue for start state");
      myplot(fname2,2,"SarsamaxT"+para+"_"+timenow,lnamesT,"episode numbers","avg Time Step");
    }
    
  }
  
public:    
  // constructor 
  Rlearning(std::string _alg, int _size, float _gamma,int _exps
            ,int _eps, float _epsilon,float _alpha,float _lambda,int _fig){
    size = _size;		
    gamma= _gamma;
    exps = _exps;
    eps= _eps;    
    epsilon=_epsilon;
    alpha=_alpha;
    alg=_alg;
    lambda=_lambda;
    isfig=_fig;
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    std::stringstream ssh;
    std::stringstream ssm;
    std::stringstream sss;
    ssh<<now->tm_hour;
    ssm<<now->tm_min;
    sss<<now->tm_sec;
    timenow = ssh.str()+ssm.str()+sss.str();
    std::stringstream ssize;
    std::stringstream sepsilon;
    std::stringstream salpha;
    std::stringstream slambda;
    ssize<<size;
    sepsilon<<(epsilon*100);
    salpha<<(alpha*100);
    slambda<<(lambda*100);
    para = ssize.str()+"_"+sepsilon.str()+"_"+salpha.str()+"_"+slambda.str();
		
    gridworld = new StateNode<ActionNode> **[size];
    for(int i=0;i<size;i++){
      gridworld[i] = new StateNode<ActionNode> *[size];
      for(int j=0;j<size;j++){
        gridworld[i][j] = new StateNode<ActionNode>(i,j);	
      }
    }
    gridworld[size-1][size-1]->isterminal = true;
    gridworld[size-1][size-1]->reward=5;
    
    //getactions
    for(int i=0;i<size;i++){     
      for(int j=0;j<size;j++){
        getActions(gridworld[i][j]);
      }
    }
  };

  
  //run learning algorithms
  void run(){
    if(alg=="q"){
      updateQ();
    }
    else{
      updateSarsa();
    }
    return ;
  }
	 
  // destructor
  ~Rlearning(){
    for(int i=0;i<size;i++){			
      for(int j=0;j<size;j++){
        gridworld[i][j]->clear();
        delete gridworld[i][j];
      }
      delete [] gridworld[i];
    }
    delete [] gridworld;
  };
		 
		
		
		
	

};
	
#endif
