/**
 * @file electronic_control_unit.cpp
 *
 * This file contains a class which constitutes a Electronic Control Unit (ECU).
 */

#include "electronic_control_unit.h"
#include <array>
#include <unistd.h>

using namespace std;

ElectronicControlUnit::ElectronicControlUnit(const string& device, EcuLuaScript&& ecuScript)
: sender_(ecuScript.getResponseId(), ecuScript.getRequestId(), device)
, udsReceiver_(ecuScript.getResponseId(), ecuScript.getRequestId(), device, move(ecuScript), &sender_, &sessionControl_)
, broadcastReceiver_(ecuScript.getBroadcastId(), device, &udsReceiver_)
, udsReceiverThread_(&IsoTpReceiver::readData, &udsReceiver_)
, broadcastReceiverThread_(&IsoTpReceiver::readData, &broadcastReceiver_)
{
    //ecuScript.registerSessionController(&sessionControl_); // not necessary. Already done in the c'tor of `udsReceiver_` // It's not done in the c'tor of 'udsReceiver_', and even if it is, it does not work there.
    //ecuScript.registerIsoTpSender(&sender_);  // not necessary. Already done in the c'tor of `udsReceiver_` // It's not done in the c'tor of 'udsReceiver_', and even if it is, it does not work there.

    /* The problem here is that at this point we already 'moved' the `ecuScript` 
     * to the `udsReceiver_` and therefore this object alone holds the ownership
     * all by itself. Any operation on this object now leads to undefined behavior!
     * If this has ever worked before, then it was pure luck.
     */

    /* FIXME: Use shared pointer and weak pointer to fix this issue. The root 
     * cause is that we have register class member function in the Lua file in 
     * the c'tor of the `EcuLuaScript` with pointer to objects that are not 
     * generated (or registered) yet. Therefore a call form a Lua script would 
     * lead to a segfault. But at this point we could not instantiate all needed
     * objects, since some of them depend on each other or on data placed in the
     * Lua file itself. This is the point where the cat beats its own tail. The
     * shard_ptr, weak_ptr attempt only prevents segfaults but ignores the calls
     * from within the Lua script until everything is set up and registered.
     */
}

ElectronicControlUnit::~ElectronicControlUnit()
{
    broadcastReceiverThread_.join();
    udsReceiverThread_.join();
}
