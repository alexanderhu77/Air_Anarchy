#include "fm.h"
#include <fstream>
#include <sstream>


bool operator<(const FlightSegment& a, const FlightSegment& b) {

    if (a.departure_time != b.departure_time) {
        return a.departure_time < b.departure_time;
    }
    return a.flight_no < b.flight_no;

}

bool FlightManager::load_flight_data(string filename) {
    ifstream file(filename);
    if (!file) {
        return false;
    }
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string airline, temp_flight_no, source, destination, temp_dep_time, temp_arr_time, temp_duration;
        if (!getline(iss, airline, ',') ||
            !getline(iss, temp_flight_no, ',') ||
            !getline(iss, source, ',') ||
            !getline(iss, destination, ',') ||
            !getline(iss, temp_dep_time, ',') ||
            !getline(iss, temp_arr_time, ',') ||
            !getline(iss, temp_duration, ','))
        {
            continue;
        }
        int dep_time = stoi(temp_dep_time);
        int arr_time = stoi(temp_arr_time);
        int flight_no = stoi(temp_flight_no);
        
        int duration = stoi(temp_duration);

        FlightSegment flight(airline, flight_no, source, destination, dep_time, duration);
        m_flights[source].insert(flight);
    }
    return true;
}

vector<FlightSegment> FlightManager::find_flights(string source_airport, int start_time, int end_time) const {
    vector<FlightSegment> flights;
    unordered_map<string, BSTSet<FlightSegment>>::const_iterator it = m_flights.find(source_airport);
    if (it == m_flights.end()) {
        return flights;
    }

    const BSTSet<FlightSegment>& flightSet = it->second;
    FlightSegment temp("", 0, source_airport, "", start_time, 0);
    BSTSet<FlightSegment>::SetIterator iter = flightSet.find_first_not_smaller(temp);

    for (const FlightSegment* flight_ptr = iter.get_and_advance();
        flight_ptr != nullptr;
        flight_ptr = iter.get_and_advance())
    {
        if (flight_ptr->departure_time >= start_time) {
            if (flight_ptr->departure_time < end_time) {
                flights.push_back(*flight_ptr);
            }
            else {
                break;
            }
        }
    }

    return flights;

}