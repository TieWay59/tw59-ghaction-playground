# tw59-ghaction-playground

![build pdf action](https://github.com/TieWay59/tw59-ghaction-playground/actions/workflows/buildpdfaction.yml/badge.svg)

tw59-ghaction-playground

## 实验与需求

在普通 push 的时候，会自动构建结果，存入 Artifact。并且能够上传到仓库里备份。

在 push tag 的时候，自动生成 GitHub Release，以下是对当前 commit 做标记的示例：

```shell
git tag -a v0.0.8 -m "my version 0.0.8"
git push --tags
```

> [git tag 详细文档](https://git-scm.com/book/zh/v2/Git-%E5%9F%BA%E7%A1%80-%E6%89%93%E6%A0%87%E7%AD%BE)

## 问题与解决

- [ ] `softprops/action-gh-release@v1` 和 `git push` 存在顺序冲突，后一项执行会报错。
- [ ] KaTeX 支持两种无穷的写法，`\infin`和`\infty`。其中前者会在 xelatex 变异的时候报错`Undefined control sequence`，这就说明潜在很多兼容性问题了。
- [ ] 行内公式内侧`$`与公式之间不要有空格，否则不会被解析成行内公式。
- [ ] 主线任务：把 md 文档，按照某种良好的模板（解决中文字体，代码排版等问题），借助 pandoc 和 GitHub Action，自动转换成 pdf 打印材料。

目前正在 GitHub Action 执行流程层面进行探索。

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

> `chmod 777 <file>` 授予执行权限。

### Alpine 中文字体安装

除了本地贴入容器的方案，我选择谷歌字体出品的 [font-noto](https://pkgs.alpinelinux.org/package/edge/community/x86/font-noto)，APK（Alpine Package Keeper）包含中文的分支包是 font-noto-cjk。可以在其[官网](https://www.google.com/get/noto/#sans-hans)查看字体的效果。

字体名字是：

- Noto Serif CJK SC
- Noto Sans CJK SC

```bash
apk add --update font-noto-cjk
fc-cache -f # 强制刷新缓存确定更新
fc-list     # 查看本地字体列表本行不用写入 Dockerfile
```

`docker://pandoc/latex` 镜像配置完上面两条基本就可以正常编译使用了。

但此刻还需要关注行内公式的格式，稍微多一个空格就会导识别出错。所以建议使用比较严格的 lint 去格式化自己的文档。

## 参考文献

<https://github.com/Wandmalfarbe/pandoc-latex-template>

<https://github.com/pandoc/pandoc-action-example>

<https://github.com/pandoc/dockerfiles#available-images>

<https://docs.github.com/cn/actions/reference/workflow-syntax-for-github-actions>
