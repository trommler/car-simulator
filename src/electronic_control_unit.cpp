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
: sender_(ecuScript.getRequestId(), ecuScript.getResponseId(), device)
, udsReceiver_(ecuScript.getRequestId(), ecuScript.getResponseId(), device, move(ecuScript), &sender_, &sessionControl_)
, broadcastReceiver_(ecuScript.getBroadcastId(), device, &udsReceiver_)
, udsReceiverThread_(&IsoTpReceiver::readData, &udsReceiver_)
, broadcastReceiverThread_(&IsoTpReceiver::readData, &broadcastReceiver_)
{
    //ecuScript.registerSessionController(&sessionControl_); // not necessary. Already done in the c'tor of `udsReceiver_`
    //ecuScript.registerIsoTpSender(&sender_);  // not necessary. Already done in the c'tor of `udsReceiver_`
}

ElectronicControlUnit::~ElectronicControlUnit()
{
    broadcastReceiverThread_.join();
    udsReceiverThread_.join();
}
