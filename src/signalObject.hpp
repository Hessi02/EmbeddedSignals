/**
 *  \file   signalObject.hpp
 *  \brief  The file implements a base object containing signal functions.
 */

#ifndef SIGNAL_OBJECT_HPP
#define SIGNAL_OBJECT_HPP

#include <type_traits>

#include "connection.hpp"

/**
 *  \class  SignalObject
 *  \brief  The class represents a base object containing signal functions. 
 */
class SignalObject
{
public:
    /**
     *  \fn         connect()
     *  \brief      Connects a sender's signal to a receiver's slot.
     *  \tparam     Sender passes the sender's typename.
     *  \tparam     Receiver passes the receiver's typename.
     *  \tparam     SenderBase passes the signal's implementaion class.
     *  \tparam     ReceiveBase passes the slot's implementation class.
     *  \tparam     ParamPack passes the signal's and slot's parameter pack.
     *  \param[in]  sender passes a pointer to the sender instance.
     *  \param[in]  receiver passes a pointer to the receiver instance.
     *  \param[in]  signal passes a method pointer to the sender's signal.
     *  \param[in]  slot passes a method pointer to the receiver's slot.
     */
    template<class Sender, class Receiver, class SenderBase, class ReceiverBase, class... ParamPack>
    requires isDerived<SenderBase, Sender> && isDerived<ReceiverBase, Receiver>
    static void connect(Sender* sender,
                        Receiver* receiver,
                        void(SenderBase::*signal)(ParamPack...),
                        void(ReceiverBase::*slot)(ParamPack...)
    )
    {
        Connection<ParamPack...>* connection = new Connection(sender,
                                                              receiver,
                                                              signal,
                                                              slot);
        
        _connections<ParamPack...>.push_back(connection);
    }

    /**
     *  \fn         disconnect()
     *  \brief      Disconnects a sender's signal from a receiver's slot.
     *  \tparam     Sender passes the sender's typename.
     *  \tparam     Receiver passes the receiver's typename.
     *  \tparam     SenderBase passes the signal's implementaion class.
     *  \tparam     ReceiveBase passes the slot's implementation class.
     *  \tparam     ParamPack passes the signal's and slot's parameter pack.
     *  \param[in]  sender passes a pointer to the sender instance.
     *  \param[in]  receiver passes a pointer to the receiver instance.
     *  \param[in]  signal passes a method pointer to the sender's signal.
     *  \param[in]  slot passes a method pointer to the receiver's slot.
     */
    template<class Sender, class Receiver, class SenderBase, class ReceiverBase, class... ParamPack>
    requires isDerived<SenderBase, Sender> && isDerived<ReceiverBase, Receiver>
    static void disconnect(Sender* sender,
                           Receiver* receiver,
                           void(SenderBase::*signal)(ParamPack...),
                           void(ReceiverBase::*slot)(ParamPack...)
    )
    {
        int iterator = 0;

        for (Connection<ParamPack...>* connection : _connections<ParamPack...>)
        {
            if (connection->isSender(static_cast<SignalObject*>(sender)) &&
                connection->isReceiver(static_cast<SignalObject*>(receiver)) &&
                connection->isSignal(static_cast<void(SignalObject::*)(ParamPack...)>(signal)) &&
                connection->isSlot(static_cast<void(SignalObject::*)(ParamPack...)>(slot))
            )
            {
                delete connection;
                _connections<ParamPack...>.erase(_connections<ParamPack...>.begin() + iterator);
            }
        
            iterator++;
        }
    }

    /**
     *  \fn         fireAllSlots()
     *  \brief      Calls all connected slots of the specified signal.
     *  \tparam     Sender passes the sender's typename.
     *  \tparam     SenderBase passes the signal's implementaion class.
     *  \tparam     ParamPack passes the signal's and slot's parameter pack.
     *  \param[in]  sender passes a pointer to the sender instance.
     *  \param[in]  signal passes a method pointer to the sender's signal.
     *  \param[in]  args passes the signal's parameter pack.
     */
    template<class Sender, class SenderBase, class... ParamPack>
    requires isDerived<SenderBase, Sender>
    static void fireAllSlots(Sender* sender,
                             void(SenderBase::*signal)(ParamPack...),
                             ParamPack... args
    )
    {
        for (Connection<ParamPack...>* connection : _connections<ParamPack...>)
        {
            if (sender &&
                connection &&
                connection->isSender(static_cast<SignalObject*>(sender)) &&
                connection->isSignal(static_cast<void(SignalObject::*)(ParamPack...)>(signal))
            )
            {
                connection->fireSlot(args...);
            }
        }
    }

private:
    /**
     *  \var    _connections
     *  \brief  Vector of connections between various signals and slots.
     *  \tparam ParamPack passes the signal's and slot's parameter pack.
     */
    template<class... ParamPack>
    inline static std::vector<Connection<ParamPack...>*> _connections;
};

#endif //SIGNAL_OBJECT_HPP
