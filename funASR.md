# 1. offline

## 1.1 基础镜像

```sh
# 1. 拉取CPU版本的Docker镜像
docker pull registry.cn-hangzhou.aliyuncs.com/funasr_repo/funasr:funasr-runtime-sdk-cpu-0.4.7
# 这个目录存储模型，服务启动后会下载模型
mkdir -p ./funasr-runtime-resources/models
# 启动容器
docker run -p 10095:10095 -dit --privileged=true \
	--name myfunasr \
  	-v $PWD/funasr-runtime-resources/models:/workspace/models \
  	registry.cn-hangzhou.aliyuncs.com/funasr_repo/funasr:funasr-runtime-sdk-cpu-0.4.7

docker run -p 10095:10095 -dit --privileged=true \
	--name myfunasr \
  	-v $PWD/funasr-runtime-resources/models:/workspace/models \
  	funasr-with-ffmpeg:runtime-sdk-cpu-0.4.7

# 查看容器ID
docker ps

# 进入容器（用你实际的容器ID）
docker exec -it myfunasr bash  
# 启动服务
cd /workspace/FunASR/runtime
chmod +x *.sh

# 启动服务，模型会自动下载到 /workspace/models 目录（宿主机挂载的 ./funasr-runtime-resources/models）
nohup bash run_server.sh \
  --download-model-dir /workspace/models \
  --vad-dir damo/speech_fsmn_vad_zh-cn-16k-common-onnx \
  --model-dir damo/speech_paraformer-large-vad-punc_asr_nat-zh-cn-16k-common-vocab8404-onnx \
  --punc-dir damo/punc_ct-transformer_cn-en-common-vocab471067-large-onnx \
  --itn-dir thuduj12/fst_itn_zh \
  --certfile 0 > log.txt 2>&1 &
 
# 查看日志,会看到在下载模型
tail -f log.txt
# 检查服务进程
ps aux | grep run_server

# 测试接口（在宿主机另开终端）
curl http://127.0.0.1:10095/

```

（3）配置 local.properties

下载的模型，应该包含以下及个：

- `damo/speech_fsmn_vad_zh-cn-16k-common-onnx/` （VAD模型）

- `damo/speech_paraformer-large-vad-punc_asr_nat-zh-cn-16k-common-vocab8404-onnx/` （主ASR模型）

- `damo/punc_ct-transformer_cn-en-common-vocab471067-large-onnx/` （标点模型）

- `thuduj12/fst_itn_zh/` （逆文本正则化）

需要在容器内安装ffmpeg，安装好之后，再打包成一个新镜像。

```sh
# 进入容器
docker exec -it myfunasr bash

# 更新包列表并安装 ffmpeg
apt update && apt install -y ffmpeg

# 验证安装
ffmpeg -version
# 退出容器
exit

# 提交为新的容器
docker commit myfunasr funasr-with-ffmpeg:runtime-sdk-cpu-0.4.7
docker rm myfunasr
```

## 1.2 完整镜像

运行新容器

```sh
# 保证目录 /data/funasr-runtime-resources 下面有已经下载好的模型文件
docker run -p 10095:10095 -dit --privileged=true \
	--name myfunasr \
  	-v /data/funasr-runtime-resources/models:/workspace/models \
  	funasr-with-ffmpeg:runtime-sdk-cpu-0.4.7
  	
# 启动服务
# 设置环境变量
docker exec -it myfunasr bash
export MODELSCOPE_DISABLE_DOWNLOAD=1
export HF_HUB_DISABLE_TELEMETRY=1
export FUNASR_DISABLE_DOWNLOAD=1
cd /workspace/FunASR/runtime/websocket/build/bin
# 启动
nohup ./funasr-wss-server \
  --model-dir /workspace/models/damo/speech_paraformer-large-vad-punc_asr_nat-zh-cn-16k-common-vocab8404-onnx \
  --vad-dir /workspace/models/damo/speech_fsmn_vad_zh-cn-16k-common-onnx \
  --punc-dir /workspace/models/damo/punc_ct-transformer_cn-en-common-vocab471067-large-onnx \
  --itn-dir /workspace/models/thuduj12/fst_itn_zh \
  --lm-dir /workspace/models/damo/speech_ngram_lm_zh-cn-ai-wesp-fst \
  --port 10095 \
  --certfile "" \
  --decoder-thread-num 4 \
  --io-thread-num 1 \
  --model-thread-num 1 \
   > /workspace/FunASR/runtime/server.log 2>&1 &
   
# 跟踪日志，看到  asr model init finished. listen on port:10095,服务启动成功
tail -f /workspace/FunASR/runtime/server.log
```

简单版

```sh
docker run -p 10095:10095 -dit --privileged=true --name myfunasr \
  -v /data/funasr-runtime-resources/models:/workspace/models \
  -e MODELSCOPE_DISABLE_DOWNLOAD=1 \
  -e HF_HUB_DISABLE_TELEMETRY=1 \
  -e FUNASR_DISABLE_DOWNLOAD=1 \
  funasr-with-ffmpeg:runtime-sdk-cpu-0.4.7 \
  /bin/bash -c "cd /workspace/FunASR/runtime/websocket/build/bin && ./funasr-wss-server --model-dir /workspace/models/damo/speech_paraformer-large-vad-punc_asr_nat-zh-cn-16k-common-vocab8404-onnx --vad-dir /workspace/models/damo/speech_fsmn_vad_zh-cn-16k-common-onnx --punc-dir /workspace/models/damo/punc_ct-transformer_cn-en-common-vocab471067-large-onnx --itn-dir /workspace/models/thuduj12/fst_itn_zh --lm-dir /workspace/models/damo/speech_ngram_lm_zh-cn-ai-wesp-fst --port 10095 --certfile '' --decoder-thread-num 4 --io-thread-num 1 --model-thread-num 1"

# 查看日志
docker logs -f myfunasr
```

MacOS 无法创建 /data 目录，就在用户当前目录下执行（可能还得在Docker 管理界面的Resource中添加当前的这个路径：/User/xxx//funasr-runtime-resources/models）

可以创建容器之间互相访问的网桥

```sh
docker network create llm_net
```





```sh
# 进入当前用户的根目录
cd ~
# 确保 ./funasr-runtime-resources/models 存在
docker run -p 10095:10095 -dit --privileged=true --name myfunasr \
  --network llm_net \
  -v ./funasr-runtime-resources/models:/workspace/models \
  -e MODELSCOPE_DISABLE_DOWNLOAD=1 \
  -e HF_HUB_DISABLE_TELEMETRY=1 \
  -e FUNASR_DISABLE_DOWNLOAD=1 \
  funasr-with-ffmpeg:runtime-sdk-cpu-0.4.7 \
  /bin/bash -c "cd /workspace/FunASR/runtime/websocket/build/bin && ./funasr-wss-server --model-dir /workspace/models/damo/speech_paraformer-large-vad-punc_asr_nat-zh-cn-16k-common-vocab8404-onnx --vad-dir /workspace/models/damo/speech_fsmn_vad_zh-cn-16k-common-onnx --punc-dir /workspace/models/damo/punc_ct-transformer_cn-en-common-vocab471067-large-onnx --itn-dir /workspace/models/thuduj12/fst_itn_zh --lm-dir /workspace/models/damo/speech_ngram_lm_zh-cn-ai-wesp-fst --port 10095 --certfile '' --decoder-thread-num 4 --io-thread-num 1 --model-thread-num 1"
```



使用官方的脚本

```sh
# 转换脚本
ffmpeg -i input.m4a output.wav
cd /workspace/FunASR/runtime/python/websocket
python ./funasr_wss_client.py --host "127.0.0.1" --port 10095 --ssl 0 --mode offline --audio_in "/home/rd/Downloads/output.wav" --output_dir "./results"
```



# 2. online（尚未验证）

1. 拉取并启动 Docker 镜像

  ## 2.1 拉取实时语音听写镜像
```sh
sudo docker pull \
    registry.cn-hangzhou.aliyuncs.com/funasr_repo/funasr:funasr-runtime-sdk-online-cpu-0.1.13
```

  

  ## 2.2 创建模型目录
```sh
mkdir -p ./funasr-runtime-resources/models
```

  ## 2.3 启动容器

**（1）映射端口和模型目录**

```sh
sudo docker run -p 10096:10095 -it --privileged=true \
    -v $PWD/funasr-runtime-resources/models:/workspace/models \
    registry.cn-hangzhou.aliyuncs.com/funasr_repo/funasr:funasr-runtime-sdk-online-cpu-0.1.13  
```

**（2）在容器内启动服务**

```sh
cd /workspace/FunASR/runtime
  nohup bash run_server_2pass.sh \
    --download-model-dir /workspace/models \
    --vad-dir damo/speech_fsmn_vad_zh-cn-16k-common-onnx \
    --model-dir damo/speech_paraformer-large_asr_nat-zh-cn-16k-common-vocab8404-onnx \
    --online-model-dir damo/speech_paraformer-large_asr_nat-zh-cn-16k-common-vocab8404-online-onnx \
    --punc-dir damo/punc_ct-transformer_zh-cn-common-vad_realtime-vocab272727-onnx \
    --itn-dir thuduj12/fst_itn_zh \
    --hotword /workspace/models/hotwords.txt > log.txt 2>&1 &
```

 **（3）客户端测试**

```sh
python3 funasr_wss_client.py --host "127.0.0.1" --port 10096 --mode 2pass
```

  可选：HTML5 网页客户端

  启动一个 Web 页面，支持浏览器/手机端访问：

```sh
cd /home/rd/workspace/FunASR-main/runtime/html5
python h5Server.py --host 0.0.0.0 --port 1337  
```

  然后浏览器打开 https://127.0.0.1:1337/static/index.html，输入 wss 地址即可使用。



# 3. 对比

  


 服务启动后，WebSocket 地址是 ws://your-server:10095。两个镜像的区别

```sh
  ┌──────┬──────────────────────┬──────────────────────────────────────┐
  │      │ offline 			    │          online                      │
  ├──────┼──────────────────────┼──────────────────────────────────────┤
  │ 输入  │ 完整 WAV 文件         │ WebSocket 流式 PCM chunks              │
  ├──────┼──────────────────────┼──────────────────────────────────────┤
  │ 输出  │ 等全部识别完一次返回 │ 实时增量推送给客户端                       │
  ├──────┼──────────────────────┼──────────────────────────────────────┤
  │ 模型  │ 一遍 Paraformer       │ 2pass（一遍在线模型 + 二遍离线精修）     │
  ├──────┼──────────────────────┼──────────────────────────────────────┤
  │ VAD  │ 不需要                │ 内置 FSMN-VAD 实时检测语音边界          │
  ├──────┼──────────────────────┼──────────────────────────────────────┤
  │ 协议  │ HTTP POST            │ WebSocket                            │
  ├──────┼──────────────────────┼──────────────────────────────────────┤
  │ 适合  │ 文件转写、批量处理     │ 实时对话、语音助手                   │
  └──────┴──────────────────────┴──────────────────────────────────────┘
```

  注意：0.1.12 版本有一个已知的内存泄漏 issue，WebSocket 连接断开后内存不释放，长时间运行需要加定时重启