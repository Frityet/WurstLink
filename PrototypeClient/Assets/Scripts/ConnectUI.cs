using System;
using TMPro;
using UnityEngine;
using UnityEngine.UI;
using WurstLink.Managers;

namespace WurstLink
{
    public class ConnectUI : MonoBehaviour
    {
        public TMP_InputField AddressInput;
        public Button ConnectButton;

        public void Connect()
        {
            Debug.Log("Connecting to server...");
            NetworkManager.Instance.Connect();
            AddressInput.interactable = false;
            ConnectButton.interactable = false;
        }
    }
}