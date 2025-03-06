# 1. setup

```sh
# 安装 flask 框架
pip3 install flask
# 安装 web容器
pip3 install gunicorn
```

# 2. demo

​		`vi my_flask.py`, 创建一个 my_flask.py 文件，内容如下。

```python
#! /usr/bin/python3

from flask import Flask, request, jsonify

app = Flask(__name__)


@app.route('/')
def hello_world():
    """
     A hello world test
    curl -s --noproxy '*' http://127.0.0.1:19000 | jq
    :return:
    """
    return '{"status":200, "msg":"Hello World!"}'

@app.route('/greet/<name>')
def greet(name):
    """
    get param from URI path
    :param name:
    :return:
    """
    return f'Hello {name}'


@app.route('/msg')
def get_msg():
    """
    return a  JSON msg
    :return:
    """
    return '{"status":200, "msg":"a new message"}'

@app.route('/submit', methods=['POST'])
def submit():
    """
    form submit, get data from form
    :return:
    """
    username = request.form.get('username')
    return f'Hello, {username}!'


@app.route('/data', methods=['POST'])
def get_data():
    """
    JSON submit, get data from application JSON
    curl -s --noproxy '*' -X POST  'http://127.0.0.1:19000/data' -H "Content-Type: application/json"  -d '{"msg":"who are you?"}'
    :return:
    """
    data = request.get_json()
    print(data)
    return jsonify({"message": "Data received successfully!", "data": data}), 200


if __name__ == '__main__':
    """
    just for test， 生产环境不建议这么做
    """
    app.run(host='0.0.0.0', port=19000)

```

# 3. deploy

## 3.1 package

​		生成项目依赖，在项目根目录下执行

```sh
pip3 freeze > requirements.txt
```

​		编写 Dockerfile, 文件名为 my_flask_Dockerfile， 内容如下所示

```dockerfile
# 使用本地镜像文件名称
FROM python:3.10-slim

WORKDIR /app

COPY requirements.txt requirements.txt
RUN pip install -r requirements.txt

COPY . .

CMD ["gunicorn", "-b", "0.0.0.0:8000", "my_flask:app"]
```

 		run

```sh
docker build -f my_flask_Dockerfile ./ -t my-flask-app
docker run -p 19000:8000 my_flask-app
```



## 3.1 run		

在生产环境进行部署

```sh
# -w 4表示使用4个工作进程。
# -b 127.0.0.1:8000表示绑定到本地主机的8000端口。
# test_flask是Python文件名，app是Flask实例
gunicorn -w 4 -b 127.0.0.1:19000 my_flask:app
```



