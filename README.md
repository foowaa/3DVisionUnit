# SuperVehicle
This project works on the Nvidia Jetson TX2 for 3D reconstruction of binocular vision.

项目构成：

| preproc |SSCA | grid | utils | test |
|------------|-----------|--------|--------|---------|
| 预处理（包括基线矫正，图片处理等） | 深度测距 | 栅格地图构建 | I/O以及其他便捷函数 | 测试|

CUDA API:
[http://docs.nvidia.com/cuda/cuda-runtime-api/index.html](http://docs.nvidia.com/cuda/cuda-runtime-api/index.html)

TX2 Resources:
[devtalk.nvidia.com/default/topic/998129/jetson-tx2/links-to-jetson-tx2-resources/](devtalk.nvidia.com/default/topic/998129/jetson-tx2/links-to-jetson-tx2-resources/)
