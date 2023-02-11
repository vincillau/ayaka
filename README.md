# Ayaka

`Ayaka`是一个**HTTP/1.1**服务器，目前仅支持在**Linux**上运行。

## 构建

Ayaka 使用 git submodule 管理依赖，因此需要首先 clone 该仓库和它的 submodule：

```bash
git clone --recurse-submodules git@github.com:vincillau/ayaka.git
```

然后使用 cmake 和 make 构建：

```bash
cd ayaka
mkdir build
cd build
cmake ..
make -j
```

构建后的可执行文件位于 build 目录内。

## 运行

将 build 目录中的 ayaka 可执行文件移动到 res 目录，然后运行：

```bash
cd ../res
./ayaka
```

访问 [http://localhost:8080](http://localhost:8080) 查看网页。

## 配置

Ayaka 会在当前目录中的查找名为 ayaka.json 的文件作为配置文件。配置文件示例如下：

```jsonc
{
    "server": {
        "listen": {
            "type": "ipv4",
            "ip": "127.0.0.1",
            "port": 8080
        },
        "worker_threads": 4
    },
    "http": {
        "mime": "@PROJECT_SOURCE_DIR@/res/mime.types"
    },
    "route": [
        {
            "type": "path",
            "url": "/",
            "path": "@PROJECT_SOURCE_DIR@/res/html/index.html"
        },
        {
            "type": "dir",
            "url": "/static",
            "path": "@PROJECT_SOURCE_DIR@/res/html/"
        }
    ]
}
```

