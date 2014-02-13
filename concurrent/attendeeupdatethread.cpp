#include "attendeeupdatethread.h"

AttendeeUpdateThread::AttendeeUpdateThread()
{
}

void AttendeeUpdateThread::run(){

    vector<cogaEmotionalAttendee*>::iterator iter_ea;

    for (iter_ea = attendees.begin(); iter_ea != attendees.end(); ++iter_ea){
        cogaEmotionalAttendee* ea = (*iter_ea);
        ea->update();
    }
}

void AttendeeUpdateThread::addAttendee(cogaEmotionalAttendee *attendee){
    attendees.push_back(attendee);
}

int AttendeeUpdateThread::getSize(){
    return attendees.size();
}
