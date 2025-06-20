/***************************************************//**
 * @file    OOISpectrometerProtocol.cpp
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
#include <string>
#include "vendors/OceanOptics/protocols/ooi/impls/OOISpectrometerProtocol.h"
#include "vendors/OceanOptics/protocols/ooi/impls/OOIProtocol.h"
#include "common/ByteVector.h"
#include "common/Data.h"
#include "common/UShortVector.h"
#include "common/DoubleVector.h"
#include "common/exceptions/ProtocolBusMismatchException.h"
#include "common/Log.h"

using namespace seabreeze;
using namespace seabreeze::ooiProtocol;
using namespace std;

OOISpectrometerProtocol::OOISpectrometerProtocol(
        IntegrationTimeExchange *integrationTime,
        Transfer *requestSpectrum,
        Transfer *unformattedSpectrum,
        Transfer *spectrumTransfer,
        TriggerModeExchange *triggerMode)
            : SpectrometerProtocolInterface(new OOIProtocol()) {
    this->integrationTimeExchange = integrationTime;
    this->unformattedSpectrumExchange = unformattedSpectrum;
    this->requestSpectrumExchange = requestSpectrum;
    this->spectrumTransferExchange = spectrumTransfer;
    this->triggerModeExchange = triggerMode;
}

OOISpectrometerProtocol::~OOISpectrometerProtocol() {
    delete this->integrationTimeExchange;
    delete this->unformattedSpectrumExchange;
    delete this->requestSpectrumExchange;
    delete this->spectrumTransferExchange;
    delete this->triggerModeExchange;
}

vector<byte_> *OOISpectrometerProtocol::readUnformattedSpectrum(const Bus &bus)
        noexcept(false) {
    LOG(__FUNCTION__);

    Data *result;
    TransferHelper *helper;

    helper = bus.getHelper(this->unformattedSpectrumExchange->getHints());
    if (NULL == helper) {
        string error("Failed to find a helper to bridge given protocol and bus.");
        logger.error(error.c_str());
        throw ProtocolBusMismatchException(error);
    }

    /* This transfer() may cause a ProtocolException to be thrown. */
    result = this->unformattedSpectrumExchange->transfer(helper);

    if (NULL == result) {
        string error("Got NULL when expecting spectral data which was unexpected.");
        logger.error(error.c_str());
        throw ProtocolException(error);
    }

    ByteVector *bv = static_cast<ByteVector *>(result);

    vector<byte_> *retval = new vector<byte_ > (bv->getByteVector());

    delete result;

    /* FIXME: this method should probably return (Data *) so that
     * metadata is preserved.  In that case, this should just return
     * the above result without any additional work.  The current
     * implementation has an extra allocate/copy/destroy overhead.
     */

    return retval;
}

vector<double> *OOISpectrometerProtocol::readSpectrum(const Bus &bus)
        noexcept(false) {

    LOG(__FUNCTION__);

    TransferHelper *helper;
    Data *result;
    unsigned int i;

    helper = bus.getHelper(this->spectrumTransferExchange->getHints());
    if (NULL == helper) {
        string error("Failed to find a helper to bridge given protocol and bus.");
        logger.error(error.c_str());
        throw ProtocolBusMismatchException(error);
    }

    /* This transfer() may cause a ProtocolException to be thrown. */
    result = this->spectrumTransferExchange->transfer(helper);

    if (NULL == result) {
        string error("Got NULL when expecting spectral data which was unexpected.");
        logger.error(error.c_str());
        throw ProtocolException(error);
    }

    /* FIXME: not knowing whether doubles or shorts will be returned
     * requires an RTTI lookup with dynamic_cast.  It might be better
     * to have the conversion to doubles done at a lower level (if that
     * can be done without preventing readout of raw spectra) so that
     * this is not blind as to the correct type.  Also, doing both
     * of the dynamic_casts even though only one of them will be
     * valid is adding some overhead.  Hopefully it isn't much.
     */
    vector<double> *retval = NULL;
    UShortVector *usv = dynamic_cast<UShortVector *>(result);
    DoubleVector *dv = dynamic_cast<DoubleVector *>(result);
    if(NULL != usv) {
        vector<unsigned short> shortVec = usv->getUShortVector();

        retval = new vector<double>(shortVec.size());

        for (i = 0; i < shortVec.size(); i++) {
            (*retval)[i] = shortVec[i];
        }

    } else if(NULL != dv) {
        vector<double> doubleVec= dv->getDoubleVector();
        retval = new vector<double>(doubleVec.size());

        for (i = 0; i < doubleVec.size(); i++) {
            (*retval)[i] = doubleVec[i];
        }
    }
    delete result; /* a.k.a. usv or dv */

    return retval;
}

void OOISpectrometerProtocol::requestSpectrum(const Bus &bus)
        noexcept(false) {
    LOG(__FUNCTION__);

    TransferHelper *helper;
    helper = bus.getHelper(this->requestSpectrumExchange->getHints());

    if (NULL == helper) {
        string error("Failed to find a helper to bridge given protocol and bus.");
        logger.error(error.c_str());
        throw ProtocolBusMismatchException(error);
    }

    /* This transfer() may cause a ProtocolException to be thrown. */
    this->requestSpectrumExchange->transfer(helper);
}

void OOISpectrometerProtocol::setIntegrationTimeMicros(const Bus &bus,
        unsigned long integrationTime_usec) noexcept(false) {
    TransferHelper *helper;

    helper = bus.getHelper(this->integrationTimeExchange->getHints());

    if (NULL == helper) {
        string error("Failed to find a helper to bridge given protocol and bus.");
        throw ProtocolBusMismatchException(error);
    }

    this->integrationTimeExchange->setIntegrationTimeMicros(integrationTime_usec);
    /* This transfer() may cause a ProtocolException to be thrown. */
    this->integrationTimeExchange->transfer(helper);
}

void OOISpectrometerProtocol::setTriggerMode(const Bus &bus,
            SpectrometerTriggerMode &mode) noexcept(false) {
    LOG(__FUNCTION__);
    TransferHelper *helper;

    helper = bus.getHelper(this->triggerModeExchange->getHints());

    if (NULL == helper) {
        string error("Failed to find a helper to bridge given protocol and bus.");
        logger.error(error.c_str());
        throw ProtocolBusMismatchException(error);
    }

    this->triggerModeExchange->setTriggerMode(mode);

    /* This transfer() may cause a ProtocolException to be thrown. */
    this->triggerModeExchange->transfer(helper);
}
