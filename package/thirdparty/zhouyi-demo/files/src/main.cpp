#include <cstdio>
#include <string>
#include <vector>
#include <sstream>
#include <getopt.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

#include "standard_api_v2.0.0.h"

static void help_msg(void)
{
    printf("\n");
    printf("USAGE:\n");
    printf("\tzhouyi-demo -b BIN_FILE -i INPUT_FILE [OPTIONS]\n");
    printf("\n");
    printf("REQUIRED ARGUMENTS:\n");
    printf("\t-b,--bin BIN_FILE\t: the AIPU binary file to run\n");
    printf("\t-i,--idata FILE0,FILE1,...\t: the input data files (comma separated)\n");
    printf("OPTIONS:\n");
    printf("\t-c,--check CHECK_FILE\t: the ground truth check file\n");
    printf("\t-d,--dump-dir DUMP_DIR\t: the directory to dump output file\n");
    printf("\t-f,--frame-cnt CNT\t: how many frames to run (default is 1)\n");
    printf("\t-t,--time\t\t: measure execution time of each frame\n");
    printf("\n");
}

static void parse_idata_files_string(const char* idata_files_string,
        std::vector<std::string>* idata_files)
{
    std::string s(idata_files_string);

    while (1) {
        auto pos = s.find(',');
        if (pos == std::string::npos) {
            idata_files->push_back(s);
            break;
        } else {
            idata_files->push_back(s.substr(0, pos));
            s = s.substr(pos + 1);
        }
    }
}

static int load_data_from_file(const char* file_name, void* dst, int size)
{
    int ret = 0;
    FILE* fp = fopen(file_name, "rb");
    if (!fp) {
        printf("[DEMO ERROR] Failed to open \"%s\" to read\n", file_name);
        ret = -1;
        goto out;
    }
    ret = fread(dst, 1, size, fp);
    if (ret < size) {
        if (ferror(fp)) {
            printf("[DEMO ERROR] Read error: \"%s\"\n", file_name);
            clearerr(fp);
            ret = -1;
            goto close_file;
        }
        if (feof(fp)) {
            printf("[DEMO ERROR] The size of file \"%s\" is less than %d bytes\n",
                    file_name, size);
            clearerr(fp);
            ret = -1;
            goto close_file;
        }
    }
    ret = 0;
close_file:
    fclose(fp);
out:
    return ret;
}

static int dump_data_to_file(const char* file_name, void* src, int size)
{
    int ret = 0;
    FILE* fp = fopen(file_name, "wb");
    if (!fp) {
        printf("[DEMO ERROR] Failed to open \"%s\" to write\n", file_name);
        ret = -1;
        goto out;
    }
    ret = fwrite(src, 1, size, fp);
    if (ret < size) {
        printf("[DEMO ERROR] Write error \"%s\"\n", file_name);
        ret = -1;
        goto close_file;
    }
    ret = 0;
close_file:
    fclose(fp);
out:
    return ret;
}

static int check_output_data(const char* check_file, void* output_data, int size)
{
    int ret = 0;
    int fd = 0;
    void *check_data = nullptr;

    if (!check_file|| !output_data) {
        printf("[DEMO ERROR] Invalid arguments (check_file: %p, output_data: %p)\n",
                check_file, output_data);
        ret = -EINVAL;
        goto out;
    }

    fd = open(check_file, O_RDONLY);
    if (fd < 0) {
        printf("[DEMO ERROR] Failed to open file: %s (errno = %d)\n",
                check_file, errno);
        ret = -1;
        goto out;
    }

    check_data = mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (check_data == MAP_FAILED) {
        printf("[DEMO ERROR] Failed in mapping file: %s (errno = %d)\n",
                check_file, errno);
        ret = -1;
        goto close_fd;
    }

    for (int i = 0; i < size; ++i) {
        if (*(char*)((unsigned long)output_data + i)
                != *(char*)((unsigned long)check_data + i)) {
            /* check failed */
            ret = -1;
            goto mem_unmap;
        }
    }

    ret = 0;

mem_unmap:
    munmap(check_data, size);
close_fd:
    close(fd);
out:
    return ret;
}

int main(int argc, char *argv[])
{
    int ret = 0;

    const struct option opts[] = {
        { "help", no_argument, NULL, 'h' },
        { "bin", required_argument, NULL, 'b' },
        { "idata", required_argument, NULL, 'i' },
        { "check", required_argument, NULL, 'c' },
        { "dump-dir", required_argument, NULL, 'd' },
        { "frame-cnt", required_argument, NULL, 'f' },
        { "time", no_argument, NULL, 't' },
    };
    std::string bin_file;
    std::vector<std::string> idata_files;
    std::string check_file;
    std::string dump_dir;
    int frame_cnt = 1;
    bool measure_time = false;

    aipu_ctx_handle_t* ctx = nullptr;
    aipu_status_t status = AIPU_STATUS_SUCCESS;
    const char* status_msg = nullptr;
    aipu_graph_desc_t gdesc;
    aipu_buffer_alloc_info_t info;
    bool pass = true;

    int opt;
    while ((opt = getopt_long(argc, argv, "hb:i:c:d:f:t", opts, NULL)) != -1) {
        switch (opt) {
        case 'h':
            help_msg();
            ret = 0;
            goto out;
        case 'b':
            bin_file = optarg;
            printf("[DEMO INFO] bin file: %s\n", bin_file.c_str());
            break;
        case 'i':
            parse_idata_files_string(optarg, &idata_files);
            printf("[DEMO INFO] idata file:");
            for (std::vector<std::string>::size_type i = 0; i < idata_files.size(); ++i) {
                printf(" %s", idata_files[i].c_str());
            }
            printf("\n");
            break;
        case 'c':
            check_file = optarg;
            printf("[DEMO INFO] check file: %s\n", check_file.c_str());
            break;
        case 'd':
            dump_dir = optarg;
            printf("[DEMO INFO] dump dir: %s\n", dump_dir.c_str());
            break;
        case 'f':
            frame_cnt = atoi(optarg);
            break;
        case 't':
            measure_time = true;
            break;
        default:
            printf("[DEMO ERROR] Invalid option\n");
            help_msg();
            ret = -1;
            goto out;
        }
    }

    if (bin_file.empty() || idata_files.empty()) {
        printf("[DEMO ERROR] Required arguments not complete\n");
        help_msg();
        ret = -1;
        goto out;
    }

    status = AIPU_init_ctx(&ctx);
    if (status != AIPU_STATUS_SUCCESS) {
        AIPU_get_status_msg(status, &status_msg);
        printf("[DEMO ERROR] AIPU_init_ctx: %s\n", status_msg);
        ret = -1;
        goto out;
    }

    status = AIPU_load_graph_helper(ctx, bin_file.c_str(), &gdesc);
    if (status != AIPU_STATUS_SUCCESS) {
        AIPU_get_status_msg(status, &status_msg);
        printf("[DEMO ERROR] AIPU_load_graph_helper: %s\n", status_msg);
        ret = -1;
        goto deinit_ctx;
    }
    printf("[DEMO INFO] AIPU load graph successfully.\n");

    status = AIPU_alloc_tensor_buffers(ctx, &gdesc, &info);
    if (status != AIPU_STATUS_SUCCESS) {
        AIPU_get_status_msg(status, &status_msg);
        printf("[DEMO ERROR] AIPU_alloc_tensor_buffers: %s\n", status_msg);
        ret = -1;
        goto unload_graph;
    }

    for (int frame = 0; frame < frame_cnt; ++frame) {
        uint32_t job_id = 0;
        int32_t time_out = -1;
        bool finish_job_successfully = true;
        struct timeval start, end, interval;
        printf("[DEMO INFO] frame: %d\n", frame);

        for (std::vector<std::string>::size_type i = 0; i < idata_files.size(); ++i) {
            if (0 != load_data_from_file(idata_files[i].c_str(),
                        info.inputs.tensors[i].va, info.inputs.tensors[i].size)) {
                printf("[DEMO ERROR] Failed to load input from file \"%s\"\n",
                        idata_files[i].c_str());
                ret = -1;
                goto free_tensor_buffers;
            }
        }

        if (measure_time) {
            gettimeofday(&start, NULL);
        }

        status = AIPU_create_job(ctx, &gdesc, info.handle, &job_id);
        if (status != AIPU_STATUS_SUCCESS) {
            AIPU_get_status_msg(status, &status_msg);
            printf("[DEMO ERROR] AIPU_create_job: %s\n", status_msg);
            ret = -1;
            goto free_tensor_buffers;
        }

        status = AIPU_finish_job(ctx, job_id, time_out);
        if (status != AIPU_STATUS_SUCCESS) {
            AIPU_get_status_msg(status, &status_msg);
            printf("[DEMO ERROR] AIPU_finish_job: %s\n", status_msg);
            finish_job_successfully = false;
        } else {
            finish_job_successfully = true;
        }

        if (measure_time) {
            gettimeofday(&end, NULL);
            if (end.tv_usec >= start.tv_usec) {
                interval.tv_usec = end.tv_usec - start.tv_usec;
                interval.tv_sec = end.tv_sec - start.tv_sec;
            } else  {
                interval.tv_usec = 1000000 + end.tv_usec - start.tv_usec;
                interval.tv_sec = end.tv_sec - 1 - start.tv_sec;
            }
            printf("[DEMO INFO] cost: %lus, %luus\n", interval.tv_sec, interval.tv_usec);
        }

        printf("[DEMO INFO] AIPU finish job successfully\n");

        /* check output data... */
        /* this demo only support single output graph */
        /* for multi-output graphs, just check as this one by one */
        if (!check_file.empty() && finish_job_successfully) {
            ret = check_output_data(check_file.c_str(),
                    info.outputs.tensors[0].va, info.outputs.tensors[0].size);
            if (ret == 0) {
                printf("[DEMO INFO] Result check PASS!\n");
            } else {
                printf("[DEMO ERROR] Result check FAILED!\n");
                pass = false;
            }
        }

        /* dump output data... */
        /* this demo only support single output graph */
        /* for multi-output graphs, just dump as this one by one */
        if (!dump_dir.empty() && finish_job_successfully) {
            std::string dump_file;
            std::ostringstream ostr;
            ostr << "/Graph" << gdesc.id << "_Frame" << frame << "_Job" << job_id << "_Output.bin";
            dump_file = dump_dir + ostr.str();
            ret = dump_data_to_file(dump_file.c_str(),
                    info.outputs.tensors[0].va, info.outputs.tensors[0].size);
            if (ret == 0) {
                printf("[DEMO INFO] Dump output data to: %s\n", dump_file.c_str());
            } else {
                printf("[DEMO ERROR] Failed to dump output data to: %s\n", dump_file.c_str());
            }
        }

        status = AIPU_clean_job(ctx, job_id);
        if (status != AIPU_STATUS_SUCCESS) {
            AIPU_get_status_msg(status, &status_msg);
            printf("[TEST ERROR] AIPU_clean_job: %s\n", status_msg);
            ret = -1;
            goto free_tensor_buffers;
        }
    }

    ret = pass ? 0 : -1;

free_tensor_buffers:
    status = AIPU_free_tensor_buffers(ctx, info.handle);
    if (status != AIPU_STATUS_SUCCESS) {
        AIPU_get_status_msg(status, &status_msg);
        printf("[DEMO ERROR] AIPU_free_tensor_buffers: %s\n", status_msg);
        ret = -1;
    }
unload_graph:
    status = AIPU_unload_graph(ctx, &gdesc);
    if (status != AIPU_STATUS_SUCCESS) {
        AIPU_get_status_msg(status, &status_msg);
        printf("[DEMO ERROR] AIPU_unload_graph: %s\n", status_msg);
        ret = -1;
    }
deinit_ctx:
    status = AIPU_deinit_ctx(ctx);
    if (status != AIPU_STATUS_SUCCESS) {
        AIPU_get_status_msg(status, &status_msg);
        printf("[DEMO ERROR] AIPU_deinit_ctx: %s\n", status_msg);
        ret = -1;
    }
out:
    return ret;
}
