#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long time) { 
    int hour, minute, second;
    string sh,sm,ss;
    hour = time / 3600;
    minute =  time / 60 - hour * 60;
    second = time - hour * 3600 - minute * 60;
    if(hour >= 10) {
        sh = std::to_string(hour);
    } else {
        sh = "0"+std::to_string(hour);
    }
    if(minute >= 10) {
        sm = std::to_string(minute);
    } else {
        sm = "0"+std::to_string(minute);
    }
    if(second >= 10) {
        ss = std::to_string(second);
    } else {
        ss = "0"+std::to_string(second);
    }
    return sh+":"+sm+":"+ss; 
    }