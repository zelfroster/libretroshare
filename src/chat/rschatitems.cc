
/*
 * libretroshare/src/serialiser: rsbaseitems.cc
 *
 * RetroShare Serialiser.
 *
 * Copyright 2007-2008 by Robert Fernie.
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
 * Please report all bugs and problems to "retroshare@lunamutt.com".
 *
 */

#include <stdexcept>
#include <time.h>
#include "serialiser/rsbaseserial.h"
#include "serialiser/rstlvbase.h"

#include "serialization/rstypeserializer.h"

#include "chat/rschatitems.h"

//#define CHAT_DEBUG 1

static const uint32_t RS_CHAT_SERIALIZER_FLAGS_NO_SIGNATURE = 0x0001;

#ifdef TO_BE_REMOVED
std::ostream& RsChatMsgItem::print(std::ostream &out, uint16_t indent)
{
	printRsItemBase(out, "RsChatMsgItem", indent);
	uint16_t int_Indent = indent + 2;
	printIndent(out, int_Indent);
	out << "QblogMs " << chatFlags << std::endl;

	printIndent(out, int_Indent);
	out << "sendTime:  " << sendTime  << " (" << time(NULL)-sendTime << " secs ago)" << std::endl;

	printIndent(out, int_Indent);

	std::string cnv_message(message.begin(), message.end());
	out << "msg:  " << cnv_message  << std::endl;

	printRsItemEnd(out, "RsChatMsgItem", indent);
	return out;
}

std::ostream& RsChatLobbyListItem::print(std::ostream &out, uint16_t indent)
{
	printRsItemBase(out, "RsChatLobbyListItem", indent);

    for(uint32_t i=0;i<lobbies.size();++i)
	{
		printIndent(out, indent+2);
        out << "lobby 0x" << std::hex << lobbies[i].id << std::dec << " (name=\"" << lobbies[i].name << "\", topic="<< lobbies[i].topic
            << "\", count=" << lobbies[i].count << ", flags = " << lobbies[i].flags << std::endl ;
	}

	printRsItemEnd(out, "RsChatLobbyListItem", indent);
	return out;
}
std::ostream& RsChatLobbyListRequestItem::print(std::ostream &out, uint16_t indent)
{
	printRsItemBase(out, "RsChatLobbyListRequestItem", indent);
	printRsItemEnd(out, "RsChatLobbyListRequestItem", indent);
	return out;
}
std::ostream& RsChatLobbyBouncingObject::print(std::ostream &out, uint16_t indent)
{
	printIndent(out, indent); out << "Lobby ID: " << std::hex << lobby_id << std::dec << std::endl;
	printIndent(out, indent); out << "Msg ID: " << std::hex << msg_id << std::dec << std::endl;
	printIndent(out, indent); out << "Nick: " << nick << std::endl;
    printIndent(out, indent); out << "Sign: " << signature.keyId << std::endl;

	return out;
}
std::ostream& RsChatLobbyUnsubscribeItem::print(std::ostream &out, uint16_t indent)
{
	printRsItemBase(out, "RsChatLobbyUnsubscribeItem", indent);
	printIndent(out, indent);
	out << "Lobby id: " << std::hex << lobby_id << std::dec << std::endl;
	printRsItemEnd(out, "RsChatLobbyUnsubscribeItem", indent);
	return out;
}
std::ostream& RsChatLobbyEventItem::print(std::ostream &out, uint16_t indent)
{
	printRsItemBase(out, "RsChatLobbyEventItem", indent);
	RsChatLobbyBouncingObject::print(out,indent) ;
	printIndent(out, indent); out << "Event type  : " << event_type  << std::endl;
	printIndent(out, indent); out << "String param: " << string1  << std::endl;
	printIndent(out, indent); out << "Send time: " << sendTime  << " (" << time(NULL)-sendTime << " secs ago)" << std::endl;
	printRsItemEnd(out, "RsChatLobbyEventItem", indent);
	return out;
} 
std::ostream& RsChatLobbyConnectChallengeItem::print(std::ostream &out, uint16_t indent)
{
	printRsItemBase(out, "RsChatLobbyConnectChallengeItem", indent);
	printIndent(out, indent);
	out << "Challenge Code: " << std::hex << challenge_code << std::dec << std::endl;
	printRsItemEnd(out, "RsChatLobbyConnectChallengeItem", indent);
	return out;
}
std::ostream& RsChatLobbyMsgItem::print(std::ostream &out, uint16_t indent)
{
	RsChatMsgItem::print(out,indent) ;
	RsChatLobbyBouncingObject::print(out,indent) ;

	printRsItemEnd(out, "RsChatLobbyMsgItem", indent);
	return out;
}

std::ostream& RsChatLobbyInviteItem::print(std::ostream &out, uint16_t indent)
{
	printRsItemBase(out, "RsChatLobbyInviteItem", indent);
	uint16_t int_Indent = indent + 2;

	printIndent(out, int_Indent);
	out << "peerId:  " << PeerId()  << std::endl;

	printIndent(out, int_Indent);
	out << "lobby id: " << std::hex << lobby_id << std::dec << std::endl;

	printIndent(out, int_Indent);
	out << "lobby name: " << lobby_name << std::endl;
	
	printIndent(out, int_Indent);
	out << "lobby topic: " << lobby_topic << std::endl;

	printRsItemEnd(out, "RsChatLobbyInviteItem", indent);
	return out;
}
std::ostream& RsPrivateChatMsgConfigItem::print(std::ostream &out, uint16_t indent)
{
	printRsItemBase(out, "RsPrivateChatMsgConfigItem", indent);
	uint16_t int_Indent = indent + 2;

	out << "peerId:  " << configPeerId  << std::endl;

	printIndent(out, int_Indent);
	out << "QblogMs " << chatFlags << std::endl;

	printIndent(out, int_Indent);
	out << "QblogMs " << configFlags << std::endl;

	printIndent(out, int_Indent);
	out << "sendTime:  " << sendTime  << " (" << time(NULL)-sendTime << " secs ago)" << std::endl;

	printIndent(out, int_Indent);

	std::string cnv_message(message.begin(), message.end());
	out << "msg:  " << cnv_message  << std::endl;

	printRsItemEnd(out, "RsPrivateChatMsgConfigItem", indent);
	return out;
}
std::ostream& RsPrivateChatDistantInviteConfigItem::print(std::ostream &out, uint16_t indent)
{
	printRsItemBase(out, "RsPrivateChatDistantInviteConfigItem", indent);
	uint16_t int_Indent = indent + 2;

	printIndent(out, int_Indent);
	out << "radix string: " << encrypted_radix64_string << std::endl;

	printIndent(out, int_Indent);
	out << "hash: " << hash << std::endl;

	printIndent(out, int_Indent);
	out << "destination pgp_id:  " << destination_pgp_id  << std::endl;

	printIndent(out, int_Indent);
	out << "time of validity:  " << time_of_validity  << std::endl;

	printIndent(out, int_Indent);
	out << "time of last hit:  " << last_hit_time  << std::endl;

	printIndent(out, int_Indent);
	out << "flags:  " << flags  << std::endl;

	printRsItemEnd(out, "RsPrivateChatDistantInviteConfigItem", indent);
	return out;
}
std::ostream& RsChatLobbyConfigItem::print(std::ostream &out, uint16_t indent)
{
    printRsItemBase(out, "RsChatLobbyConfigItem", indent);
    uint16_t int_Indent = indent + 2;

    printIndent(out, int_Indent);
    out << "lobby_Id: " << lobby_Id << std::endl;
    out << "flags   : " << flags << std::endl;

    printRsItemEnd(out, "RsChatLobbyConfigItem", indent);
    return out;
}
std::ostream& RsChatStatusItem::print(std::ostream &out, uint16_t indent)
{
	printRsItemBase(out, "RsChatStatusItem", indent);
	uint16_t int_Indent = indent + 2;
	printIndent(out, int_Indent);
	out << "Status string: " << status_string << std::endl;
	out << "Flags : " << std::hex << flags << std::dec << std::endl;

	printRsItemEnd(out, "RsChatStatusItem", indent);
	return out;
}

std::ostream& RsChatAvatarItem::print(std::ostream &out, uint16_t indent)
{
	printRsItemBase(out, "RsChatAvatarItem", indent);
	uint16_t int_Indent = indent + 2;
	printIndent(out, int_Indent);
	out << "Image size: " << image_size << std::endl;
	printRsItemEnd(out, "RsChatStatusItem", indent);

	return out;
}
#endif

RsItem *RsChatSerialiser::create_item(uint16_t service_id,uint8_t item_sub_id)
{
    if(service_id != RS_SERVICE_TYPE_CHAT)
        return NULL ;

	switch(item_sub_id)
	{
        case RS_PKT_SUBTYPE_DEFAULT:				return new RsChatMsgItem() ;
		case RS_PKT_SUBTYPE_PRIVATECHATMSG_CONFIG:	return new RsPrivateChatMsgConfigItem() ;
        case RS_PKT_SUBTYPE_CHAT_STATUS:			return new RsChatStatusItem() ;
        case RS_PKT_SUBTYPE_CHAT_AVATAR:			return new RsChatAvatarItem() ;
		case RS_PKT_SUBTYPE_CHAT_LOBBY_SIGNED_MSG:	return new RsChatLobbyMsgItem() ;
        case RS_PKT_SUBTYPE_CHAT_LOBBY_INVITE:		return new RsChatLobbyInviteItem() ;
        case RS_PKT_SUBTYPE_CHAT_LOBBY_CHALLENGE:	return new RsChatLobbyConnectChallengeItem() ;
		case RS_PKT_SUBTYPE_CHAT_LOBBY_UNSUBSCRIBE:	return new RsChatLobbyUnsubscribeItem() ;
        case RS_PKT_SUBTYPE_CHAT_LOBBY_SIGNED_EVENT:return new RsChatLobbyEventItem() ;
		case RS_PKT_SUBTYPE_CHAT_LOBBY_LIST_REQUEST:return new RsChatLobbyListRequestItem() ;
		case RS_PKT_SUBTYPE_CHAT_LOBBY_LIST:        return new RsChatLobbyListItem() ;
        case RS_PKT_SUBTYPE_CHAT_LOBBY_CONFIG:  	return new RsChatLobbyConfigItem() ;
		default:
			std::cerr << "Unknown packet type in chat!" << std::endl ;
			return NULL ;
	}
}

void RsChatMsgItem::serial_process(RsItem::SerializeJob j,SerializeContext& ctx)
{
    RsTypeSerializer::TlvString_proxy tt(message,TLV_TYPE_STR_MSG) ;

    RsTypeSerializer::serial_process(j,ctx,chatFlags,"chatflags") ;
    RsTypeSerializer::serial_process(j,ctx,sendTime,"sendTime") ;
    RsTypeSerializer::serial_process(j,ctx,tt,"message") ;
}

#ifdef TO_BE_REMOVED
uint32_t RsChatMsgItem::serial_size()
{
	uint32_t s = 8; /* header */
	s += 4; /* chatFlags */
    s += 4; /* sendTime  */
	s += GetTlvStringSize(message);

	return s;
}

uint32_t RsChatLobbyUnsubscribeItem::serial_size()
{
	uint32_t s = 8; /* header */
	s += 8;			// challenge code
	return s ;
}


uint32_t RsChatLobbyConnectChallengeItem::serial_size()
{
	uint32_t s = 8; /* header */
	s += 8;			// challenge code
	return s ;
}
uint32_t RsChatLobbyBouncingObject::serialized_size(bool include_signature)
{
	uint32_t s = 0;              // no header!
	s += 8;                      // lobby_id
	s += 8;                      // msg_id
	s += GetTlvStringSize(nick); // nick

	if (include_signature)
		s += signature.TlvSize();  // signature

	return s ;
}
uint32_t RsChatLobbyEventItem::signed_serial_size()
{
    uint32_t s = 8 ;	// header
    s += 1 ; // event_type
    s += GetTlvStringSize(string1) ;	// string1
    s += 4 ; // send time
    s += RsChatLobbyBouncingObject::serialized_size(false) ;

    return s ;
}
uint32_t RsChatLobbyEventItem::serial_size()
{
	uint32_t s = 8 ;	// header
	s += 1 ; // event_type
	s += GetTlvStringSize(string1) ;	// string1
	s += 4 ; // send time
    s += RsChatLobbyBouncingObject::serialized_size(true) ;

	return s ;
}
uint32_t RsChatLobbyListRequestItem::serial_size()
{
	uint32_t s = 8 ;	// header
	return s ;
}
uint32_t RsChatLobbyListItem::serial_size()
{
    uint32_t s = 8 ;	// header
    s += 4 ; 		// number of elements in the vectors

    for(uint32_t i=0;i<lobbies.size();++i)
    {
        s +=  8 ;	// id
        s += GetTlvStringSize(lobbies[i].name) ;	// lobby_names
        s += GetTlvStringSize(lobbies[i].topic) ;	// lobby_topics
        s += 4 ;	// lobby_counts
        s += 4 ;	// lobby_flags
    }
    return s ;
}
uint32_t RsChatLobbyMsgItem::serial_size()
{
    uint32_t s = RsChatMsgItem::serial_size() ;	// parent
    s += 8;						// parent_msg_id
    s += RsChatLobbyBouncingObject::serialized_size(true) ;

	return s;
}
uint32_t RsChatLobbyMsgItem::signed_serial_size()
{
    uint32_t s = RsChatMsgItem::serial_size() ;	// parent
    s += 8;						// parent_msg_id
    s += RsChatLobbyBouncingObject::serialized_size(false) ;

    return s;
}
uint32_t RsChatLobbyInviteItem::serial_size()
{
	uint32_t s = 8; /* header */
	s += 8;											// lobby_id
	s += GetTlvStringSize(lobby_name) ;		// lobby_name
	s += 4;											// lobby_privacy_level

	return s;
}
uint32_t RsPrivateChatMsgConfigItem::serial_size()
{
	uint32_t s = 8; /* header */
	s += 4; /* version */
	s += configPeerId.serial_size();
	s += 4; /* chatFlags */
	s += 4; /* configFlags */
	s += 4; /* sendTime  */
	s += GetTlvStringSize(message);
	s += 4; /* recvTime  */

	return s;
}
uint32_t RsPrivateChatDistantInviteConfigItem::serial_size()
{
	uint32_t s = 8; /* header */
    s += hash.serial_size();
	s += GetTlvStringSize(encrypted_radix64_string);
	s += destination_pgp_id.serial_size();
	s += 16; /* aes_key */
	s += 4; /* time_of_validity */
	s += 4; /* last_hit_time  */
	s += 4; /* flags  */

	return s;
}
uint32_t RsChatStatusItem::serial_size()
{
	uint32_t s = 8; /* header */
	s += 4 ; // flags
	s += GetTlvStringSize(status_string); 			 /* status */

	return s;
}

uint32_t RsChatAvatarItem::serial_size()
{
	uint32_t s = 8; /* header */
	s += 4 ;						// size
	s += image_size ;			// data

	return s;
}
uint32_t RsChatLobbyConfigItem::serial_size()
{
    uint32_t s = 8; /* header */
    s += 8;/* lobby_Id */
    s += 4;/* flags */

    return s;
}
#endif

/*************************************************************************/

RsChatAvatarItem::~RsChatAvatarItem()
{
	if(image_data != NULL)
	{
		delete[] image_data ;
		image_data = NULL ;
	}
}

#ifdef TO_BE_REMOVED
/* serialise the data to the buffer */
bool RsChatMsgItem::serialise(void *data, uint32_t& pktsize)
{
	uint32_t tlvsize = RsChatMsgItem::serial_size() ;
	uint32_t offset = 0;

	if (pktsize < tlvsize)
		return false; /* not enough space */

	pktsize = tlvsize;

	bool ok = true;

	ok &= setRsItemHeader(data, tlvsize, PacketId(), tlvsize);

#ifdef CHAT_DEBUG
	std::cerr << "RsChatSerialiser::serialiseItem() Header: " << ok << std::endl;
	std::cerr << "RsChatSerialiser::serialiseItem() Size: " << tlvsize << std::endl;
#endif

	/* skip the header */
	offset += 8;

	/* add mandatory parts first */
	ok &= setRawUInt32(data, tlvsize, &offset, chatFlags);
	ok &= setRawUInt32(data, tlvsize, &offset, sendTime);
	ok &= SetTlvString(data, tlvsize, &offset, TLV_TYPE_STR_MSG, message);
#ifdef CHAT_DEBUG
	std::cerr << "Serialized the following message:" << std::endl;
	std::cerr << "========== BEGIN MESSAGE =========" << std::endl;
	for(uint32_t i=0;i<message.length();++i)
		std::cerr << (char)message[i] ;
	std::cerr << std::endl;
	std::cerr << "=========== END MESSAGE ==========" << std::endl;
#endif

	if (offset != tlvsize)
	{
		ok = false;
		std::cerr << "RsChatSerialiser::serialiseItem() Size Error! " << std::endl;
	}
#ifdef CHAT_DEBUG
	std::cerr << "computed size: " << 256*((unsigned char*)data)[6]+((unsigned char*)data)[7] << std::endl ;
#endif
	return ok ;
}
#endif

void RsChatLobbyBouncingObject::serial_process(RsItem::SerializeJob j, SerializeContext& ctx, bool include_signature)
{
    RsTypeSerializer::TlvString_proxy tt(nick,TLV_TYPE_STR_NAME) ;

    RsTypeSerializer::serial_process(j,ctx,lobby_id,"lobby_id") ;
    RsTypeSerializer::serial_process(j,ctx,msg_id  ,"msg_id") ;
    RsTypeSerializer::serial_process(j,ctx,tt      ,"nick") ;

    if(include_signature)
    	RsTypeSerializer::serial_process(j,ctx,signature,"signature") ;
}

void RsChatLobbyMsgItem::serial_process(RsItem::SerializeJob j,SerializeContext& ctx)
{
    RsChatMsgItem::serial_process(j,ctx) ;
    RsTypeSerializer::serial_process(j,ctx,parent_msg_id,"parent_msg_id") ;
    RsChatLobbyBouncingObject::serial_process(j,ctx,true) ;
}
void RsChatLobbyMsgItem::serial_process_for_signature(RsItem::SerializeJob j,SerializeContext& ctx)
{
    RsChatMsgItem::serial_process(j,ctx) ;
    RsTypeSerializer::serial_process(j,ctx,parent_msg_id,"parent_msg_id") ;
    RsChatLobbyBouncingObject::serial_process(j,ctx,false) ;
}
#ifdef TO_BE_REMOVED
/* serialise the data to the buffer */
bool RsChatLobbyMsgItem::serialise(void *data, uint32_t& pktsize)
{
	uint32_t tlvsize = serial_size() ;

	if (pktsize < tlvsize)
		return false; /* not enough space */

	bool ok = true;
	ok &= RsChatMsgItem::serialise(data,pktsize) ;						// first, serialize parent

	uint32_t offset = pktsize;
	ok &= setRsItemHeader(data, tlvsize, PacketId(), tlvsize);		// correct header!
	pktsize = tlvsize;

	ok &= setRawUInt64(data, tlvsize, &offset, parent_msg_id);

    // The signature is at the end of the serialised data, so that the signed data is *before* the signature.

    ok &= RsChatLobbyBouncingObject::serialise_to_memory(data,tlvsize,offset,true) ;

    /* add mandatory parts first */
	if (offset != tlvsize)
	{
		ok = false;
		std::cerr << "RsChatSerialiser::serialiseItem() Size Error! " << std::endl;
	}
#ifdef CHAT_DEBUG
	std::cerr << "computed size: " << 256*((unsigned char*)data)[6]+((unsigned char*)data)[7] << std::endl ;
#endif
	return ok ;
}

/* serialise the data to the buffer */
bool RsChatLobbyMsgItem::serialise_signed_part(void *data, uint32_t& pktsize)
{
    uint32_t tlvsize = signed_serial_size() ;

    if (pktsize < tlvsize)
        return false; /* not enough space */

    bool ok = true;
    ok &= RsChatMsgItem::serialise(data,pktsize) ;						// first, serialize parent

    uint32_t offset = pktsize;
    ok &= setRsItemHeader(data, tlvsize, PacketId(), tlvsize);		// correct header!
    pktsize = tlvsize;

    //ok &= setRawUInt8(data, tlvsize, &offset, subpacket_id);	// don't serialise sub-part id.
    ok &= setRawUInt64(data, tlvsize, &offset, parent_msg_id);

    // The signature is at the end of the serialised data, so that the signed data is *before* the signature.

    ok &= RsChatLobbyBouncingObject::serial_process_special(data,tlvsize,offset,false) ;

    /* add mandatory parts first */
    if (offset != tlvsize)
    {
        ok = false;
        std::cerr << "RsChatSerialiser::serialiseItem() Size Error! " << std::endl;
    }
#ifdef CHAT_DEBUG
    std::cerr << "computed size: " << 256*((unsigned char*)data)[6]+((unsigned char*)data)[7] << std::endl ;
#endif
    return ok ;
}
#endif

void RsChatLobbyListRequestItem::serial_process(RsItem::SerializeJob j,SerializeContext& ctx)
{
    // nothing to do. This is an empty item.
}

#ifdef TO_BE_REMOVED
bool RsChatLobbyListRequestItem::serialise(void *data, uint32_t& pktsize)
{
	uint32_t tlvsize = serial_size() ;
	bool ok = true ;
	ok &= setRsItemHeader(data, tlvsize, PacketId(), tlvsize);		// correct header!

	if (pktsize < tlvsize)
		return false; /* not enough space */

	pktsize = tlvsize ;
	return ok ;
}
#endif

template<> void RsTypeSerializer::serial_process(RsItem::SerializeJob j,SerializeContext& ctx,VisibleChatLobbyInfo& info,const std::string& name)
{
	RsTypeSerializer::serial_process<uint64_t>(j,ctx,info.id,"info.id") ;

    TlvString_proxy tt1(info.name ,TLV_TYPE_STR_NAME) ;
    TlvString_proxy tt2(info.topic,TLV_TYPE_STR_NAME) ;

	RsTypeSerializer::serial_process(j,ctx,tt1,"info.name") ;
	RsTypeSerializer::serial_process(j,ctx,tt2,"info.topic") ;
	RsTypeSerializer::serial_process<uint32_t>(j,ctx,info.count,"info.count") ;
	RsTypeSerializer::serial_process(j,ctx,info.flags,"info.flags") ;
}

void RsChatLobbyListItem::serial_process(RsItem::SerializeJob j,SerializeContext& ctx)
{
    RsTypeSerializer::serial_process(j,ctx,lobbies,"lobbies") ;
}

#ifdef TO_BE_REMOVED
bool RsChatLobbyListItem::serialise(void *data, uint32_t& pktsize)
{
	uint32_t tlvsize = serial_size() ;
	bool ok = true ;
	ok &= setRsItemHeader(data, tlvsize, PacketId(), tlvsize);		// correct header!

	if (pktsize < tlvsize)
		return false; /* not enough space */

	pktsize = tlvsize ;

	uint32_t offset = 8 ;
    ok &= setRawUInt32(data, tlvsize, &offset, lobbies.size());

    for(uint32_t i=0;i<lobbies.size();++i)
	{
        ok &= setRawUInt64(data, tlvsize, &offset, lobbies[i].id);
        ok &= SetTlvString(data, tlvsize, &offset, TLV_TYPE_STR_NAME, lobbies[i].name);
        ok &= SetTlvString(data, tlvsize, &offset, TLV_TYPE_STR_NAME, lobbies[i].topic);
        ok &= setRawUInt32(data, tlvsize, &offset, lobbies[i].count);
        ok &= setRawUInt32(data, tlvsize, &offset, lobbies[i].flags.toUInt32());
	}
	if (offset != tlvsize)
	{
		ok = false;
		std::cerr << "RsChatSerialiser::serialiseItem() Size Error! " << std::endl;
	}
	return ok ;
}
#endif

void RsChatLobbyEventItem::serial_process(RsItem::SerializeJob j,SerializeContext& ctx)
{
    RsTypeSerializer::TlvString_proxy tt(string1,TLV_TYPE_STR_NAME) ;

    RsTypeSerializer::serial_process<uint8_t>(j,ctx,event_type,"event_type") ;
    RsTypeSerializer::serial_process         (j,ctx,tt        ,"string1") ;
    RsTypeSerializer::serial_process<uint32_t>(j,ctx,sendTime ,"sendTime") ;

    RsChatLobbyBouncingObject::serial_process(j,ctx,true) ;
}
void RsChatLobbyEventItem::serial_process_for_signature(RsItem::SerializeJob j,SerializeContext& ctx)
{
    RsTypeSerializer::TlvString_proxy tt(string1,TLV_TYPE_STR_NAME) ;

    RsTypeSerializer::serial_process<uint8_t>(j,ctx,event_type,"event_type") ;
    RsTypeSerializer::serial_process         (j,ctx,tt        ,"string1") ;
    RsTypeSerializer::serial_process<uint32_t>(j,ctx,sendTime ,"sendTime") ;

    RsChatLobbyBouncingObject::serial_process(j,ctx,false) ;
}
#ifdef TO_BE_REMOVED
bool RsChatLobbyEventItem::serialise(void *data, uint32_t& pktsize)
{
    uint32_t tlvsize = serial_size() ;
    bool ok = true ;
    ok &= setRsItemHeader(data, tlvsize, PacketId(), tlvsize);		// correct header!

    if (pktsize < tlvsize)
        return false; /* not enough space */

    uint32_t offset = 8 ;

    ok &= setRawUInt8(data, tlvsize, &offset, event_type);
    ok &= SetTlvString(data, tlvsize, &offset, TLV_TYPE_STR_NAME, string1);
    ok &= setRawUInt32(data, tlvsize, &offset, sendTime);

    ok &= RsChatLobbyBouncingObject::serialise_to_memory(data,tlvsize,offset,true) ;	// at the end, serialize parent

    pktsize = tlvsize ;

    /* add mandatory parts first */
    if (offset != tlvsize)
    {
        ok = false;
        std::cerr << "RsChatSerialiser::serialiseItem() Size Error! " << std::endl;
    }
#ifdef CHAT_DEBUG
    std::cerr << "computed size: " << 256*((unsigned char*)data)[6]+((unsigned char*)data)[7] << std::endl ;
#endif
    return ok ;
}
bool RsChatLobbyEventItem::serialise_signed_part(void *data, uint32_t& pktsize)
{
    uint32_t tlvsize = signed_serial_size() ;
	bool ok = true ;
	ok &= setRsItemHeader(data, tlvsize, PacketId(), tlvsize);		// correct header!

	if (pktsize < tlvsize)
		return false; /* not enough space */

	uint32_t offset = 8 ;

	ok &= setRawUInt8(data, tlvsize, &offset, event_type);
	ok &= SetTlvString(data, tlvsize, &offset, TLV_TYPE_STR_NAME, string1);
	ok &= setRawUInt32(data, tlvsize, &offset, sendTime);

    ok &= RsChatLobbyBouncingObject::serialise_to_memory(data,tlvsize,offset,false) ;	// at the end, serialize parent

    pktsize = tlvsize ;

	/* add mandatory parts first */
	if (offset != tlvsize)
	{
		ok = false;
		std::cerr << "RsChatSerialiser::serialiseItem() Size Error! " << std::endl;
	}
#ifdef CHAT_DEBUG
	std::cerr << "computed size: " << 256*((unsigned char*)data)[6]+((unsigned char*)data)[7] << std::endl ;
#endif
	return ok ;
}
#endif

void RsChatLobbyUnsubscribeItem::serial_process(RsItem::SerializeJob j,SerializeContext& ctx)
{
    RsTypeSerializer::serial_process<uint64_t>(j,ctx,lobby_id,"lobby_id") ;
}

#ifdef TO_BE_REMOVED
bool RsChatLobbyUnsubscribeItem::serialise(void *data, uint32_t& pktsize)
{
	uint32_t tlvsize = serial_size() ;

	if (pktsize < tlvsize)
		return false; /* not enough space */

	bool ok = true ;
	ok &= setRsItemHeader(data, tlvsize, PacketId(), tlvsize);		// correct header!
	uint32_t offset = 8 ;

	ok &= setRawUInt64(data, tlvsize, &offset, lobby_id);

	if (offset != tlvsize)
	{
		ok = false;
		std::cerr << "RsChatSerialiser::serialiseItem() Size Error! " << std::endl;
	}
#ifdef CHAT_DEBUG
	std::cerr << "computed size: " << 256*((unsigned char*)data)[6]+((unsigned char*)data)[7] << std::endl ;
#endif
	pktsize = tlvsize ;
	return ok ;
}
#endif

void RsChatLobbyConnectChallengeItem::serial_process(RsItem::SerializeJob j,SerializeContext& ctx)
{
    RsTypeSerializer::serial_process<uint64_t>(j,ctx,challenge_code,"challenge_code") ;
}

#ifdef TO_BE_REMOVED
bool RsChatLobbyConnectChallengeItem::serialise(void *data, uint32_t& pktsize)
{
	uint32_t tlvsize = serial_size() ;

	if (pktsize < tlvsize)
		return false; /* not enough space */

	bool ok = true ;
	ok &= setRsItemHeader(data, tlvsize, PacketId(), tlvsize);		// correct header!
	uint32_t offset = 8 ;

	ok &= setRawUInt64(data, tlvsize, &offset, challenge_code);

	if (offset != tlvsize)
	{
		ok = false;
		std::cerr << "RsChatSerialiser::serialiseItem() Size Error! " << std::endl;
	}
#ifdef CHAT_DEBUG
	std::cerr << "computed size: " << 256*((unsigned char*)data)[6]+((unsigned char*)data)[7] << std::endl ;
#endif
	pktsize = tlvsize ;
	return ok ;
}
#endif

void RsChatLobbyInviteItem::serial_process(RsItem::SerializeJob j,SerializeContext& ctx)
{
    RsTypeSerializer::serial_process<uint64_t>(j,ctx,lobby_id,"lobby_id") ;

    RsTypeSerializer::TlvString_proxy s(lobby_name,TLV_TYPE_STR_NAME) ;

    RsTypeSerializer::serial_process(j,ctx,s,"lobby_name") ;
    RsTypeSerializer::serial_process(j,ctx,lobby_flags,"lobby_flags") ;
}

#ifdef TO_BE_REMOVED
bool RsChatLobbyInviteItem::serialise(void *data, uint32_t& pktsize)
{
	uint32_t tlvsize = serial_size() ;

	if (pktsize < tlvsize)
		return false; /* not enough space */

	bool ok = true ;
	ok &= setRsItemHeader(data, tlvsize, PacketId(), tlvsize);		// correct header!
	uint32_t offset = 8 ;

	ok &= setRawUInt64(data, tlvsize, &offset, lobby_id);
	ok &= SetTlvString(data, tlvsize, &offset, TLV_TYPE_STR_NAME, lobby_name);
    ok &= setRawUInt32(data, tlvsize, &offset, lobby_flags.toUInt32());

	if (offset != tlvsize)
	{
		ok = false;
		std::cerr << "RsChatSerialiser::serialiseItem() Size Error! " << std::endl;
	}
#ifdef CHAT_DEBUG
	std::cerr << "computed size: " << 256*((unsigned char*)data)[6]+((unsigned char*)data)[7] << std::endl ;
#endif
	pktsize = tlvsize ;
	return ok ;
}
#endif

void RsPrivateChatMsgConfigItem::serial_process(RsItem::SerializeJob j,SerializeContext& ctx)
{
    uint32_t x=0 ;
    RsTypeSerializer::TlvString_proxy s(message,TLV_TYPE_STR_MSG) ;

    RsTypeSerializer::serial_process<uint32_t>(j,ctx,x,"place holder value") ;
    RsTypeSerializer::serial_process          (j,ctx,configPeerId,"configPeerId") ;
    RsTypeSerializer::serial_process<uint32_t>(j,ctx,chatFlags,"chatFlags") ;
    RsTypeSerializer::serial_process<uint32_t>(j,ctx,sendTime,"sendTime") ;
    RsTypeSerializer::serial_process          (j,ctx,s,"message") ;
    RsTypeSerializer::serial_process<uint32_t>(j,ctx,recvTime,"recvTime") ;
}

#ifdef TO_BE_REMOVED
bool RsPrivateChatMsgConfigItem::serialise(void *data, uint32_t& pktsize)
{
	uint32_t tlvsize = serial_size() ;
	uint32_t offset = 0;

	if (pktsize < tlvsize)
		return false; /* not enough space */

	pktsize = tlvsize;

	bool ok = true;

	ok &= setRsItemHeader(data, tlvsize, PacketId(), tlvsize);

#ifdef CHAT_DEBUG
	std::cerr << "RsChatSerialiser::serialiseItem() Header: " << ok << std::endl;
	std::cerr << "RsChatSerialiser::serialiseItem() Size: " << tlvsize << std::endl;
#endif

	/* skip the header */
	offset += 8;

	/* add mandatory parts first */
	ok &= setRawUInt32(data, tlvsize, &offset, 0);
	ok &= configPeerId.serialise(data, tlvsize, offset) ;
	ok &= setRawUInt32(data, tlvsize, &offset, chatFlags);
	ok &= setRawUInt32(data, tlvsize, &offset, configFlags);
	ok &= setRawUInt32(data, tlvsize, &offset, sendTime);
	ok &= SetTlvString(data, tlvsize, &offset, TLV_TYPE_STR_MSG, message);
	ok &= setRawUInt32(data, tlvsize, &offset, recvTime);

	if (offset != tlvsize)
	{
		ok = false;
		std::cerr << "RsChatSerialiser::serialiseItem() Size Error! " << std::endl;
	}

	return ok;
}

bool RsPrivateChatDistantInviteConfigItem::serialise(void *data, uint32_t& pktsize)
{
	uint32_t tlvsize = serial_size() ;
	uint32_t offset = 0;

	if (pktsize < tlvsize)
		return false; /* not enough space */

	pktsize = tlvsize;

	bool ok = true;

	ok &= setRsItemHeader(data, tlvsize, PacketId(), tlvsize);

#ifdef CHAT_DEBUG
	std::cerr << "RsChatSerialiser::serialiseItem() Header: " << ok << std::endl;
	std::cerr << "RsChatSerialiser::serialiseItem() Size: " << tlvsize << std::endl;
#endif

	/* skip the header */
	offset += 8;

	/* add mandatory parts first */
    ok &= hash.serialise(data, tlvsize, offset) ;
	ok &= SetTlvString(data, tlvsize, &offset, TLV_TYPE_STR_LINK, encrypted_radix64_string);
	ok &= destination_pgp_id.serialise(data, tlvsize, offset);

	memcpy(&((unsigned char *)data)[offset],aes_key,16) ;
	offset += 16 ;

	ok &= setRawUInt32(data, tlvsize, &offset, time_of_validity);
	ok &= setRawUInt32(data, tlvsize, &offset, last_hit_time);
	ok &= setRawUInt32(data, tlvsize, &offset, flags);

	if (offset != tlvsize)
	{
		ok = false;
		std::cerr << "RsChatSerialiser::serialiseItem() Size Error! " << std::endl;
	}

	return ok;
}
#endif

void RsChatStatusItem::serial_process(RsItem::SerializeJob j,SerializeContext& ctx)
{
    RsTypeSerializer::serial_process(j,ctx,flags,"flags") ;

    RsTypeSerializer::TlvString_proxy tt(status_string,TLV_TYPE_STR_MSG) ;

    RsTypeSerializer::serial_process(j,ctx,tt,"status_string") ;
}

#ifdef TO_BE_REMOVED
bool RsChatStatusItem::serialise(void *data, uint32_t& pktsize)
{
	uint32_t tlvsize = serial_size() ;
	uint32_t offset = 0;

	if (pktsize < tlvsize)
		return false; /* not enough space */

	pktsize = tlvsize;

	bool ok = true;

	ok &= setRsItemHeader(data, tlvsize, PacketId(), tlvsize);

#ifdef CHAT_DEBUG
	std::cerr << "RsChatSerialiser serialising chat status item." << std::endl;
	std::cerr << "RsChatSerialiser::serialiseItem() Header: " << ok << std::endl;
	std::cerr << "RsChatSerialiser::serialiseItem() Size: " << tlvsize << std::endl;
#endif

	/* skip the header */
	offset += 8;

	/* add mandatory parts first */
	ok &= setRawUInt32(data, tlvsize, &offset, flags);
	ok &= SetTlvString(data, tlvsize, &offset,TLV_TYPE_STR_MSG, status_string);

	if (offset != tlvsize)
	{
		ok = false;
		std::cerr << "RsChatSerialiser::serialiseItem() Size Error! " << std::endl;
	}
#ifdef CHAT_DEBUG
	std::cerr << "computed size: " << 256*((unsigned char*)data)[6]+((unsigned char*)data)[7] << std::endl ;
#endif

	return ok;
}
#endif

void RsChatAvatarItem::serial_process(RsItem::SerializeJob j,SerializeContext& ctx)
{
    RsTypeSerializer::TlvMemBlock_proxy b(image_data,image_size) ;

    RsTypeSerializer::serial_process(j,ctx,b,"image data") ;
}

#ifdef TO_BE_REMOVED
bool RsChatAvatarItem::serialise(void *data, uint32_t& pktsize)
{
	uint32_t tlvsize = serial_size() ;
	uint32_t offset = 0;

	if (pktsize < tlvsize)
		return false; /* not enough space */

	pktsize = tlvsize;

	bool ok = true;

	ok &= setRsItemHeader(data, tlvsize, PacketId(), tlvsize);

#ifdef CHAT_DEBUG
	std::cerr << "RsChatSerialiser serialising chat avatar item." << std::endl;
	std::cerr << "RsChatSerialiser::serialiseItem() Header: " << ok << std::endl;
	std::cerr << "RsChatSerialiser::serialiseItem() Size: " << tlvsize << std::endl;
#endif

	/* skip the header */
	offset += 8;

	/* add mandatory parts first */
	ok &= setRawUInt32(data, tlvsize, &offset,image_size);

	memcpy((void*)( (unsigned char *)data + offset),image_data,image_size) ;
	offset += image_size ;

	if (offset != tlvsize)
	{
		ok = false;
		std::cerr << "RsChatSerialiser::serialiseItem() Size Error! " << std::endl;
	}
#ifdef CHAT_DEBUG
	std::cerr << "computed size: " << 256*((unsigned char*)data)[6]+((unsigned char*)data)[7] << std::endl ;
#endif

	return ok;
}
#endif

void RsChatLobbyConfigItem::serial_process(RsItem::SerializeJob j,SerializeContext& ctx)
{
    RsTypeSerializer::serial_process<uint64_t>(j,ctx,lobby_Id,"lobby_Id") ;
    RsTypeSerializer::serial_process(j,ctx,flags,"flags") ;
}

#ifdef TO_BE_REMOVED
bool RsChatLobbyConfigItem::serialise(void *data, uint32_t& pktsize)
{
    uint32_t tlvsize = serial_size() ;
    uint32_t offset = 0;

    if (pktsize < tlvsize)
        return false; /* not enough space */

    pktsize = tlvsize;

    bool ok = true;

    ok &= setRsItemHeader(data, tlvsize, PacketId(), tlvsize);

#ifdef CHAT_DEBUG
    std::cerr << "RsChatLobbyConfigItem::serialiseItem() Header: " << ok << std::endl;
    std::cerr << "RsChatLobbyConfigItem::serialiseItem() Size: " << tlvsize << std::endl;
#endif

    /* skip the header */
    offset += 8;

    /* add mandatory parts first */
    ok &= setRawUInt64(data, tlvsize, &offset, lobby_Id);
    ok &= setRawUInt32(data, tlvsize, &offset, flags   );

    if (offset != tlvsize)
    {
        ok = false;
        std::cerr << "RsChatLobbyConfigItem::serialise() Size Error! " << std::endl;
    }

    return ok;
}

RsChatMsgItem::RsChatMsgItem(void *data,uint32_t /*size*/,uint8_t subtype)
	: RsChatItem(subtype)
{
	uint32_t offset = 8; // skip the header 
	uint32_t rssize = getRsItemSize(data);
	bool ok = true ;

#ifdef CHAT_DEBUG
		std::cerr << "Received packet result: " ;
	for(int i=0;i<20;++i)
		std::cerr << (int)((uint8_t*)data)[i] << " " ;
	std::cerr << std::endl ;
#endif

	/* get mandatory parts first */
	ok &= getRawUInt32(data, rssize, &offset, &chatFlags);
	ok &= getRawUInt32(data, rssize, &offset, &sendTime);
	ok &= GetTlvString(data, rssize, &offset, TLV_TYPE_STR_MSG, message);

#ifdef CHAT_DEBUG
	std::cerr << "Building new chat msg item." << std::endl ;
#endif
	if (getRsItemSubType(getRsItemId(data)) == RS_PKT_SUBTYPE_DEFAULT && offset != rssize)
		std::cerr << "Size error while deserializing." << std::endl ;
	if (!ok)
		std::cerr << "Unknown error while deserializing." << std::endl ;
}

RsChatLobbyMsgItem::RsChatLobbyMsgItem(void *data,uint32_t /*size*/)
    : RsChatMsgItem(data,0,RS_PKT_SUBTYPE_CHAT_LOBBY_SIGNED_MSG)
{
	uint32_t rssize = getRsItemSize(data);
	bool ok = true ;

	uint32_t offset = RsChatMsgItem::serial_size() ;

	ok &= getRawUInt64(data, rssize, &offset, &parent_msg_id);

    ok &= RsChatLobbyBouncingObject::deserialise_from_memory(data,rssize,offset) ;

#ifdef CHAT_DEBUG
	std::cerr << "Building new chat lobby msg item." << std::endl ;
#endif
	if (offset != rssize)
		std::cerr << "Size error while deserializing." << std::endl ;
	if (!ok)
		std::cerr << "Unknown error while deserializing." << std::endl ;
}
RsChatLobbyListRequestItem::RsChatLobbyListRequestItem(void *data,uint32_t)
	: RsChatItem(RS_PKT_SUBTYPE_CHAT_LOBBY_LIST_REQUEST)
{
	uint32_t rssize = getRsItemSize(data);
	bool ok = true ;
	uint32_t offset = 8; // skip the header 

	if (offset != rssize)
		std::cerr << "Size error while deserializing." << std::endl ;
	if (!ok)
		std::cerr << "Unknown error while deserializing." << std::endl ;
}
RsChatLobbyListItem::RsChatLobbyListItem(void *data,uint32_t)
	: RsChatItem(RS_PKT_SUBTYPE_CHAT_LOBBY_LIST)
{
	uint32_t rssize = getRsItemSize(data);
	bool ok = true ;
	uint32_t offset = 8; // skip the header 

	uint32_t n=0 ;
	ok &= getRawUInt32(data, rssize, &offset, &n);

    lobbies.resize(n) ;

	for(uint32_t i=0;i<n;++i)
    {
        ok &= getRawUInt64(data, rssize, &offset, &lobbies[i].id);
        ok &= GetTlvString(data, rssize, &offset, TLV_TYPE_STR_NAME, lobbies[i].name);
        ok &= GetTlvString(data, rssize, &offset, TLV_TYPE_STR_NAME, lobbies[i].topic);
        ok &= getRawUInt32(data, rssize, &offset, &lobbies[i].count);

        uint32_t fl=0 ;
        ok &= getRawUInt32(data, rssize, &offset, &fl);

        lobbies[i].flags = ChatLobbyFlags(fl) ;
    }

	if (offset != rssize)
		std::cerr << "Size error while deserializing." << std::endl ;
	if (!ok)
		std::cerr << "Unknown error while deserializing." << std::endl ;
}
	
bool RsChatLobbyBouncingObject::deserialise_from_memory(void *data,uint32_t rssize,uint32_t& offset)
{
	bool ok = true ;
	/* get mandatory parts first */
	ok &= getRawUInt64(data, rssize, &offset, &lobby_id);
	ok &= getRawUInt64(data, rssize, &offset, &msg_id);
	ok &= GetTlvString(data, rssize, &offset, TLV_TYPE_STR_NAME, nick);

    ok &= signature.GetTlv(data, rssize, &offset);

    return ok ;
}

RsChatLobbyEventItem::RsChatLobbyEventItem(void *data,uint32_t /*size*/)
    : RsChatItem(RS_PKT_SUBTYPE_CHAT_LOBBY_SIGNED_EVENT)
{
	uint32_t rssize = getRsItemSize(data);
	bool ok = true ;

	uint32_t offset = 8 ;

	ok &= getRawUInt8(data, rssize, &offset, &event_type);
	ok &= GetTlvString(data, rssize, &offset, TLV_TYPE_STR_NAME, string1);
	ok &= getRawUInt32(data, rssize, &offset, &sendTime);

    ok &= RsChatLobbyBouncingObject::deserialise_from_memory(data,rssize,offset) ;

#ifdef CHAT_DEBUG
	std::cerr << "Building new chat lobby status item." << std::endl ;
#endif
	if (offset != rssize)
		std::cerr << "Size error while deserializing." << std::endl ;
	if (!ok)
		std::cerr << "Unknown error while deserializing." << std::endl ;
}

RsChatLobbyUnsubscribeItem::RsChatLobbyUnsubscribeItem(void *data,uint32_t /*size*/)
	: RsChatItem(RS_PKT_SUBTYPE_CHAT_LOBBY_UNSUBSCRIBE)
{
	uint32_t rssize = getRsItemSize(data);
	bool ok = true ;

#ifdef CHAT_DEBUG
	std::cerr << "RsChatLobbyUnsubscribeItem: rsitem size is " << rssize << std::endl;
#endif
	uint32_t offset = 8 ;

	/* get mandatory parts first */
	ok &= getRawUInt64(data, rssize, &offset, &lobby_id);

	if (offset != rssize)
		std::cerr << "Size error while deserializing." << std::endl ;
	if (!ok)
		std::cerr << "Unknown error while deserializing." << std::endl ;
}
RsChatLobbyConnectChallengeItem::RsChatLobbyConnectChallengeItem(void *data,uint32_t /*size*/)
	: RsChatItem(RS_PKT_SUBTYPE_CHAT_LOBBY_CHALLENGE)
{
	uint32_t rssize = getRsItemSize(data);
	bool ok = true ;

#ifdef CHAT_DEBUG
	std::cerr << "RsChatLobbyConnectChallengeItem: rsitem size is " << rssize << std::endl;
#endif
	uint32_t offset = 8 ;

	/* get mandatory parts first */
	ok &= getRawUInt64(data, rssize, &offset, &challenge_code);

	if (offset != rssize)
		std::cerr << "Size error while deserializing." << std::endl ;
	if (!ok)
		std::cerr << "Unknown error while deserializing." << std::endl ;
}
RsChatLobbyInviteItem::RsChatLobbyInviteItem(void *data,uint32_t /*size*/)
	: RsChatItem(RS_PKT_SUBTYPE_CHAT_LOBBY_INVITE)
{
	uint32_t rssize = getRsItemSize(data);
	bool ok = true ;

#ifdef CHAT_DEBUG
	std::cerr << "RsChatLobbyInviteItem: rsitem size is " << rssize << std::endl;
#endif
	uint32_t offset = 8 ;

	/* get mandatory parts first */
	ok &= getRawUInt64(data, rssize, &offset, &lobby_id);
    ok &= GetTlvString(data, rssize, &offset, TLV_TYPE_STR_NAME, lobby_name);

    uint32_t fl ;
    ok &= getRawUInt32(data, rssize, &offset, &fl) ;

    lobby_flags = ChatLobbyFlags(fl) ;

#ifdef CHAT_DEBUG
	std::cerr << "Building new chat msg item." << std::endl ;
#endif
	if (offset != rssize)
		std::cerr << "Size error while deserializing." << std::endl ;
	if (!ok)
		std::cerr << "Unknown error while deserializing." << std::endl ;
}

RsPrivateChatMsgConfigItem::RsPrivateChatMsgConfigItem(void *data,uint32_t /*size*/)
	: RsChatItem(RS_PKT_SUBTYPE_PRIVATECHATMSG_CONFIG)
{
	uint32_t offset = 8; // skip the header 
	uint32_t rssize = getRsItemSize(data);
	bool ok = true ;

	/* get mandatory parts first */
	uint32_t version = 0;
	ok &= getRawUInt32(data, rssize, &offset, &version);
	ok &= configPeerId.deserialise(data, rssize, offset);
	ok &= getRawUInt32(data, rssize, &offset, &chatFlags);
	ok &= getRawUInt32(data, rssize, &offset, &configFlags);
	ok &= getRawUInt32(data, rssize, &offset, &sendTime);
	ok &= GetTlvString(data, rssize, &offset, TLV_TYPE_STR_MSG, message);
	ok &= getRawUInt32(data, rssize, &offset, &recvTime);

#ifdef CHAT_DEBUG
	std::cerr << "Building new chat msg config item." << std::endl ;
#endif
	if (offset != rssize)
		std::cerr << "Size error while deserializing." << std::endl ;
	if (!ok)
		std::cerr << "Unknown error while deserializing." << std::endl ;
}

RsPrivateChatDistantInviteConfigItem::RsPrivateChatDistantInviteConfigItem(void *data,uint32_t /*size*/)
	: RsChatItem(RS_PKT_SUBTYPE_DISTANT_INVITE_CONFIG)
{
	uint32_t offset = 8; // skip the header 
	uint32_t rssize = getRsItemSize(data);
	bool ok = true ;

	/* get mandatory parts first */
    ok &= hash.deserialise(data, rssize, offset) ;
	ok &= GetTlvString(data, rssize, &offset, TLV_TYPE_STR_LINK, encrypted_radix64_string);
	ok &= destination_pgp_id.serialise(data, rssize, offset);

	memcpy(aes_key,&((unsigned char*)data)[offset],16) ;
	offset += 16 ;

	ok &= getRawUInt32(data, rssize, &offset, &time_of_validity);
	ok &= getRawUInt32(data, rssize, &offset, &last_hit_time);

	if(offset+4 == rssize)												// flags are optional for retro-compatibility.
		ok &= getRawUInt32(data, rssize, &offset, &flags);
	else
		flags = 0 ;

#ifdef CHAT_DEBUG
	std::cerr << "Building new chat msg config item." << std::endl ;
#endif
	if (offset != rssize)
		std::cerr << "Size error while deserializing." << std::endl ;
	if (!ok)
		std::cerr << "Unknown error while deserializing." << std::endl ;
}
RsChatLobbyConfigItem::RsChatLobbyConfigItem(void *data,uint32_t /*size*/)
    : RsChatItem(RS_PKT_SUBTYPE_CHAT_LOBBY_CONFIG)
{
    uint32_t offset = 8; // skip the header
    uint32_t rssize = getRsItemSize(data);
    bool ok = true ;

    /* get mandatory parts first */
    ok &= getRawUInt64(data, rssize, &offset, &lobby_Id);
    ok &= getRawUInt32(data, rssize, &offset, &flags);

#ifdef CHAT_DEBUG
    std::cerr << "Building new chat msg config item." << std::endl ;
#endif
    if (offset != rssize)
        std::cerr << "Size error while deserializing." << std::endl ;
    if (!ok)
        std::cerr << "Unknown error while deserializing." << std::endl ;
}
#endif

/* set data from RsChatMsgItem to RsPrivateChatMsgConfigItem */
void RsPrivateChatMsgConfigItem::set(RsChatMsgItem *ci, const RsPeerId& /*peerId*/, uint32_t confFlags)
{
	PeerId(ci->PeerId());
	configPeerId = ci->PeerId();
	chatFlags = ci->chatFlags;
	configFlags = confFlags;
	sendTime = ci->sendTime;
	message = ci->message;
	recvTime = ci->recvTime;
}

/* get data from RsPrivateChatMsgConfigItem to RsChatMsgItem */
void RsPrivateChatMsgConfigItem::get(RsChatMsgItem *ci)
{
	ci->PeerId(configPeerId);
	ci->chatFlags = chatFlags;
	//configFlags not used
	ci->sendTime = sendTime;
	ci->message = message;
	ci->recvTime = recvTime;
}

#ifdef TO_BE_REMOVED
RsChatStatusItem::RsChatStatusItem(void *data,uint32_t /*size*/)
	: RsChatItem(RS_PKT_SUBTYPE_CHAT_STATUS)
{
	uint32_t offset = 8; // skip the header 
	uint32_t rssize = getRsItemSize(data);
	bool ok = true ;

#ifdef CHAT_DEBUG
	std::cerr << "Building new chat status item." << std::endl ;
#endif
	/* get mandatory parts first */
	ok &= getRawUInt32(data, rssize, &offset, &flags);
	ok &= GetTlvString(data, rssize, &offset,TLV_TYPE_STR_MSG, status_string);

	if (offset != rssize)
		std::cerr << "Size error while deserializing." << std::endl ;
	if (!ok)
		std::cerr << "Unknown error while deserializing." << std::endl ;
}

RsChatAvatarItem::RsChatAvatarItem(void *data,uint32_t /*size*/)
	: RsChatItem(RS_PKT_SUBTYPE_CHAT_AVATAR)
{
	uint32_t offset = 8; // skip the header 
	uint32_t rssize = getRsItemSize(data);
	bool ok = true ;

#ifdef CHAT_DEBUG
	std::cerr << "Building new chat status item." << std::endl ;
#endif
	/* get mandatory parts first */
	ok &= getRawUInt32(data, rssize, &offset,&image_size);

        // ensure invalid image length does not overflow data
        if( (offset + image_size) <= rssize){
            image_data = new unsigned char[image_size] ;
            memcpy(image_data,(void*)((unsigned char*)data+offset),image_size) ;
            offset += image_size ;
        }else{
            ok = false;
            std::cerr << "offset+image_size exceeds rssize" << std::endl;
        }

	if (offset != rssize)
		std::cerr << "Size error while deserializing." << std::endl ;
	if (!ok)
		std::cerr << "Unknown error while deserializing." << std::endl ;
}

#endif

uint32_t RsChatLobbyBouncingObject::serial_size_for_signature() const
{
	SerializeContext ctx(NULL,0);

	ctx.mOffset = 8;

	const_cast<RsChatLobbyBouncingObject*>(this)->serial_process_for_signature(RsItem::SIZE_ESTIMATE,ctx) ;

	return ctx.mOffset ;
}

bool RsChatLobbyBouncingObject::serialize_for_signature(uint8_t *data,uint32_t size) const
{
	SerializeContext ctx(data,0);

	uint32_t tlvsize = serial_size_for_signature() ;

	if(tlvsize > size)
		throw std::runtime_error("Cannot serialise: not enough room.") ;

	if(!setRsItemHeader(data, tlvsize, PacketId(), tlvsize))
	{
		std::cerr << "RsSerializer::serialise_item(): ERROR. Not enough size!" << std::endl;
		return false ;
	}
	ctx.mOffset = 8;
	ctx.mSize = tlvsize;

	const_cast<RsChatLobbyBouncingObject*>(this)->serial_process_for_signature(RsItem::SERIALIZE,ctx) ;

	if(ctx.mSize != ctx.mOffset)
	{
		std::cerr << "RsChatSerializer::serialise_item(): ERROR. offset does not match expected size!" << std::endl;
		return false ;
	}
	return true ;
}

