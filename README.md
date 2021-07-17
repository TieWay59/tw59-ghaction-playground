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
- [ ] 主线任务：把 md 文档，按照某种良好的模板（解决中文字体，代码排版等问题），借助 pandoc 和 GitHub Action，自动转换成 pdf 打印材料。

目前正在 GitHub Action 执行流程层面进行探索。
