# 1. 文档

官方文档详见 https://docs.ultralytics.com/zh。

# 2. 环境

创建开发环境。

```sh
# 创建虚拟环境
virtual env yolo_env
source yolo_env/bin/activate
# 安装/更新 ultralytics 组件
pip install -U ultralytics
# 安装
```



# 3. 标注工具

主流开源的图片标注工具如下所示。

| 工具名称         | 核心特点与优势                                               | 适用场景                                                     | 上手难度 |
| :--------------- | :----------------------------------------------------------- | :----------------------------------------------------------- | :------- |
| **CVAT**         | **视频与复杂标注的首选**。开源社区最强大的工具之一，基于Web，支持多人协作。尤其在视频标注方面，有**插值和自动跟踪**功能，能极大提高效率。 | 项目涉及视频、需要多人协作、需要处理复杂的多边形或关键点标注。 | ⭐⭐⭐      |
| **Label Studio** | **多面手与格式转换器**。一个通用的、可配置的标注平台，支持图像、文本、音频等几乎所有数据类型。它最大的优势是**可以直接导出YOLO格式**，减少了格式转换的麻烦。 | 需要标注多种类型的数据、希望一个工具搞定所有格式、追求灵活的界面配置。 | ⭐⭐       |
| **LabelImg**     | **轻量级经典工具**。这是最经典的图像标注工具，如果你只需要标注**矩形框（Bounding Box）** 来训练YOLO，它是最简单、直接的选择。安装快，操作直观，支持Windows/Linux/macOS。 | 任务单一（纯图片）、只做目标检测、追求快速上手和简单操作。   | ⭐        |
| **annobel**      | **YOLO全家桶式助手**。这是一个较新的Python工具，巧妙地将YOLO模型用于**自动预标注**，然后让你手动修正。它生来就为YOLO格式服务，可以极大地提高你的标注效率。 | 希望利用AI加速标注过程、想在本地用Python脚本自动化处理、追求从标注到训练的紧密集成。 | ⭐⭐       |



```sh
pip install labelimg
pip install label-studio
```

# 4. Label-Studio

下面以 label-studio 为例，说明数据标注过程及格式。

**（1）启动服务**

```sh
label-studio start
```

**（2）创建项目**。项目创建后，需要配置具体的任务信息， 即数据使用的具体用途。

**（3）上传图片。**对图片进行标注。

**（4）导出数据。**按照所需要的数据格式，导出具体的数据， 例如 `COCO`,`YOLO` 等格式。

以进行目标检测任务的 `YOLOv8` `OBB` with Images 数据格式为例（`OBB`， Oriented Bounding Box ，可旋转的矩形框），其导出的数据格式如下所示。

```
.
├── classes.txt
├── images
│   ├── a.png
│   ├── b.png
│   ├── c.png
│   └── d.png
├── labels
│   ├── a.txt
│   ├── b.txt
│   ├── c.txt
│   └── d.txt
└── notes.json
```

其总 `classes.txt` 内容如下，包含所有需要检测的目标的枚举值，这些信息是在创建项目的时候配置的。

```
bird
cat
dog
elephant
grass
human
tiger
```

`images` 文件夹为用户上传的原始图片， `labels` 下面为每个图片具体的标签信息。以a.txt 为例, 其文本信息如下(实际数据小数点后的位数较多，此处省略)

```sh
3 0.8581 0.2821 0.9967 0.2829 0.9967 0.4302 0.8581 0.4302
2 0.5758 0.0338 0.7656 0.0338 0.7656 0.1877 0.5758 0.1877
```

其含义如下所示

```sh
6 0.6095 0.6779 0.7244 0.6779 0.7244 0.7835 0.6095 0.7835
│ │      │      │      │      │      │      │      │
│ │      │      │      │      │      │      │      └── 点4 Y
│ │      │      │      │      │      │      └────────── 点4 X
│ │      │      │      │      │      └───────────────── 点3 Y
│ │      │      │      │      └──────────────────────── 点3 X
│ │      │      │      └──────────────────────────────── 点2 Y
│ │      │      └─────────────────────────────────────── 点2 X
│ │      └────────────────────────────────────────────── 点1 Y
│ └───────────────────────────────────────────────────── 点1 X
└────────────────────────────────────────────────────── 类别ID
```

导出的数据格式，还需要添加一个 `data.yaml` 配置文件，才能作为 `YOLO` 进行模型训练的数据集，可以通过下面的脚本自动生成。

```python
import yaml
import os

# 读取 classes.txt
with open('classes.txt', 'r') as f:
    classes = [line.strip() for line in f.readlines()]

# 获取当前目录的绝对路径
current_path = os.path.abspath('.')

# 构建 data.yaml 内容
data_config = {
    'path': current_path,      # 数据集根目录
    'train': 'images',          # 训练图片目录
    'val': 'images',            # 验证图片目录（暂时和训练一样）
    'nc': len(classes),         # 类别数量
    'names': classes            # 类别名称列表
}

# 写入 data.yaml
with open('data.yaml', 'w', encoding='utf-8') as f:
    yaml.dump(data_config, f, sort_keys=False, allow_unicode=True)

print(f"✅ 成功创建 data.yaml")
print(f"   - 类别数量: {len(classes)}")
print(f"   - 类别名称: {classes}")
```

创建的 data.yaml 文件内容如下所示

```yaml
path: /home/rd/Downloads/yolo_data_input
train: images
val: images
nc: 7
names:
- bird
- cat
- dog
- elephant
- grass
- human
- tiger
```



# 5. `CVAT`

Computer Vision Annotation Tool (`CVAT`)， 计算机视觉标注工具。

**（1）安装**

可以通过源代码安装，也可以采用docker 镜像。详见  https://github.com/cvat-ai/cvat。

```sh
git clone git@github.com:cvat-ai/cvat.git
```



