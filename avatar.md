# 严肃企业培训场景 — 数字人技术路线

## 1. 背景

- **场景**：严肃企业级——内部培训 / 数字人
- **环境**：内网开发，Linux 开发机，需代理访问公网
- **需求**：写实风格的 3D 数字人 + 语音驱动 + 表情控制 + 浏览器内交互
- **约束**：不适合使用 VRM 日式动漫风格路线

---

## 2. 推荐路线：glTF + three.js（⭐ 首选）

### 2.1 技术栈

```
three.js
  ├── glTF 2.0 模型（PBR 写实材质）
  ├── Skeletal Animation（骨骼动画）
  ├── Morph Targets / Blend Shapes（表情系统）
  │     └── 推荐 ARKit 52 blendshapes 标准
  ├── TTS 文字转语音
  │     └── 音频 → 音素分析 → 驱动口型 blendshapes
  └── 可选：MediaPipe / WebRTC 面捕或手势交互
```

### 2.2 为什么选这条路线

| 维度     | 说明                                     |
| -------- | ---------------------------------------- |
| 写实度   | PBR 材质天然支持写实渲染，与 VRM 的 MToon 卡通着色完全不同 |
| 兼容性   | three.js 原生支持 glTF，无需插件         |
| 部署     | 纯静态 web 应用，内网直接部署，零外部依赖 |
| 生态     | glTF 是 Khronos 标准（OpenGL 那家），工具链成熟 |
| 表情     | morph targets 与 ARKit blendshapes 一一对应，表达能力丰富 |

---

## 3. 角色创作工具对比

### 3.1 Character Creator 4（CC4）

| 项目       | 说明                                           |
| ---------- | ---------------------------------------------- |
| 开发商     | Reallusion（台湾）                              |
| 类型       | Windows 桌面软件                               |
| 价格       | 约 $199 ~ $999（一次性购买）                    |
| 写实度     | ⭐⭐⭐⭐                                           |
| 导出格式   | FBX、glTF、USD、OBJ                            |
| 特点       | 大量服装/发型资源库，一键导出带骨骼+表情的角色 |
| **推荐度** | ⭐⭐⭐⭐⭐（最适合你的场景）                        |

### 3.2 Daz 3D

| 项目       | 说明                                                 |
| ---------- | ---------------------------------------------------- |
| 开发商     | Daz Productions                                      |
| 类型       | Windows / Mac 桌面软件                               |
| 价格       | 软件免费，角色/服装/发型按个买                         |
| 写实度     | ⭐⭐⭐⭐                                                 |
| 导出格式   | FBX、OBJ（导出 glTF 需要 blender 等工具中转）         |
| 特点       | 免费起步，入门门槛低                                  |
| **推荐度** | ⭐⭐⭐（适合预算有限时先验证效果）                       |

### 3.3 MetaHuman

| 项目       | 说明                                                           |
| ---------- | -------------------------------------------------------------- |
| 开发商     | Epic Games                                                     |
| 类型       | 在线服务（免费） + Unreal Engine 插件                            |
| 写实度     | ⭐⭐⭐⭐⭐（顶级）                                                   |
| 导出格式   | 主要面向 Unreal Engine，导出 glTF 需额外处理                     |
| 特点       | 手机扫描真人脸可直接生成数字分身                                |
| **推荐度** | ⭐⭐（强绑定 Unreal 引擎，不适合你的 three.js 路线）              |

### 3.4 其他可选工具

| 工具                       | 说明                                          |
| -------------------------- | --------------------------------------------- |
| **Blender + addons**       | 开源免费，学习曲线陡峭，但完全可控               |
| **MakeHuman**              | 开源的人类角色生成器，社区版                    |
| **Ready Player Me**        | 在线服务，生成半写实角色，导出 glTF，但偏游戏风 |
| **VRoid Studio**           | 免费，但风格为日式动漫（VRM 生态）               |

---

## 4. 生产 Pipeline 流程

```
┌──────────────────────────────────────────┐
│ 1. 角色创作                                │
│    Character Creator 4 / Daz 3D           │
│    → 捏人（中国企业员工形象）                │
│    → 西装/工装                          │
│    → 绑定骨骼 + ARKit 52 blendshapes     │
└──────────────┬───────────────────────────┘
               ▼
┌──────────────────────────────────────────┐
│ 2. 导出 / 转换                             │
│    导出 glTF 2.0 (.glb 或 .gltf + .bin)   │
│    可选：Blender 中转优化/减面               │
└──────────────┬───────────────────────────┘
               ▼
┌──────────────────────────────────────────┐
│ 3. Web 集成                               │
│    three.js GLTFLoader 加载               │
│    → PBR 材质自动渲染                     │
│    → 骨骼动画播放                         │
│    → Morph targets 表情控制               │
└──────────────┬───────────────────────────┘
               ▼
┌──────────────────────────────────────────┐
│ 4. AI 驱动                                │
│    TTS 文字→语音                          │
│    → 音频分析提取音素                      │
│    → 映射到口型 blendshapes               │
│    → 实时驱动角色说话                      │
└──────────────────────────────────────────┘
```

---

## 5. 关键技术点

### 5.1 表情系统（Blend Shapes）

推荐使用 **ARKit 52 blendshapes** 标准，覆盖几乎所有人类表情：

```
眼睛: eyeBlinkLeft, eyeBlinkRight, eyeLookUp, eyeLookDown,
      eyeLookInLeft, eyeLookOutLeft, eyeLookInRight, eyeLookOutRight
嘴巴: jawOpen, jawForward, jawLeft, jawRight,
      mouthFunnel, mouthPucker, mouthLeft, mouthRight,
      mouthSmileLeft, mouthSmileRight, mouthFrownLeft, mouthFrownRight,
      ...
脸颊: cheekPuff, cheekSquintLeft, cheekSquintRight
鼻子: noseSneerLeft, noseSneerRight
眉毛: browDownLeft, browDownRight, browInnerUp, browOuterUpLeft, ...
```

Character Creator 4 和 Daz 3D 均支持导出此标准。

### 5.2 语音驱动口型（Lip-sync）

```
TTS 引擎 → 音频流 + 音素时间戳
    → 将音素映射到 viseme blendshape weights
    → 每帧更新 morph target 权重
```

可用方案：

| 方案                     | 类型       | 说明                         |
| ------------------------ | ---------- | ---------------------------- |
| **rhubarb-lip-sync**     | 开源 CLI   | 离线生成音素序列，精准度高   |
| **Azure Speech SDK**     | 云服务     | TTS + viseme 一起输出        |
| **Web Speech API**       | 浏览器内置 | 免费，但无法获取音素数据     |
| **Oculus Lipsync**       | 本地 SDK   | Meta 出品，实时效果优秀      |

### 5.3 内网部署

```
nginx / CDN → 静态文件
    ├── index.html
    ├── three.js (ES module)
    ├── model.glb (角色模型)
    └── audio assets (TTS 音频)
```

纯前端方案，无需服务端渲染，内网直接部署。

---

## 6. 如果仍然想用 VRM 生态

VRM 并非不能用，而是风格限制：

| 场景                     | VRM 适用？          |
| ------------------------ | ------------------- |
| VTuber / 虚拟主播         | ✅ 非常适合          |
| 游戏角色                 | ✅ 非常适合          |
| 年轻化品牌形象           | ✅ 可以              |
| 严肃企业培训             | ⚠️ 风格偏卡通        |
| 银行/保险/政务           | ❌ 不够正式          |

如果企业接受"偏写实但略带风格化"的外观，可以用 VRM + 自定义 PBR 材质替换 MToon，但需要额外开发。

---

## 7. 下一步建议

1. **验证可行性**：用 Daz 3D（免费）捏一个写实角色 → 导出 FBX → Blender 转 glTF → three.js 加载，跑通整个流程
2. **确认产出**：效果满意后，购买 Character Creator 4 做正式生产
3. **集成 AI**：接入 TTS + lip-sync，实现语音驱动
4. **打磨细节**：眼神、微表情、手势、身体语言

---

## 8. 参考资源

- [three.js glTF 文档](https://threejs.org/docs/#manual/en/introduction/Loading-3D-models)
- [glTF 2.0 规范](https://registry.khronos.org/glTF/)
- [ARKit Blendshapes 参考](https://developer.apple.com/documentation/arkit/arfaceanchor/blendshapelocation)
- [Character Creator 4](https://www.reallusion.com/character-creator/)
- [Daz 3D](https://www.daz3d.com/)
- [rhubarb-lip-sync](https://github.com/DanielSWolf/rhubarb-lip-sync)

## 9. 其他



加载查看 vrm 文件

https://www.opensourceavatars.com/en/vrminspector



https://hub.vroid.com/





文本生成 3d model



git@github.com:KokiichiiCoding/Stunning-Modeler.git

https://github.com/KokiichiiCoding/Stunning-Modeler.git





3d model可下载



https://www.cgmodel.com/model/537967.html