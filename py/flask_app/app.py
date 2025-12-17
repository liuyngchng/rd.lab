from flask import Flask, render_template, request, jsonify

# 创建 Flask 应用实例
app = Flask(__name__)

# 模拟数据
books = [
    {"id": 1, "title": "Python编程入门", "author": "张三", "price": 59.9},
    {"id": 2, "title": "Flask Web开发", "author": "李四", "price": 79.9},
    {"id": 3, "title": "数据结构与算法", "author": "王五", "price": 89.9},
    {"id": 4, "title": "机器学习实战", "author": "赵六", "price": 99.9},
]

# 主页路由
@app.route('/')
def index():
    return render_template('index.html', books=books, title="首页")

# 关于页面
@app.route('/about')
def about():
    return render_template('index.html', books=[], title="关于我们", show_about=True)

# 添加新书（API接口）
@app.route('/api/add_book', methods=['POST'])
def add_book():
    try:
        data = request.json
        new_book = {
            "id": len(books) + 1,
            "title": data.get('title'),
            "author": data.get('author'),
            "price": float(data.get('price', 0))
        }
        books.append(new_book)
        return jsonify({"success": True, "message": "添加成功!", "book": new_book})
    except Exception as e:
        return jsonify({"success": False, "message": str(e)}), 400

# 获取所有书籍（API接口）
@app.route('/api/books')
def get_books():
    return jsonify({"books": books})

# 搜索书籍
@app.route('/search')
def search():
    query = request.args.get('q', '').lower()
    if query:
        results = [book for book in books 
                  if query in book['title'].lower() 
                  or query in book['author'].lower()]
    else:
        results = []
    
    return render_template('index.html', 
                          books=results, 
                          title=f"搜索结果: {query}",
                          search_query=query,
                          show_search=True)

# 运行应用的入口， 例如执行 python app.py ，将从这里开始执行
if __name__ == '__main__':
    app.run(debug=True, port=5000)
