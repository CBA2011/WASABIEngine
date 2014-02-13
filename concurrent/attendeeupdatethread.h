#ifndef ATTENDEEUPDATETHREAD_H
#define ATTENDEEUPDATETHREAD_H

#include "thread.h"
#include <cogaEmotionalAttendee.h>
#include <vector>

using namespace std;

class AttendeeUpdateThread : public Thread
{
private:
   vector<cogaEmotionalAttendee*> attendees;

protected:
    void run();

public:
    AttendeeUpdateThread();
    void addAttendee(cogaEmotionalAttendee* attendee);
    int getSize();
};

#endif // ATTENDEEUPDATETHREAD_H
