#ifndef WASABIEXPORTS_H
#define WASABIEXPORTS_H

#include "WASABIEngine.h"
#include "WASABIEngine_global.h"
#include "Mutex.h"

static WASABIEngine* wasabi = NULL;
static Mutex mutex;

void checkWasabiInstance();
extern "C" WASABIENGINESHARED_EXPORT bool instanceExists();
extern "C" WASABIENGINESHARED_EXPORT int addAgent(char* globalId);
extern "C" WASABIENGINESHARED_EXPORT char* getPad(int localId);
extern "C" WASABIENGINESHARED_EXPORT bool isInitialized(int localId);
extern "C" WASABIENGINESHARED_EXPORT void updateWasabi();
extern "C" WASABIENGINESHARED_EXPORT void removeAttendee(int localId);
extern "C" WASABIENGINESHARED_EXPORT void removeAllAttendees();
extern "C" WASABIENGINESHARED_EXPORT void onDestroy();

#endif // WASABIEXPORTS_H
