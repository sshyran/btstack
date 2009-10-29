/*
 * Copyright (C) 2009 by Matthias Ringwald
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY MATTHIAS RINGWALD AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MATTHIAS
 * RINGWALD OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

/*
 *  l2cap.h
 *
 *  Logical Link Control and Adaption Protocl (L2CAP)
 *
 *  Created by Matthias Ringwald on 5/16/09.
 */

#pragma once

#include "hci.h"
#include "l2cap_signaling.h"
#include <btstack/utils.h>
#include "socket_connection.h"

#define L2CAP_SIG_ID_INVALID 0

typedef enum {
    L2CAP_STATE_CLOSED,           // no baseband
    L2CAP_STATE_WAIT_CONNECT,     // from application
    L2CAP_STATE_WAIT_CONNECT_RSP, // from peer
    L2CAP_STATE_WAIT_CONFIG_REQ_RSP,
    L2CAP_STATE_WAIT_CONFIG_REQ,
    L2CAP_STATE_OPEN,
    L2CAP_STATE_WAIT_DISCONNECT,  // from application
} L2CAP_STATE;

typedef struct {
    // linked list - assert: first field
    linked_item_t    item;

    L2CAP_STATE state;
    uint8_t   sig_id;
    uint16_t  source_cid;
    uint16_t  dest_cid;
    bd_addr_t address;
    uint16_t  psm;
    hci_con_handle_t handle;
    connection_t *connection;
    // uint16_t mtu_incoming;
    // uint16_t mtu_outgoing;
    // uint16_t flush_timeout_incoming;
    // uint16_t flush_timeout_outgoing;
} l2cap_channel_t;

typedef struct {
    
} l2cap_service_t;

void l2cap_init();
void l2cap_register_event_packet_handler(void (*handler)(uint8_t *packet, uint16_t size));
void l2cap_create_channel_internal(connection_t * connection, bd_addr_t address, uint16_t psm);
void l2cap_disconnect_internal(uint16_t source_cid, uint8_t reason);
void l2cap_send_internal(uint16_t source_cid, uint8_t *data, uint16_t len);
void l2cap_acl_handler( uint8_t *packet, uint16_t size );
void l2cap_event_handler( uint8_t *packet, uint16_t size );

void l2cap_set_capture_connection(connection_t * connection);

void l2cap_finialize_channel_close(l2cap_channel_t *channel);
void l2cap_close_channels_for_connection(connection_t *connection);

void l2cap_emit_channel_opened(l2cap_channel_t *channel, uint8_t status);
void l2cap_emit_channel_closed(l2cap_channel_t *channel);
