using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Rotator : MonoBehaviour
{
    void Update()
    {
        // 在每一帧更新物体的旋转
        // 使用 Time.deltaTime 使旋转速度平滑地适应不同的帧率
        transform.Rotate(new Vector3(15, 30, 45) * Time.deltaTime);
    }
}
