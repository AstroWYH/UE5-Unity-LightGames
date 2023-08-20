using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraController : MonoBehaviour
{
    public GameObject player; // 玩家对象
    private Vector3 offset; // 相机与玩家的位置偏移

    void Start()
    {
        offset = transform.position - player.transform.position; // 计算初始的位置偏移
    }

    void LateUpdate()
    {
        // 在 LateUpdate 中更新相机位置，以确保在玩家移动后再更新相机位置
        transform.position = player.transform.position + offset; // 更新相机位置，保持相对位置不变
    }
}
