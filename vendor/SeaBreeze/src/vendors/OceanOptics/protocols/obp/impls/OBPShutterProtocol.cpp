/***************************************************//**
 * @file    OBPShutterProtocol.cpp
 * @date    February 2011
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
 ******************************************************/

#include "common/globals.h"
#include "vendors/OceanOptics/protocols/obp/impls/OBPShutterProtocol.h"
#include "vendors/OceanOptics/protocols/obp/exchanges/OBPShutterExchange.h"
#include "vendors/OceanOptics/protocols/obp/impls/OceanBinaryProtocol.h"
#include "common/exceptions/ProtocolBusMismatchException.h"

using namespace seabreeze;
using namespace seabreeze::oceanBinaryProtocol;
using namespace std;

OBPShutterProtocol::OBPShutterProtocol()
        : ShutterProtocolInterface(new OceanBinaryProtocol()) {
    this->shutterExchange = new OBPShutterExchange();
}

OBPShutterProtocol::~OBPShutterProtocol() {
    delete this->shutterExchange;
}

void OBPShutterProtocol::setShutterOpen(const Bus &bus, bool opened)
                noexcept(false) {

    TransferHelper *helper;

    helper = bus.getHelper(this->shutterExchange->getHints());
    if (NULL == helper) {
        string error("Failed to find a helper to bridge given protocol and bus.");
        throw ProtocolBusMismatchException(error);
    }

    this->shutterExchange->setShutterOpen(opened);

    /* This may cause a ProtocolException to be thrown. */
    this->shutterExchange->sendCommandToDevice(helper);

    /* FIXME: this could check the return value and throw an exception or retry if
     * it was false for any reason.
     */
}
