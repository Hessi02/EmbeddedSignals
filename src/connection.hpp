/**
 *  \file       connection.hpp
 *  \brief      The file implements a connection between signals and slots.
 */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <vector>

/**
 *  \concept    isDerived
 *  \brief      Checks if the class are related by inheritance.     
 *  \tparam     Base passes the base class.
 *  \tparam     Derived passes the derived class.
 */
template<class Base, class Derived>
concept isDerived = std::is_base_of<Base, Derived>::value;

class SignalObject;

/**
 *  \class  Connection
 *  \brief  The class represents a connection between signal and slot.
 *  \tparam ParamPack passes the signal's and slot's parameter pack.
 */
template<class... ParamPack>
class Connection
{
public:
    /**
     *  \fn         Connection()
     *  \brief      The constructor initializes the instance.
     *  \tparam     Sender passes the sender's typename.
     *  \tparam     Receiver passes the receiver's typename.
     *  \tparam     SenderBase passes the signal's implementaion class.
     *  \tparam     ReceiveBase passes the slot's implementation class.
     *  \param[in]  sender passes a pointer to the sender instance.
     *  \param[in]  receiver passes a pointer to the receiver instance.
     *  \param[in]  signal passes method to connect as signal.
     *  \param[in]  slot passes the method to connect as slot.
     */
    template<class Sender, class Receiver, class SenderBase, class ReceiverBase>
    requires isDerived<SenderBase, Sender> && isDerived<ReceiverBase, Receiver>
    Connection(Sender* sender,
               Receiver* receiver,
               void(SenderBase::*signal)(ParamPack...),
               void(ReceiverBase::*slot)(ParamPack...)
    ) :
            _sender(static_cast<SignalObject*>(sender)),
            _receiver(static_cast<SignalObject*>(receiver)),
            _signal(static_cast<void(SignalObject::*)(ParamPack...)>(signal)),
            _slot(static_cast<void(SignalObject::*)(ParamPack...)>(slot))
    {}

    /**
     *  \fn         fireSlot(ParamPack... args)
     *  \brief      Calls the receivers slot.
     *  \param[in]  args passes the slot's argument list.
     */
    void fireSlot(ParamPack... args)
    {
        if (_receiver && _slot)
        {
            (*_receiver.*_slot)(args...);
        }
    }

    /**
     *  \fn         isSender(const SignalObject* sender) const
     *  \brief      Checks if passed instance is connection's sender.
     *  \param[in]  sender passes a pointer to the instance to check.
     *  \return     Boolean indicating result of check.
     */ 
    bool isSender(const SignalObject* sender) const
    {
        return sender == _sender;
    }

    /**
     *  \fn         isReceiver(const SignalObject* receiver) const
     *  \brief      Checks if passed instance is connection's receiver.
     *  \param[in]  receiver passes a pointer to the instance to check.
     *  \return     Boolean indicating result of check.
     */
    bool isReceiver(const SignalObject* receiver) const
    {
        return receiver == _receiver;
    }

    /**
     *  \fn         isSignal(void(SignalObject::*signal)(ParamPack...)) const
     *  \brief      Checks if passed method pointer is connection's signal.
     *  \param[in]  signal passes a method pointer to check.
     *  \return     Boolean indicating result of check.
     */
    bool isSignal(void(SignalObject::*signal)(ParamPack...)) const
    {
        return signal == _signal;
    }

    /**
     *  \fn         isSlot(void(SignalObject::*slot)(ParamPack...)) const
     *  \brief      Checks if passed method pointer is connection's slot.
     *  \param[in]  slot passes a method pointer to check.
     *  \return     Boolean indicating result of check.
     */
    bool isSlot(void(SignalObject::*slot)(ParamPack...)) const
    {
        return slot == _slot;
    }

private:
    /**
     *  \var    _sender
     *  \brief  Pointer to connection's sender instance.
     */
    SignalObject* _sender;

    /**
     *  \var    _receiver
     *  \brief  Pointer to connection's receiver instance.
     */
    SignalObject* _receiver;

    /**
     *  \var    _signal
     *  \brief  Method pointer to the connection's signal.
     */
    void(SignalObject::*_signal)(ParamPack...);
    
    /**
     *  \var    _slot
     *  \brief  Method pointer to the connection's slot.
     */
    void(SignalObject::*_slot)(ParamPack...);
};

#endif //CONNECTION_HPP