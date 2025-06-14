/***************************************************//**
 * @file    OBPGetSaturationExchange.cpp
 * @date    March 2016
 * @author  Ocean Optics, Inc.
 *
 * LICENSE:
 *
 * SeaBreeze Copyright (C) 2016, Ocean Optics Inc
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
#include "vendors/OceanOptics/protocols/obp/exchanges/OBPGetSaturationExchange.h"
#include "vendors/OceanOptics/protocols/obp/constants/OBPMessageTypes.h"
#include "vendors/OceanOptics/protocols/obp/hints/OBPControlHint.h"
#include <vector>

using namespace seabreeze;
using namespace seabreeze::oceanBinaryProtocol;
using namespace std;

OBPGetSaturationExchange::OBPGetSaturationExchange() {
    this->messageType = OBPMessageTypes::OBP_GET_SATURATION_LEVEL;

    this->hints->push_back(new OBPControlHint());
}

OBPGetSaturationExchange::~OBPGetSaturationExchange() {

}

unsigned int OBPGetSaturationExchange::querySaturationLevel(
        TransferHelper *helper) noexcept(false) {

    unsigned int saturation;
    vector<byte_> *result;

    result = this->queryDevice(helper);
    if(NULL == result || result->size() < 4) {
        if(NULL != result) {
            delete result;
        }
        throw ProtocolException("Got a short read when querying saturation level.");
    }

    saturation = (     ((*result)[0] & 0x00FF)
                    | (((*result)[1] & 0x00FF) << 8)
                    | (((*result)[2] & 0x00FF) << 16)
                    | (((*result)[3] & 0x00FF) << 24));

    delete result;

    return saturation;
}
