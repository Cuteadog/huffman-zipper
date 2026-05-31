# Huffman Zipper

基于霍夫曼编码，面向Windows系统的多文件压缩程序

## 主要特色：

1. 通过命令行指令运行
2. 支持多文件压缩与解压缩
3. 测算压缩效果并打印
4. 支持通配符

## 程序分工：

| 模块               | 作用                                     |
| ---------------- | -------------------------------------- |
| `main.c\h`       | 程序入口，解析命令行参数、处理通配符、分发压缩或解压任务           |
| `file_io.c\h`    | 文件I/O操作，负责路径解析、文件名提取、目录创建，驱动压缩/解压的整体流程 |
| `format_zip.c\h` | 压缩包格式定义，负责读写.hzip文件结构、CRC32校验及压缩效果统计   |
| `huffman.c\h`    | 霍夫曼编码核心，实现霍夫曼树构建、编码表生成与解码功能            |

[流程图](call_graph.md)

## 使用说明：

1. 运行gcc main.c file_io.c format_zip.c huffman.c -o zipper得到可执行文件（或直接从Releases获取）
2. 在zipper.exe所在目录下，通过命令行运行.\zipper查看帮助信息（直接双击会秒退）
3. 输入.\zipper -en [文件路径]，即可压缩指定文件，将压缩包输出到第一个文件所在目录
4. 输入.\zipper -de [压缩包路径]，即可分别解压各个压缩包

[演示视频](https://www.bilibili.com/video/BV1SnVg61E6p/?spm_id_from=333.1387.homepage.video_card.click&vd_source=6624c1addeacc90d567ea3a68fcb0919)

## 补充说明：

本项目为一个课程大作业，仅用于学习与展示