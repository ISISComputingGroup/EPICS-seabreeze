/***************************************************//**
 * @file    OOITECProtocol.cpp
 * @date    February 2009
 * @author  Ocean Optics, Inc.
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

#include "common/globals.h"
#include "vendors/OceanOptics/protocols/ooi/impls/OOITECProtocol.h"
#include "vendors/OceanOptics/protocols/ooi/impls/OOIProtocol.h"
#include "common/DoubleVector.h"
#include "common/exceptions/ProtocolBusMismatchException.h"

using namespace seabreeze;
using namespace seabreeze::ooiProtocol;
using namespace std;

OOITECProtocol::OOITECProtocol(TECEnableExchange *enableExchange,
        WriteTECSetPointExchange *writeSetPointExchange,
        ReadTECTemperatureExchange *readTECTempExchange)
            : ThermoElectricProtocolInterface(new OOIProtocol()) {

    this->tecEnable = enableExchange;
    this->tecWriteSetPoint = writeSetPointExchange;
    this->tecReadTemperature = readTECTempExchange;
}

OOITECProtocol::~OOITECProtocol() {
    delete this->tecEnable;
    delete this->tecWriteSetPoint;
    delete this->tecReadTemperature;
}

double OOITECProtocol::readThermoElectricTemperatureCelsius(const Bus &bus)
        noexcept(false) {
    TransferHelper *helper;

    helper = bus.getHelper(this->tecReadTemperature->getHints());
    if (NULL == helper) {
        string error("Failed to find a helper to bridge given protocol and bus.");
        throw ProtocolBusMismatchException(error);
    }

    /* This transfer() may cause a ProtocolException to be thrown. */
    Data *result = this->tecReadTemperature->transfer(helper);
    if (NULL == result) {
        string error("Expected Transfer::transfer to produce a non-null result "
                "containing raw temperature data.  Without this data, it is not possible to "
                "generate a valid TEC temperature.");
        throw ProtocolException(error);
    }

    /* FIXME: this cast is known to be safe for now, but this needs
     * to do some sort of check to make sure the cast is valid.
     */
    DoubleVector *dv = (DoubleVector *) result;
    vector<double> vec = dv->getDoubleVector();

    double retval = vec[0];

    delete result;

    return retval;
}

void OOITECProtocol::writeThermoElectricEnable(const Bus &bus, bool enable)
        noexcept(false) {
    TransferHelper *helper;

    helper = bus.getHelper(this->tecEnable->getHints());
    if (NULL == helper) {
        string error("Failed to find a helper to bridge given protocol and bus.");
        throw ProtocolBusMismatchException(error);
    }

    this->tecEnable->setEnable(enable);
    /* This transfer() may cause a ProtocolException to be thrown. */
    this->tecEnable->transfer(helper);
}

void OOITECProtocol::writeThermoElectricSetPointCelsius(const Bus &bus, double degreesC)
        noexcept(false) {
    TransferHelper *helper;

    helper = bus.getHelper(this->tecWriteSetPoint->getHints());
    if (NULL == helper) {
        string error("Failed to find a helper to bridge given protocol and bus.");
        throw ProtocolBusMismatchException(error);
    }

    this->tecWriteSetPoint->setSetPointCelsius(degreesC);
    /* This transfer() may cause a ProtocolException to be thrown. */
    this->tecWriteSetPoint->transfer(helper);
}
