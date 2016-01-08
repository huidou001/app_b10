#ifndef __YS_FILE_H__
#define __YS_FILE_H__

#include "Data_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 文件操作接口
 */
typedef struct ys_file_handler_t ys_file_handler_t;

/*
 * 文件打开模式
 */
typedef enum {
    fmode_r   = 1,   // 只读
    fmode_w   = 2,   // 只写。如果文件不存在，则创建新文件，否则清空文件。
    fmode_a   = 3,   // 追加。如果文件不存在，则创建新文件。
} file_mode_t;

/*
 * 文件具有的基本属性
 */
typedef struct {
    u8  file_type[10];   // 文件类型，可以当作文件后缀，比如 ".avi", ".jpg", ".mp3" 等
    u32 file_type_len;   // file_type的长度
    u8  file_key[60];    // filekey，用于标记该文件，具有唯一性，可以看作文件名
    u32 file_key_len;    // filekey的长度，最多不超过60字节
    u32 file_size;       // 文件大小，以字节计算
} file_info_t;

/************************ file 接口 *********************************/

/*
 * 接口说明：以mode方式打开文件，文件类型为file_type
 * 参数说明: file_type:文件类型，'\0'结尾字符串，即文件后缀，如"amr"
             file_key:以'\0'结尾的字符串，如果filekey为空，函数内部应该自己生成filekey
             mode：用于任务配对的任务id，在相应的回调函数中会携带
 */
extern ys_file_handler_t* YS_file_open(u8 *file_type, u8 *file_key, file_mode_t mode);

/*
 * 从file中读取数据到buf[0:n)中
 * 返回值为-1：表示读取发生错误
 * 返回值为0：表示已经读取完毕
 * 返回值为正数：表示读取到的字节数
 */
extern S32 YS_file_read(ys_file_handler_t *file, u8 *buf, u32 n);

/*
 * 向file中写入buf[0:n)
 * 返回值为-1：表示写入发生错误
 * 返回值为0：表示没有写入数据
 * 返回值为正数：表示写入的字节数
 */
extern S32 YS_file_write(ys_file_handler_t *file, u8 *buf, u32 n);

/*
 * 获取file的基本属性
 * 返回值为-1：表示获取file信息失败
 * 返回值为0：表示获取file信息成功
 */
extern S32 YS_file_get_info(ys_file_handler_t *file, file_info_t *info);

/*
 * 移动文件读写指针到文件的第pos个字节
 * pos为0：表示指向文件头部，也就是刚打开文件的状态
 * pos为0xffffffff：表示指向文件末尾，无需知道文件的大小
 */
extern S32 YS_file_seek(ys_file_handler_t *file, u32 pos);

/*
 * 关闭文件
 */
extern S32 YS_file_close(ys_file_handler_t *file);


#endif
