/*
 * libretroshare/src/chat: distantchat.cc
 *
 * Services for RetroShare.
 *
 * Copyright 2014 by Cyril Soler
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License Version 2 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * Please report all bugs and problems to "csoler@users.sourceforge.net".
 *
 */


#include <unistd.h>

#include "openssl/rand.h"
#include "openssl/dh.h"
#include "openssl/err.h"

#include "util/rsaes.h"
#include "util/rsmemory.h"

#include <serialiser/rsmsgitems.h>

#include <retroshare/rsmsgs.h>
#include <retroshare/rsidentity.h>
#include <retroshare/rsiface.h>

#include <rsserver/p3face.h>
#include <services/p3idservice.h>
#include <gxs/gxssecurity.h>
#include <turtle/p3turtle.h>
#include <retroshare/rsids.h>
#include "distantchat.h"

//#define DEBUG_DISTANT_CHAT

static const uint32_t DISTANT_CHAT_KEEP_ALIVE_TIMEOUT = 6 ; // send keep alive packet so as to avoid tunnel breaks.

static const uint32_t RS_DISTANT_CHAT_DH_STATUS_UNINITIALIZED = 0x0000 ;
static const uint32_t RS_DISTANT_CHAT_DH_STATUS_HALF_KEY_DONE = 0x0001 ;
static const uint32_t RS_DISTANT_CHAT_DH_STATUS_KEY_AVAILABLE = 0x0002 ;

static const uint32_t DISTANT_CHAT_GXS_TUNNEL_SERVICE_ID = 0xa0001 ;

typedef RsGxsTunnelService::RsGxsTunnelId RsGxsTunnelId;

void DistantChatService::connectToGxsTunnelService(RsGxsTunnelService *tr)
{
	mGxsTunnels = tr ;
	tr->registerClientService(DISTANT_CHAT_GXS_TUNNEL_SERVICE_ID,this) ;
}

bool DistantChatService::handleOutgoingItem(RsChatItem *item)
{
    RsGxsTunnelId tunnel_id ;
    
    {
        RS_STACK_MUTEX(mDistantChatMtx) ;

        std::map<DistantChatPeerId,DistantChatContact>::const_iterator it=mDistantChatContacts.find(DistantChatPeerId(item->PeerId()));

        if(it == mDistantChatContacts.end())
            return false ;
    }

#ifdef CHAT_DEBUG
    std::cerr << "p3ChatService::handleOutgoingItem(): sending to " << item->PeerId() << ": interpreted as a distant chat virtual peer id." << std::endl;
#endif
    
    uint32_t size = item->serial_size() ;
    RsTemporaryMemory mem(size) ;
    
    if(!item->serialise(mem,size))
    {
        std::cerr << "(EE) serialisation error. Something's really wrong!" << std::endl;
        return false;
    }
    
    mGxsTunnels->sendData( RsGxsTunnelId(item->PeerId()),DISTANT_CHAT_GXS_TUNNEL_SERVICE_ID,mem,size);
    return true;
}

void DistantChatService::handleRecvChatStatusItem(RsChatStatusItem *cs)
{
    if(cs->flags & RS_CHAT_FLAG_CLOSING_DISTANT_CONNECTION)
        markDistantChatAsClosed(DistantChatPeerId(cs->PeerId())) ;

    // nothing more to do, because the decryption routing will update the last_contact time when decrypting.

    if(cs->flags & RS_CHAT_FLAG_KEEP_ALIVE)
        std::cerr << "DistantChatService::handleRecvChatStatusItem(): received keep alive packet for inactive chat! peerId=" << cs->PeerId() << std::endl;
}

void DistantChatService::notifyTunnelStatus(const RsGxsTunnelService::RsGxsTunnelId &tunnel_id, uint32_t tunnel_status)
{
    std::cerr << "DistantChatService::notifyTunnelStatus(): got notification " << std::hex << tunnel_status << std::dec << " for tunnel " << tunnel_id << std::endl;
#warning do something here
}

void DistantChatService::receiveData(const RsGxsTunnelService::RsGxsTunnelId &tunnel_id, unsigned char *data, uint32_t data_size)
{
    std::cerr << "DistantChatService::receiveData(): got data of size " << data_size << " for tunnel " << tunnel_id << std::endl;
#warning do something here
}

void DistantChatService::markDistantChatAsClosed(const DistantChatPeerId& dcpid)
{
    mGxsTunnels->closeExistingTunnel(RsGxsTunnelService::RsGxsTunnelId(dcpid)) ;
    
    RS_STACK_MUTEX(mDistantChatMtx) ;
    
    std::map<DistantChatPeerId,DistantChatContact>::iterator it = mDistantChatContacts.find(dcpid) ;
    
    if(it != mDistantChatContacts.end())
        mDistantChatContacts.erase(it) ;
}

bool DistantChatService::initiateDistantChatConnexion(const RsGxsId& to_gxs_id, const RsGxsId& from_gxs_id, DistantChatPeerId& dcpid, uint32_t& error_code)
{
    RsGxsTunnelId tunnel_id ;
    
    if(!mGxsTunnels->requestSecuredTunnel(to_gxs_id,from_gxs_id,tunnel_id,error_code))
	    return false ;
    
    dcpid = DistantChatPeerId(tunnel_id) ;

    DistantChatContact& dc_contact(mDistantChatContacts[dcpid]) ;

    dc_contact.from_id = from_gxs_id ;
    dc_contact.to_id = to_gxs_id ;

    error_code = RS_DISTANT_CHAT_ERROR_NO_ERROR ;

    return true ;
}

bool DistantChatService::getDistantChatStatus(const DistantChatPeerId& tunnel_id, DistantChatPeerInfo& cinfo) 
{
    RsStackMutex stack(mDistantChatMtx); /********** STACK LOCKED MTX ******/

    RsGxsTunnelService::GxsTunnelInfo tinfo ;

    if(!mGxsTunnels->getTunnelInfo(RsGxsTunnelId(tunnel_id),tinfo))
	    return false;

    cinfo.to_id  = tinfo.destination_gxs_id;
    cinfo.own_id = tinfo.source_gxs_id;
    cinfo.peer_id = tunnel_id;
    cinfo.status = tinfo.tunnel_status;		// see the values in rsmsgs.h

    return true ;
}

bool DistantChatService::closeDistantChatConnexion(const DistantChatPeerId &tunnel_id)
{
    mGxsTunnels->closeExistingTunnel(RsGxsTunnelId(tunnel_id)) ;
    
    // also remove contact. Or do we wait for the notification?
    
    return true ;
}


