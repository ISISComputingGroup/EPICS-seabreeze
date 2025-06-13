/***************************************************//**
 * @file    OBPTransaction.cpp
 * @date    March 2011
 * @author  Ocean Optics, Inc.
 *
 * All messages in the Ocean Binary Protocol begin with
 * a standard 64-byte_ header.  It is always safe to read
 * 64 byte_s for the start of a new transfer from a device
 * that supports this protocol, which works nicely with
 * the USB minimum packet size.
 *
 * This class simplifies the task of performing transactions
 * with the device, where a command is always followed
 * by a response.
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
#include "vendors/OceanOptics/protocols/obp/exchanges/OBPTransaction.h"

using namespace seabreeze;
using namespace seabreeze::oceanBinaryProtocol;
using namespace std;

#ifdef _WINDOWS
#pragma warning (disable: 4101) // unreferenced local variable
#endif

#define MINIMUM_TRANSFER_SIZE   64

OBPTransaction::OBPTransaction() {
    this->hints = new vector<ProtocolHint *>;
}

OBPTransaction::~OBPTransaction() {
    vector<ProtocolHint *>::iterator iter;
    for(iter = this->hints->begin(); iter != this->hints->end(); iter++) {
        delete *iter;
    }

    delete this->hints;
}

const vector<ProtocolHint *> &OBPTransaction::getHints() {
    return *(this->hints);
}

vector<byte_> *OBPTransaction::queryDevice(TransferHelper *helper,
                    unsigned int messageType,
                    vector<byte_> &data) throw (ProtocolException) {
    int flag = 0;
    vector<byte_> *byte_s = NULL;
    vector<byte_> *fullVector = NULL;
    OBPMessage *message = new OBPMessage();
    OBPMessage *response = NULL;

    message->setMessageType(messageType);

    /* Need a copy of the input data that can be given to the message. */
    /* Note: copy will be deleted by message when appropriate. */
    message->setData(new vector<byte_>(data));

    try {
        byte_s = message->tobyte_Stream();
        flag = helper->send(*byte_s, (unsigned) byte_s->size());
        if(((unsigned int)flag) != byte_s->size()) {
            /* FIXME: retry, throw exception, something here */
        }
    } catch (BusException &be) {
        if(NULL != byte_s) {
            delete byte_s;
        }
        delete message;
        string error("Failed to write to bus.");
        /* FIXME: previous exception should probably be bundled up into the new exception */
        /* FIXME: there is probably a more descriptive type for this than ProtocolException */
        throw ProtocolException(error);
    }

    delete message;
    delete byte_s;
    byte_s = NULL;

    try {
        /* Read the 64-byte_ OBP header.  This may indicate that more data
         * must be absorbed afterwards.
         */
        byte_s = new vector<byte_>(64);
        flag = helper->receive(*byte_s, (unsigned) byte_s->size());
        if(((unsigned int)flag) != byte_s->size()) {
            /* FIXME: retry, throw exception, something here */
        }

        /* Parse out the header and see if there is an extended payload. */
        try {
            response = OBPMessage::parseHeaderFrombyte_Stream(byte_s);
        } catch (IllegalArgumentException &iae) {
            response = NULL;
        }
        if(NULL == response || true == response->isNackFlagSet()
                || response->getMessageType() != messageType) {
            if(NULL != byte_s) {
                delete byte_s;
            }

            if(NULL != response) {
                delete response;
            }
            /* There may be a legitimate reason to not return a message
             * (e.g. tried to read an unprogrammed value).  Just return
             * NULL here instead of throwing an exception and let the
             * caller figure it out.
             */
            return NULL;
        }
        unsigned int byte_sToRead = response->getbyte_sRemaining() - 20; /* omit footer and checksum */
        if(byte_sToRead > 0) {
            fullVector = new vector<byte_>(byte_sToRead + byte_s->size());
            /* Safely stl::copy() the header into a full-sized block and
             * delete the existing buffer
             */
            vector<byte_>::iterator iter = copy(byte_s->begin(), byte_s->end(), fullVector->begin());
            delete byte_s;
            byte_s = NULL;
            /* TransferHelper expects a vector, so create a new one for it */
            vector<byte_> *remainder = new vector<byte_>(byte_sToRead);
            flag = helper->receive(*remainder, (unsigned) remainder->size());
            if(((unsigned int)flag) != byte_sToRead) {
                /* FIXME: retry, throw exception, something here */
            }
            copy(remainder->begin(), remainder->end(), iter);
            delete remainder;
        } else {
            fullVector = byte_s;
            byte_s = NULL;  /* To prevent double deletion */
        }
        /* Delete and recreate response using the full message */
        delete response;
        try {
            response = OBPMessage::parsebyte_Stream(fullVector);
        } catch (IllegalArgumentException &iae) {
            response = NULL;
        }
        delete fullVector;
        fullVector = NULL;
    } catch (BusException &be) {
        if(NULL != byte_s) {
            delete byte_s;
        }
        if(NULL != fullVector) {
            delete fullVector;
        }
        string error("Failed to read from bus.");
        /* FIXME: previous exception should probably be bundled up into the new exception */
        /* FIXME: there is probably a more descriptive type for this than ProtocolException */
        throw ProtocolException(error);
    }
    if(NULL == response) {
        /* This could happen if the footer or checksum failed for
         * some reason, but that would be very unusual.  This can only happen
         * if the header was already verified, but there was some error in the
         * rest of the payload.
         */
        string error("Failed to parse extended message");
        throw ProtocolException(error);
    }

    /* Make a copy of the response buffer so response can be deleted */
    byte_s = new vector<byte_>(*response->getData());
    
    delete response;
    return byte_s;
}


bool OBPTransaction::sendCommandToDevice(TransferHelper *helper,
                    unsigned int messageType,
                    vector<byte_> &data) throw (ProtocolException) {

    bool retval = false;
    int flag = 0;
    vector<byte_> *byte_s = NULL;
    OBPMessage *message = new OBPMessage();
    OBPMessage *response = NULL;

    message->setMessageType(messageType);
    message->setAckRequestedFlag();

    /* Need a copy of the input data that can be given to the message. */
    /* Note: copy will be deleted by message when appropriate. */
    message->setData(new vector<byte_>(data));

    try {
        byte_s = message->tobyte_Stream();
        flag = helper->send(*byte_s, (unsigned) byte_s->size());
        if(((unsigned int)flag) != byte_s->size()) {
            /* FIXME: retry, throw exception, something here */
        }
    } catch (BusException &be) {
        if(NULL != byte_s) {
            delete byte_s;
        }
        delete message;
        string error("Failed to write to bus.");
        /* FIXME: previous exception should probably be bundled up into the new exception */
        /* FIXME: there is probably a more descriptive type for this than ProtocolException */
        throw ProtocolException(error);
    }

    delete message;
    delete byte_s;
    byte_s = NULL;

    try {
        /* Read the 64-byte_ OBP header. */
        byte_s = new vector<byte_>(64);
        flag = helper->receive(*byte_s, (unsigned) byte_s->size());
        if(((unsigned int)flag) != byte_s->size()) {
            /* FIXME: retry, throw exception, something here */
        }

        /* Parse out the header. */
        try {
            response = OBPMessage::parseHeaderFrombyte_Stream(byte_s);
        } catch (IllegalArgumentException &iae) {
            response = NULL;
        }
    } catch (BusException &be) {
        if(NULL != byte_s) {
            delete byte_s;
        }
        string error("Failed to read from bus.");
        /* FIXME: previous exception should probably be bundled up into the new exception */
        /* FIXME: there is probably a more descriptive type for this than ProtocolException */
        throw ProtocolException(error);
    }

    delete byte_s;

    if(NULL == response || true == response->isNackFlagSet()
            || response->getMessageType() != messageType) {
        retval = false;
    } else if(true == response->isAckFlagSet()) {
        retval = true;
    } else {
        delete response;
        string error("Illegal device response");
        throw ProtocolException(error);
    }
    delete response;
    return retval;
}
