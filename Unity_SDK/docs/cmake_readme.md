# cmake构建介绍
    cmake是C类程序通用构建系统，可以生成产品，也可以生成开发和调试工程。

## 构建环境
    cmake 3.15版本以上
    构建需要`emclient-linux`项目（`CMakelists.txt`的`CORE_ROOT_DIRS`）

## 构建产品
    ```
    mkdir build && cd build
    cmake ..
    make
    make install
    ```
### 可调整构建参数
介绍如下：
    **`HC_BUILD_SHARED_LIBS`**：默认构建动态库（Windows是动态库，MacOS是framework），可以设置为`OFF`则构建静态库（Windows是`.dll`，MacOS是`.a`）  
    **`CMAKE_BUILD_TYPE`**: 默认构建调试版本，可选值为`Debug` 和`Release`  
    **`HC_ENABLE_COMPILE_WARNING`**： 默认禁止警告，可以设置`ON`打开警告  

## 构建工程
    ```
    mkdir build && cd build
    cmake .. -G Xcode # (macOS)
    cmake .. -G "Visual Studio 16 2019" # (Windows)
    ```
## 建议构建方式
**采用以下方式进行构建，省去手动输入命令。**

安装`visual studio code`软件, 简称`vscode`  
安装`vscode`插件：
  * [CMake](https://marketplace.visualstudio.com/items?itemName=twxs.cmake)
  * [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
  
当通过`vscode`打开工程之后，cmake插件自动提示这是cmake构建系统（根目录存在`CMakeLists.txt`），自动完成对应提示，就可以编译产品或者生成开发工程。
在`MacOS`下默认是生成非`Xcode`工程，需要手动设置一下。
  * 在项目中添加文件夹`.vscode`
  * 在`.vscode`文件夹下创建`settings.json`文件
  * 在`settings.json`添加如下内容`{"cmake.generator": "Xcode"}`

## 注意事项

    目前，仅支持windows平台和macOS平台。
    其他平台，后续开放。