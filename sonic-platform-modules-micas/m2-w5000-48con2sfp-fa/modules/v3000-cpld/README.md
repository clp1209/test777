# AMD WB CPLD Driver

## 概述

WB CPLD驱动用于访问AMD WB平台上的两块CPLD设备（CPU CPLD和CTRL CPLD）。
通过eSPI Peripheral Channel进行通信，使用GPIO控制的片选信号切换目标设备。

## 架构设计

### 三层架构

```
┌─────────────────────────────────────────┐
│  User Space (dd/cat/echo/open/read)     │
└────────────────┬────────────────────────┘
                 │ /dev/cpld_cpu, /dev/cpld_ctrl
┌────────────────▼────────────────────────┐
│  Device Layer (wb_cpld_dev.c)        │
│  - miscdevice: cpld_cpu, cpld_ctrl      │
│  - file_operations: read/write/llseek   │
└────────────────┬────────────────────────┘
                 │ wb_cpld_read/write_reg
┌────────────────▼────────────────────────┐
│  Bus Layer (wb_cpld_bus.c)           │
│  - Mutex lock (serialize transactions)  │
│  - GPIO CS_CTL control                  │
│  - Transaction: select→access→restore   │
└────────────────┬────────────────────────┘
                 │ wb_espi_read/write8
┌────────────────▼────────────────────────┐
│  eSPI Backend (wb_cpld_espi.c)       │
│  - Direct inb/outb via eSPI             │
│  - Vendor SDK abstraction               │
└─────────────────────────────────────────┘
```

### 片选控制逻辑

- **CS_CTL[1:0]**: 由AGPIO9/AGPIO10控制
  - `00`: 两块CPLD均不接通（默认状态）
  - `01`: CTRL CPLD选中
  - `10`: CPU CPLD选中
  - `11`: 保留

- **事务原子性**:
  ```c
  mutex_lock();
  set_cs_ctl(target);     // 01 or 10
  espi_io_read/write();   // 8-bit access
  set_cs_ctl(00);         // restore
  mutex_unlock();
  ```

## 编译与加载

### 外部模块编译

```bash
cd /path/to/wb-cpld/
make KDIR=/path/to/kernel/source
sudo insmod wb-cpld.ko
```

### 查看模块参数

```bash
modinfo wb-cpld.ko
```

支持的参数：
- `io_base`: eSPI IO基地址（默认0x700）
- `io_size`: IO窗口大小（默认0x100）
- `cs_delay_us`: CS切换延时（默认10us）

### 加载时指定参数

```bash
sudo insmod wb-cpld.ko io_base=0x500 cs_delay_us=20
```

## 用户态接口

### 字符设备节点

驱动加载后会创建以下字符设备：

```
/dev/cpld_cpu    (主设备号: 自动分配, 次设备号: 0)
/dev/cpld_ctrl   (主设备号: 自动分配, 次设备号: 1)
```

### 寄存器访问示例

#### 读取单个寄存器

```bash
# 读取CPU CPLD的寄存器0x10
dd if=/dev/cpld_cpu bs=1 skip=16 count=1 | hexdump -C
```

#### 批量读取

```bash
# 读取0x00~0x05（版本信息）
dd if=/dev/cpld_ctrl bs=1 skip=0 count=6 | hexdump -C
```

#### 写入寄存器

```bash
# 写入0x42到寄存器0x20
printf '\x42' | dd of=/dev/cpld_cpu bs=1 seek=32 conv=notrunc
```

### Python访问示例

```python
import os

# 读取寄存器
def cpld_read(dev, offset):
    path = f"/dev/cpld_{dev}"
    with open(path, "rb") as f:
        f.seek(offset)
        return ord(f.read(1))

# 写入寄存器
def cpld_write(dev, offset, value):
    path = f"/dev/cpld_{dev}"
    with open(path, "r+b") as f:
        f.seek(offset)
        f.write(bytes([value]))

# 读取版本
val = cpld_read("cpu", 0x00)
print(f"Major version: {val}")

# 写入控制寄存器
cpld_write("ctrl", 0x20, 0x42)
```

## TODO列表

### 必须完成（阻塞上线）

1. **ACPI资源配置**
   - 确认HID/UID（当前占位符: `WBCPLD`）
   - 确认GPIO资源名（`cs-ctl0`/`cs-ctl1`）或使用索引
   - 验证eSPI IO decode range已在BIOS中配置

2. **GPIO极性确认**
   - 当前假设ACTIVE_HIGH
   - 需硬件团队确认AGPIO9/10的电气特性
   - 如需要可调整为GPIOD_OUT_LOW | GPIOD_ACTIVE_LOW

3. **CS切换延时调整**
   - 当前默认10us
   - 需实测确认是否足够或可省略

### 可选优化（后续迭代）

1. **eSPI SDK对接**
   - 当前直接使用inb/outb
   - 如需要可改为调用厂商SDK API

2. **寄存器表映射**
   - 扩展`id_dump`显示更多寄存器
   - 或提供Kconfig选项禁用id_dump

3. **错误处理增强**
   - 添加eSPI超时检测
   - 添加CPLD在线检测

## 故障排查

### 驱动加载失败

1. **检查ACPI匹配**
   ```bash
   dmesg | grep wb
   # 如果看到 "ACPI: No matching device" 说明HID不匹配
   ```

2. **检查GPIO资源**
   ```bash
   # 查看可用GPIO
   cat /sys/kernel/debug/gpio
   ```

3. **检查eSPI驱动**
   ```bash
   lsmod | grep espi
   # 确保espi-amd已加载
   ```

### 访问失败（读到0xFF或0x00）

1. **检查CS_CTL信号**
   - 使用逻辑分析仪测量AGPIO9/10
   - 确认切换时序正确

2. **检查eSPI通道**
   ```bash
   # 查看eSPI寄存器状态（需要工具）
   ```

3. **增大CS延时**
   ```bash
   echo 50 > /sys/module/wb_cpld/parameters/cs_delay_us
   ```

## 开发者备注

### 代码风格

- 遵循Linus Torvalds代码哲学
- 函数不超过50行、缩进不超过3层
- 使用`dev_err_probe`简化错误处理
- 优先使用devm_资源管理

### 测试建议

1. **单元测试**: 分别测试两个CPLD的读写
2. **并发测试**: 多进程同时访问
3. **压力测试**: 长时间循环读写
4. **断电恢复**: 验证CS_CTL恢复到00

### 提交前检查清单

- [ ] 使用`checkpatch.pl`检查代码风格
- [ ] 确认无编译警告（`make W=1`）
- [ ] 更新MODULE_AUTHOR信息
- [ ] 完善ACPI HID/GPIO资源名
- [ ] 实测验证所有功能

## 联系方式

如有问题请联系平台驱动团队。
