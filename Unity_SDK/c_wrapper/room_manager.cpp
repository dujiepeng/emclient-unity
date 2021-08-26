//
//  room_manager.cpp
//  hyphenateCWrapper
//
//  Created by Bingo Zhao on 2021/7/11.
//  Copyright © 2021 easemob. All rights reserved.
//
#include <thread>
#include "room_manager.h"
#include "emclient.h"
#include "tool.h"

using namespace easemob;
EMChatroomManagerListener *gRoomManagerListener = nullptr;

AGORA_API void RoomManager_AddListener(void *client, FUNC_OnChatRoomDestroyed onChatRoomDestroyed, FUNC_OnMemberJoined onMemberJoined,
                                       FUNC_OnMemberExitedFromRoom onMemberExited,FUNC_OnRemovedFromChatRoom onRemovedFromChatRoom,
                                       FUNC_OnMuteListAdded onMuteListAdded, FUNC_OnMuteListRemoved onMuteListRemoved,FUNC_OnAdminAdded onAdminAdded,
                                       FUNC_OnAdminRemoved onAdminRemoved, FUNC_OnOwnerChanged onOwnerChanged, FUNC_OnAnnouncementChanged onAnnouncementChanged)
{
    if(nullptr == gRoomManagerListener) { //only set once!
        gRoomManagerListener = new RoomManagerListener(client, onChatRoomDestroyed,  onMemberJoined, onMemberExited, onRemovedFromChatRoom, onMuteListAdded, onMuteListRemoved, onAdminAdded, onAdminRemoved, onOwnerChanged, onAnnouncementChanged);
       CLIENT->getChatroomManager().addListener(gRoomManagerListener);
    }
}

AGORA_API void RoomManager_AddRoomAdmin(void * client, const char * roomId, const char * memberId, FUNC_OnSuccess_With_Result onSuccess, FUNC_OnError onError)
{
    std::thread t([=](){
        EMError error;
        if(!MandatoryCheck(roomId, memberId, error)) {
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
            return;
        }
        EMChatroomPtr result = CLIENT->getChatroomManager().addChatroomAdmin(roomId, memberId, error);
        if(EMError::EM_NO_ERROR == error.mErrorCode) {
            LOG("RoomManager_AddRoomAdmin succeeds: %s", result->chatroomSubject().c_str());
            if(onSuccess) {
                RoomTO *datum = RoomTO::FromEMChatRoom(result);
                RoomTO *data[1] = {datum};
                datum->LogInfo();
                onSuccess((void **)data, DataType::Room, 1);
                delete datum;
            }
        }else{
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
        }
    });
    t.join();
}

AGORA_API void RoomManager_BlockChatroomMembers(void * client, const char * roomId, const char * memberArray[], int size, FUNC_OnSuccess_With_Result onSuccess, FUNC_OnError onError)
{
    std::thread t([=](){
        EMError error;
        if(!MandatoryCheck(roomId, error)) {
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
            return;
        }
        EMMucMemberList memberList;
        for(int i=0; i<size; i++) {
            memberList.push_back(memberArray[i]);
        }
        EMChatroomPtr chatRoomPtr = CLIENT->getChatroomManager().blockChatroomMembers(roomId, memberList, error);
        if(EMError::EM_NO_ERROR == error.mErrorCode) {
            LOG("RoomManager_BlockChatroomMembers succeeds: %s", chatRoomPtr->chatroomSubject().c_str());
            if(onSuccess) {
                RoomTO *datum = RoomTO::FromEMChatRoom(chatRoomPtr);
                RoomTO *data[1] = {datum};
                datum->LogInfo();
                onSuccess((void **)data, DataType::Room, 1);
                delete datum;
            }
        }else{
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
        }
    });
    t.join();
}

AGORA_API void RoomManager_CreateRoom(void *client, const char * subject, const char * desc, const char * welcomMsg, int maxUserCount, const char * memberArray[], int size, FUNC_OnSuccess_With_Result onSuccess, FUNC_OnError onError)
{
    std::thread t([=](){
        EMError error;
        if(!MandatoryCheck(subject, error)) {
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
            return;
        }
        EMMucSetting setting(EMMucSetting::EMMucStyle::DEFAUT, maxUserCount, false);
        EMMucMemberList memberList;
        for(int i=0; i<size; i++) {
            memberList.push_back(memberArray[i]);
        }
        std::string descStr = OptionalStrParamCheck(desc);
        std::string welcomMsgStr = OptionalStrParamCheck(welcomMsg);
        EMChatroomPtr result = CLIENT->getChatroomManager().createChatroom(subject, descStr, welcomMsgStr, setting, memberList, error);
        if(EMError::EM_NO_ERROR == error.mErrorCode) {
            LOG("RoomManager_CreateRoom succeeds: %s", result->chatroomId().c_str());
            if(onSuccess) {
                RoomTO *data[1] = {RoomTO::FromEMChatRoom(result)};
                onSuccess((void **)data, DataType::Room, 1);
                delete (RoomTO*)data[0];
            }
        }else{
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
        }
    });
    t.join();
}

AGORA_API void RoomManager_ChangeRoomSubject(void *client, const char * roomId, const char * newSubject, FUNC_OnSuccess_With_Result onSuccess, FUNC_OnError onError)
{
    std::thread t([=](){
        EMError error;
        if(!MandatoryCheck(roomId, newSubject, error)) {
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
            return;
        }
        EMChatroomPtr result = CLIENT->getChatroomManager().changeChatroomSubject(roomId, newSubject, error);
        if(EMError::EM_NO_ERROR == error.mErrorCode) {
            LOG("RoomManager_ChangeRoomSubject succeeds: %s", result->chatroomSubject().c_str());
            if(onSuccess) {
                RoomTO *datum = RoomTO::FromEMChatRoom(result);
                RoomTO *data[1] = {datum};
                datum->LogInfo();
                onSuccess((void **)data, DataType::Room, 1);
                delete datum;
            }
        }else{
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
        }
    });
    t.join();
}

AGORA_API void RoomManager_RemoveRoomMembers(void * client, const char * roomId, const char * memberArray[], int size, FUNC_OnSuccess onSuccess, FUNC_OnError onError)
{
    std::thread t([=](){
        EMError error;
        if(!MandatoryCheck(roomId, error)) {
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
            return;
        }
        EMMucMemberList memberList;
        for(int i=0; i<size; i++) {
            memberList.push_back(memberArray[i]);
        }
        EMChatroomPtr result = CLIENT->getChatroomManager().removeChatroomMembers(roomId, memberList, error);
        if(EMError::EM_NO_ERROR == error.mErrorCode) {
            LOG("RoomManager_RemoveRoomMembers succeeds: %s", result->chatroomSubject().c_str());
            if(onSuccess) onSuccess();
        }else{
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
        }
    });
    t.join();
}

AGORA_API void RoomManager_TransferChatroomOwner(void * client, const char * roomId, const char * newOwner, FUNC_OnSuccess_With_Result onSuccess, FUNC_OnError onError)
{
    std::thread t([=](){
        EMError error;
        if(!MandatoryCheck(roomId, newOwner, error)) {
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
            return;
        }
        EMChatroomPtr result = CLIENT->getChatroomManager().transferChatroomOwner(roomId, newOwner, error);
        if(EMError::EM_NO_ERROR == error.mErrorCode) {
            LOG("RoomManager_TransferChatroomOwner succeeds: %s", result->chatroomSubject().c_str());
            if(onSuccess) {
                RoomTO *datum = RoomTO::FromEMChatRoom(result);
                RoomTO *data[1] = {datum};
                datum->LogInfo();
                onSuccess((void **)data, DataType::Room, 1);
                delete datum;
            }
        }else{
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
        }
    });
    t.join();
}

AGORA_API void RoomManager_ChangeChatroomDescription(void * client, const char * roomId, const char * newDescription, FUNC_OnSuccess_With_Result onSuccess, FUNC_OnError onError)
{
    std::thread t([=](){
        EMError error;
        if(!MandatoryCheck(roomId, newDescription, error)) {
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
            return;
        }
        EMChatroomPtr result = CLIENT->getChatroomManager().changeChatroomDescription(roomId, newDescription, error);
        if(EMError::EM_NO_ERROR == error.mErrorCode) {
            LOG("RoomManager_ChangeChatroomDescription succeeds: %s", result->chatroomSubject().c_str());
            if(onSuccess) {
                RoomTO *datum = RoomTO::FromEMChatRoom(result);
                RoomTO *data[1] = {datum};
                datum->LogInfo();
                onSuccess((void **)data, DataType::Room, 1);
                delete datum;
            }
        }else{
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
        }
    });
    t.join();
}

AGORA_API void RoomManager_DestroyChatroom(void *client, const char * roomId, FUNC_OnSuccess onSuccess, FUNC_OnError onError)
{
    std::thread t([=](){
        EMError error;
        if(!MandatoryCheck(roomId, error)) {
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
            return;
        }
        CLIENT->getChatroomManager().destroyChatroom(roomId, error);
        if(EMError::EM_NO_ERROR == error.mErrorCode) {
            if(onSuccess) onSuccess();
        }else{
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
        }
    });
    t.join();
}

AGORA_API void RoomManager_FetchChatroomsWithPage(void *client, int pageNum, int pageSize, FUNC_OnSuccess_With_Result onSuccess, FUNC_OnError onError)
{
    std::thread t([=](){
        EMError error;
        EMPageResult pageResult = CLIENT->getChatroomManager().fetchChatroomsWithPage(pageNum, pageSize, error);
        if(EMError::EM_NO_ERROR == error.mErrorCode) {
            if(onSuccess) {
                int size = (int)pageResult.result().size();
                LOG("Found rooms with num:%d", size);
                RoomTO* data[size];
                for(int i=0; i<size; i++) {
                    EMChatroomPtr charRoomPtr = std::dynamic_pointer_cast<EMChatroom>(pageResult.result().at(i));
                    data[i] = RoomTO::FromEMChatRoom(charRoomPtr);
                    LOG("Room %d, id:%s, name:%s", i, charRoomPtr->chatroomId().c_str(), charRoomPtr->chatroomSubject().c_str());
                }
                onSuccess((void **)data, DataType::Room, (int)size);
                for(int i=0; i<size; i++) {
                    delete (RoomTO*)data[i];
                }
            }
        }else{
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
        }
    });
    t.join();
}

AGORA_API void RoomManager_FetchChatroomAnnouncement(void *client, const char * roomId, FUNC_OnSuccess_With_Result onSuccess, FUNC_OnError onError)
{
    std::thread t([=](){
        EMError error;
        if(!MandatoryCheck(roomId, error)) {
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
            return;
        }
        std::string announcement = CLIENT->getChatroomManager().fetchChatroomAnnouncement(roomId, error);
        if(EMError::EM_NO_ERROR == error.mErrorCode) {
            LOG("RoomManager_FetchChatroomAnnouncement succeeds: %s", roomId);
            if(onSuccess) {
                const char *data[1];
                data[0]= announcement.c_str();
                onSuccess((void **)data, DataType::String, 1);
            }
        }else{
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
        }
    });
    t.join();
}

AGORA_API void RoomManager_FetchChatroomBans(void *client, const char * roomId, int pageNum, int pageSize, FUNC_OnSuccess_With_Result onSuccess, FUNC_OnError onError)
{
    std::thread t([=](){
        EMError error;
        if(!MandatoryCheck(roomId, error)) {
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
            return;
        }
        EMMucMemberList banList = CLIENT->getChatroomManager().fetchChatroomBans(roomId, pageNum, pageSize, error);
        if(EMError::EM_NO_ERROR == error.mErrorCode) {
            LOG("RoomManager_FetchChatroomBans succeeds, roomId: %s", roomId);
            if(onSuccess) {
                size_t size = banList.size();
                const char * data[size];
                for(size_t i=0; i<size; i++) {
                    data[i] = banList[i].c_str();
                }
                onSuccess((void **)data, DataType::String, (int)size);
            }
        }else{
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
        }
    });
    t.join();
}

AGORA_API void RoomManager_FetchChatroomSpecification(void * client, const char * roomId, FUNC_OnSuccess_With_Result onSuccess, FUNC_OnError onError)
{
    std::thread t([=](){
        EMError error;
        if(!MandatoryCheck(roomId, error)) {
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
            return;
        }
        EMChatroomPtr result = CLIENT->getChatroomManager().fetchChatroomSpecification(roomId, error, false);
        if(EMError::EM_NO_ERROR == error.mErrorCode) {
            LOG("RoomManager_FetchChatroomSpecification succeeds roomId: %s", result->chatroomId().c_str());
            if(onSuccess) {
                RoomTO *datum = RoomTO::FromEMChatRoom(result);
                RoomTO *data[1] = {datum};
                datum->LogInfo();
                onSuccess((void **)data, DataType::Room, 1);
                delete (RoomTO*)datum;
            }
        }else{
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
        }
    });
    t.join();
}

AGORA_API void RoomManager_FetchChatroomMembers(void * client, const char * roomId, const char * cursor, int pageSize, FUNC_OnSuccess_With_Result_V2 onSuccess, FUNC_OnError onError)
{
    std::thread t([=](){
        EMError error;
        if(!MandatoryCheck(roomId, error)) {
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
            return;
        }
        std::string cursorStr = OptionalStrParamCheck(cursor);
        EMCursorResultRaw<std::string> msgCursorResult = CLIENT->getChatroomManager().fetchChatroomMembers(roomId, cursorStr, pageSize, error);
        if(EMError::EM_NO_ERROR == error.mErrorCode) {
            if(onSuccess) {
                //header
                CursorResultTOV2 cursorResultTo;
                cursorResultTo.NextPageCursor = msgCursorResult.nextPageCursor().c_str();
                cursorResultTo.Type = DataType::ListOfString;
                //items
                int size = (int)msgCursorResult.result().size();
                const char * data[size];
                for(int i=0; i<size; i++) {
                    data[i] = msgCursorResult.result().at(i).c_str();
                }
                onSuccess((void *)&cursorResultTo, (void **)data, DataType::CursorResult, size);
            }
        }else{
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
        }
    });
    t.join();
}

AGORA_API void RoomManager_FetchChatroomMutes(void * client, const char * roomId, int pageNum, int pageSize, FUNC_OnSuccess_With_Result onSuccess, FUNC_OnError onError)
{
    std::thread t([=](){
        EMError error;
        if(!MandatoryCheck(roomId, error)) {
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
            return;
        }
        EMMucMuteList muteList = CLIENT->getChatroomManager().fetchChatroomMutes(roomId, pageNum, pageSize, error);
        if(EMError::EM_NO_ERROR == error.mErrorCode) {
            LOG("RoomManager_FetchChatroomMutes succeeds, roomId: %s", roomId);
            if(onSuccess) {
                size_t size = muteList.size();
                const char * data[size];
                for(size_t i=0; i<size; i++) {
                    data[i] = muteList[i].first.c_str();
                }
                onSuccess((void **)data, DataType::String, (int)size);
            }
        }else{
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
        }
    });
    t.join();
}

AGORA_API void RoomManager_GetAllRoomsFromLocal(void * client, FUNC_OnSuccess_With_Result onSuccess, FUNC_OnError onError)
{
    //mdata private??
    //loadAllChatroomsFromDB
    //CLIENT->getChatroomManager()
}

AGORA_API void RoomManager_JoinedChatroomById(void * client, const char * roomId, FUNC_OnSuccess_With_Result onSuccess, FUNC_OnError onError)
{
    std::thread t([=](){
        EMError error;
        if(!MandatoryCheck(roomId, error)) {
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
            return;
        }
        EMChatroomPtr result = CLIENT->getChatroomManager().joinedChatroomById(roomId);
        if(result != nullptr)
        {
            LOG("RoomManager_JoinedChatroomById succeeds roomId: %s", result->chatroomId().c_str());
            if(onSuccess) {
                RoomTO *datum = RoomTO::FromEMChatRoom(result);
                RoomTO *data[1] = {datum};
                datum->LogInfo();
                onSuccess((void **)data, DataType::Room, 1);
                delete (RoomTO*)datum;
            }
        }else{
            LOG("RoomManager_JoinedChatroomById succeeds, but no room is found");
        }
    });
    t.join();
}

AGORA_API void RoomManager_JoinChatroom(void *client, const char * roomId, FUNC_OnSuccess_With_Result onSuccess, FUNC_OnError onError)
{
    std::thread t([=](){
        EMError error;
        if(!MandatoryCheck(roomId, error)) {
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
            return;
        }
        EMChatroomPtr result = CLIENT->getChatroomManager().joinChatroom(roomId, error);
        if(EMError::EM_NO_ERROR == error.mErrorCode) {
            LOG("RoomManager_JoinChatroom succeeds: roomId:%s", roomId);
            if(onSuccess) {
                RoomTO *data[1];
                if(result) {
                    data[0] = {RoomTO::FromEMChatRoom(result)};
                    onSuccess((void **)data, DataType::Room, 1);
                    delete (RoomTO*)data[0];
                    LOG("RoomManager_JoinChatroom return room with id:%s", roomId);
                } else {
                    onSuccess((void **)data, DataType::Room, 0);
                    LOG("RoomManager_JoinChatroom NO room returned");
                }
            }
        }else{
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
        }
    });
    t.join();
}

AGORA_API void RoomManager_LeaveChatroom(void *client, const char * roomId, FUNC_OnSuccess onSuccess, FUNC_OnError onError)
{
    std::thread t([=](){
        EMError error;
        if(!MandatoryCheck(roomId, error)) {
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
            return;
        }
        CLIENT->getChatroomManager().leaveChatroom(roomId, error);
        if(EMError::EM_NO_ERROR == error.mErrorCode) {
            if(onSuccess) onSuccess();
        }else{
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
        }
    });
    t.join();
}

AGORA_API void RoomManager_MuteChatroomMembers(void * client, const char * roomId, const char * memberArray[], int size, int muteDuration, FUNC_OnSuccess onSuccess, FUNC_OnError onError)
{
    std::thread t([=](){
        EMError error;
        if(!MandatoryCheck(roomId, error)) {
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
            return;
        }
        EMMucMemberList memberList;
        for(int i=0; i<size; i++) {
            memberList.push_back(memberArray[i]);
        }
        EMChatroomPtr chatRoomPtr = CLIENT->getChatroomManager().muteChatroomMembers(roomId, memberList, muteDuration, error);
        if(EMError::EM_NO_ERROR == error.mErrorCode) {
            if(onSuccess) onSuccess();
        }else{
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
        }
    });
    t.join();
}

AGORA_API void RoomManager_RemoveChatroomAdmin(void *client, const char * roomId, const char * adminId, FUNC_OnSuccess_With_Result onSuccess, FUNC_OnError onError)
{
    std::thread t([=](){
        EMError error;
        if(!MandatoryCheck(roomId, adminId, error)) {
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
            return;
        }
        EMChatroomPtr result = CLIENT->getChatroomManager().removeChatroomAdmin(roomId, adminId, error);
        if(EMError::EM_NO_ERROR == error.mErrorCode) {
            //success
            LOG("RoomManager_RemoveChatroomAdmin succeeds: roomId:%s", roomId);
            if(onSuccess) {
                if(result) {
                    RoomTO *data[1];
                    data[0] = {RoomTO::FromEMChatRoom(result)};
                    onSuccess((void **)data, DataType::Room, 1);
                    delete (RoomTO*)data[0];
                    LOG("RoomManager_RemoveChatroomAdmin return room with id:%s", roomId);
                } else {
                    onSuccess(nullptr, DataType::Room, 0);
                    LOG("RoomManager_RemoveChatroomAdmin NO room returned");
                }
            }
        }else{
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
        }
    });
    t.join();
}

AGORA_API void RoomManager_UnblockChatroomMembers(void * client, const char * roomId, const char * memberArray[], int size, FUNC_OnSuccess onSuccess, FUNC_OnError onError)
{
    std::thread t([=](){
        EMError error;
        if(!MandatoryCheck(roomId, error)) {
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
            return;
        }
        EMMucMemberList memberList;
        for(int i=0; i<size; i++) {
            memberList.push_back(memberArray[i]);
        }
        EMChatroomPtr chatRoomPtr = CLIENT->getChatroomManager().unblockChatroomMembers(roomId, memberList, error);
        if(EMError::EM_NO_ERROR == error.mErrorCode) {
            if(onSuccess) onSuccess();
        }else{
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
        }
    });
    t.join();
}

AGORA_API void RoomManager_UnmuteChatroomMembers(void * client, const char * roomId, const char * memberArray[], int size, FUNC_OnSuccess onSuccess, FUNC_OnError onError)
{
    std::thread t([=](){
        EMError error;
        if(!MandatoryCheck(roomId, error)) {
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
            return;
        }
        EMMucMemberList memberList;
        for(int i=0; i<size; i++) {
            memberList.push_back(memberArray[i]);
        }
        EMChatroomPtr chatRoomPtr = CLIENT->getChatroomManager().unmuteChatroomMembers(roomId, memberList, error);
        if(EMError::EM_NO_ERROR == error.mErrorCode) {
            if(onSuccess) onSuccess();
        }else{
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
        }
    });
    t.join();
}

AGORA_API void RoomManager_UpdateChatroomAnnouncement(void *client, const char * roomId, const char * newAnnouncement, FUNC_OnSuccess onSuccess, FUNC_OnError onError)
{
    std::thread t([=](){
        EMError error;
        if(!MandatoryCheck(roomId, newAnnouncement, error)) {
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
            return;
        }
        EMChatroomPtr chatRoomPtr = CLIENT->getChatroomManager().updateChatroomAnnouncement(roomId, newAnnouncement, error);
        if(EMError::EM_NO_ERROR == error.mErrorCode) {
            if(onSuccess) onSuccess();
        }else{
            if(onError) onError(error.mErrorCode, error.mDescription.c_str());
        }
    });
    t.join();
}
