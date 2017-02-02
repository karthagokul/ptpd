/* Copyright (c) 2016 Wojciech Owczarek,
 *
 * All Rights Reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file   ttransport_udp_common.h
 * @date   Sat Jan 9 16:14:10 2016
 *
 * @brief  common structure definitions for UDP/IP-based transports
 *
 */

#ifndef CCK_TTRANSPORT_UDP_COMMON_H_
#define CCK_TTRANSPORT_UDP_COMMON_H_

#include <libcck/ttransport.h>
#include <libcck/net_utils.h>
#include <libcck/transport_address.h>

typedef struct {
	TTsocketTimestampConfig tsConfig;
} TTransportData_udp_common;

typedef struct {
	CckTransportAddressList *multicastStreams;
	int _family;
	char interface [IFNAMSIZ];
	CckTransportAddress sourceAddress;
	int multicastTtl;
	int dscpValue;
	uint16_t listenPort;
	uint32_t udpBufferSize;
	bool bindToAddress;
	bool disableChecksums;
} TTransportConfig_udp_common;

/*
 * initialisation / destruction of any extra data in our private config object
 * when we create a global config object outside of the transport
 */
void _initTTransportConfig_udp_common(TTransportConfig_udp_common *myConfig, const int family);
void _freeTTransportConfig_udp_common(TTransportConfig_udp_common *myConfig);

/* any other common functions */
bool initTimestamping_udp_common(TTransport *self,  TTsocketTimestampConfig *config);

#endif /* CCK_TTRANSPORT_UDP_COMMON_H_ */
