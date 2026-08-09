# 1. basic cmd

```sh
# 已经在github server 上配置了ssh key的情况下，可直接将代码 clone 到本地
git clone git@github.com:your_space/your_project.git

# 如果是本地已经有代码想提交至git
# 进入你的项目所在目录
cd your_project_dir
# git 初始化SCM 管理
git init
# 添加要管理的文件
git add *
# 本地提交修改
git commit -m 'you comment'
# 配置需要提交的分支，以及提交至哪个git server 地址， your_git_server_project_addr 需要事先在 git server 上创建
git remote add your_branch_name your_git_server_project_addr
# 提交代码， 指定向服务器提交本地的哪个分支(you_local_branch_name)，以及对应远程服务器的哪个分支(you_remote_branch_name)
git push --set-upstream you_local_branch_name you_remote_branch_name
# 一般来说，刚开始这么写就行，即将本地的 master 分支提交至远程服务器的 master 分支
git push --set-upstream master master
# 查看本地提交状态
git status
# 查看本地目前所处的分支
git branch
# 创建一个新的分支 dev，一般来说开发新的需求需要拉取自己的分支，等调试、测试OK之后，再将自己的分支合并(merge)至 master分支
# master分支代表可在生产环境运行的代码
# git branch dev 会以当前master 的已提交的代码版本为基础，拷贝代码版本至 dev 分支
git branch dev
# 切换至自己的分支
git checkout dev
# 此时可以开始修改自己的代码了，通过测试各个feature 都 OK 后， git commit 本地的所有更改
# 切换至本地的 master分支
git checkout master
#拉取最新的master 分支，（自从你上次从 master 分支拉取修改dev 这个分支的这段时间， master分支可能有新的改动）
git pull
# 将 自己本地的 dev 分支 合并至 master 分支，注意此处为本地的分支合并，还有一种在服务端合并的方法，此处不做介绍
git merge dev
# 可能会有冲突，不同的开发者修改了同一个文件的同一个位置，需要按照 git 的提示解决冲突，然后提交修改
git commit -m 'merge my dev branch to master'
# 提交master分支
git push 

# 删除本地分支
git branch -a
git branch -d branch_want_to_be_deleted
# 如果这个分支没有合并到master， 则需要通过 -D 删除
git branch -D branch_want_to_be_deleted

# 删除远程分支
git branch -a
git push origin --delete branch_want_to_be_deleted

```

# 2. cross Linux  and Windows git repository

git clone  repository cross Linux and Windows



```sh
# disables Git's automatic line-ending conversion globally
git config --global core.autocrlf false
# disables Git's tracking of file permission changes (executable bit) globally
git config --global core.fileMode false
# 设置检出时统一使用 LF
git config core.eol lf
```

