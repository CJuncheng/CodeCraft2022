# 项目介绍
> 这是华为2022软挑比赛项目的SDK, 使用C++语言
# 环境配置
- **系统**：Ubuntu 18.04 虚拟机
- **C/C++编译环境**：
    - **make**: GNU make 4.1
    - **cmake** :cmake version 3.22.0
    - **gcc** : 7.3.0
    - **g++** : 7.3.0

# 运行命令
- 编译与运行
```bash
sh build_and_run.sh
```
- 压缩文件以上传结果
```bash
sh CodeCraft_zip.sh
```

# 文件路径
- 数据集文件路径(根目录下)
```bash
/data/demand.csv
/data/qos.csv
/data/site_bandwidth.csv
/data/config.ini
```
- 生成结果文件路径(根目录下)
```bash
/output/solution.txt
```