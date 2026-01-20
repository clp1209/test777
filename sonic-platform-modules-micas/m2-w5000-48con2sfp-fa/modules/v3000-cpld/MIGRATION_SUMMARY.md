# WB CPLD Driver Migration Summary

## 从 sysfs bin_attribute 迁移到 miscdevice

### 修改时间
2026-01-12

### 迁移原因
参考现有LPC CPLD驱动（lpc_dbg.c），采用标准字符设备接口，提供更统一的访问方式。

---

## 核心变更

### 1. 用户接口变更

**之前 (sysfs bin_attribute):**
```bash
# 读取寄存器
dd if=/sys/devices/platform/wb-cpld-bus.0/cpld_cpu/regs bs=1 skip=16 count=1

# 写入寄存器
printf '\x42' | dd of=/sys/.../cpld_cpu/regs bs=1 seek=32 conv=notrunc
```

**现在 (miscdevice):**
```bash
# 读取寄存器
dd if=/dev/cpld_cpu bs=1 skip=16 count=1

# 写入寄存器
printf '\x42' | dd of=/dev/cpld_cpu bs=1 seek=32 conv=notrunc
```

### 2. 代码变更

#### wb_cpld.h
- 添加 `#include <linux/miscdevice.h>`
- 在 `struct wb_cpld_dev` 中添加 `struct miscdevice miscdev` 成员
- 添加 `WB_CPLD_MAX_DEVS` 常量 (2)
- 添加全局设备数组 `g_wb_cpld_devs[WB_CPLD_MAX_DEVS]`

#### wb_cpld_dev.c
完全重写为miscdevice驱动：
- 实现 `file_operations` 结构体：
  - `cpld_dev_open()` - 打开设备
  - `cpld_dev_release()` - 关闭设备
  - `cpld_dev_read()` - 读取寄存器
  - `cpld_dev_write()` - 写入寄存器
  - `cpld_dev_llseek()` - 定位操作
  - `cpld_dev_ioctl()` - 保留接口
- 使用 `misc_register()` 创建 /dev/cpld_cpu 和 /dev/cpld_ctrl
- 保持与Bus层的调用接口不变（wb_cpld_read_reg/write_reg）

#### wb_cpld_bus.c
**无需修改** - Bus层接口保持不变

#### wb_cpld_espi.c
**无需修改** - eSPI后端保持不变

---

## 架构优势

### 保留的设计
✅ 三层架构完全保留（Device → Bus → eSPI Backend）
✅ GPIO CS_CTL控制逻辑不变
✅ Mutex事务串行化不变
✅ ACPI平台驱动不变

### 接口改进
✅ 更符合Linux字符设备规范
✅ 与现有LPC CPLD驱动风格一致
✅ 支持标准文件操作（open/read/write/lseek）
✅ 自动设备节点创建（udev）

---

## 编译验证

```bash
cd wb-cpld/
make -f Kbuild
ls -lh wb-cpld.ko
# -rw-rw-r-- 1 micasrd micasrd 729K Jan 12 00:54 wb-cpld.ko

modinfo wb-cpld.ko | head -10
# filename:       wb-cpld.ko
# license:        GPL
# description:    AMD WB CPLD device layer
# ...
```

编译成功 ✅

---

## 代码统计

| 文件 | 行数变化 |
|-----|---------|
| wb_cpld.h | 83 → 87 (+4) |
| wb_cpld_dev.c | 248 → 276 (+28) |
| wb_cpld_bus.c | 267 (不变) |
| wb_cpld_espi.c | 161 (不变) |
| **总计** | **759 → 787** (+28) |

---

## 测试计划

### 基础功能测试
1. 加载驱动后检查设备节点是否存在
   ```bash
   ls -l /dev/cpld_cpu /dev/cpld_ctrl
   ```

2. 读取CPLD版本寄存器
   ```bash
   dd if=/dev/cpld_cpu bs=1 skip=0 count=6 | hexdump -C
   ```

3. 写入测试（需确认安全寄存器地址）

### Python集成测试
```python
with open("/dev/cpld_cpu", "rb") as f:
    f.seek(0x00)
    version = f.read(6)
    print(version.hex())
```

---

## 后续工作

### 立即需要
- [ ] 在实际硬件上测试加载模块
- [ ] 验证CPLD寄存器读写功能
- [ ] 确认GPIO CS_CTL切换时序

### 可选优化
- [ ] 添加 ioctl 命令支持批量读写
- [ ] 添加 debugfs 统计接口
- [ ] 性能测试与优化

---

## 兼容性说明

### 破坏性变更
⚠️ 用户空间接口从 `/sys/.../regs` 改为 `/dev/cpld_*`
⚠️ 上层Python代码需更新设备路径

### 保持兼容
✅ 模块参数不变（io_base, io_size, cs_delay_us）
✅ ACPI绑定不变（HID: WBCPLD）
✅ 寄存器地址映射不变

---

## 参考文档

- [wb_cpld/README.md](README.md) - 完整用户手册
- [wb_cpld/PROJECT_SUMMARY.md](PROJECT_SUMMARY.md) - 项目总结
- [ats资料/.../lpc_dbg.c](../../ats资料/cust_wba_202205/common/modules/lpc_dbg.c) - LPC参考实现

---

**迁移状态**: ✅ 完成
**编译状态**: ✅ 通过
**文档状态**: ✅ 已更新
**测试状态**: ⏳ 等待硬件验证

