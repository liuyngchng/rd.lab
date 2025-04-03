# 1. 目标

在个人有限的GPU资源（消费别显卡）的条件下，如何使用自己的私有知识库对大模型进行训练？

最终能够输出一个开箱即用的大模型，直接调用大模型即可按照自己的私有知识库进行文本输出，这是本文的目标。

# 2. 基本概念

**（1）训练和推理。**训练是对大模型本身进行修改。推理是使用大模型。二者不同的场景对硬件的要求是有区别的。

**（2）经验法则。**10亿参数，模型微调训练至少需要4-6GB的内存。

**（3）模型精度。**大模型的输入(x)和输出(y)是建立在函数y=f(x) 基础上的。假定 f(x) =ax+b。精度就是a和b的数据精度以及在计算a,b的过程中的计算精度。

**（4）量化。**大模型量化的本质是对大模型中的参数取近似值，假定大模型的输入输出是建立在 f(x) =ax+b 上的。那么， 那么量化，就是把a和b的值的精度降低。例如本来a=3.141592637, 量化后 a =3.14，或者 a =3。 以上只是简单举例，实际量化更复杂，还包括缩放因子（通过缩放（scale）将浮点范围映射到整数（如 INT8）），以及整数计算（如 (a=3.14) 可能量化为 (a_{quant}=31)（scale=0.1））。核心目标是用精度损失换取存储/计算效率提升。

**（5）微调。** 在硬件资源有限的条件下，进行全参数调整是比较困难的。通过Paremer-Efficient Fine-Tuning (PEFT) 进行有限参数子集的调整是一种可行的手段。 PEFT技术中， LoRA（Low Ranking Adaption）比较流行，通过低秩矩阵分解微调模型，仅训练少量参数（原始参数的0.1%-1%），保持基础模型冻结。**不涉及量化**，显存占用仍依赖基础模型精度（如FP16）。

**（6）QLoRA**。Quantized LoRA, LoRA的量化版本，在LoRA的基础上，对权重W进行量化，如图2所示，以进一步减少对GPU显存的需求。

总结一句话，LoRA选择只调整原始模型的部分参数， 而QLoRA在此基础上对选择的这部分参数进行了量化。

（7）微调和RAG 的区别。

| 维度     | 微调（Fine-tuning）                          | RAG（检索增强生成）                          |
| -------- | -------------------------------------------- | -------------------------------------------- |
| 技术路径 | 对模型参数进行训练（会修改模型权重）         | 推理时进行检索（不会修改模型参数）           |
| 知识来源 | 依赖模型参数中存储的“隐性知识”               | 结合模型内部知识和外部“显性知识库”           |
| 资源需求 | 需要大量算力和标注数据（训练成本较高）       | 对算力要求较低，但需要维护知识库（存储成本） |
| 时效性   | 若要更新知识，需重新训练模型                 | 可实时获取最新数据（只需更新知识库）         |
| 输出依赖 | 仅依赖模型自身的参数                         | 依赖检索结果的质量（如检索召回率、相关性）   |
| 适用场景 | 任务固定、数据隐私要求高、需要深度优化的场景 | 知识更新快、多源数据、需要精确事实的场景     |

# 3. 场景

在智能客服等场景中，采用RAG的传统技术方案能够快速投入使用，可以解决大多数问题，少部分问题通过转人工达成客户满意度。

在RAG 技术方案中，对用户提出问题的回答依赖于 RAG 检索知识库结果的质量，而实际场景中用户的提问种类繁多。大模型对自然语言的理解能力目前已经较为成熟， 但是在RAG 方案中，用户提问的问题和RAG检索中使用的知识库之间进行相关性检索有时候会比较困难，此时会导致最终生成的回答很泛化，使得知识库本身的价值没有得到很好的体现。

基于这种情况，考虑通过使用特定场景下的问答对话语料库对大模型进行微调训练，实现基于实际业务的问答深度优化，提升用户满意度。

# 3. 软硬件环境

## 3.1硬件

（1）显卡。Geforce RXT 3090Ti，功率消耗400W（额定450W）, 显存占用 23GB(共24GB)

（2）主机。办公笔记本，ThinkPad_T14_Gen_2_Intel(https://psref.lenovo.com/WDProduct/ThinkPad/ThinkPad_T14_Gen_2_Intel)。

通过Thunderbolt4（雷电4）接口外接显卡扩展坞, CPU 为 11th Gen Intel® Core™ i7-1165G7 × 8， 内存 16GB

## 3.2 软件

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

# 4. 微调训练过程

## 4.1 train log

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

## 4.2 test log

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

## 4.3 训练过程监控

tensorboard 监控过程如下图所示。

![](/home/rd/workspace/rd.lab/img/tensor_board.png)

# 5. 微调



# 6. Reference

[1] 《QLORA: Efficient Finetuning of Quantized LLMs》,  https://arxiv.org/pdf/2305.14314

[2]  QLORA code, https://github.com/artidoro/qlora
