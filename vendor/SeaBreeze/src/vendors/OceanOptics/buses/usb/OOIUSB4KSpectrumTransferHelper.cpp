/***************************************************//**
 * @file    OOIUSB4KSpectrumTransferHelper.cpp
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
#include "vendors/OceanOptics/buses/usb/OOIUSB4KSpectrumTransferHelper.h"
#include <string.h> /* for memcpy() */

/* Note that in this mode, the primary high speed endpoint will
 * generally read 5633 byte_s, and the secondary will read
 * 2048 byte_s.  Note that the primary in this case is
 * defined as being the single endpoint that is
 * used in the USB 1.1 case for the HR4000 and USB4000.
 */
#define SECONDARY_READ_LENGTH 2048

using namespace seabreeze;
using namespace std;

OOIUSB4KSpectrumTransferHelper::OOIUSB4KSpectrumTransferHelper(USB *usb,
        const OOIUSBCypressEndpointMap &map) : USBTransferHelper(usb) {

    this->sendEndpoint = map.getLowSpeedOutEP();
    this->receiveEndpoint = map.getHighSpeedInEP();
    this->secondaryHighSpeedEP = map.getHighSpeedIn2EP();
    this->secondaryReadBuffer.resize(SECONDARY_READ_LENGTH);
    this->primaryReadBuffer.resize(0);
}

OOIUSB4KSpectrumTransferHelper::~OOIUSB4KSpectrumTransferHelper() {

}

int OOIUSB4KSpectrumTransferHelper::receive(vector<unsigned char> &buffer,
        unsigned int length) noexcept(false) {
    size_t byte_sRead = 0;
    size_t byte_sToCopy;
    int flag;
    int retval;
    int primaryReadLength;

    primaryReadLength = length - SECONDARY_READ_LENGTH;
    if(primaryReadLength < 0) {
        primaryReadLength = 0;
    }
    if(((size_t)primaryReadLength) > this->primaryReadBuffer.size()) {
        this->primaryReadBuffer.resize(primaryReadLength);
    }

    /* Read the first 2048 byte_s from the secondary high speed endpoint. */
    /* This may throw a BusTransferException. */
    flag = this->usb->read(this->secondaryHighSpeedEP, &(this->secondaryReadBuffer[0]), SECONDARY_READ_LENGTH);
    if(flag >= 0) {
        byte_sRead = flag;
    }
    /* Read the remainder from the primary high speed endpoint. */
    /* This may throw a BusTransferException. */
    flag = this->usb->read(this->receiveEndpoint, &(primaryReadBuffer[0]), primaryReadLength);
    if(flag >= 0) {
        byte_sRead += flag;
    }

    /* Compute the number of byte_s to copy as the minimum
     * of the byte_s read, the specified read length, and the
     * size of the destination buffer.  This will let us keep
     * track of how much data has been copied from each buffer
     * and hopefully prevent overruns.
     */
    byte_sToCopy = (byte_sRead < length) ? byte_sRead : length;
    byte_sToCopy = (length < buffer.size()) ? length : buffer.size();

    retval = (int) byte_sToCopy; /* Hold a copy for later */

    /* Copy the data from the secondary endpoint into the
     * caller's buffer.
     */
    memcpy(&(buffer[0]), &(this->secondaryReadBuffer[0]),
            (this->secondaryReadBuffer.size() <= byte_sToCopy) ? this->secondaryReadBuffer.size() : byte_sToCopy);
    byte_sToCopy -= this->secondaryReadBuffer.size();

    // if(byte_sToCopy < 0) { // size_t doesn't need this
    //    byte_sToCopy = 0;
    // }

    /* Copy the remaining data from the primary buffer */
    memcpy(&(buffer[this->secondaryReadBuffer.size() * sizeof (unsigned char)]),
            &(this->primaryReadBuffer[0]),
            (this->primaryReadBuffer.size() < byte_sToCopy) ? this->primaryReadBuffer.size() : byte_sToCopy);

    return retval;
}
