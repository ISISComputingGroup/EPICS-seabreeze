/***************************************************//**
 * @file    ThermoElectricFeatureInterface.h
 * @date    February 2011
 * @author  Ocean Optics, Inc.
 *
 * This feature provides an interface to the thermo-
 * electric cooler (TEC) feature.
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

#ifndef THERMOELECTRICFEATUREINTERFACE_H
#define THERMOELECTRICFEATUREINTERFACE_H

#include "common/buses/Bus.h"
#include "common/protocols/Protocol.h"
#include "common/exceptions/FeatureException.h"
#include "common/exceptions/IllegalArgumentException.h"

namespace seabreeze {

    class ThermoElectricFeatureInterface {
    public:
        virtual ~ThermoElectricFeatureInterface() = 0;
        virtual void setThermoElectricEnable(const Protocol &protocol,
                const Bus &bus, bool enable) noexcept(false) = 0;
        virtual double getTemperatureCelsius(const Protocol &protocol,
                const Bus &bus) noexcept(false) = 0;
        virtual void setTemperatureSetPointCelsius(const Protocol &protocol,
                const Bus &bus, double degreesC)
                noexcept(false) = 0;

        virtual double getDefaultSetPointCelsius(const Protocol &protocol,
                const Bus &bus) noexcept(false) = 0;
        virtual bool getDefaultThermoElectricEnable(const Protocol &protocol,
                const Bus &bus) noexcept(false) = 0;
    };

    /* Default implementation for (otherwise) pure virtual destructor */
    inline ThermoElectricFeatureInterface::~ThermoElectricFeatureInterface() {}
}

#endif /* THERMOELECTRICFEATUREINTERFACE_H */
