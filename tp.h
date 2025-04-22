#ifndef TP_H
#define TP_H

#include "provided.h"
#include "fm.h"
using namespace std;
class TravelPlanner : public TravelPlannerBase {
public:
    TravelPlanner(const FlightManagerBase& flight_manager, const AirportDB& airport_db);
    virtual ~TravelPlanner() {}
    virtual bool plan_travel(string source_airport, string destination_airport, int start_time, Itinerary& itinerary) const;
    virtual void add_preferred_airline(string airline);

private:
    vector<string> m_preferredAirlines;
};

#endif