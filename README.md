# GAVV_Custom_Gochizo
A custom gavv gochizo design~

# PCB Design
[OSHWHUB](https://oshwhub.com/kamenrider-s/gochizo_gavv)

# About
* Used TPL0501 to change the res value so that the driver can load different type of Gochizos...
* Will support BLE so you can change the Gochizo easily...(Todo)
* need a magnet to wake up ESP32C3.
* As I added a gpio to G02, the whole system will wake up when gochizo is pluged into the driver

# 关于
* 使用了TPL0501数字电位器实现的阻值切换
* 会支持蓝牙，这样就可以直接换饱藏了~
* 由于加了霍尔传感器，所以得要磁铁来唤醒。。。
* 现在支持使用插入动作来唤醒饱藏了（因为加了个默认输入的PIN，就不会影响识别~）