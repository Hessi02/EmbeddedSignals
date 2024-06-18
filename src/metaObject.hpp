/**
 *  \file   metaObject.hpp
 *  \brief  The file implements a meta object class.
 */

#ifndef META_OBJECT_HPP
#define META_OBJECT_HPP

#include "signalObject.hpp"

/**
 *  \class  MetaObject
 *  \brief  The class represents a base class for signal usage.
 */
class MetaObject : public SignalObject
{
public:
    /**
     *  \fn     ~MetaObject(void)
     *  \brief  Destroys the object.
     */
    ~MetaObject(void)
    {
        destroyed();
    }

    /**
     *  \fn     destroyed(void)
     *  \brief  This signal is emitted immediately before the object is destroyed.
     */
    void destroyed(void)
    {
        fireAllSlots(this, &BaseObject::destroyed);
    }
};

#endif //META_OBJECT_HPP