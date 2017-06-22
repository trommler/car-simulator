/** 
 * @file session_controller.cpp
 * 
 */

#include "session_controller.h"

/**
 * Starts a UDS session and returns back to the default session state when 
 * no (re-)start is done within 5000 milliseconds.  
 */
void SessionController::startSession_01()
{
    start(5'000);
    session_ = UdsSession::SESSION_01;
}

/**
 * Returns the current UDS session state.
 * 
 * @return the active UDS state
 */
UdsSession SessionController::getCurretnUdsSession() const noexcept
{
    return session_;
}

/**
 * Overridden function which is called after the timer expired. Since the 
 * `session_`-member is atomic, we don't need to use a mutex.
 */
void SessionController::wakeup()
{
    session_ = UdsSession::DEFAULT;
}