#ifndef VACCINEROUTER_H
#define VACCINEROUTER_H

#include "VaccineRunner.h"
#include "BinaryHeap.h"
#include "StackAr.h"


class EdgePtr{
  public:
  EdgePtr();
  EdgePtr(Edge edge, int flow, int source, bool status);
  int id;
  int origin;
  //int edgeflow;
  int curflow;
  bool forward;
  bool operator<(const EdgePtr& rhs) const;
  
};

class City{
  public:
  int id;
  int pop; //why do we need this
  Edge edges[10];
  Edge backflows[1000];
  int adjCount;
  int backFlowNum;
  int prev;
  bool backFlowUsed;
  bool sink;
  bool known;
  City(CityInfo city);
  City();
};

class VaccineRouter
{
public:
  VaccineRouter(CityInfo cities[], int numCities);
  int setFlow(int numAirports, int sinks[], int numSinks);
  bool becomingKnown(EdgePtr edge);

  City graph[5012];
  BinaryHeap<EdgePtr> heap;
  StackAr<int> stack;
  int endCity;
  int currentFlow;
  int totalFlow;

private:

};

#endif /* VACCINEROUTER_H */

