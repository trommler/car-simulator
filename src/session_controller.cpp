/**
 * @file session_controller.cpp
 *
 */

#include "session_controller.h"
#include <iostream>

using namespace std;

void SessionController::startSession()
{
    start(5000);
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

void SessionController::setCurrentUdsSession(UdsSession s) noexcept
{
    session_ = s;
}

/**
 * Overridden function which is called after the timer expired. Since the
 * `session_`-member is atomic, we don't need to use a mutex.
 */
void SessionController::wakeup()
{
    if (session_ == UdsSession::DEFAULT)
        cout << "timer finished - DEFAULT" << endl;
    if (session_ == UdsSession::PROGRAMMING)
        cout << "timer finished - PROGRAMMING" << endl;
    if (session_ == UdsSession::EXTENDED)
        cout << "timer finished - EXTENDED" << endl;

    session_ = UdsSession::DEFAULT;
}
