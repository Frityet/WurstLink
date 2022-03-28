using System;
using System.Net;
using UnityEditor;
using UnityEngine;

namespace WurstLink.Common
{
    [Serializable]
    public struct IPv4Address
    {
        public byte Element1, Element2, Element3, Element4;

        public UInt16 Port;

        public IPv4Address(byte e1, byte e2, byte e3, byte e4, UInt16 port)
        {
            Element1    = e1;
            Element2    = e2;
            Element3    = e3;
            Element4    = e3;
            Port        = port;
        }

        public override string ToString() => $"{Element1}.{Element2}.{Element3}.{Element4}:{Port}";

        public IPAddress ToIPAddress() => new IPAddress(new byte[] { Element1, Element2, Element3, Element4 });
        

        public static implicit operator string(IPv4Address addr) => addr.ToString();
        public static implicit operator IPAddress(IPv4Address addr) => addr.ToIPAddress();
        
        [CustomPropertyDrawer(typeof(IPv4Address))]
        public class IPv4AddressDrawer : PropertyDrawer
        {
            public override float GetPropertyHeight(SerializedProperty property, GUIContent label)
                => EditorGUIUtility.singleLineHeight * (EditorGUIUtility.wideMode ? 1 : 2);
        
            public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
            {
                SerializedProperty  elem1   = property.FindPropertyRelative(nameof(Element1)),
                                    elem2   = property.FindPropertyRelative(nameof(Element2)),
                                    elem3   = property.FindPropertyRelative(nameof(Element3)),
                                    elem4   = property.FindPropertyRelative(nameof(Element4)),
                                    port    = property.FindPropertyRelative(nameof(Port));
        
                EditorGUI.BeginProperty(position, label, property);
                {
                    float posx = position.x + 156, posy = position.y;
                    EditorGUI.LabelField(new Rect(new Vector2(position.x, posy + 3), new Vector2(64, 12)), $"{property.name}");

                    elem1.intValue = EditorGUI.IntField(new Rect(new Vector2(posx, posy), new Vector2(28, 18)), elem1.intValue);
                    EditorGUI.LabelField(new Rect(new Vector2(posx += 28, posy + 8), new Vector2(8, 8)), "."/*, new GUIStyle() { fontSize = 14 }*/);
                    
                    elem2.intValue = EditorGUI.IntField(new Rect(new Vector2(posx += 6, posy), new Vector2(28, 18)), elem2.intValue);
                    EditorGUI.LabelField(new Rect(new Vector2(posx += 28, posy + 8), new Vector2(8, 8)), "."/*, new GUIStyle() { fontSize = 14 }*/);
                    
                    elem3.intValue = EditorGUI.IntField(new Rect(new Vector2(posx += 6, posy), new Vector2(28, 18)), elem3.intValue);
                    EditorGUI.LabelField(new Rect(new Vector2(posx += 28, posy + 8), new Vector2(8, 8)), "."/*, new GUIStyle() { fontSize = 14 }*/);
                    
                    elem4.intValue = EditorGUI.IntField(new Rect(new Vector2(posx += 6, posy), new Vector2(28, 18)), elem4.intValue);
                    
                    EditorGUI.LabelField(new Rect(new Vector2(posx += 28, posy + 4), new Vector2(8, 8)), ":"/*, new GUIStyle() { fontSize = 14 }*/);
                    port.intValue = EditorGUI.IntField(new Rect(new Vector2(posx += 6, posy), new Vector2(42, 18)), port.intValue);
                }
                EditorGUI.EndProperty();

                elem1.intValue = elem1.intValue > Byte.MaxValue ? Byte.MaxValue : elem1.intValue;
                elem2.intValue = elem2.intValue > Byte.MaxValue ? Byte.MaxValue : elem2.intValue;
                elem3.intValue = elem3.intValue > Byte.MaxValue ? Byte.MaxValue : elem3.intValue;
                elem4.intValue = elem4.intValue > Byte.MaxValue ? Byte.MaxValue : elem4.intValue;
                
                port.intValue = port.intValue > UInt16.MaxValue ? UInt16.MaxValue : port.intValue;
            }
        }
    }
}