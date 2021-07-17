﻿using UnityEngine;

namespace ChatSDK
{
    class Client_Android : IClient
    {
        private AndroidJavaObject wrapper;

        public Client_Android()
        {

            using (AndroidJavaClass aj = new AndroidJavaClass("com.hyphenate.unity_chat_sdk.EMClientWrapper"))
            {
                wrapper = aj.CallStatic<AndroidJavaObject>("wrapper");
            }
        }

        public override void InitWithOptions(Options options, WeakDelegater<IConnectionDelegate> connectionDelegater)
        {
            wrapper.Call("init", options.ToJsonString());
        }

        public override void CreateAccount(string username, string password, CallBack callBack = null)
        {
            wrapper.Call("createAccount", username, password, null);
        }

        public override void Login(string username, string pwdOrToken, bool isToken = false, CallBack callBack = null)
        {
            wrapper.Call("login", username, pwdOrToken, isToken, callBack?.callbackId);
        }

        public override void Logout(bool unbindDeviceToken, CallBack callBack = null)
        {
            wrapper.Call("logout", unbindDeviceToken, callBack?.callbackId);
        }
        public override string CurrentUsername()
        {
            return wrapper.Call<string>("currentUsername");
        }

        public override bool IsConnected()
        {
            return wrapper.Call<bool>("isConnected");
        }

        public override bool IsLoggedIn()
        {
            return wrapper.Call<bool>("isLoggedIn");
        }

        public override string AccessToken()
        {
            return wrapper.Call<string>("accessToken");
        }

        public override void StartLog(string logFilePath)
        {
            //throw new System.NotImplementedException();
        }

        public override void StopLog()
        {
            //throw new System.NotImplementedException();
        }
    }
}