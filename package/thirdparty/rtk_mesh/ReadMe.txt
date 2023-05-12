编译test app：
1.进入mesh-wrapper文件夹，执行make，在mesh-wrapper上一级目录中生成libmesh-wrapper.so
2.进入mesh_thread文件夹，执行make，在mesh_thread上一级文件夹中生成libmesh-api.so和可执行文件mesh_thread_provisioner

测试应用mesh_thread_provisioner依赖libmesh-api.so、libmesh-wrapper.so、libmesh-lib.so
测试应用操作说明见Mesh SDK User Guide