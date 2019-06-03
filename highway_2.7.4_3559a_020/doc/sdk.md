# 测试环境配置

* 硬件环境：3559a

# SDK调用说明

## 获取版本信息

* 原型

```c
const char* __stdcall seemmo_version(void);
```

* 返回值

  sdk版本信息

* 说明

  获取sdk的版本信息

## 进程级初始化

* 原型

```c
int32_t __stdcall seemmo_process_init(
		const char* base_dir,
		uint32_t img_core_num,
		uint32_t video_core_num,
		const char* auth_server,
		int32_t auth_type,
		bool log_init_swith);
```

* 参数

  * **base_dir** 算法初始化目录，该目录下应包含model、data、conf等文件夹

  * **img_core_num** SDK图片核数，识别线程数决定（不能超过授权最大值）

  * **video_core_num** SDK视频核数，视频检测跟踪线程数决定（不能超过授权最大值）

    （例：24路视频需要至少24个视频核和24个图片核）

  * **auth_server** 授权服务器，例如："192.168.1.198:12821"

  * **auth_type** 授权类型：1. 本地； 2. 网络

  * **log_init_swith glog** 初始化开关: 

    true: SDK外部没有初始化，依赖该函数进行初始化，log默认路径为base_dir下log文件夹

    fasle: SDK外部已经初始化

* 返回值

  见附录返回值说明

* 说明

  根据参数初始化glog，初始化SDK

## 线程级初始化

* 原型

```c
int32_t __stdcall seemmo_thread_init( 
    	int type,
    	int device,
    	int batch);
```

* 参数

  * **type** 线程加载的模型类型：

    SEEMMO_LOAD_TYPE_FILTER：用于视频的检测跟踪 SEEMMO_LOAD_TYPE_RECOG：用于识别视频的跟踪结果 SEEMMO_LOAD_TYPE_ALL：用于图片的检测与识别

  * **device** 设备序号，-1表示CPU, 0表示第一个GPU，1表示第二个GPU，依此类推

  * **batch** 检测和识别模型每批可处理的图像数

    <font color=red> 注意 </font>：batch越大，每批可处理的帧数越多，但每批处理的时间会更长（<font color=red> 建议batch为4的倍数</font>）

    当type为SEEMMO_LOAD_TYPE_FILTER时，消耗<font color=red>视频处理核数量</font>为batch * video_core_num

    当type为SEEMMO_LOAD_TYPE_RECOG时，消耗<font color=red>图片处理核数量</font>为batch * image_core_num（<font color=red>用于pvc结果识别</font>）

    当type为SEEMMO_LOAD_TYPE_ALL时，消耗<font color=red>图片以及视频处理核数量</font>各为batch * image_core_num

    当type为SEEMMO_LOAD_TYPE_FACE时，消耗<font color=red>图片处理核数量</font>为batch * image_core_num（<font color=red>用于face结果识别</font>）

* 返回值

  见附录返回值说明

* 说明

  加载算法模型库，与指定的设备（CPU/GPU）绑定

## 线程级清理

* 原型

```c
const int32_t __stdcall seemmo_thread_uninit();
```

​	注：线程在退出时必须并发同时退出，不能串行退出，否则会导致死锁

* 返回值

  0表示成功，否则异常

* 说明

  清理当前线程

## 相似度计算初始化

* 原型

```c
long __stdcall seemmo_match_init(bool beta_swith);
```

* 参数

  * **beta_swith** 默认设置为true，开启beta模式

* 返回值

  见附录返回值说明

* 说明

  初始化相似度计算模块,用于调seemmo_pvc_match

## 卸载

* 原型

```c
int32_t __stdcall seemmo_uninit(void);
```

* 返回值

  见附录返回值说明

* 说明

  卸载清理SDK

## 视频检测跟踪

* 原型

```c
int32_t __stdcall seemmo_video_pvc( 
    	int32_t frame_num,
    	const int32_t* video_chn,
    	const uint64_t* timestamp,
    	const uint8_t** ppbgr24,
		const uint32_t* height,
    	const uint32_t* width,
    	const char** calc_params,
    	char* rsp_buf,
    	int32_t* buff_len,
    	int32_t timeout);
```

* 参数

  * **frame_num** 视频帧的数目
  * **video_chn** 每个视频帧的通路编号(例如:1,2,3,4)数组，数组大小为frame_num
  * **timestamp** 每个视频帧的时间戳数组，数组大小为frame_num
  * **ppbgr24** 解码后多张图片输入，为一指针数组，每个指针指向一帧的图像数据，数组的大小为frame_num
  * **height** 每帧图片的高度，数组大小为frame_num
  * **width** 每帧图片的宽度，数组大小为frame_num
  * **calc_params** 每路视频的请求参数，以json格式表示，指针数组大小为frame_num，每个指针指向一个完整json串，代表一路视频的参数。json格式如下：

  ```json
      "Detect": {
          "DetectRegion": [
              [ 745, 119 ],
              [ 1189, 41],
              [ 1201, 349 ],
              [ 1207, 361 ],
              [ 847, 1045 ],
              [ 463, 1047 ]],
          "IgnoreRegion": [
              [ 745, 119 ],
              [ 1189, 41],
              [ 1201, 349 ],
              [ 1207, 361 ],
              [ 847, 1045 ],
              [ 463, 1047 ]],
          "IsDet": true 
      } 
  }
  ```

  *<font color=red>json字段值说明：</font>*

  ------

  ​	**DetectRegion**：*视频检测的多个多边形区域，采用[[[x1,y1],[x2,y2],...[xn,yn]]]表示，其中[x,y]表示其中一个多边形的一个顶点。*

  ​	**IgnoreRegion**：*视频否定的多个多边形区域，采用[[[x1,y1],[x2,y2],...[xn,yn]]]表示，其中[x,y]表示其中一	个多边形的一个顶点。*

  ​	 **IsDet**：*是否对该路视频进行检测跟踪*

  ------

  * **rsp_buf** 返回结果时使用的缓冲区，缓冲区存储的是json字符串，具体含义见json说明

  ```json
  {
      "Code":0,
      "Message":"ok",
      "ImageResults":[
          {
              "Code":0,
              "Message":"ok",
              "Vehicles":[
                  {
                      "Type":5,
                   	"GUID":"7b16c8c1-b33e-44ea-bb86-2d0f3786a80a",
                   	"Trail":[-3,4],
                   	"Detect":{
                       	"Code":0,
                       	"Message":"ok",
                       	"Body":{
                           	"Rect":[631,364,118,184],
                           	"Score":27,
                           	"Width":0,
                           	"Height":0}
                   	},
                   	"Recognize":{}
                  },
                  {
                      "Type":4,
                   	"GUID":"db247ae3-da1d-4306-824b-58357801bb76",
                   	"Trail":[0,0],
                   	"Detect":{
                       	"Code":0,
                       	"Message":"ok",
                       	"Body":{
                           	"Rect":[815,92,86,64],
                           	"Score":56,
                           	"Width":0,
                           	"Height":0}
                   	},
                   	"Recognize":{}
                  },
                  {
                      "Type":6,
                   	"GUID":"aa9bb8d2-e467-4947-aca6-ccca7e305fb8",
                   	"Trail":[0,0],
                   	"Detect":{
                       	"Code":0,
                          "Message":"ok",
                          "Body":{
                              "Rect":[937,67,90,96],
                              "Score":30,
                              "Width":0,
                              "Height":0}
                      },
                      "Recognize":{}
                  }
              ],
              "Pedestrains":[
                  {
                      "Type":1,
                      "GUID":"06dad580-c04b-430c-897c-427845701bdd",
                      "Trail":[-2,0],
                      "Face":{},
                      "Detect":{
                          "Code":0,
                          "Message":"ok",
                          "Body":{
                              "Rect":[563,65,31,81],
                              "Score":25,
                              "Width":0,
                              "Height":0}
                      },
                      "Recognize":{}
                  },
                  {
                      "Type":1,
                      "GUID":"421cafae-a506-4459-a750-626097b9110b",
                      "Trail":[-1,0],
                      "Face":{},
                      "Detect":{
                          "Code":0,
                          "Message":"ok",
                          "Body":{
                              "Rect":[331,178,52,134],
                              "Score":61,
                              "Width":0,
                              "Height":0}
                      },
                      "Recognize":{}
                  }
              ],
              "Bikes":[
                  {
                      "Type":3,
                      "GUID":"c9c6b52a-e722-47fe-a966-2e4169e706df",
                      "Trail":[0,0],
                      "Detect":{
                          "Code":0,
                          "Message":"ok",
                          "Body":{
                              "Rect":[703,84,35,75],
                              "Score":20,
                              "Width":0,
                              "Height":0}
                      },
                      "Recognize":{},
                      "Persons":[]
                  },
                  {
                      "Type":3,
                      "GUID":"3d521c02-39bc-4f95-9348-1183d7cc2fc3",
                      "Trail":[0,4],
                      "Detect":{
                          "Code":0,
                          "Message":"ok",
                          "Body":{
                              "Rect":[675,159,51,104],
                              "Score":57,
                              "Width":0,
                              "Height":0}
                      },
                      "Recognize":{},
                      "Persons":[]
                  }
              ],
              "VideoChannel":0,
              "IsCache":true
          }
      ],
      "FilterResults":[
          {
              "Code":0,
              "Message":"ok",
              "Vehicles":[],
              "Pedestrains":[],
              "Bikes":[],
              "VideoChannel":0,
              "ReleaseCacheFrames":[1450,1456,1462]
          }
      ]
  }
  ```

  *<font color=red>json字段说明：</font>*

  ------

  ​	**FilterResults**：*保存目标跟踪和择优后的结果*

  ​	**ImageResults**：*保存当前输入帧的检测结果* 

  ​	**Bikes**：*自行车的检测/择优结果* 

  ​	**Pedestrains**：*行人的检测/择优结果* 

  ​	**Vehicles**：*机动车的检测/择优结果* 

  ​	**ContextCode**：*上下文信息，异步识别时必须传入* 

  ​	**Rect**：*目标的粗检矩形框，以[x,y,width,heigh]表示，若要获取精准人矩形框，请使用识别返回的结果(ImageResults)中的坐标信息（包含车上人的坐标等等重要信息）。* 

  ​	**Timestamp**：*该目标所处的帧的编号*
  ​	**Trail**：*该目标相对于前帧的位移* 

  ​	**Type**：*目标的类型* 

  ​	**VideoChannel**：*目标所在的视频通路* 

  ​	**IsCache**：*当前输入的帧是否需要缓存* 

  ​	**ReleaseCacheFrames**：*可以释放的帧编号*

  ------

  * **buff_len** 双向参数，作为输入参数，表示输入缓存区的大小，作为输出参数，表示缓冲区中json字符串的长度。

  * **timeout** 超时时间，目前该参数无效

* 返回值

  见附录返回值说明

* 说明

  对多个通路的视频帧进行目标检测，跟踪和择优。

  注意：同一帧图像不能重复调用该接口，否则可能会导致缓存结果异常

## 视频检测跟踪结束帧

* 原型

```c
int32_t __stdcall seemmo_video_pvc_end( 
    	int32_t frame_num, 
    	const int32_t* video_chn, 
    	const char** calc_params, 
    	char* rsp_buf, 
    	int32_t* buff_len, 
    	int32_t timeout);
```

* 参数

  * **frame_num** 总的视频通路数
  * **video_chn** 每个视频帧的通路编号(例如:1,2,3,4)数组，数组大小为frame_num
  * **calc_params** 每路视频的请求参数，以json格式表示，指针数组大小为frame_num，每个指针指向一个完整json串，代表一路视频的参数。详见seemmo_video_pvc中对该参数的说明
  * **rsp_buf** 返回结果时使用的缓冲区，缓冲区储存是json字串，详见seemmo_video_pvc中对该参数的说明
  * **buff_len** 双向参数，作为输入参数，表示输入缓存区的大小，作为输出参数，表示缓冲区中json字符串的长度
  * **timeout** 超时时间，目前该参数无效

* 返回值

  见附录返回值说明

*  说明

  视频结构化结束帧，当视频流结束后，调该函数可将检测跟踪的结果强制输出

## pvc图像识别

* 原型

```c
int32_t __stdcall seemmo_image_pvc_recog(
    	int32_t image_num,
        const uint8_t** ppbgr24,
        const uint32_t* height,
        const uint32_t* width,
        const char* calc_param, 
        char* rsp_buf,
        int32_t* buff_len,
        int32_t timeout);
```

* 参数

  * **image_num** 图片个数
  * **ppbgr24** 解码后多张图片输入
  * **height** 每张图片高度，大小为image_num
  * **width** 每张图片宽度，大小为image_num
  * **calc_param** 请求参数,参数中包含识别参数，json格式如下：

  ```json
  { 
      "Detect": { 
          "IsDet": true, 
          "Threshold": 1,
          "Mode": 0 
      }, 
      "Recognize": { 
          "Vehicle": { 
              "Brand": { 
                  "IsRec": true 
              }, 
              "Plate": { 
                  "IsRec": true 
              }, 
              "Color": { 
                  "IsRec": true 
              }, 
              "Marker": { 
                  "IsRec": true 
              },
  			"Type": { 
                  "IsRec": true 
              }, 
              "Belt": { 
                  "IsRec": true 
              }, 
              "Call": { 
                  "IsRec": true 
              }, 
              "Crash": { 
                  "IsRec": true 
              }, 
              "Danger": { 
                  "IsRec": true 
              }, 
              "Rack": { 
                  "IsRec": true 
              }, 
              "Sunroof": { 
                  "IsRec": true 
              }, 
              "SpareTire": { 
                  "IsRec": true 
              }, 
              "Slag": { 
                  "IsRec": true 
              } 
          }, 
          "Person": { 
              "IsRec": true 
          }, 
          "Feature": { 
              "IsRec": true 
          } 
      }
  }
  ```

  *<font color=red>json字段说明： </font>*

  ------

  ​	**IsRec**：*是否针对该属性进行识别* 

  ​	**Feature**：*目标特征码* 

  ​	**Brand**：*品牌型号* 

  ​	**Plate**：*号牌号码* 

  ​	**Color**：*车辆颜色* 

  ​	**Type**：*车辆类型* 

  ​	**Slag**：*渣土车识别* 

  ​	**Rack**：*行李架识别* 

  ​	**Sunroof**：*天窗识别* 

  ​	**SpareTire**：*备胎识别* 

  ​	**Belt**：*安全带识别* 

  ​	**Call**：*打电话识别* 

  ​	**Crash**：*车辆撞损识别* 

  ​	**Danger**：*危险品车辆识别* 

  ​	**Person**：*人的各项特征识别* 

  ​	**Trail**：*目标的位移*

  ------

  * **rsp_buf** 返回结果的json缓冲区,json结果示例如下：

  ```json
  {
      "Code":0,
      "Message":"ok",
      "ImageResults":[
          {
              "Code":0,
              "Message":"ok",
              "Vehicles":[
                  {
                      "Type":6,
                      "GUID":"unset_guid",
                      "Trail":[0,0],
                      "Detect":{
                          "Code":0,
                          "Message":"ok",
                          "Body":{
                              "Rect":[39,39,395,392],
                              "Score":98,
                              "Width":474,
                              "Height":470
                          },
                          "Car":{
                              "Rect":[49,24,396,400],
                              "Score":100,
                              "Width":0,
                              "Height":0
                          },
                          "Plate":{
                              "Rect":[254,386,61,21],
                              "Score":68,
                              "Width":0,
                              "Height":0
                          },
                          "Window":{
                              "Rect":[134,116,305,162],
                              "Score":71,
                              "Width":0,
                              "Height":0}
                      },
                      "Recognize":{
                          "Feature":{
                              "Code":0,
                              "Message":"ok",
                              "Feature":"AQAAAAAQAAAAAAAAAAAAAAAAAAA.*"
                          },
                          "Mistake":{
                              "Code":0,
                              "Message":"ok",
                              "TopList":[
                                  {
                                      "Code":"0",
                                      "Name":"front",
                                      "Score":99
                                  }
                              ]
                          },
                          "Color":{
                              "Code":0,
                              "Message":"ok",
                              "TopList":[
                                  {
                                      "Code":"4",
                                      "Name":"绿色",
                                      "Score":100
                                  }
                              ]
                          },
                          "Type":{
                              "Code":0,
                              "Message":"ok",
                              "TopList":[
                                  {
                                      "Code":"8",
                                      "Name":"大型客车",
                                      "Score":98
                                  }
                              ]
                          },
                          "Brand":{
                              "Code":0,
                              "Message":"ok",
                              "TopList":[
                                  {
                                      "Code":"4B23DCEA28BB457096BF08A3AFCDE49C",
                                      "Name":"飞驰-FSQ6810JCG",
                                      "Score":99
                                  }
                              ]
                          },
                          "Plate":{
                              "Code":0,
                              "Message":"ok",
                              "Color":{
                                  "Code":"1",
                                  "Name":"黄",
                                  "Score":92
                              },
                              "Type":1,
                              "Flag":1,
                              "Score":11,
                              "Shelter":false,
                              "Destain":false,
                              "Licence":"粤P04462"
                          },
                          "Marker":{
                              "Code":0,
                              "Message":"ok"
                          }
                      }
                  }
              ],
              "Pedestrains":[],
              "Bikes":[]
          }
      ],
     	"FilterResults":[]
  }
  ```

  *<font color=red>json字段说明</font>*

  ------

  ​	**ImageResults**：*保存当前输入帧的识别结果* 

  ​	**Person**：*二轮车上人的识别结果* 

  ​	**Pedestrains**：*行人的识别结果* 

  ​	**Vehicles**：*机动车的识别结果* 

  ​	**Vehicles/Detect/Car/Rect**：*机动车的精准检测的矩形框，以[x,y,width,heigh]表示* 				

  ​	**Pedestrains/Detect/Body/Rect**：*行人的精准检测的矩形框，以[x,y,width,heigh]表示* 	

  ​	**Person/Detect/Body/Rect**：*非机动车上人的精准检测的矩形框，以[x,y,width,heigh]表示* 

  ​	**Feature**：*目标的特征码* 

  ​	**Brand**：*品牌型号,如果无法识别品牌，brand对应的Message会 置为“error recog mistake result”,Code置为15* 

  ​	**Plate**：*号牌号码* 

  ​	**Color**：*车身颜色* 

  ​	**Type**：*车辆类型* 

  ​	**Slag**：*渣土车* 

  ​	**Sunroof**：*天窗* 

  ​	**SpareTire**：*备胎* 

  ​	**Rack**：*行李架* 

  ​	**Belt**：*是否有系安全带识别结果(MainDrive为主驾,Codriver为副驾)* 

  ​	**Call**：*打电话识别结果* 

  ​	**Crash**：*车辆撞损结果* 

  ​	**Danger**：*危化品车辆结果*

  ​	**Age**：*人的年龄* 

  ​	**Bag**：*是否有拎东西* 

  ​	**Baby**：*是否抱小孩* 

  ​	**BottomColor**：*下身衣服颜色* 

  ​	**BottomType**：*下身衣服类型* 

  ​	**Orientation**：*人的朝向（前，后，侧）* 

  ​	**Umbrella**：*是否打伞* 

  ​	**Hair**：*头发类型* 

  ​	**Hat**：*是否带帽子* 

  ​	**Sex**：*性别* 

  ​	**UpperColor**：*上身衣服颜色* 

  ​	**UpperType**：*上身衣服类型* 

  ​	**Knapsack**：*是否背包* 

  ​	**TopList**：*识别出的topN结果* 

  ​	**Licence**：*车牌* 

  ​	**Score**：*检测/识别打分*

  <font color=red>注：以上json实例并未展示所有的识别结果，具体以使用过程中的json字段为准</font>

  ------

  * **buff_len** 返回缓冲区rsp_buf大小
  * **timeout** 超时时间，目前该参数无效

* 返回值

  见附录返回值说明

* 说明

  对全图或者目标抠图进行pvc识别，<font color=red>只用于图片识别</font>

## 人脸图像识别

* 原型

```c
int32_t __stdcall seemmo_image_face_recog(
		const uint8_t** ppbgr24,
        uint32_t image_num,
        const uint32_t *height,
        const uint32_t *width,
        const char** calc_params,
        char* rsp_buf,
        int32_t* buff_len,
        int32_t timeout = 0);
```

* 参数

  * **ppbgr24** 解码后多张图片输入

  - **image_num** 图片个数
  - **height** 每张图片高度，大小为image_num
  - **width** 每张图片宽度，大小为image_num
  - **calc_param** 请求参数,参数中包含识别参数，json格式如下：

  ```json
  { 
      "Detect": { 
          "IsDet": true, 
          "Threshold": 1,
          "Mode": 0 
      }, 
      "Recognize": { 
          "Feature": { 
              "IsRec": true 
          } 
      }
  }
  ```

  * **rsp_buf** 返回结果的json缓冲区,json结果示例如下：

  ```json
  [
      {
          "Code": 0,
          "Message": "ok",
          "Mistake": {
              "Code": 0,
              "Message": "ok",
              "IsMistake": false,
              "MistakeScore": 2,
              "IntegrityScore": 97,
              "QualityScore": 44,
              "LandmarkScore": 66
          },
          "FaceRect": [
              18,
              15,
              35,
              47
          ],
          "FaceFeature": "BwAAAAAIAAAAAAAAAAAAAAAAAAAAAAAAAAAA.*"
      }
  ]
  ```

  *<font color=red>json字段说明：</font>*

  ------

  ​	**Mistake**：*是否误检结果* 

  ​	**Mistake/IsMistake**：*人脸是否误检* 

  ​	**FaceRect**：*人脸位置* 

  ------

  * **buff_len** 返回缓冲区rsp_buf大小

  - **timeout** 超时时间，目前该参数无效

* 返回值

  见附录返回值说明

* 说明

  对全图或者目标抠图进行人脸识别，<font color=red>只用于图片识别</font>

## pvc视频识别

* 原型

```c
SEEMMO_GPU_API int32_t __stdcall seemmo_video_pvc_recog(
		int32_t image_num,
        const char** guids,
        const char* calc_param,
        char* rsp_buf,
        int32_t* buff_len,
        uint8_t** ppbgr24_buf,
        int32_t timeout = 0);
```

* 参数

  - **image_num** 图片个数
  - **guids** 通过检测跟踪得到的物体的guid，guids的大小与image_num保持一致
  - **calc_param** 请求参数,参数中包含识别参数，具体参数参见[pvc图像识别](#pvc图像识别)calc_param参数
  - **rsp_buf** 返回结果的json缓冲区，json字段含义参见[pvc图像识别](#pvc图像识别)rsp_buf参数
  - **buff_len** 返回缓冲区rsp_buf大小
  - **ppbgr24_buf** 传入guids相对应的识别图片，用于识别结果保存，此参数内存空间由使用者自行分配（<font color=red>若是识别更不上的话，可能存在图片缺失</font>）

  <font color=red>（注：此块内存构造的是用于存储guid对应的图片，构造的图片的大小与image_num一致，每一张图片的长宽的通过检测跟踪出来的FilterResult中的物体的` obj["Detect"]["Body"]["Width"]`以及` obj["Detect"]["Body"]["Height"]`来获取）</font>

  - **timeout** 超时时间，目前该参数无效

* 返回值

  见附录返回值说明

* 说明

  用于pvc视频检测跟踪过程中的pvc结果识别

## 人脸视频识别

* 原型

```c
SEEMMO_GPU_API int32_t __stdcall seemmo_video_face_recog(
		int32_t image_num,
        const char** guids,
        const char* calc_param,
        char* rsp_buf,
        int32_t* buff_len,
        uint8_t** ppbgr24_buf,
        int32_t timeout = 0);
```

* 参数

  - **image_num** 图片个数
  - **guids** 通过检测跟踪得到的物体的guid，guids的大小与image_num保持一致
  - **calc_param** 请求参数,参数中包含识别参数，具体参数参见[人脸图像识别](#人脸图像识别)calc_param参数
  - **rsp_buf** 返回结果的json缓冲区，json字段含义参见[人脸图像识别](#人脸图像识别)rsp_buf参数
  - **buff_len** 返回缓冲区rsp_buf大小
  - **ppbgr24_buf** 传入guids相对应的识别图片，用于识别结果保存，此参数内存空间由使用者自行分配（<font color=red>若是识别更不上的话，可能存在图片缺失</font>）

  <font color=red>（注：此块内存构造的是用于存储guid对应的图片，构造的图片的大小与image_num一致，每一张图片的长宽的通过检测跟踪出来的FilterResult中物体的` obj["Detect"]["Body"]["Width"]`以及` obj["Detect"]["Body"]["Height"]`来获取）</font>

  - **timeout** 超时时间，目前该参数无效

* 返回值

  见附录返回值说明

* 说明

  用于pvc视频检测跟踪过程中的人脸结果识别

## 特征匹配

* 原型

```c
long __stdcall seemmo_pvc_match( 
		const char* feature_src,         
        int feature_src_len,
        const char* feature_to,          
        int feature_to_len,
        int type,
        float* score,
        float* proi=NULL);
```

* 参数

  * **feature_src** 物体的特征字符串，在目标识别中返回
  * **feature_src_len** 物体特征字符串长度
  * **feature_to** 物体特征字符串，在目标识别中返回
  * **feature_to_len** 物体特征字符串长度
  * **type** 物体的类型
  * **score** 得分（0-100）
  * **proi** 局部识别[x, y, width, height]

* 返回值

  见附录返回值说明

* 说明

  计算两个物体的相似度

# 附录

## 接口返回值说明

| 返回值 |           返回值宏           |                             说明                             |
| :----: | :--------------------------: | :----------------------------------------------------------: |
|   0    |         SEEMMO_SUCC          |                           成功调用                           |
|   -1   |   SEEMMO_INIT_LOG_DIR_FAIL   |                      初始化日志目录失败                      |
|   -2   |  SEEMMO_INIT_LOG_INIT_FAIL   |                        初始化日志失败                        |
|   -3   |   SEEMMO_INIT_REPREAT_INIT   |                          重复初始化                          |
|   -4   |  SEEMMO_INIT_SDK_INIT_FAIL   |                        初始化sdk失败                         |
|   -5   |   SEEMMO_INIT_SDK_NEW_FAIL   |                  初始化sdk时，申请内存失败                   |
|   -6   | SEEMMO_INIT_LOAD_MODEL_FAIL  |                         加载模型失败                         |
|   -7   |  SEEMMO_RECOG_NULL_POINTER   |                        传入的指针为空                        |
|   -8   |    SEEMMO_RECOG_MAT_FAIL     |                      构造图像矩阵时失败                      |
|   -9   |   SEEMMO_RECOG_EMPTY_IMAGE   |                      输入的图像数据为空                      |
|  -10   |  SEEMMO_RECOG_ALGPROC_FAIL   |                         算法执行出错                         |
|  -11   |  SEEMMO_RECOG_RSPBUF_SMALL   |         用于储存结果数据的缓存太小，无法容纳输出结果         |
|  -12   |    SEEMMO_RECOG_NOT_INIT     |     SDK进程未初始化,请检查是否已执行seemmo_process_init      |
|  -13   |    SEEMMO_THREAD_NOT_INIT    |     SDK线程未初始化，请检查是否已执行seemmo_thread_init      |
|  -14   |     SEEMMO_BASE64_DECODE     |                        base64解码错误                        |
|  -15   |     SEEMMO_EMPTY_REQUEST     |               空请求，输出的图片数frame_num为0               |
|  -16   |  SEEMMO_INIT_LIMIT_CAPACITY  |              申请的路数超过了授权允许的最大路数              |
|  -17   |   SEEMMO_LOAD_TYPE_UNMATCH   | 线程所使用的功能和加载的功能不匹配，比如使用的是识别接口，加载的却是检测跟踪的功能 |
|  -110  | SEEMMO_ERROR_INVALID_LICENCE |                           授权失败                           |

<font color=red>当收到返回值为SEEMMO_ERROR_INVALID_LICENCE时，请及时以正常流程结束程序（相关uninit接口），否则sdk会在一定时间内自行结束程序，造成一
切未知损失后果自负。</font> 

## 目标类型说明

| 类型值 |  类型值宏  |     说明     |
| :----: | :--------: | :----------: |
|   -1   |  INVALID   | 未定义的类型 |
|   0    |  UNKNOWN   |   未知类型   |
|   1    | PEDESTRAIN |     行人     |
|   2    |    BIKE    |    自行车    |
|   3    |  MOTOBIKE  |    摩托车    |
|   4    |    CAR     |    小汽车    |
|   5    |  TRICYCLE  |    三轮车    |
|   6    |    BUS     |    公交车    |
|   7    |    VAN     |    面包车    |
|   8    |   TRUCK    |     卡车     |

## 配置项（sdk.conf）说明

|         配置项         |                             说明                             |
| :--------------------: | :----------------------------------------------------------: |
|     sc_filter_topn     |        择优后，每个目标返回的最优结果的个数，默认为1         |
|  sc_rec_filter_person  |             识别时，是否进行行人识别，默认为true             |
| sc_rec_filter_vehicle  |             识别时，是否进行车辆识别，默认为true             |
| sc_rec_filter_feature  |           识别时，是否计算目标的特征码，默认为true           |
|    sc_rec_type_slag    |                渣土车识别项开关，默认为false                 |
| sc_rec_new_car_feature | 新特征识别项开关（包含打电话、安全带、危化品、撞损、备胎、行李架、天窗），默认为true |
|      sc_rec_face       |                  人脸识别开关，默认为false                   |
|    sc_rec_side_car     |                侧面车识别项开关，默认为false                 |

