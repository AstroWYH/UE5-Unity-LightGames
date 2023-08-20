using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.InputSystem;
using TMPro;

public class PlayerController : MonoBehaviour
{
    public float speed; // 玩家移动速度
    private Rigidbody rb; // 刚体组件
    private int count; // 收集的物体数量
    private float movementX; // 水平移动输入
    private float movementY; // 垂直移动输入
    public TextMeshProUGUI countText; // 显示物体数量的文本
    public TextMeshProUGUI timerText; // 显示计时器的文本
    public GameObject winTextObject; // 胜利文本的游戏对象
    public GameObject authorTextObject; // 作者文本的游戏对象
    private float startTime; // 游戏开始时间
    private bool isGameStarted = false; // 游戏是否已开始
    private bool isTimerOn = true; // 计时器是否运行
    private float elapsedTime; // 经过的时间

    void Start()
    {
        count = 0; // 初始物体数量为0
        startTime = Time.time; // 记录游戏开始时间
        isGameStarted = true; // 标记游戏已开始
        rb = GetComponent<Rigidbody>(); // 获取刚体组件
        SetCountText(); // 更新物体数量文本
        winTextObject.SetActive(false); // 初始时隐藏胜利文本
        authorTextObject.SetActive(false); // 初始时隐藏作者文本
    }

    void OnMove(InputValue movementValue)
    {
        Vector2 movementVector = movementValue.Get<Vector2>(); // 获取输入的移动向量
        movementX = movementVector.x; // 获取水平移动输入
        movementY = movementVector.y; // 获取垂直移动输入
    }

    void SetCountText()
    {
        countText.text = "Count: " + count.ToString(); // 更新物体数量文本

        // 根据物体数量设置胜利文本的显示内容和颜色
        if (count < 6)
        {
            winTextObject.SetActive(false); // 物体数量未达到6个时隐藏胜利文本
        }
        else if (6 <= count && count < 12)
        {
            winTextObject.SetActive(true);
            winTextObject.GetComponent<TextMeshProUGUI>().text = "Fighting";
            winTextObject.GetComponent<TextMeshProUGUI>().color = new Color(1.0f, 0.0f, 0.0f); // 设置文本颜色为红色
        }
        else
        {
            winTextObject.SetActive(true);
            winTextObject.GetComponent<TextMeshProUGUI>().text = "You Win!\nTime Elapsed: "
                + elapsedTime.ToString("F2"); // 显示胜利和经过的时间
            winTextObject.GetComponent<TextMeshProUGUI>().color = new Color(
                151.0f / 255.0f, 255.0f / 255.0f, 172.0f / 255.0f); // 设置文本颜色为浅绿色
            isTimerOn = false; // 停止计时器
            authorTextObject.SetActive(true); // 显示作者文本
        }
    }

    void Update()
    {
        if (isGameStarted && isTimerOn)
        {
            elapsedTime = Time.time - startTime; // 计算经过的时间
            timerText.text = "Time: " + elapsedTime.ToString("F2"); // 更新计时器文本
        }
    }

    void FixedUpdate()
    {
        Vector3 movement = new Vector3(movementX, 0.0f, movementY); // 构造移动向量
        rb.AddForce(movement * speed); // 施加移动力

        float boundary = 10.0f; // 场景边界
        // 检测玩家是否越界，如果越界则显示游戏结束文本，停止计时器
        if (transform.position.x < -boundary || transform.position.x > boundary ||
            transform.position.z < -boundary || transform.position.z > boundary)
        {
            winTextObject.SetActive(true);
            winTextObject.GetComponent<TextMeshProUGUI>().text = "Game Over!";
            winTextObject.GetComponent<TextMeshProUGUI>().color = Color.yellow; // 设置文本颜色为黄色
            isTimerOn = false; // 停止计时器
            authorTextObject.SetActive(true); // 显示作者文本
        }
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.CompareTag("PickUp")) // 当玩家与收集物体碰撞时
        {
            other.gameObject.SetActive(false); // 隐藏物体
            count = count + 1; // 增加物体数量
            SetCountText(); // 更新物体数量文本
        }
    }
}
