using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace WurstLink.Managers
{
    public abstract class Manager<T>    : MonoBehaviour 
                            where T     : Manager<T>
    {
        public static T Instance { get; private set; }

        private void Awake()
        {
            if (Instance == null)
            {
                Instance = (T)this;
            }
        }
    }
}