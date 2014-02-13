#include "attendeeworkerthread.h"

void AttendeeWorkerThread::run()
{
    attendee->update();
}

void AttendeeWorkerThread::setAttendee(cogaEmotionalAttendee* attendee)
{
    //std::cout << "setAttendee accessed" << std::endl;
    //std::cout << "Attendee id: " << attendee->getGlobalID() << std::endl;
    this->attendee = attendee;
    //this->attendee = new cogaEmotionalAttendee;
    //std::cout << "setAttendee finshed" << std::endl;
}
