/***************************************************//**
 * @file    TransferHelper.h
 * @date    February 2009
 * @author  Ocean Optics, Inc.
 *
 * This is effectively an interface for wrappers around bus
 * activity.  These wrappers may be selected from a Bus
 * based on certain hints provided by a Protocol or its
 * various Exchanges.  All that this specifies is that a
 * given object must be able to send() and receive() data
 * across its particular (encapsulated) Bus.
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

#ifndef SEABREEZE_TRANSFERHELPER_H
#define SEABREEZE_TRANSFERHELPER_H

#include "common/SeaBreeze.h"
#include "common/exceptions/BusTransferException.h"
#include <vector>

namespace seabreeze {

    class TransferHelper {
    public:
        TransferHelper();
        virtual ~TransferHelper();
        virtual int receive(std::vector<byte_> &buffer, unsigned int length)
            noexcept(false) = 0;
        virtual int send(const std::vector<byte_> &buffer, unsigned int length) const
            noexcept(false) = 0;
    };

}

#endif
