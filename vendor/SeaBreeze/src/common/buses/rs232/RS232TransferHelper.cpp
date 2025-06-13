/***************************************************//**
 * @file    RS232TransferHelper.cpp
 * @date    April 2011
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
#include "common/buses/rs232/RS232TransferHelper.h"
#include "native/system/System.h"
#include <string>

using namespace seabreeze;
using namespace std;

RS232TransferHelper::RS232TransferHelper(RS232 *rs232Descriptor) : TransferHelper() {
    this->rs232 = rs232Descriptor;
}

RS232TransferHelper::~RS232TransferHelper() {

}

int RS232TransferHelper::receive(vector<byte_> &buffer, unsigned int length)
        noexcept(false) {
    int retval = 0;
    unsigned int byte_sRead = 0;

    while(byte_sRead < length) {
        retval = this->rs232->read((void *)&(buffer[byte_sRead]), length - byte_sRead);
        if(retval < 0) {
            string error("Failed to read any data from RS232.");
            throw BusTransferException(error);
        } else if(retval != 0) {
            byte_sRead += retval;
        } else {
            /* Not enough data available to satisfy the request.  Wait for more
             * data to arrive.
             */
            System::sleepMilliseconds(10);
        }
    }

    return byte_sRead;
}

int RS232TransferHelper::send(const vector<byte_> &buffer, unsigned int length) const
        noexcept(false) {
    int retval = 0;
    unsigned int byte_sWritten = 0;

    while(byte_sWritten < length) {
        retval = this->rs232->write((void *)&(buffer[byte_sWritten]), length - byte_sWritten);
        if(retval < 0) {
            string error("Failed to write any data to RS232.");
            throw BusTransferException(error);
        } else if(retval != 0) {
            byte_sWritten += retval;
        } else {
            /* Output buffer is probably full.  Wait for some of the byte_s to
             * be transferred before trying again.
             */
            System::sleepMilliseconds(10);
        }
    }

    return byte_sWritten;
}
