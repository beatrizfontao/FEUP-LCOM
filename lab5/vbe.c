#include "vbe.h"

#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "VBE_macros.h"

void* vg_init1(uint16_t mode){

  struct vbe_mode_info_t info;

  if(vbe_get_mode_info(mode, &info))
    return NULL;
/*
  mr.mr_base = BASEADDRESS;
  mr.mr_limit = MiBIT;
  */
  hres = info.XResolution;
  vres = info.YResolution;
  bitsPerPixel = info.BitsPerPixel;
  unsigned int vram_baseinfo.PhysBasePtr;
  unsigned int vram_size = info.XResolution*info.YResolution*info.BitsPerPixel/8;

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  static void *video_mem;

  struct minix_mem_range mr;
  int r;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  reg86_t reg86;

  reg86.intno = INT;
  reg86.ah = FUNCTION_CALL;
  reg86.al = FUNCTION_SETMODE;
  r.bx = 1<<14|mode;

  if(sys_int86(&reg86))
    return NULL;

  return video_mem;
}