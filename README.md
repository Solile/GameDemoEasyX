# C++游戏开发 + EasyX
## 《空洞骑士》

> from Voidmatrix in Bilibili  [BV1sW42197dD](https://www.bilibili.com/video/BV1sW42197dD/?spm_id_from=333.1387.collection.video_card.click&vd_source=3e973ec0e34e920793ed03940c43c111)

空洞骑士是一个pve的单人游戏。黄蜂女回根据当前的血量和状态概率选择下一环节会出的招式。玩家通过主动攻击和滑铲规避对方的伤害。最终打败黄蜂女，获得胜利。

- 主控 AD-移动 W-跳跃 S-滑铲 鼠标左键-攻击

> 游戏界面
![游戏界面](md_resources/1.png "游戏界面-游戏进行时")

### 从项目中学到的东西
- 状态机
    - 把角色不同阶段的代码划分到不同的区域中，有条理进行处理代码
- 碰撞管理器
    - 存放碰撞外形的数据，并统一进行处理优化
    - 后续处理时，不同对象只需要持有管理器中存放碰撞管理箱的引用，通过回调函数执行碰撞发生后的行为
- 资源管理器
    - 同一管理游戏资源(图片、动画、音频)，方便管理使用
- 子弹时间
    - 通过改变帧的大小，模拟出慢动作的感觉。

## 简单的改动
- 帮黄蜂女添加血条
- 改变黄蜂女的出招逻辑，使其AI更灵动

### 未来想做的
- 攻击黄蜂女，随机掉落心，增加玩家的持续性。同时减少玩家伤害。
- 将背景设置为动态(即加一个Camera，随着玩家跳跃而移动)，使其更生动。
- 失败和成功的结算画面更华丽，而不是一个简单的弹窗。