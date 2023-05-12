/*
 * This confidential and proprietary software should be used
 * under the licensing agreement from Allwinner Technology.
 *
 * Copyright (C) 2020 Allwinner Technology Limited
 * All rights reserved.
 *
 * Author: Albert Yu <yuxyun@allwinnertech.com>
 *
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from Allwinner Technology Limited.
 */

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#define DATA_SIZE 3
#define KERNEL_SRC_FILE "cl_vec3_add.cl"

int main(void)
{
	int i;
	FILE *fp;
	char *source_str = NULL;
	int a[DATA_SIZE] = { 1, 2, 3};
	int b[DATA_SIZE] = { 4, 5, 6};
	int c[DATA_SIZE] = { 0 };
	size_t source_size;
	cl_int cl_ret;
	cl_platform_id platform_id;
	cl_device_id device_id;
	cl_context context;
	cl_command_queue command_queue;
	cl_mem mem_a, mem_b, mem_c;
	cl_program program;
	cl_kernel kernel;
	size_t global_item_size = DATA_SIZE;
	size_t local_item_size = DATA_SIZE;

	fp = fopen(KERNEL_SRC_FILE, "r");
	if (!fp) {
		printf("Failed to open %s\n", KERNEL_SRC_FILE);
		return -1;
	}

	if (fseek(fp, 0, SEEK_END))
		goto exit;

	source_size = ftell(fp);

	rewind(fp);

	source_str = (char*)malloc(source_size);
	if (!source_str) {
		printf("Failed to allocate memory for source_str\n");
		goto exit;
	}

	/* Step 1: read kernel source code from kernel file */
	source_size = fread(source_str, 1, source_size, fp);
	if (source_size <= 0) {
		printf("Failed to read kernel source code from kernel file\n");
		goto exit;
	}

	/* Step 2: get the platform id */
	cl_ret = clGetPlatformIDs(1, &platform_id, NULL);
	if (cl_ret != CL_SUCCESS) {
		printf("clGetPlatformIDs failed, cl_ret=%d\n", cl_ret);
		goto exit;
	}

	/* Step 3: get the device id */
	cl_ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
	if (cl_ret != CL_SUCCESS) {
		printf("clGetDeviceIDs failed, cl_ret=%d\n", cl_ret);
		goto exit;
	}

	/* Step 4: create an OpenCL context */
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &cl_ret);
	if (!context || cl_ret != CL_SUCCESS) {
		printf("clCreateContext failed, cl_ret=%d\n", cl_ret);
		goto exit;
	}

	/* Step 5: create a command-queue on the specific device */
	command_queue = clCreateCommandQueue(context, device_id, 0, &cl_ret);
	if (!command_queue || cl_ret != CL_SUCCESS) {
		printf("clCreateCommandQueue failed, cl_ret=%d\n", cl_ret);
		goto exit;
	}

    /* Step 6: allocate buffers on the device */
	mem_a = clCreateBuffer(context, CL_MEM_READ_ONLY, DATA_SIZE * sizeof(int), NULL, &cl_ret);
	if (!mem_a || cl_ret != CL_SUCCESS) {
		printf("Failed to allocate buffer for a, cl_ret=%d\n", cl_ret);
		goto exit;
	}
	mem_b = clCreateBuffer(context, CL_MEM_READ_ONLY, DATA_SIZE * sizeof(int), NULL, &cl_ret);
	if (!mem_b || cl_ret != CL_SUCCESS) {
		printf("Failed to allocate buffer for b, cl_ret=%d\n", cl_ret);
		goto exit;
	}
	mem_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY, DATA_SIZE * sizeof(int), NULL, &cl_ret);
	if (!mem_c || cl_ret != CL_SUCCESS) {
		printf("Failed to allocate buffer for c, cl_ret=%d\n", cl_ret);
		goto exit;
	}

	/* Step 7: copy the data to the device */
	cl_ret = clEnqueueWriteBuffer(command_queue, mem_a, CL_TRUE, 0, DATA_SIZE * sizeof(int), a, 0, NULL, NULL);
	if (cl_ret != CL_SUCCESS) {
		printf("clEnqueueWriteBuffer for a failed, cl_ret=%d\n", cl_ret);
		goto exit;
	}
	cl_ret = clEnqueueWriteBuffer(command_queue, mem_b, CL_TRUE, 0, DATA_SIZE * sizeof(int), b, 0, NULL, NULL);
	if (cl_ret != CL_SUCCESS) {
		printf("clEnqueueWriteBuffer for b failed, cl_ret=%d\n", cl_ret);
		goto exit;
	}

	/* Step 8: create a program object from the kernel source */
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &cl_ret);
	if (!program || cl_ret != CL_SUCCESS) {
		printf("clCreateProgramWithSource failed, cl_ret=%d\n", cl_ret);
		goto exit;
	}

	/* Step 9: build the program */
	cl_ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	if (cl_ret != CL_SUCCESS) {
		printf("clBuildProgram failed, cl_ret=%d\n", cl_ret);
		goto exit;
	}

	/* Step 10: create a kernel object */
	kernel = clCreateKernel(program, "vec3_add", &cl_ret);
	if (!kernel || cl_ret != CL_SUCCESS) {
		printf("clCreateKernel failed, cl_ret=%d\n", cl_ret);
		goto exit;
	}

	/* Step 11: set the arguments of the kernel */
	cl_ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&mem_a);
	if (cl_ret != CL_SUCCESS) {
		printf("clSetKernelArg for mem_a failed, cl_ret=%d\n", cl_ret);
		goto exit;
	}
	cl_ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&mem_b);
	if (cl_ret != CL_SUCCESS) {
		printf("clSetKernelArg for mem_b failed, cl_ret=%d\n", cl_ret);
		goto exit;
	}
	cl_ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&mem_c);
	if (cl_ret != CL_SUCCESS) {
		printf("clSetKernelArg for mem_c failed, cl_ret=%d\n", cl_ret);
		goto exit;
	}

	/* Step 12: enqueues a command to execute a kernel on the device */
	cl_ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
	if (cl_ret != CL_SUCCESS) {
		printf("clEnqueueNDRangeKernel failed, cl_ret=%d\n", cl_ret);
		goto exit;
	}

	/* Step 13: read back the result from the device */
	cl_ret = clEnqueueReadBuffer(command_queue, mem_c, CL_TRUE, 0, DATA_SIZE * sizeof(int), c, 0, NULL, NULL);
	if (cl_ret != CL_SUCCESS) {
		printf("clEnqueueReadBuffer failed, cl_ret=%d\n", cl_ret);
		goto exit;
	}

	/* Show the result */
	for(i = 0; i < DATA_SIZE; i++)
		printf("%d + %d -> expected result: %d, opencl result: %d\n", a[i], b[i], a[i] + b [i], c[i]);

exit:
	if (fp)
		fclose(fp);

	if (source_str)
		free(source_str);

	if (command_queue) {
		clFlush(command_queue);
		clFinish(command_queue);
		clReleaseCommandQueue(command_queue);
	}

	if (kernel)
		clReleaseKernel(kernel);

	if (program)
		clReleaseProgram(program);

	if (mem_a)
		clReleaseMemObject(mem_a);
	if (mem_b)
		clReleaseMemObject(mem_b);
	if (mem_c)
		clReleaseMemObject(mem_c);

	if (context)
		clReleaseContext(context);

	return 0;
}
