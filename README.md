## imx6ull_board-level_test_tool

### 系统架构图

![系统框图](picture\系统框图.jpg)

### 显示管理器

* **数据显示部分**在**显示管理器**的管理之下，**支持多种方式**进行数据显示，**显示管理器**可以**向主线程提供数据显示的通用API**，也可以**向底层显示设备提供通用的结构体**，以**方便底层显示设备可以注册进显示管理器**供**主线程使用**

* **显示管理器**向主线程提供的通用**API**如下：

  ```C++
  void dispDevRegister(void); //注册显示设备
  int selectDefaultDispDev(char* pName); //选择一个已经注册的显示设备
  int initDefaultDispDev(void); //初始化选择的显示设备
  void dispDefaultDev(int x, int y, unsigned char c); //在buffer中显示数据
  int flushDispRegion(PDispRegion_S pDispRegion, PDispBuffer_S pDispBuffer); //把buffer中的数据刷到显示设备的内存中
  PDispBuffer_S getDispBuffer(void); //获取buffer
  ```

* 显示管理器用`DispDev`结构体描述底层显示设备，用`DispBuffer`结构体描述显示`buffer`，用结构体`DispRegion`描述`buffer`刷新到硬件显示内存的哪个区域，只有当显示`buffer`刷新到硬件显存之后，数据才会真正的显示；底层显示设备使用显示管理器提供的`int putPixel(int x, int y, unsigned int dwColor)`函数构造自己的数据显示函数，从而在提供给显示管理器，底层显示设备使用`void registerDispDev(PDispDev_S pDispDev)`函数把自己的`DispDev`结构体注册进显示管理器，显示管理器通过链表的形式把所有显示设备连接起来，需要使用的时候就在链表中查询

### 输入管理器

* **数据输入部分**在**输入管理器**的管理之下，支持多种方式输入数据，输入管理器主要**向主线程提供通用的API函数**，屏蔽掉底层硬件复杂的操作逻辑，同时对底层硬件进行抽象管理，用链表连接每个硬件设备，对于需要使用的硬件设备，只需要**注册进输入管理器**即可

* 输入管理器提供给主线程的通用API：

  ```C++
  void inputDevRegister(void);//注册底层硬件输入设备到输入管理器
  int  initInputDevice(void); //初始化所有输入设备
  int userGetInputEventData(PInputEvent_S pInputEvent); //获取输入数据
  ```

  输入管理器抽象出`InputDevice`结构体用来描述每个硬件输入设备

  ```C++
  typedef struct InputDevice
  {
      char* name;             //设备名
  	int   (*inputDevInit)(void); //设备初始化函数
  	int   (*inputDevExit)(void); //设备卸载函数
  	int   (*getInputEvent)(PInputEvent_S pInputEvent); //获取设备的输入数据
  	struct InputDevice* pNext; //指向注册进输入管理器的下一个设备InputDevice结构体
  }InputDevice_S, * PInputDevice_S;
  ```

  其中`InputEvent`结构体用来描述**硬件设备输入的一次完整数据**

  ```C++
  typedef struct InputEvent
  {
      int type;              //数据类型，触摸输入 or 网络输入
      int x;                 //触摸屏X坐标
  	int y;                 //触摸屏Y坐标
  	int pressure;          //压力
  	char str[1024];        //网络输入的字符串
  	struct timeval time;   //系统系统后的时间
  }InputEvent_S, * PInputEvent_S;
  ```

  **底层硬件输入设备**通过输入管理器提供的函数`registerInputDev`把自己**注册进输入管理器**

### 字体管理器

* 交叉编译**zlib-1.2.11**：

  1. 下载

  ` wget http://www.zlib.net/fossils/zlib-1.2.11.tar.gz `

  2. 解压

  `tar -zxvf  zlib-1.2.11.tar.gz `

  3. 指定工具链

  `export CC=arm-linux-gnueabihf-gcc` 

  4. 编译

  `cd /zlib-1.2.11`

  ` ./configure --prefix=./tmp `

  `make`

  `make install`

  5. 把交叉编译生成的头文件放入工具链

  `cd zlib-1.2.11/tmp`

  `cp include/* /home/yhd_wsl2/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../arm-linux-gnueabihf/libc/usr/include`

  `cp -d lib/*so* /home/yhd_wsl2/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../arm-linux-gnueabihf/libc/usr/lib/`

* **交叉编译**`libpng1.6.37`

  1. 解压

  `tar xJf libpng-1.6.37.tar.xz`

  2. 编译

  `./configure --host=arm-linux-gnueabihf --prefix=$PWD/tmp`

  `make`

  `make install`

  3. 把交叉编译生成的头文件放入工具链

  `libpng-1.6.37/tmp`

  `cp include/* -rf /home/yhd_wsl2/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../arm-linux-gnueabihf/libc/usr/include`

  `cp lib/* -rfd /home/yhd_wsl2/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../arm-linux-gnueabihf/libc/usr/lib/`

* **交叉编译**`bzip2-1.0.6`

  1. 下载

  `wget https://sourceforge.net/projects/bzip2/files/bzip2-1.0.6.tar.gz`

  2. 解压

  `tar -zxvf bzip2-1.0.6.tar.gz`

  3. 编译

  `cd bzip2-1.0.6 ` 

  `vim Makefile`

  ![bzip2](H:\oneself_project\git_repo\imx6ull_board-level_test_tool\picture\bzip2.jpg)

  `vim Makefile-libbz2_so`

  ![bzip22](H:\oneself_project\git_repo\imx6ull_board-level_test_tool\picture\bzip22.jpg)

`make && make install`

4. 把交叉编译生成的头文件放入工具链

`cd bzip2-1.0.6/tmp`

`cp include/* -rf /home/yhd_wsl2/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../arm-linux-gnueabihf/libc/usr/include`

`cp lib/* -rfd /home/yhd_wsl2/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../arm-linux-gnueabihf/libc/usr/lib/`

`cd bzip2-1.0.6`

`ln -s libbz2.so.1.0.6 libbz2.so`

`cp libbz2.so* -rfd /home/yhd_wsl2/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../arm-linux-gnueabihf/libc/usr/lib/`

* **交叉编译**`freetype-2.10.2`

  1. 解压

  `tar xJf freetype-2.10.2`

  2. 编译

  `./configure --host=arm-linux-gnueabihf --prefix=$PWD/tmp`

  `make`

  `make install`

  3. 把交叉编译生成的头文件放入工具链

  `cd freetype-2.10.2/tmp`

  `cp include/* -rf /home/yhd_wsl2/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../arm-linux-gnueabihf/libc/usr/include`

  `cp lib/* -rfd /home/yhd_wsl2/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../arm-linux-gnueabihf/libc/usr/lib/`

* 