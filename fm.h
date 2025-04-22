#ifndef FM_H
#define FM_H

#include "provided.h"
#include "bstset.h"
#include <unordered_map>
#include <vector>
using namespace std;
class FlightManager : public FlightManagerBase {
public:
    FlightManager() {}
    virtual ~FlightManager() {}
    virtual bool load_flight_data(string filename);
    vector<FlightSegment> find_flights(string source_airport, int start_time, int end_time) const;

private:
    unordered_map<string, BSTSet<FlightSegment>> m_flights;
};

#endif