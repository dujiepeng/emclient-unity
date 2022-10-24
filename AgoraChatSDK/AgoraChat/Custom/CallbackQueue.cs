﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

#if _WIN32
#else
using UnityEngine;
#endif

namespace AgoraChat
{
 
    internal class CallbackQueue
    {
        private Queue<Action> queue = new Queue<Action>();

        public void ClearQueue()
        {
            lock(queue)
            {
                queue.Clear();
            }
        }

        public void EnQueue(Action action)
        {
            lock (queue)
            {
                if (queue.Count >= 250)
                {
                    queue.Dequeue();
                }
                queue.Enqueue(action);
            }
        }

        public Action DeQueue()
        {
            Action action = null;
            lock (queue)
            {
                if (queue.Count > 0)
                {
                    action = queue.Dequeue();
                }
            }
            return action;
        }
    }

#if _WIN32
    internal class CallbackQueue_ThreadMode
    {
        Task worker;
        bool turn_on;
        CallbackQueue queue = new CallbackQueue();

        private static CallbackQueue_ThreadMode instance;
        internal static CallbackQueue_ThreadMode Instance()
        {
            if (null == instance)
            {
                instance = new CallbackQueue_ThreadMode();
            }
            return instance;
        }

        public void EnQueue(Action action)
        {
            queue.EnQueue(action);
        }

        public void StartRun()
        {
            queue.ClearQueue();
            turn_on = true;
            worker = new Task(Process);
            worker.Start();
        }

        public void Stop()
        {
            turn_on = false;
            queue.ClearQueue();
        }

        private void Process()
        {
            while(turn_on)
            {
                Action action = queue.DeQueue();
                if (null != action)
                {
                    action();
                }
                action = null;
                Thread.Sleep(20); // avoid running too fast
            }
        }
    }

#else
    internal sealed class CallbackQueue_UnityMode : MonoBehaviour
    {
        CallbackQueue queue = new CallbackQueue();

        static string callback_queue_name = "CallbackQueue_UnityMode";
        private static bool application_is_quitting = false;
        private static CallbackQueue_UnityMode instance;
        internal static CallbackQueue_UnityMode Instance()
        {
            if(application_is_quitting == true)
            {
                return null;
            }
            if (null == instance)
            {
                GameObject callback_gameobj = new GameObject(callback_queue_name);
                DontDestroyOnLoad(callback_gameobj);
                instance = callback_gameobj.AddComponent<CallbackQueue_UnityMode>();
            }
            return instance;
        }

        public void EnQueue(Action action)
        {
            queue.EnQueue(action);
        }

        public void StartRun()
        {
            queue.ClearQueue();
        }

        public void Stop()
        {
            queue.ClearQueue();
        }

        private void Process()
        {
            Action action = queue.DeQueue();
            if (null != action)
            {
                action();
            }
            action = null;
        }

        void Awake()
        {

        }

        void Update()
        {
            Process();
        }

        void OnDestory()
        {
            queue.ClearQueue();
            application_is_quitting = true;
        }
    }
#endif

    internal class CallbackQueue_Worker
    {
#if _WIN32
        CallbackQueue_ThreadMode callback_queue_ = CallbackQueue_ThreadMode.Instance();
#else
        CallbackQueue_UnityMode callback_queue_ = CallbackQueue_UnityMode.Instance();
#endif

        private static CallbackQueue_Worker instance;
        internal static CallbackQueue_Worker Instance()
        {
            if (null == instance)
            {
                instance = new CallbackQueue_Worker();
            }
            return instance;
        }

        public void EnQueue(Action action)
        {
            callback_queue_.EnQueue(action);
        }

        public void StartRun()
        {
            callback_queue_.StartRun();
        }

        public void Stop()
        {
            callback_queue_.Stop();
        }
    }
}