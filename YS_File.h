#ifndef __YS_FILE_H__
#define __YS_FILE_H__

#include "Data_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * �ļ������ӿ�
 */
typedef struct ys_file_handler_t ys_file_handler_t;

/*
 * �ļ���ģʽ
 */
typedef enum {
    fmode_r   = 1,   // ֻ��
    fmode_w   = 2,   // ֻд������ļ������ڣ��򴴽����ļ�����������ļ���
    fmode_a   = 3,   // ׷�ӡ�����ļ������ڣ��򴴽����ļ���
} file_mode_t;

/*
 * �ļ����еĻ�������
 */
typedef struct {
    u8  file_type[10];   // �ļ����ͣ����Ե����ļ���׺������ ".avi", ".jpg", ".mp3" ��
    u32 file_type_len;   // file_type�ĳ���
    u8  file_key[60];    // filekey�����ڱ�Ǹ��ļ�������Ψһ�ԣ����Կ����ļ���
    u32 file_key_len;    // filekey�ĳ��ȣ���಻����60�ֽ�
    u32 file_size;       // �ļ���С�����ֽڼ���
} file_info_t;

/************************ file �ӿ� *********************************/

/*
 * �ӿ�˵������mode��ʽ���ļ����ļ�����Ϊfile_type
 * ����˵��: file_type:�ļ����ͣ�'\0'��β�ַ��������ļ���׺����"amr"
             file_key:��'\0'��β���ַ��������filekeyΪ�գ������ڲ�Ӧ���Լ�����filekey
             mode������������Ե�����id������Ӧ�Ļص������л�Я��
 */
extern ys_file_handler_t* YS_file_open(u8 *file_type, u8 *file_key, file_mode_t mode);

/*
 * ��file�ж�ȡ���ݵ�buf[0:n)��
 * ����ֵΪ-1����ʾ��ȡ��������
 * ����ֵΪ0����ʾ�Ѿ���ȡ���
 * ����ֵΪ��������ʾ��ȡ�����ֽ���
 */
extern S32 YS_file_read(ys_file_handler_t *file, u8 *buf, u32 n);

/*
 * ��file��д��buf[0:n)
 * ����ֵΪ-1����ʾд�뷢������
 * ����ֵΪ0����ʾû��д������
 * ����ֵΪ��������ʾд����ֽ���
 */
extern S32 YS_file_write(ys_file_handler_t *file, u8 *buf, u32 n);

/*
 * ��ȡfile�Ļ�������
 * ����ֵΪ-1����ʾ��ȡfile��Ϣʧ��
 * ����ֵΪ0����ʾ��ȡfile��Ϣ�ɹ�
 */
extern S32 YS_file_get_info(ys_file_handler_t *file, file_info_t *info);

/*
 * �ƶ��ļ���дָ�뵽�ļ��ĵ�pos���ֽ�
 * posΪ0����ʾָ���ļ�ͷ����Ҳ���Ǹմ��ļ���״̬
 * posΪ0xffffffff����ʾָ���ļ�ĩβ������֪���ļ��Ĵ�С
 */
extern S32 YS_file_seek(ys_file_handler_t *file, u32 pos);

/*
 * �ر��ļ�
 */
extern S32 YS_file_close(ys_file_handler_t *file);


#endif
