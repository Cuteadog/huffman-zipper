# 项目函数调用关系图 (Mermaid)

## 1. main 模块 (`main.c`)

```mermaid
flowchart TD
    main["main()"] -->|"-en"| file_zipping["file_zipping()"]
    main -->|"-de"| file_unzipping["file_unzipping()"]

    subgraph 说明
        style_A["🟢 绿色: 本模块函数"]
        style_B["🔵 蓝色: 外部模块函数"]
    end

    classDef local fill:#e1f5e1,stroke:#2e7d32
    classDef external fill:#bbdefb,stroke:#1565c0
    class main local
    class file_zipping,file_unzipping external
```

---

## 2. file_io 模块 (`file_io.c`)

### 2.1 压缩流程

```mermaid
flowchart TD
    file_zipping["file_zipping()"] --> check_file["check_file()"]
    file_zipping --> get_filename["get_filename()"]
    file_zipping --> get_filedir["get_filedir()"]
    file_zipping --> output_zip["output_zip() ↗ format_zip"]

    subgraph 内部辅助函数
        check_file
        get_filename
        get_filedir
    end

    subgraph 外部调用
        output_zip
    end

    classDef local fill:#e1f5e1,stroke:#2e7d32
    classDef external fill:#bbdefb,stroke:#1565c0
    class file_zipping,check_file,get_filename,get_filedir local
    class output_zip external
```

### 2.2 解压流程

```mermaid
flowchart TD
    file_unzipping["file_unzipping()"] --> check_file2["check_file()"]
    file_unzipping --> get_filename2["get_filename()"]
    file_unzipping --> get_filedir2["get_filedir()"]
    file_unzipping --> decode_zip["decode_zip() ↗ format_zip"]

    subgraph 内部辅助函数
        check_file2
        get_filename2
        get_filedir2
    end

    subgraph 外部调用
        decode_zip
    end

    classDef local fill:#e1f5e1,stroke:#2e7d32
    classDef external fill:#bbdefb,stroke:#1565c0
    class file_unzipping,check_file2,get_filename2,get_filedir2 local
    class decode_zip external
```

---

## 3. format_zip 模块 (`format_zip.c`)

### 3.1 压缩：`output_zip()`

```mermaid
flowchart TD
    output_zip["output_zip()"]

    %% 中间调用的函数
    collect_file_data["collect_file_data()"]
    calcu_char_and_len["calcu_char_and_len()"]
    encode["encode() ↗ huffman"]
    write_encoded_str["write_encoded_str()"]
    update_crc32["update_crc32()"]

    %% 底部四个输出区域（水平并列）
    subgraph 输出区域 [输出区域]
        direction LR
        header["📄 文件头<br/>(MAGIC + VERSION + file_cnt)"]
        metadata["📁 元数据区<br/>(namelen + name + len2)"]
        codetable["🔢 码表区<br/>(letter + len + str)"]
        data["💾 压缩数据区<br/>(编码数据 + CRC32)"]
    end

    %% output_zip 直接或间接连接
    output_zip --> write_header["写入文件头"] --> header
    output_zip --> collect_file_data
    output_zip --> write_encoded_str

    %% collect_file_data 内部流程
    collect_file_data --> calcu_char_and_len
    collect_file_data --> encode
    calcu_char_and_len --> calc_len["计算 len1/len2"] --> metadata

    %% encode 生成码表后写入码表区
    encode --> provide_codetable["生成码表"] --> codetable

    %% write_encoded_str 写入压缩数据区
    write_encoded_str --> update_crc32
    update_crc32 --> data

    %% 样式
    classDef local fill:#e1f5e1,stroke:#2e7d32
    classDef external fill:#bbdefb,stroke:#1565c0
    classDef area fill:#fff3e0,stroke:#e65100,stroke-dasharray:5 5

    class output_zip,collect_file_data,calcu_char_and_len,write_encoded_str,update_crc32,calc_len,write_header,provide_codetable local
    class encode external
    class header,metadata,codetable,data area
```

### 3.2 解压：`decode_zip()`

```mermaid
flowchart TD
    decode_zip["decode_zip()"]

    %% 底部四个区域（水平并列）
    subgraph 压缩包结构 [压缩包结构]
        direction LR
        header["📄 文件头<br/>(MAGIC + VERSION + file_cnt)"]
        metadata["📁 元数据区<br/>(namelen + name + len2)"]
        codetable["🔢 码表区<br/>(letter + len + str)"]
        data["💾 压缩数据区<br/>(编码数据 + CRC32)"]
    end

    %% decode_zip 直接连接四个区域
    decode_zip --> header
    decode_zip --> metadata
    decode_zip --> codetable
    decode_zip --> data

    %% 从各区域延伸出对应的处理函数
    header --> read_magic["校验 MAGIC + VERSION"]
    metadata --> read_metadata["读取元数据<br/>(填充 files[])"]
    codetable --> read_codetable["读取码表<br/>(填充 code_table)"]
    data --> read_data["读取压缩数据"]

    %% 码表区后续解码
    read_codetable --> decode["decode() ↗ huffman"]
    decode --> build_tree["重建霍夫曼树"]

    %% 压缩数据区后续写出
    read_data --> write_decoded_file["write_decoded_file()"]
    write_decoded_file --> update_crc32["update_crc32()"]
    write_decoded_file --> traverse["遍历霍夫曼树解码"]
    build_tree --> traverse

    %% 样式
    classDef local fill:#e1f5e1,stroke:#2e7d32
    classDef external fill:#bbdefb,stroke:#1565c0
    classDef area fill:#fff3e0,stroke:#e65100,stroke-dasharray:5 5

    class decode_zip,read_magic,read_metadata,read_codetable,read_data,write_decoded_file,update_crc32,traverse,build_tree local
    class decode external
    class header,metadata,codetable,data area
```

---

## 4. huffman 模块 (`huffman.c`)

### 4.1 编码：`encode()`

```mermaid
flowchart TD
    encode["encode()"]

    %% 单字符特判
    encode -->|"所有字符相同"| single["直接设 len=1 并返回"]
    encode -->|"多字符"| heapify_build["heapify_build()"]

    %% heapify_build 内部
    heapify_build --> nodes_init["nodes_init()<br/>(从 freq_table 初始化 nodes 数组)"]
    nodes_init --> insert_loop["对每个初始节点<br/>heapify_insert()"]
    insert_loop --> heapify_up["heapify_up()"]

    heapify_build --> build_loop["while(heap.len > 1)"]
    build_loop --> extract_left["heapify_extract()<br/>(取出最小节点)"]
    build_loop --> extract_right["heapify_extract()<br/>(取出次小节点)"]
    extract_left --> heapify_down["heapify_down()"]
    extract_right --> heapify_down
    build_loop --> nodes_add["nodes_add(l, r, cnt)<br/>(创建父节点)"]
    nodes_add --> heapify_insert["heapify_insert(父节点)"]
    heapify_insert --> heapify_up
    heapify_insert --> build_loop

    build_loop -->|"堆中只剩一个节点"| root["返回根节点"]

    %% 生成编码
    encode --> form_code["form_code(root, codestr, 0)"]
    form_code --> recurse["递归遍历树"]
    recurse -->|"到达叶节点"| shrink_code["shrink_code()<br/>(位压缩编码串)"]
    shrink_code --> store["存入 code_table"]

    classDef local fill:#e1f5e1,stroke:#2e7d32
    class encode,heapify_build,nodes_init,heapify_insert,heapify_up,heapify_extract,heapify_down,nodes_add,form_code,shrink_code,single,root local
```

（精简版）

```mermaid
flowchart LR
    freq["freq_table"] --> nodes_init["初始化节点"]
    nodes_init --> heap["最小堆"]
    heap --> extract_combine["反复提取最小两个节点<br/>创建父节点并插回堆"]
    extract_combine --> huffman_tree["霍夫曼树"]
    huffman_tree --> form_code["递归生成编码"]
    form_code --> code_table["code_table"]
    code_table --> shrink_code["位压缩"]
    shrink_code --> done["✅ 编码完成"]

    classDef data fill:#fff9c4,stroke:#f9a825
    class freq,code_table data
```

### 4.2 解码：`decode()`

```mermaid
flowchart TD
    decode["decode(letter_cnt)<br/>解码入口"]

    decode --> init["初始化霍夫曼树<br/>清空 nodes 数组，以 nodes[0] 为根节点"]
    init --> loop["遍历码表中的每个字符<br/>(共 letter_cnt 个)"]

    loop --> revert["还原压缩编码<br/>将码表中存储的位压缩编码<br/>转换为 '0'/'1' 字符串"]
    revert --> walk["逐位遍历编码字符串"]

    walk -->|"当前位 = '0'"| left["如果当前节点的左子为空<br/>则新建左子节点"]
    walk -->|"当前位 = '1'"| right["如果当前节点的右子为空<br/>则新建右子节点"]
    left --> move_left["移动到左子节点"]
    right --> move_right["移动到右子节点"]

    move_left --> more_bits["是否还有更多位？"]
    move_right --> more_bits
    more_bits -->|是| walk
    more_bits -->|否| mark["到达叶节点<br/>记录该字符的 letter<br/>并返回根节点"]

    mark --> loop
    loop -->|处理完所有字符| done["返回霍夫曼树根节点<br/>解码准备就绪"]

    classDef local fill:#e1f5e1,stroke:#2e7d32
    class decode,init,loop,revert,walk,left,right,move_left,move_right,more_bits,mark,done local
```

---

## 5. 全局跨模块调用总览

```mermaid
flowchart TD
    main["main()"] -->|"压缩"| file_zipping["file_zipping (file_io)"]
    main -->|"解压"| file_unzipping["file_unzipping (file_io)"]

    file_zipping --> output_zip["output_zip (format_zip)"]
    file_unzipping --> decode_zip["decode_zip (format_zip)"]

    output_zip --> encode["encode (huffman)"]
    decode_zip --> decode["decode (huffman)"]

    classDef mod_main fill:#e8eaf6,stroke:#3949ab
    classDef mod_io fill:#e1f5e1,stroke:#2e7d32
    classDef mod_zip fill:#fff3e0,stroke:#e65100
    classDef mod_huff fill:#fce4ec,stroke:#c62828

    class main mod_main
    class file_zipping,file_unzipping mod_io
    class output_zip,decode_zip mod_zip
    class encode,decode mod_huff
```

---
