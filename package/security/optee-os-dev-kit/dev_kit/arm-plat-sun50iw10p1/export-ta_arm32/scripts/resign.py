#!/usr/bin/env python
#
# Copyright (c) 2015, Linaro Limited
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

def get_args():
	from argparse import ArgumentParser

	parser = ArgumentParser()
	parser.add_argument('--key', required=True, help='Name of key file')
	parser.add_argument('--in', required=True, dest='inf', \
			help='Name of in file')
	parser.add_argument('--out', required=True, help='Name of out file')
	parser.add_argument('--old_aes_key', help='Name of aes key file',dest='aes_keyf')
	parser.add_argument('--new_aes_key', help='Name of aes key file',dest='aes_keyf_new')
	return parser.parse_args()

def pad(data ,padSize):
	pad = padSize - (len(data) % padSize)
	if pad == 0 :
		pad = padSize

	data = data + chr(pad) * pad
	return data

def unpad(data):
	pad=data[-1]
	pad=int(pad.encode('hex'),16)
	return data[0:-pad]

def main():
	from Crypto.Signature import PKCS1_v1_5
	from Crypto.Hash import SHA256
	from Crypto.PublicKey import RSA
	from Crypto.Cipher import AES
	import struct

	args = get_args()

	f = open(args.key, 'rb')
	key = RSA.importKey(f.read())
	f.close()

	f = open(args.inf, 'rb')
	ta_raw = f.read()
	f.close()

	is_a_encrypted_ta = False
	sunxi_magic=0x41775461
	shdr_magic = 0x4f545348	# SHDR_MAGIC
	sunxi_ta_head_s = struct.Struct('<IIII')
	sunxi_ta_head_data = sunxi_ta_head_s.unpack(ta_raw[0:sunxi_ta_head_s.size])
	if sunxi_magic == sunxi_ta_head_data[0]:
		#encrypted decrypted first
		if args.aes_keyf is None:
			print('ta encrypted, but no old key provided')
			return -1
		f=open(args.aes_keyf,'rb')
		old_aes_key=f.read()
		f.close()
		cipher=AES.new(old_aes_key,AES.MODE_ECB)
		ta_with_sign=cipher.decrypt(ta_raw[sunxi_ta_head_s.size:])
		ta_with_sign = unpad(ta_with_sign)
		old_sunxi_aes_flag = sunxi_ta_head_data[2]
		is_a_encrypted_ta=True
	else:
		old_sunxi_aes_flag=0
		ta_with_sign=ta_raw

	old_head_s = struct.Struct('<IIIIHH')
	old_head_data = old_head_s.unpack(ta_with_sign[0:old_head_s.size])
	if shdr_magic != old_head_data[0]:
		print('magic no correct')
		return -1

	digest_len = old_head_data[4]
	sig_len = old_head_data[5]

	ta_without_sign = ta_with_sign[old_head_s.size+digest_len+sig_len:]
	img = ta_without_sign
        if old_head_data[3] == 0x70004830:
	    signer = PKCS1_v1_5.new(key)
        else:
            print("unsupported signer")
	h = SHA256.new()
	digest_len = h.digest_size
	sig_len = len(signer.sign(h))
	img_size = old_head_data[2]

	magic = old_head_data[0]
	img_type = old_head_data[1]
        if digest_len != old_head_data[4]:
            print("invalid digest size")
	algo = old_head_data[3]
	shdr = struct.pack('<IIIIHH', \
		magic, img_type, img_size, algo, digest_len, sig_len)

	h.update(shdr)
	h.update(img)
	sig = signer.sign(h)


	if not is_a_encrypted_ta:
		f = open(args.out, 'wb')
		f.write(shdr)
		f.write(h.digest())
		f.write(sig)
		f.write(img)
		f.close()
	else:
		if args.aes_keyf_new is None:
			print("did not assigned new aes key for reencrypt")
			return -1
		f = open(args.out, 'wb')
		f.write(shdr)
		f.write(h.digest())
		f.write(sig)
		f.write(img)
		f.close()
		f=open(args.aes_keyf_new,'rb')
		aes_key=f.read()
		f.close()
		f=open(args.out,'rb')
		img=f.read()
		f.close()
		img=pad(img,AES.block_size)
		cipher=AES.new(aes_key,AES.MODE_ECB)
		img=cipher.encrypt(img)
		f=open(args.out,'wb')
		sunxi_magic=0x41775461
		image_size=len(img)
		flag = old_sunxi_aes_flag
		sunxi_ta_head=struct.pack('<IIII',\
			sunxi_magic,image_size,flag,0)
		f.write(sunxi_ta_head)
		f.write(img)
		f.close()

if __name__ == "__main__":
	main()
