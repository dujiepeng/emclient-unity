﻿using System;
using SimpleJSON;

namespace ChatSDK {
    /// <summary>
    /// 推送配置
    /// </summary>
    public class PushConfig
    {
        /// <summary>
        /// 免打扰
        /// </summary>
        public bool NoDisturb { get; internal set; }

        /// <summary>
        /// 免打扰开始时间
        /// </summary>
        public int NoDisturbStartHour { get; internal set; }

        /// <summary>
        /// 免打扰结束时间
        /// </summary>
        public int NoDisturbEndHour { get; internal set; }

        /// <summary>
        /// 推送显示类型
        /// </summary>
        public PushStyle Style { get; internal set; }

        internal PushConfig(string jsonString)
        {
            if (jsonString != null)
            {
                JSONNode jn = JSON.Parse(jsonString);
                if (!jn.IsNull && jn.IsObject)
                {
                    JSONObject jo = jn.AsObject;
                    NoDisturb = jo["noDisturb"].AsBool;
                    NoDisturbStartHour = jo["noDisturbStartHour"].AsInt;
                    NoDisturbEndHour = jo["noDisturbEndHour"].AsInt;
                    NoDisturb = jo["noDisturb"].AsBool;
                    if (jo["pushStyle"].AsInt == 0)
                    {
                        Style = PushStyle.Simple;
                    }
                    else
                    {
                        Style = PushStyle.Summary;
                    }
                }
            }
        }
    }

}

