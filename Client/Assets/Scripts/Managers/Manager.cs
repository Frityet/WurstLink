using System;
using JetBrains.Annotations;
using UnityEngine;

namespace WurstLink.Managers
{
    public class Manager<T> : MonoBehaviour 
                    where T : Manager<T>
    {
        [CanBeNull] public static T Instance { get; private set; }

        private void Awake()
        {
            if (Instance != null && Instance != this)
                Destroy(this);
            else
                Instance = (T)this;
        }
    }
}