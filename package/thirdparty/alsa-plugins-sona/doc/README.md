# Sona 音效插件

这是 Sona 音效库的 ALSA 插件，可在 ALSA 配置文件（如 /etc/asound.conf）中定义 `sona_audioaef` 类型的 PCM 来调用 Sona 音效库。

**注意**：出于代码兼容性的考虑，该插件目前只支持单声道、S16_LE 的音频数据。

## 基本用法

在 ALSA 配置文件中类似如下定义即可使用，使用的 Sona 配置文件默认为 /etc/sona_audioaef.conf。

``` alsaconf
pcm.my_pcm {
    type sona_audioaef
    slave.pcm "hw:0,0"
}
```

## 其他配置

其他配置项默认可省略，可在需要的时候进行配置。

- `config_file`：用于指定 Sona 配置文件的路径。默认值为 `"/etc/sona_audioaef.conf"`。

- `max_frames`：Sona 音效库内部 buffer 大小的最大值（以 frames 为单位）。它决定了 Sona 音效库每次可处理的数据量大小。默认值等于 ALSA `hw_params` 中的 `period_size`。

- `runtime_config`：是否使能 RPC 功能，用于在运行过程中通过 PC 软件调节音效参数。默认值为 `false`。

- `save_runtime_config`：若它的值为 `true`，当在 PC 软件发送“保存”指令时，设备端会将当前内存中的配置参数覆盖写入到 `config_file` 指定的文件中。默认值为 `false`。

因此，若需要使用 PC 软件调试音效参数，可以设置如下：

```
pcm.my_pcm {
    type sona_audioaef
    slave.pcm "hw:0,0"
    runtime_config true
}
```

PC 工具的使用说明与注意事项请参考 PC 软件工具的[文档](../pc_tools/doc/README.md)。
