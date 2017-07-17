/**
 * @file electronic_control_unit.cpp
 *
 * This file contains a class which constitutes a Electronic Control Unit (ECU).
 */

#include "electronic_control_unit.h"
#include <array>
#include <unistd.h>

using namespace std;

ElectronicControlUnit::ElectronicControlUnit(const string& device, unique_ptr<EcuLuaScript> pEcuScript)
: sender_(pEcuScript->getResponseId(), pEcuScript->getRequestId(), device)
, udsReceiver_(pEcuScript->getResponseId(), pEcuScript->getRequestId(), device, move(pEcuScript), &sender_, &sessionControl_)
, broadcastReceiver_(pEcuScript->getBroadcastId(), device, &udsReceiver_)
, udsReceiverThread_(&IsoTpReceiver::readData, &udsReceiver_)
, broadcastReceiverThread_(&IsoTpReceiver::readData, &broadcastReceiver_)
{
}

ElectronicControlUnit::~ElectronicControlUnit()
{
    broadcastReceiverThread_.join();
    udsReceiverThread_.join();
}
