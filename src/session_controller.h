/** 
 * @file session_controller.h
 *
 */

#ifndef SESSION_CONTROLLER_H
#define SESSION_CONTROLLER_H

#include "ecuTimer.h"
#include <atomic>

enum class UdsSession
{
    DEFAULT, ///< the default UDS session
    SESSION_01, // TODO: rename the placeholder (Strg+R) and remove this comment
    SESSION_02,
    SESSION_03
};

class SessionController : public ecuTimer
{
public:
    SessionController() = default;
    SessionController(const SessionController& orig) = default;
    SessionController& operator =(const SessionController& orig) = default;
    SessionController(SessionController&& orig) = default;
    SessionController& operator =(SessionController&& orig) = default;
    virtual ~SessionController() = default;

    void startSession_01(); // TODO: refactor this function name (Strg+R) and remove this comment
    UdsSession getCurretnUdsSession() const noexcept;

private:
    std::atomic<UdsSession> session_{UdsSession::DEFAULT};
    virtual void wakeup() override;
};

#endif /* SESSION_CONTROLLER_H */
