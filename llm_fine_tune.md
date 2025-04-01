# 1. 软硬件环境

## 1.1硬件

（1）显卡。Geforce RXT 3090Ti，功率消耗400W（额定450W）, 显存占用 23GB(共24GB)

（2）主机。办公笔记本，ThinkPad_T14_Gen_2_Intel(https://psref.lenovo.com/WDProduct/ThinkPad/ThinkPad_T14_Gen_2_Intel)。

通过Thunderbolt4（雷电4）接口外接显卡扩展坞, CPU 为 11th Gen Intel® Core™ i7-1165G7 × 8， 内存 16GB

## 1.2 软件

（1）基础环境

```sh
rd@rd-t:~$ python3 -V
Python 3.12.3
rd@rd-t:~$ pip -V
pip 24.0 from /usr/lib/python3/dist-packages/pip (python 3.12)
```

（3）算力

```sh
+-----------------------------------------------------------------------------------------+
| NVIDIA-SMI 550.120                Driver Version: 550.120        CUDA Version: 12.4     |
|-----------------------------------------+------------------------+----------------------+
| GPU  Name                 Persistence-M | Bus-Id          Disp.A | Volatile Uncorr. ECC |
| Fan  Temp   Perf          Pwr:Usage/Cap |           Memory-Usage | GPU-Util  Compute M. |
|                                         |                        |               MIG M. |
|=========================================+========================+======================|
|   0  NVIDIA GeForce MX450           Off |   00000000:01:00.0 Off |                  N/A |
| N/A   39C    P0             N/A /    9W |       1MiB /   2048MiB |      0%      Default |
|                                         |                        |                  N/A |
+-----------------------------------------+------------------------+----------------------+
|   1  NVIDIA GeForce RTX 3090 Ti     Off |   00000000:52:00.0 Off |                  Off |
|  0%   43C    P8             15W /  450W |       2MiB /  24564MiB |      0%      Default |
|                                         |                        |                  N/A |
+-----------------------------------------+------------------------+----------------------+
                                                                                         
+-----------------------------------------------------------------------------------------+
| Processes:                                                                              |
|  GPU   GI   CI        PID   Type   Process name                              GPU Memory |
|        ID   ID                                                               Usage      |
|=========================================================================================|
|  No running processes found                                                             |
+-----------------------------------------------------------------------------------------+
```

（4）问答训练样本。约10000条。

```json
{"instruction":"xxx", "input":"xxx", "output":"xxx"}
```

# 2. 微调训练过程

## 2.1 train log

```sh
(pytorch_env) rd@rd-t:~/workspace/pytorch$ ./llm_tune.py 
instruction(使用方法说明)：
	1 - for LLM train task(启动模型训练)
	2 - for LLM  test task(启动模型测试)
	other(其他选项) - nothing done(程序退出)
请输入要执行的任务(1/2):1
2025-04-01 15:41:06,719 - __main__ - INFO -<module> - [228]- 启动模型训练
2025-04-01 15:41:06,775 - __main__ - INFO -check_gpu - [52]- GPU 0: UUID[99b29e6e-b59b-2d02-714f-16bc83525830], name[NVIDIA GeForce RTX 3090 Ti], mem[23.7GB]
2025-04-01 15:41:06,775 - __main__ - INFO -check_gpu - [54]- 当前显存占用: 0.0GB / 23.7GB
Persistence mode is already Enabled for GPU 00000000:01:00.0.
Persistence mode is already Enabled for GPU 00000000:22:00.0.
All done.
2025-04-01 15:41:06,831 - __main__ - INFO -peft_train - [144]- start peft_train, load local model and tokenizer from ../DeepSeek-R1-Distill-Llama-8B
2025-04-01 15:41:06,831 - __main__ - INFO -get_model - [84]- loading model ../DeepSeek-R1-Distill-Llama-8B
Loading checkpoint shards: 100%|██████████████████████████████████████████████████████████████████████████| 2/2 [00:08<00:00,  4.16s/it]
2025-04-01 15:41:15,355 - __main__ - INFO -get_model - [105]- _model_instance.dtype: torch.float16
2025-04-01 15:41:15,357 - __main__ - INFO -peft_train - [147]- parameter efficient fine-tuning
2025-04-01 15:41:15,889 - __main__ - INFO -peft_train - [158]- load local dataset from my.txt
2025-04-01 15:41:15,889 - __main__ - INFO -token_txt - [61]- load localized dataset for txt
2025-04-01 15:41:17,591 - __main__ - INFO -peft_train - [161]- start training
{'loss': 1.5916, 'grad_norm': 1.0895719528198242, 'learning_rate': 0.000152, 'epoch': 25.0}                                             
{'loss': 0.0952, 'grad_norm': 0.28339171409606934, 'learning_rate': 0.00010200000000000001, 'epoch': 50.0}                              
{'loss': 0.0596, 'grad_norm': 0.19577501714229584, 'learning_rate': 5.2000000000000004e-05, 'epoch': 75.0}                              
{'loss': 0.0495, 'grad_norm': 0.29293468594551086, 'learning_rate': 2.0000000000000003e-06, 'epoch': 100.0}                             
{'train_runtime': 248.4568, 'train_samples_per_second': 12.477, 'train_steps_per_second': 0.805, 'train_loss': 0.44900153994560243, 'epoch': 100.0}
100%|█████████████████████████████████████████████████████████████████████████████████████████████████| 200/200 [04:08<00:00,  1.24s/it]
2025-04-01 15:45:26,813 - __main__ - INFO -peft_train - [175]- save merged model to ./txt_trainer
2025-04-01 15:45:51,148 - __main__ - INFO -peft_train - [177]- tokenizer.save_pretrained(./txt_trainer)
```

## 2.2 test log

```sh
(pytorch_env) rd@rd-t:~/workspace/pytorch$ ./llm_tune.py 
instruction(使用方法说明)：
	1 - for LLM train task(启动模型训练)
	2 - for LLM  test task(启动模型测试)
	other(其他选项) - nothing done(程序退出)
请输入要执行的任务(1/2):2
2025-04-01 15:47:29,044 - __main__ - INFO -<module> - [232]- 启动模型测试
2025-04-01 15:47:29,114 - __main__ - INFO -check_gpu - [52]- GPU 0: UUID[99b29e6e-b59b-2d02-714f-16bc83525830], name[NVIDIA GeForce RTX 3090 Ti], mem[23.7GB]
2025-04-01 15:47:29,114 - __main__ - INFO -check_gpu - [54]- 当前显存占用: 0.0GB / 23.7GB
Persistence mode is already Enabled for GPU 00000000:01:00.0.
Persistence mode is already Enabled for GPU 00000000:22:00.0.
All done.
2025-04-01 15:47:29,175 - __main__ - INFO -test_model - [182]- start test_model, load base model ./txt_trainer
2025-04-01 15:47:29,175 - __main__ - INFO -get_model - [84]- loading model ./txt_trainer
Loading checkpoint shards: 100%|██████████████████████████████████████████████████████████████████████████| 2/2 [00:07<00:00,  3.77s/it]
2025-04-01 15:47:36,996 - __main__ - INFO -get_model - [105]- _model_instance.dtype: torch.float16
2025-04-01 15:47:36,996 - __main__ - INFO -test_model - [184]- PEFT base model ./txt_trainer
/home/rd/workspace/pytorch_env/lib/python3.12/site-packages/peft/tuners/tuners_utils.py:167: UserWarning: Already found a `peft_config` attribute in the model. This will lead to having multiple adapters in the model. Make sure to know what you are doing!
  warnings.warn(
2025-04-01 15:47:37,157 - __main__ - INFO -test_model - [188]- load tokenizer ./txt_trainer
2025-04-01 15:47:37,515 - __main__ - INFO -test_model - [190]- build test pipeline
2025-04-01 15:47:37,515 - __main__ - INFO -test_model - [212]- trigger test [instruction]回答燃气服务相关问题
    [input]户内拆改迁移服务怎么做？
    [output]
2025-04-01 15:48:01,171 - __main__ - INFO -test_model - [215]- test result: 户内拆改迁移服务的具体流程是什么？

好的，我现在需要帮助用户了解如何处理户内拆改迁移服务的问题。********。

```

## 2.3 训练过程监控

