import base64
import json
from Crypto.Cipher import AES
import hashlib
import os
import sys
import datetime
'''
采用AES对称加密算法
'''


class Encrypt:
    def __new__(cls, *args, **kwargs):
        if not hasattr(cls, '_instance'):
            cls._instance = object.__new__(cls)
        return cls._instance

    def __init__(self, secret_key=None):
        UPGRADE_SECRET_KEY = "BCNZXCBGDASHJDHAJKDHJSH"
        self.secret_key = UPGRADE_SECRET_KEY if not secret_key else secret_key

    @staticmethod
    def add_to_16(value):
        while len(value) % 16 != 0:
            value += '\0'
        return str.encode(value)  # 返回bytes

    def encrypt_oracle(self, text):
        # 初始化加密器
        aes = AES.new(self.add_to_16(self.secret_key), AES.MODE_ECB)
        # 先进行aes加密
        encrypt_aes = aes.encrypt(self.add_to_16(text))
        # 用base64转成字符串形式
        encrypted_text = str(base64.encodebytes(encrypt_aes), encoding='utf-8')  # 执行加密并转码返回bytes
        return encrypted_text

    # 解密方法
    def decrypt_oralce(self, text):
        # 初始化加密器
        aes = AES.new(self.add_to_16(self.secret_key), AES.MODE_ECB)
        # 优先逆向解密base64成bytes
        base64_decrypted = base64.decodebytes(text.encode(encoding='utf-8'))
        # 执行解密密并转码返回str
        decrypted_text = str(aes.decrypt(base64_decrypted), encoding='utf-8').replace('\0', '')
        return decrypted_text


def set_u_disk_tar(path, config_dir=None):
    md5_value = md5files(path)
    print("md5sum {} equal {}".format(path, md5_value))
    en = Encrypt()
    d = datetime.datetime.now()
    date_str = "%04d%02d%02d%02d%02d%02d" % (d.year, d.month, d.day, d.hour, d.minute, d.second)
    date_int = int(date_str)
    content = {
        "version": date_int,
        "hash": md5_value,
        "type": "whole_machine",
        "file": os.path.basename(path)
    }
    content = json.dumps(content)
    a1 = en.encrypt_oracle(content)
    if config_dir is None:
        config_location = path.replace(os.path.basename(path), "config.ini")
    else:
        config_location = os.path.join(config_dir, "config.ini")
    with open(config_location, 'wb') as f:
        f.write(a1.encode())
    print("create udisk install package config file[{}] success\nconfig.ini:\n{}".format(config_location, a1))
    a2 = en.decrypt_oralce(a1)
    print(json.loads(a2))


def md5files(path):
    with open(path, 'rb') as fp:
        data = fp.read()
    file_md5 = hashlib.md5(data).hexdigest()
    return file_md5


if __name__ == '__main__':
    argv_len = len(sys.argv)
    if 2 <= argv_len <= 3:
        path = sys.argv[1].rstrip()
        if len(sys.argv) == 3:
            config_path = sys.argv[2].rstrip()
            if not os.path.isdir(config_path):
                raise Exception("config path is not exists")
        else:
            config_path = None
        if os.path.isfile(path):
            if path.endswith(".swu"):
                set_u_disk_tar(path, config_path)
                sys.exit()
        raise Exception("{} is not current .swu file".format(path))
    else:
        raise Exception("params is error,correct use example:\n"
                        "create_c-smart-brain_install_config.exe tina-r818-evb2_1.0.2.34.98.swu")


