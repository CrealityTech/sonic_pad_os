#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <signal.h>
#include <signal.h>
#include <time.h>
#include <linux/fb.h>
#include <linux/kernel.h>
#include <sys/ioctl.h>
#include <math.h>
#include "g2d_driver_enh.h"
#include "ion_head.h"

#define G2D_PATH    "/dev/g2d"

static void config_src_image(g2d_blt_h *blit, uint32_t width, uint32_t height, g2d_fmt_enh format){
    
    blit->src_image_h.width = width;
    blit->src_image_h.height = height;
    blit->src_image_h.format = format;
    
    blit->src_image_h.clip_rect.x = 0;
    blit->src_image_h.clip_rect.y = 0;
    blit->src_image_h.clip_rect.w = width;
    blit->src_image_h.clip_rect.h = height;
    
    blit->src_image_h.align[0] = 0;
    blit->src_image_h.align[1] = 0;
    blit->src_image_h.align[2] = 0;
}

static void config_dst_image(g2d_blt_h *blit, uint32_t width, uint32_t height, g2d_fmt_enh format){
    
    blit->dst_image_h.width = width;
    blit->dst_image_h.height = height;
    blit->dst_image_h.format = format;

    blit->dst_image_h.clip_rect.x = 0;
    blit->dst_image_h.clip_rect.y = 0;
    blit->dst_image_h.clip_rect.w = width;
    blit->dst_image_h.clip_rect.h = height;

    blit->dst_image_h.align[0] = 0;
    blit->dst_image_h.align[1] = 0;
    blit->dst_image_h.align[2] = 0;
}

struct g2d_info_t
{
    g2d_blt_h info;
    int mem_id;
    int mem_id2;
    int g2d_fd;

    FILE *fp;
    FILE *fp2;
    struct ion_allocation_data alloc_data;
    struct ion_handle_data handle_data;
    struct ion_fd_data fd_data;
    void *src_addr;
    void *dst_addr;
    int ion_fd;
    int src_fd;
    int dst_fd;
};

static struct g2d_info_t g2d_info;

static int ion_memory_request(int *fd, void **vir_addr ,uint64_t len)
{
    struct ion_allocation_data alloc_data;
    struct ion_handle_data handle_data;
    struct ion_fd_data fd_data;
    int ret = -1, k, ion_fd;
    unsigned int fb_width, fb_height, u_width, u_height;
    void *user_addr;
    unsigned int y_pitch, u_pitch;
    unsigned int y_size, u_size;
    unsigned int mem_size;


    if((ion_fd = open(ION_DEV_NAME, O_RDWR)) < 0) {
        printf("%s(%d) err: open %s dev failed\n", __func__, __LINE__, ION_DEV_NAME);
        return -1;
    }
    printf("%s(%d): ion_fd %d\n", __func__, __LINE__, ion_fd);

    fb_width = g2d_info.info.src_image_h.width;
    fb_height = g2d_info.info.src_image_h.height;
    printf("%s image w = %d,h= %d\n", __func__, fb_width, fb_height);

    mem_size = fb_width * fb_height * 4;//y_size + u_size;////;3/2;

    /* alloc buffer */
    alloc_data.len = mem_size;
    alloc_data.align = ION_ALLOC_ALIGN;
    /*alloc_data.heap_id_mask = ION_HEAP_SYSTEM_MASK;*/
    alloc_data.heap_id_mask = ION_HEAP_TYPE_DMA_MASK; /*without iommu, use ion cma buffer*/
    //alloc_data.flags = ION_FLAG_CACHED | ION_FLAG_CACHED_NEEDS_SYNC;

    printf("%s(%d): ION_HEAP_TYPE %x, alloc_data.heap_id_mask 0x%x\n",
        __func__, __LINE__, ION_HEAP_TYPE_DMA, alloc_data.heap_id_mask);
    
    ret = ioctl(ion_fd, ION_IOC_ALLOC, &alloc_data);
    if(ret) {
        printf("%s(%d): ION_IOC_ALLOC err, ret %d, handle %p\n", __func__, __LINE__, ret, alloc_data.handle);
        goto out1;
    }
    printf("%s(%d): ION_IOC_ALLOC succes, handle %p\n", __func__, __LINE__, alloc_data.handle);

    /* map dma buffer fd */
    fd_data.handle = alloc_data.handle;
    ret = ioctl(ion_fd, ION_IOC_MAP, &fd_data);
    if(ret) {
        printf("%s(%d): ION_IOC_MAP err, ret %d, dmabuf fd 0x%08x\n", __func__, __LINE__, ret, (unsigned int)fd_data.fd);
        goto out2;
    }
    printf("%s(%d): ION_IOC_MAP succes, get dmabuf fd 0x%08x\n", __func__, __LINE__, (unsigned int)fd_data.fd);

    *fd = fd_data.fd;
    *vir_addr = mmap(NULL, alloc_data.len, PROT_READ|PROT_WRITE, MAP_SHARED, fd_data.fd, 0);

    printf("===map_addr1 %p, fd %d===\n", *vir_addr, *fd);
    if (*vir_addr == MAP_FAILED) {
        printf("%s(%d): mmap err, ret %p\n", __func__, __LINE__, vir_addr);
    }     
    g2d_info.alloc_data = alloc_data;
    g2d_info.handle_data = handle_data;
    g2d_info.fd_data = fd_data;
    g2d_info.ion_fd = ion_fd;
out1:
out2:
    return ret;
}

static int ion_memory_release(int fd, void *virt_addr)
{
    struct ion_allocation_data alloc_data;
    struct ion_handle_data handle_data;
    struct ion_fd_data fd_data;
    int ret = -1, ion_fd;

    alloc_data = g2d_info.alloc_data;
    handle_data = g2d_info.handle_data;
    fd_data = g2d_info.fd_data;
    ion_fd = g2d_info.ion_fd;
    if (NULL != virt_addr)
        munmap(virt_addr, alloc_data.len);
    close(fd);
    printf("%s(%d): close dmabuf fd succes\n", __func__, __LINE__);

    /* free buffer */
    handle_data.handle = alloc_data.handle;
    ioctl(ion_fd, ION_IOC_FREE, &handle_data);
    /* close ion device fd */
    close(ion_fd);
    return ret;
}

int init_config_g2d(uint32_t width, uint32_t height, g2d_fmt_enh fmt)
{
    int ret;
    int out_size;

    memset(&g2d_info, 0, sizeof(struct g2d_info_t));

    config_src_image(&(g2d_info.info), width, height, fmt);
    config_dst_image(&(g2d_info.info), width, height, fmt);
    g2d_info.info.flag_h = G2D_ROT_180;

    if((g2d_info.g2d_fd = open(G2D_PATH, O_RDWR)) == -1) {
        printf("open g2d device fail!\n");
        return -1;
    }

    out_size = g2d_info.info.dst_image_h.width * g2d_info.info.dst_image_h.height * 4;
    printf("src_image_size = %d, dst_image_size =%d\n", 
        g2d_info.info.src_image_h.width * g2d_info.info.src_image_h.height * 4,
        g2d_info.info.dst_image_h.width * g2d_info.info.dst_image_h.height * 4);
    
    ret = ion_memory_request(&g2d_info.src_fd, &g2d_info.src_addr, out_size);
    if(ret == 0)
        printf("ion_memory_request for src image ok.\n");
    else {
        printf("ion_memory_request for src image failed.\n");
        ion_memory_release(g2d_info.src_fd, g2d_info.src_addr);
        return -1;
    }
    
    ret = ion_memory_request(&g2d_info.dst_fd, &g2d_info.dst_addr, out_size);
    if(ret == 0)
        printf("ion_memory_request for dst image ok.\n");
    else {
        printf("ion_memory_request for dst image failed.\n");
        ion_memory_release(g2d_info.dst_fd, g2d_info.dst_addr);
        return -1;
    }

    g2d_info.info.src_image_h.fd = g2d_info.src_fd;
    g2d_info.info.dst_image_h.fd = g2d_info.dst_fd;
    
    printf("src:format=0x%x, clip[%d %d,%d x %d], img_w=%d, img_h=%d, align=%d\n\n",
        g2d_info.info.src_image_h.format,
        g2d_info.info.src_image_h.clip_rect.x,
        g2d_info.info.src_image_h.clip_rect.y,
        g2d_info.info.src_image_h.clip_rect.w,
        g2d_info.info.src_image_h.clip_rect.h,
        g2d_info.info.src_image_h.width,
        g2d_info.info.src_image_h.height,
        g2d_info.info.src_image_h.align[0]);

    printf("dst:format=0x%x, img w=%d, h=%d, clip[%d %d,%d x %d], align=%d\n\n",
        g2d_info.info.dst_image_h.format,
        g2d_info.info.dst_image_h.width,
        g2d_info.info.dst_image_h.height,
        g2d_info.info.dst_image_h.clip_rect.x,
        g2d_info.info.dst_image_h.clip_rect.y,
        g2d_info.info.dst_image_h.clip_rect.w,
        g2d_info.info.dst_image_h.clip_rect.h,
        g2d_info.info.dst_image_h.align[0]);

    return 0;
}

int uninit_config_g2d(){
    ion_memory_release(g2d_info.src_fd, g2d_info.src_addr);
    ion_memory_release(g2d_info.dst_fd, g2d_info.dst_addr);
    if (g2d_info.g2d_fd >= 0)
        close(g2d_info.g2d_fd);
}

int g2d_rotate(uint8_t *in_buffer, uint64_t len, uint8_t *out_buffer)
{    
    memcpy(g2d_info.src_addr, in_buffer, len);

    /* rotation */
    if(ioctl(g2d_info.g2d_fd , G2D_CMD_BITBLT_H ,(unsigned long)(&g2d_info.info)) < 0)
    {
        printf("[%d][%s][%s]G2D_CMD_BITBLT_H failure!\n",__LINE__, __FILE__,__FUNCTION__);
        close(g2d_info.g2d_fd);
        return -1;
    }
    //printf("[%d][%s][%s]G2D_CMD_BITBLT_H Successful!\n",__LINE__, __FILE__,__FUNCTION__);

    memcpy(out_buffer, g2d_info.dst_addr, len);

    return 0;
}
