
#include "VaccineRouter.h"
#include <cstring>
#include <iostream>
using namespace std;

EdgePtr::EdgePtr(Edge edge, int flow, int source, bool status){
  origin=source;
  id = edge.ID;
  //edgeflow = edge.flow;
  curflow = flow;
  forward=status;
}

EdgePtr::EdgePtr(){}

bool EdgePtr::operator<(const EdgePtr& rhs) const{
    return(curflow > rhs.curflow);
  }

City::City(CityInfo city){
  id = city.ID;
  pop = city.population;
  //memcpy(edges, city.edges, sizeof(city.edges));
  adjCount = city.adjCount;
  //Edge edges[adjCount];
  for(int i = 0; i < adjCount; i++){
    edges[i] = city.edges[i];
  }
  //Edge backflows[200];
  for(int i = 0; i < 1000; i++){
    backflows[i].flow = 0;
    backflows[i].ID = -1;
  }
  backFlowNum=0;
  backFlowUsed=false;
  prev = -1;
  sink = city.sink;
  known = false;
  //cout<<"Cityinfo id: "<<city.ID<<endl;
  //cout<<"adjCount: "<<adjCount<<endl;
}

City::City(){}

VaccineRouter::VaccineRouter(CityInfo cities[], int numCities) 
{
  //cout<<"numcities: "<<numCities<<endl;
  currentFlow = 0;
  totalFlow = 0;
  endCity=-1;
  stack = StackAr<int>(5020);
  //City graph[numCities];

  for(int i = 0; i<numCities; i++){
    //cout<<"in loop"<<endl;
    graph[i] = City(cities[i]);
    //cout<<graph[i]->id<<endl;
  }
  heap = BinaryHeap<EdgePtr>(5020);
  //cout<<"done copying"<<endl;
  
} // VaccineRouter()

bool VaccineRouter::becomingKnown(EdgePtr edge){
  int min = edge.curflow;
  //cout<<"Edge ID: " << edge.id <<endl;
  //cout<< "Initial Min:" << min << endl;
  /*if(edge.curflow==0){
	  cout<<"breakpoint"<<endl;
  }*/
  if(graph[edge.id].known == false && min>0){
    if(edge.forward==false){
	    graph[edge.id].backFlowUsed=true;
    }
    graph[edge.id].prev=edge.origin;
    if(graph[edge.id].sink==true && graph[edge.id].pop!=0){
        if(min>=graph[edge.id].pop){
            min=graph[edge.id].pop;
        }
        //cout<<"min: " << min<< " ID: " << edge.id << " Origin: " << edge.origin<<endl;
        /*if(totalFlow==16457672){
		cout<< "Looping" <<endl;
	}*/
	currentFlow=min;
        totalFlow+=min;
        endCity=edge.id;
        graph[edge.id].pop-=min;
        //cout<<"Sink Check" << endl;
        return true;
    }
    for(int i = 0; i<graph[edge.id].adjCount;i++){
      //cout<< "Edge pointing to: " << graph[edge.id].edges[i].ID<<endl;
      if(!graph[graph[edge.id].edges[i].ID].known && graph[edge.id].edges[i].flow!=0){
        if(graph[edge.id].edges[i].flow <= min){
	       	min = graph[edge.id].edges[i].flow;
	}
	if(min-graph[edge.id].edges[i].flow<=0){
        	heap.insert(EdgePtr(graph[edge.id].edges[i], min, edge.id, true));
	}
	min=edge.curflow;
      }
    }
    for(int i=0; i<graph[edge.id].backFlowNum; i++){
      if(!graph[graph[edge.id].backflows[i].ID].known && graph[edge.id].backflows[i].flow>0){
        if(graph[edge.id].backflows[i].flow <= min){
		min = graph[edge.id].backflows[i].flow;
	}
	if(min-graph[edge.id].backflows[i].flow<=0){
		heap.insert(EdgePtr(graph[edge.id].backflows[i], min, edge.id, false));
	}
	min=edge.curflow;
      }
    }
    //cout << "Becme known" <<endl;
    graph[edge.id].known=true;
    stack.push(edge.id);
  }
  return false;
}

int VaccineRouter::setFlow(int numAirports, int sinks[], int numSinks)
{
  //cout<<"Starting find flow" <<endl;
  for(int i = 0; i < numSinks; i++){
    graph[sinks[i]].sink = true;
  }
  bool check=false; //fail to reach a sink
  bool check2=false; //find the sink on some try
  bool check3=true;
  int stackID = 0;
  int stackID2 = 0;
  int previous = -1;
  int current = -1;

  /*EdgePtr* newEdge = new EdgePtr(graph[2]->edges[2], 100);
  EdgePtr ptr = *newEdge;
  heap.insert(ptr);*/
  while(check==false){
    currentFlow=0;
    for(int i = 0; i<2; i++){
      for(int j = 0; j< graph[i].adjCount;j++){
	  //cout<<"In loop"<<endl;
         if(graph[i].edges[j].flow!=0){
	      heap.insert(EdgePtr(graph[i].edges[j], graph[i].edges[j].flow, i, true));
	 }
      }
      graph[i].known=true;
      stack.push(i);
    }
    while(check2==false){
     if(heap.isEmpty()==false){
       check2=becomingKnown(heap.findMin());
       heap.deleteMin();
     }
     else{
       //cout<<"heap not empty"<<endl;
       check=true;
       check2=true;
     }
    }
    current=endCity;
    //cout<<"Current Flow: "<<currentFlow<<endl;
    while(current!=0 && current!=1 && check==false){
      check3=true;
      previous=graph[current].prev;
      for(int i=0; i<graph[current].backFlowNum; i++){
	      if(graph[current].backflows[i].ID==previous){
		      check3=false;
	      }
      }
      if(check3 && previous!=0 && previous!=1){
      	graph[current].backflows[graph[current].backFlowNum].ID=previous;
      	graph[current].backFlowNum++;
      }
      /*for(int i=0; i<graph[previous].backFlowNum; i++){
	      if(graph[previous].backflows[i].ID==current){
		      backFlowUsed=true;
	      }
      }*/
      /*if(graph[current].backFlowNum>100){
	      cout<<graph[current].backFlowNum<<endl;
      }*/
      //cout<< "Current: " << current << " Previous: " << previous<<endl;
      /*if(previous==-1){
	      break;
      }*/
      //graph[previous]->next=graph[current].id;
      if(graph[current].backFlowUsed==false){
      	for(int i=0; i<graph[current].backFlowNum; i++){
      	  if(graph[current].backflows[i].ID==previous){
      	   graph[current].backflows[i].flow+=currentFlow;
	   break;
      	  }
      	}
      	for(int i=0; i<graph[previous].adjCount; i++){
		 if(graph[previous].edges[i].ID==current){
	      	      graph[previous].edges[i].flow-=currentFlow;
		      break;
		}
	      }
	}
	else{
		for(int i=0; i<graph[previous].backFlowNum; i++){
			if(graph[previous].backflows[i].ID==current){
				graph[previous].backflows[i].flow-=currentFlow;
				break;
			}
		}
	for(int i=0; i<graph[current].adjCount; i++){
		if(graph[current].edges[i].ID==previous){
			graph[current].edges[i].flow+=currentFlow;
			break;
		}  
	 }
	}	
      current=previous;
    }
    //cout<<"exit iteration"<<endl;
    if(stack.isEmpty()==false){
      stackID2=stack.topAndPop();
    }
    while(stack.isEmpty()==false && check==false){
      stackID=stackID2;
      stackID2=stack.topAndPop();
      graph[stackID].known=false;
      graph[stackID].backFlowUsed=false;

      /*for(int i=0; i<graph[stackID2]->adjCount; i++){
          if(graph[stackID2]->edges[i].ID==stackID){
            graph[stackID2]->edges[i].flow-=currentFlow;
          }
          if(graph[stackID]->backflows[i].ID==stackID2){
            graph[stackID]->backflows[i].flow+=currentFlow;
          } 
        }
      }*/
    }
    check2=false;
    if(heap.isEmpty()==false){
     heap.makeEmpty();
    }
   //cout<< "Total Flow: " << totalFlow<<endl; 
  }
  //heap.insert(EdgePtr(graph[2]->edges[2], 100));
  
  /*for(int i = 0; i<14; i++){
    //cout<<"In setFlow: "<<graph[i]->id<<" "<<graph[i]->pop<<" "<<graph[i]->adjCount<<" "<<graph[i]->sink<<endl;
    cout<<"setflow edge: "<<graph[i]->edges[2].ID<<endl;
  }*/
  /*for(int i=0; i<5012; i++){
	  if(graph[i].sink==true){
		  cout<<"Ending Population: " <<graph[i].pop<<endl;
	  }
  }*/
  /*for(int i=0; i<5012; i++){
	  for(int j=0; j<graph[i].adjCount;j++){
		  if(graph[i].edges[j].flow<0){
			  cout<<"City: " <<i << " Negative Forward Flow: " << graph[i].edges[j].flow<<endl;
		  }
	  }
	  for(int j=0; j<graph[i].backFlowNum; j++){
		  if(graph[i].backflows[j].flow<0){
			   cout<<"City: " <<i << " Negative Backwards Flow: " << graph[i].backflows[j].flow<<endl;
		  }
	  }
  }*/
  return totalFlow;
}  // setFlow()
