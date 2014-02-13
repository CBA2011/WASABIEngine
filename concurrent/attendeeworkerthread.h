#ifndef ATTENDEEWORKERTHREAD_H
#define ATTENDEEWORKERTHREAD_H

#include "thread.h"
#include <cogaEmotionalAttendee.h>

class AttendeeWorkerThread : public Thread{
private:
    cogaEmotionalAttendee* attendee;

protected:
    void run();

public:
    void setAttendee(cogaEmotionalAttendee* attendee);
};

#endif
