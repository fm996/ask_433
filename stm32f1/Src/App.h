#ifndef __APP_H
#define __APP_H

#include "main.h"

#define LED_FLASH_DELAY             300             //LED指示的点亮状态延续时间，300ms
#define LED_HOLD_FLASH_DELAY        100             //LED指示的点亮状态延续时间，100ms

#define SAMPLE_FREQUENCY_DEF        10              //手势采样频率，10ms
#define SAMPLE_DOWN_LIMIT_DEF       0x000A          //采样阈值下限
#define SAMPLE_UP_LIMIT_DEF         0x0020          //采样触发阈值
#define SAMPLE_HOLD_LIMIT_DEF       0x0020//0x0040  //hold的触发阈值
#define SAMPLE_HOLD_END_SHIFT       GES_SAMPLE_ARRAY_NUM_S  //从采样数组末尾起10个作为检测基准
#define SAMPLE_HOLD_COUNT           2               //最多3个HOLD的count再次发送一次HOLD
#define SAMPLE_HOLD_LIST_NUM        10              //hold数组的最大元素个数
//#define SAMPLE_HOLD_START

#define GES_CHN_NUM                 2               //手势采样的通道数，3
#define GES_CHN_UP                  2               //上通道
#define GES_CHN_DOWN_LEFT           1               //下左通道
#define GES_CHN_DOWN_RIGHT          0               //下右通道
#define GES_CHN_INVALID             0xFF            //无效通道

#define GES_SAMPLE_AVE_NUM          1               //用以平均的数据大小：1
#define GES_SAMPLE_ARRAY_NUM_S      8               //手势采样的短数组大小：8
#define GES_SAMPLE_ARRAY_NUM_L      32              //手势采样的长数组大小：24
#define GES_SAMPLE_ARRAY_FULL       (GES_SAMPLE_ARRAY_NUM_L)//手势采样的长数组大小：24

#define GES_SAMPLE_RAW_ARRAY_NUM_S  (GES_SAMPLE_ARRAY_NUM_S*GES_SAMPLE_AVE_NUM)
#define GES_SAMPLE_RAW_ARRAY_NUM_L  (GES_SAMPLE_ARRAY_NUM_L*GES_SAMPLE_AVE_NUM)
                                                    //实际一次分析所采样的点数为：GES_SAMPLE_RAW_ARRAY_NUM=GES_SAMPLE_ARRAY_NUM*GES_SAMPLE_AVE_NUM    
                                                    //手势采样的原始数组
                                                    
#define GES_TIME_DIF_MIN            10              //手势时间差的最小值，10ms
#define GES_TIME_DIF_MAX            1000            //手势时间差的最大值，1000ms
#define GES_DISTANCE_MIN            700             //相邻手势的最小值，700ms
#define MULTIGES_DISTANCE_MIN       2000            //组合相邻手势的最小值，2.5s

#define GES_TREND_CLEARUP_MIN       300            //相邻手势的最小值，1000ms

#define POSITIVE                    1               //手势数据监测值大于基准值
#define NEGATIVE                    2               //手势数据监测值小于基准值

//手势类型
#define GES_NULL                    0x00            //无
#define GES_LEVEL1_APPROACH         0x01            //靠近
#define GES_LEVEL1_LEAVE            0x02            //离开
#define GES_LEVEL1_FULL             0x03            //具有完整的靠近->离开周期
#define GES_LEVEL1_HOLD             0x04            //hold
#define GES_LEVEL2_UP               0x11            //上
#define GES_LEVEL2_DOWN             0x12            //下
#define GES_LEVEL2_LEFT             0x13            //左
#define GES_LEVEL2_RIGHT            0x14            //右
#define GES_LEVEL2_HOLD             0x15            //HOLD模式
#define GES_LEVEL2_DCLK             0x16            //双击模式
#define GES_LEVEL2_APPROACH         0x17            //接近
#define GES_LEVEL2_LEAVE            0x18            //远离


#define GES_INVALID_INDEX           0xFFFF          //无效坐标索引值

#define CALIB_OBO                   0
#define SECTION_NUM                 3

typedef struct
{
    uint16_t rise_index;            //上升沿坐标索引，刚大于触发上限的点
    uint16_t rise_value;            //上升点值
    uint16_t peak_index;            //顶点坐标索引
    uint16_t peak_value;            //顶点值
    uint16_t fall_index;            //下降沿坐标索引，刚好到触发下限的前一点
    uint16_t fall_value;            //下降点值
}wave_struct;

typedef struct
{
    uint8_t ges;                            //手势类型
    uint64_t add_time;              //加入时间
}ges_struct;

typedef struct
{
    //uint8_t sample_start_flag;                                              //采样开始标志
    //uint8_t sample_ready_flag;                                              //采样完成标志
    //uint32_t sample_counter;                                                //采样开始计数器
    //uint32_t sample_frequency;                                              //采样频率
    uint16_t sample_size;                                                   //采样数组元素个数
    uint16_t sample_size_raw;                                               //采样raw数组元素个数
    
    uint16_t sample_base[GES_CHN_NUM];                                      //每路的基准
    uint16_t sample_base_last[GES_CHN_NUM];                                 //上一周期每路的基准
    uint16_t sample_down_limit[GES_CHN_NUM];                                //每路的触发下限，低于此值则不算接近
    uint16_t sample_up_limit[GES_CHN_NUM];                                  //每路的触发上限，高于此值则记录触发
    
    uint16_t ges_sample_raw_array[GES_CHN_NUM][GES_SAMPLE_RAW_ARRAY_NUM_L]; //手势采样原始数组
    uint16_t ges_sample_array[GES_CHN_NUM][GES_SAMPLE_ARRAY_NUM_L];         //手势采样数组
    uint16_t ges_sample_raw_cur;                                            //手势raw数组当前元素

    //uint16_t ges_dynamic_sample_raw_array[GES_CHN_NUM][GES_SAMPLE_RAW_ARRAY_NUM_L]; //手势采样原始数组
    uint16_t ges_dynamic_sample_array[GES_CHN_NUM][GES_SAMPLE_ARRAY_NUM_L];         //手势采样数组
    uint16_t ges_dynamic_sample_raw_cur;                                            //手势raw数组当前元素
    uint8_t  ges_dynamic_sample_size;
    uint8_t  ges_dynamic_start;
    
    uint16_t ges_sample_Calib_array[GES_CHN_NUM][GES_SAMPLE_ARRAY_NUM_L];           //手势采样数组
    
    uint64_t frame_start_tick;                                              //当前帧采样的起始时间
    ges_struct last_ges_record[GES_CHN_NUM];                                //手势记录，每一路的上一次手势类型
    ges_struct ges_record[GES_CHN_NUM];                                     //手势记录，每一路的这一次手势类型  
//  uint8_t ges_num[GES_CHN_NUM];                                           //手势数组的有效元素
    
    wave_struct wave[GES_CHN_NUM];
    
    uint8_t last_ges;
    uint64_t last_ges_time;

    uint8_t sample_mode;
    uint16_t hold_height_start;
    uint16_t hold_height_stop;
    uint8_t  new_hold_AorL_size;
    uint8_t approach_counter;
    uint8_t leave_counter;
    uint8_t hold_counter;
    uint8_t hold_flag;
    //uint8_t calib_force;
    uint8_t analy_fail_counter;
    uint16_t hold_list[SAMPLE_HOLD_LIST_NUM];
//  uint16_t led_flash_counter;
}gs_struct;

#if(COMB_GES==1)
typedef struct
{
    uint8_t ges[3];
    uint8_t gesid;
    uint64_t start_ges_time;

    //uint64_t last_ges_time;
}gs_comb_struct;
#endif

void App_Var_Init(void);
void App_Init(void);
void LED_Tick_Check(void);
uint8_t Ges_Add_Sample(uint16_t *ps);
//void Ges_Tick_Check(void);
//void Ges_It_Handle(void);
//uint8_t Ges_Peak_Find(uint16_t *ps,uint8_t chn,uint16_t *pindex);
//uint8_t Ges_Edge_Find(uint16_t *ps,uint8_t chn,uint16_t *pindex);
uint8_t Ges_Wave_Search(uint8_t chn,uint16_t *pindex);
uint8_t Ges_Wave_Lead(uint8_t chn1,uint8_t chn2);
uint8_t Ges_Analysis(void);
void Ges_Calib(uint8_t firstboot);
void Ges_DynamicCalib();
void Ges_Normalize(void);
void Ges_DynamicNormalize(void);
void Ges_Log(void);
//uint8_t Ges_SampleSize_Switch(void);
void App_Task(void);
void Systick_Inc(void);
uint64_t Systick_Get(void);

#endif  //__APP_H
