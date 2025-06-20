/***************************************************//**
 * @file    IrradCalProtocolInterface.h
 * @date    March 2010
 * @author  Ocean Optics, Inc.
 *
 * This is a generic interface into irradiance calibration
 * storage functionality at the protocol level, agnostic to
 * any particular protocol.  Each Protocol offering this
 * functionality should implement this interface.
 *
 * LICENSE:
 *
 * SeaBreeze Copyright (C) 2014, Ocean Optics Inc
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *******************************************************/

#ifndef IRRADCALPROTOCOLINTERFACE_H
#define IRRADCALPROTOCOLINTERFACE_H

#include "common/buses/Bus.h"
#include "common/exceptions/ProtocolException.h"
#include "common/protocols/ProtocolHelper.h"

namespace seabreeze {

    class IrradCalProtocolInterface : public ProtocolHelper {
    public:
        IrradCalProtocolInterface(Protocol *protocol);
        virtual ~IrradCalProtocolInterface();

        /**
         * Get the irradiance calibration from the device.
         */
        virtual std::vector<float> *readIrradCal(const Bus &bus)
            noexcept(false) = 0;
        /**
         * Write a new irradiance calibration to the device.
         */
        virtual int writeIrradCal(const Bus &bus, const std::vector<float> &cal)
            noexcept(false) = 0;

        /**
         * Determine whether the device has a stored irradiance collection area.
         * This will trap any exceptions so it is always safe to call.
         */
        virtual int hasCollectionArea(const Bus &bus) = 0;

        /**
         * Get the irradiance collection area from the device.
         */
        virtual float readCollectionArea(const Bus &bus)
            noexcept(false) = 0;

        /**
         * Write a new irradiance collection area to the device.
         */
        virtual void writeCollectionArea(const Bus &bus, float area)
            noexcept(false) = 0;

    };

}

#endif /* IRRADCALPROTOCOLINTERFACE_H */
