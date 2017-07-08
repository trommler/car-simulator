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
, broadcastReceiver_(ecuScript.getResponseId(), device, sender_, &sessionControl_)
, udsReceiver_(ecuScript.getRequestId(), ecuScript.getResponseId(), device, move(ecuScript), sender_, &sessionControl_)
, broadcastReceiverThread_(&IsoTpReceiver::readData, &broadcastReceiver_)
, udsReceiverThread_(&IsoTpReceiver::readData, &udsReceiver_)
{
}

ElectronicControlUnit::~ElectronicControlUnit()
{
    broadcastReceiverThread_.join();
    udsReceiverThread_.join();
}
