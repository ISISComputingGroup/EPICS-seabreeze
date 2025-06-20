/***************************************************//**
 * @file    FPGARegisterWriteExchange.cpp
 * @date    October 2012
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
#include "vendors/OceanOptics/protocols/ooi/exchanges/FPGARegisterWriteExchange.h"
#include "vendors/OceanOptics/protocols/ooi/hints/ControlHint.h"
#include "vendors/OceanOptics/protocols/ooi/constants/OpCodes.h"

using namespace seabreeze;
using namespace seabreeze::ooiProtocol;

FPGARegisterWriteExchange::FPGARegisterWriteExchange(byte_ address, unsigned short value) {
    this->hints->push_back(new ControlHint());
    this->buffer->resize(4);
    this->length = 4;
    this->direction = Transfer::TO_DEVICE;
    buffer->at(0) = OpCodes::OP_WRITE_REGISTER;
    buffer->at(1) = address;
    buffer->at(2) = (byte_) ( value       & 0xff); // lsb
    buffer->at(3) = (byte_) ((value >> 8) & 0xff); // msb

    // TODO: future devices may have 32-bit registers, requiring larger buffers
}

FPGARegisterWriteExchange::~FPGARegisterWriteExchange() {

}
