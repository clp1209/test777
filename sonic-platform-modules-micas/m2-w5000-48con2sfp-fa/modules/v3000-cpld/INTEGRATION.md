# WB CPLD驱动集成指南

## 📁 文件清单

已创建的驱动文件：

```
wb-cpld/
├── wb_cpld.h              # 头文件（数据结构、常量、函数声明）
├── wb_cpld_espi.c         # eSPI backend适配层
├── wb_cpld_bus.c          # Bus层（GPIO、锁、事务管理）
├── wb_cpld_dev.c          # Device层（sysfs接口）
├── Makefile                  # 内核树编译
├── Kbuild                    # 外部模块编译
├── Kconfig                   # 配置选项
├── README.md                 # 用户手册
├── ACPI_EXAMPLE.asl          # ACPI配置示例
└── test_wb_cpld.sh        # 测试脚本
```

## ✅ 架构验证

### 设计符合Linus哲学 ✓

1. **简洁性**
   - 每个函数职责单一，不超过50行
   - 缩进不超过3层
   - 无复杂的if/else嵌套

2. **数据结构优先**
   - 清晰的三层架构：eSPI → Bus → Device
   - 统一的事务模板消除特殊情况

3. **Never break userspace**
   - sysfs接口稳定：`regs` bin_attribute
   - 向后兼容的module参数

### 方案可行性分析 ✓

| 设计要点 | 实现状态 | 验证方法 |
|---------|---------|---------|
| eSPI Peripheral Channel | ✅ 使用inb/outb直接访问 | 已分析espi-amd.c |
| CS_CTL GPIO控制 | ✅ AGPIO9/10 via gpiod API | 支持ACPI资源映射 |
| 并发安全 | ✅ mutex串行化事务 | 单例bus锁 |
| sysfs bin_attribute | ✅ regs文件支持pread/pwrite | 符合SONiC规范 |
| 模块化设计 | ✅ 三层解耦，易于调试 | 可独立测试 |

## 🔧 编译与安装

### 方法1: 外部模块编译（推荐用于开发测试）

```bash
cd /path/to/wb-cpld/
make KDIR=/lib/modules/$(uname -r)/build
sudo insmod wb-cpld.ko
```

### 方法2: 集成到SONiC构建系统

1. **添加到platform Makefile**

编辑 `platform/broadcom/sonic-platform-modules-micas/m2-w5000-48con2sfp-fa/modules/Makefile`:

```makefile
obj-$(CONFIG_WB_CPLD) += wb-cpld/
```

2. **配置内核选项**

```bash
# 在内核配置中启用
CONFIG_WB_CPLD=m
```

3. **添加到模块加载列表**

编辑 `platform/.../cfg/modules.conf`:

```
wb-cpld
```

## 📋 上线前TODO（关键）

### 🔴 P0 - 必须完成

- [ ] **ACPI HID确认**
  - 当前占位符: `"WBCPLD"`
  - 需要: 厂商提供的真实HID
  - 修改位置: `wb_cpld_bus.c:266`

- [ ] **GPIO资源映射**
  - 当前假设: ACPI资源名 `"cs-ctl0"`, `"cs-ctl1"`
  - 需确认: AGPIO9/10的ACPI资源定义
  - 修改位置: `wb_cpld_bus.c:170-180`

- [ ] **GPIO极性验证**
  - 当前配置: `GPIOD_OUT_LOW` (active high)
  - 需确认: 硬件电路active high/low
  - 如果active low，改为: `GPIOD_OUT_HIGH | GPIOD_ACTIVE_LOW`

- [ ] **eSPI IO decode range**
  - 当前假设: BIOS已配置0x400~0x4FF
  - 需验证: 通过eSPI寄存器或硬件文档
  - 如未配置，可能需要动态配置（见下节）

### 🟡 P1 - 建议完成

- [ ] **CS切换延时调优**
  - 当前默认: 10us
  - 建议: 使用逻辑分析仪测量实际需要的延时
  - 可通过module param调整: `cs_delay_us=XX`

- [ ] **错误处理增强**
  - 添加CPLD在线检测（读版本寄存器验证）
  - 添加eSPI超时检测

- [ ] **测试覆盖**
  - 单CPLD读写正确性
  - 并发访问稳定性
  - 长时间压力测试

### 🟢 P2 - 可选优化

- [ ] 添加debugfs接口显示统计信息
- [ ] 支持动态配置eSPI decode range
- [ ] 添加寄存器访问tracepoint

## 🐛 故障排查清单

### 驱动加载失败

```bash
# 1. 检查ACPI设备
dmesg | grep -i acpi | grep -i cpld

# 2. 查看GPIO可用性
cat /sys/kernel/debug/gpio

# 3. 检查eSPI驱动
lsmod | grep espi

# 4. 查看详细日志
dmesg | grep wb
```

### 读取到全0xFF或全0x00

可能原因：
1. CS_CTL信号未正确切换 → 检查GPIO配置
2. eSPI decode range未配置 → 检查BIOS设置
3. CPLD未上电或故障 → 检查硬件

调试步骤：
```bash
# 增大延时试试
echo 50 > /sys/module/wb_cpld/parameters/cs_delay_us

# 查看GPIO实时状态
watch -n 0.1 'cat /sys/kernel/debug/gpio | grep AGPIO'
```

## 🚀 快速启动指南

### 1. 验证环境

```bash
# 确认内核版本 >= 5.10
uname -r

# 确认eSPI驱动已加载
lsmod | grep espi

# 确认GPIO可用
ls /sys/class/gpio/
```

### 2. 编译加载

```bash
cd wb-cpld/
make KDIR=/lib/modules/$(uname -r)/build
sudo insmod wb-cpld.ko

# 查看加载状态
dmesg | tail -20
```

### 3. 运行测试

```bash
sudo ./test_wb_cpld.sh
```

### 4. 手动验证

```bash
# 读取CPU CPLD版本
dd if=/sys/devices/platform/wb-cpld-bus.0/cpld_cpu/regs \
   bs=1 skip=0 count=6 | hexdump -C

# 查看版本信息（如果id_dump可用）
cat /sys/devices/platform/wb-cpld-bus.0/cpld_cpu/id_dump
```

## 📞 支持与反馈

如遇到问题，请提供以下信息：

1. 完整的dmesg日志
2. `lspci -vv` 输出（eSPI控制器部分）
3. ACPI表 (`sudo cat /sys/firmware/acpi/tables/DSDT > dsdt.dat`)
4. 硬件平台信息

---

**状态**: ✅ 驱动代码已完成，等待ACPI信息补齐后上线测试

**最后更新**: 2025-01-09
