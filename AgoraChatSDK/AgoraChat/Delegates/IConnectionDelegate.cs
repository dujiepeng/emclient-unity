﻿namespace AgoraChat
{
    /**
     * \~chinese
     * 连接回调接口。
     *
     * \~english
     * The connection callback interface.
     */
    public interface IConnectionDelegate
    {
        /**
         * \~chinese
         * SDK 成功连接到 chat 服务器时触发的回调。
         *
         * \~english
         * Occurs when the SDK successfully connects to the chat server.
         */
        void OnConnected();

        /**
         * \~chinese
         * SDK 与 chat 服务器断开连接时触发的回调。
         *
         * 断开连接的原因可能是你主动退出登录或网络中断。
         * 
         * \~english
         * Occurs when the SDK disconnects from the chat server.
         * 
         * The SDK disconnects from the chat server when you log out of the app or a network interruption occurs.
         * 
         */
        void OnDisconnected();

        /**
         *  \~chinese
         *  当前登录账号在其它设备登录时会接收到此回调。
         *
         *  \~english
         *  Occurs when the current user account has logged in to another device.
         */
        void OnLoggedOtherDevice();

        /**
         *  \~chinese
         *  当前登录账号已经被从服务器端删除时会收到该回调。
         *
         *  \~english
         *  Occurs when the current user account is removed from the server.
         */
        void OnRemovedFromServer();

        /**
         *  \~chinese
         *  当前用户账号被禁用时会收到该回调。
         *
         *  \~english
         *  Occurs when the current user account is banned.
         */
        void OnForbidByServer();

        /**
         *  \~chinese
         *  当前登录账号因密码被修改被强制退出。
         *
         *  \~english
         *  Occurs when the current user account is forced to log out because the login password is changed.
         */
        void OnChangedIMPwd();

        /**
         *  \~chinese
         *  当前登录账号登录设备数过多被强制退出。
         *
         *  \~english
         *  Occurs when the current user account is forced to log out due to too many login failures.
         */
        void OnLoginTooManyDevice();

        /**
         *  \~chinese
         *  当前登录设备账号被登录其他设备的同账号踢下线。
         *
         *  \~english
         *  Occurs when the current account is forced to log out on the current device due to login to another device.
         */
        void OnKickedByOtherDevice();

        /**
         *  \~chinese
         *  当前登录设备账号因鉴权失败强制退出。
         *
         *  \~english
         *  Occurs when the current account is forced to log out due to an authentication failure.
         */
        void OnAuthFailed();


        /**
         * \~chinese
         * Token 过期回调。
         *  
         * \~english
         * Occurs when the token has expired.
         */
        void OnTokenExpired();

        /**
         * \~chinese
         * Token 即将过期回调。
         *  
         * \~english
         * Occurs when the token is about to expire.
         */
        void OnTokenWillExpire();

    }

}