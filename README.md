# tw59-ghaction-playground

![build pdf action](https://github.com/TieWay59/tw59-ghaction-playground/actions/workflows/buildpdfaction.yml/badge.svg)

本项目是一个，可以把代码目录，整合成 md 文档，再按照某种良好的模板（解决中文字体，代码排版等问题），借助 pandoc 和 GitHub Action，自动转换成 pdf 打印材料。

本项目不需要特殊环境，只需维护自己的`./code`目录，项目预设的工作流就会借用 github 的服务器自动帮你构建代码参考书的打印材料。

## 代办

- [x] 完成基本上述功能。
- [ ] 编写使用手册，对脚本进行注释。
- [ ] 考虑正确得分离“操作”和项目本体。
- [x] 考虑编写代码文件打包的脚本。

## 使用说明

### 文件路径说明

- aritifact : 每次转换作业结束，会提交一份 build.pdf 到此目录下，作为本次作业的最终结果。在使用版本管理软件的时候，记得提交前拉取这一流水线的提交。
- assets : 每次转换作业结束，会把`code`文件夹下的所有代码，按照`python/main.py`生成的 build.md 文件提交到此处。
- backups : 每次作业的产品，会被拷贝一份附上时间戳，提交到本目录下，作为备份。可以定期删除较老的版本。如果完全不需要备份，请修改工作流配置文件。
- code : 存放代码库的目录，请以这个文件夹为根目录。本目录下的任何文件名和子目录名，都是最终结果里的一级标题。
- python : 存放一个 python 脚本，用于对代码进行打包。如果有特殊的内容需求，可以在这份脚本中定制。
- template : pandoc 构建用到的模板文件，来源是：[eisvogel](https://github.com/Wandmalfarbe/pandoc-latex-template)

### 简单使用教程

fork 仓库到自己的账户下。

把`code`路径下的文件换成自己的，然后 push，等待 github action 工作完毕。

在本仓库 push tag 的时候，自动把`Artifac/build.pdf`上传 GitHub Release。以下是对当前 commit 做标记的示例（[git tag 详细文档](https://git-scm.com/book/zh/v2/Git-%E5%9F%BA%E7%A1%80-%E6%89%93%E6%A0%87%E7%AD%BE)）：

```shell
git tag -a v0.0.8 -m "my version 0.0.8"
git push --tags
```

### 使用注意事项

#### 文件深度

建议最深的文件，深度不要超过三个文件夹：

- 根目录 code # 一级标题不会被转化
  - 第一层文件夹 ## 对应 chapter
    - 第二层文件夹 ### 对应 section
      - 第三层文件夹 #### 对应 subsection
        - 第四层的文件 ##### 对应 subsubsection

在第四层之后的路径，都会对应使用五级标题。（参考`python/main.py`）

#### 代码块溢出

一些压行过长的代码会在pdf中发生溢出，这时候我推荐使用vscode打开项目，复制pdf中表现异常的代码片段，借助vscode的项目全局搜索找到这个代码，改一下折行，一劳永逸。

这个过程不会很麻烦，只要平时习惯良好，这种额外的修改不用做很多次。

BTW 代码良好的排版，可以有效提升在打印材料上的阅读体验。

### 开发教程

leave todo

## 配置与问题

### Github Action

主要的障碍在于按照官方手册编写配置，官方的语法介绍比较散。需要广泛的操作系统，shell，docker 等知识基础。

### 配置 latex 环境

指令主要来自 [eisvogel](https://github.com/Wandmalfarbe/pandoc-latex-template) 这个模板的仓库。由于页面比较复杂，所以 `docker://pandoc/latex` 这个镜像内部的 Texlive 宏包不够用。

```bash
tlmgr install adjustbox babel-german background bidi collectbox csquotes everypage filehook footmisc footnotebackref framed fvextra letltxmacro ly1 mdframed mweights needspace pagecolor sourcecodepro sourcesanspro titling ucharcat ulem unicode-math upquote xecjk xurl zref

tlmgr install ctex # 官方镜像里的ctexhook有异常，得重装一下ctex，需要费点时间。
```

> 花边知识：
>
> 运行 alpine 容器的时候，想要接入命令行交互得写`docker run -it alpine /bin/ash`
>
> `/bin/ash` 是 Ash(Almquist Shell)由 BusyBox 提供
>
> `--rm` 退出时自动删除容器(docker run --help)
>
> `-i` 交互模式(即使没有附加也保持 STDIN 打开)
>
> `-t` 分配伪 TTY
>
> `chmod 777 <file>` `chmod +x <file>` 授予执行权限。

### 安装 Alpine 中文字体

除了本地拷贝加入容器的方案，我选择谷歌字体出品的 [font-noto](https://pkgs.alpinelinux.org/package/edge/community/x86/font-noto)，APK（Alpine Package Keeper）包含中文的分支包是 font-noto-cjk。可以在其[官网](https://www.google.com/get/noto/#sans-hans)查看字体的效果。

字体名字是：

- Noto Serif CJK SC
- Noto Sans CJK SC

```bash
apk add --update font-noto-cjk
fc-cache -f  # 强制刷新缓存确定更新
fc-list     # 查看本地字体列表本行不用写入 Dockerfile
```

`docker://pandoc/latex` 镜像配置完上面两条基本就可以正常编译使用了。

但此刻还需要关注行内公式的格式，稍微多一个空格就会导识别出错。所以建议使用比较严格的 lint 去格式化自己的文档。

### 已知的公式问题

- KaTeX 支持两种无穷的写法，`\infin`和`\infty`。其中前者会在 xelatex 变异的时候报错`Undefined control sequence`。

- 行内公式内侧`$`与公式之间不要有空格，否则不会被解析成行内公式。

- 公式内`\begin{align}`存在问题， 一般用 \begin{aligned} 可以。

- 公式内部最好不要空行，有的时候 `$$` 后面甚至不要回车。

- 公式内的`\begin{matrix}\end{matrix}`前后要用`$`包裹。

- 公式内的中文需要用`\mbox{<内容>}`包裹的，但是 typora 的 MathJax.js 或者 KaTeX.js 的脚本可以处理没有包裹的中文，所以很多人会有随便的习惯。

## 参考文献

<https://github.com/Wandmalfarbe/pandoc-latex-template>

<https://github.com/pandoc/pandoc-action-example>

<https://github.com/pandoc/dockerfiles#available-images>

<https://docs.github.com/cn/actions/reference/workflow-syntax-for-github-actions>
