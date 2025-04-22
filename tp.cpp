#include "tp.h"
#include <queue>
#include <unordered_set>
#include <algorithm>
TravelPlanner::TravelPlanner(const FlightManagerBase& flight_manager, const AirportDB& airport_db)
    : TravelPlannerBase(flight_manager, airport_db) {

}

void TravelPlanner::add_preferred_airline(string airline) {
    m_preferredAirlines.push_back(airline);
}

bool TravelPlanner::plan_travel(string source_airport, string destination_airport, int start_time, Itinerary& itinerary) const {
    struct State {
        vector<FlightSegment> flights;
        string currentAirport;
        int currentTime;
        unordered_set<string> visited;
    };

    queue<State> q;
    q.push({ {}, source_airport, start_time, {source_airport} });

    const FlightManagerBase& fm = get_flight_manager();
    
    int min_connection = get_min_connection_time();
    int max_layover = get_max_layover();
    int max_duration = get_max_duration();
    while (!q.empty()) {
        State current = q.front();
        q.pop();

        if (current.currentAirport == destination_airport) {
            int total_time = current.currentTime - start_time;
            if (total_time <= max_duration) {
                itinerary = { source_airport, destination_airport, current.flights, total_time };
                return true;
            }
        }

        int time_start;
        if (current.flights.empty()) {
            time_start = start_time;
        }
        else {
            time_start = current.currentTime + min_connection;
        }
        int time_end = time_start + max_layover;

        vector<FlightSegment> choices = fm.find_flights(current.currentAirport, time_start, time_end);

        for (const FlightSegment& flight : choices) {
            if (!m_preferredAirlines.empty() &&
                find(m_preferredAirlines.begin(), m_preferredAirlines.end(), flight.airline) == m_preferredAirlines.end()) {
                continue;
            }

            if (current.visited.count(flight.destination_airport)) {
                continue;
            }

            if (flight.departure_time + flight.duration_sec - start_time > max_duration) {
                continue;
            }

            State newState = current;
            newState.flights.push_back(flight);
            newState.currentAirport = flight.destination_airport;
            newState.currentTime = flight.departure_time + flight.duration_sec;
            newState.visited.insert(flight.destination_airport);
            q.push(newState);
        }
    }

    return false;
}