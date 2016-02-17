#include "globals.h"

void setResolution(int width, int height)
{
    DEVMODE lpDevMode;
    lpDevMode.dmFields = DM_PELSWIDTH|DM_PELSHEIGHT|DM_BITSPERPEL|DM_DISPLAYFREQUENCY;
    LONG result;
    //得到显示设备的一个图形模式设备，通过对该函数一系列的调用可以得到显示设备所有的图形模式信息
    EnumDisplaySettings(NULL,ENUM_REGISTRY_SETTINGS,&lpDevMode);
    lpDevMode.dmPelsWidth = width ;//水平分辨率
    lpDevMode.dmPelsHeight = height;//垂直分辨率
    //把显示设备在lpszDeviceName参数中定义的设置，改变为在lpDevMode参数中定义的图形模式
    //NULL值定义了缺省的显示设备。
    result = ChangeDisplaySettingsEx(NULL,&lpDevMode,NULL,0,NULL);
    if(result == DISP_CHANGE_SUCCESSFUL) //改变分辨率成功
    {
        //使用CDS_UPDATEREGISTRY表示此次修改是持久的，并在注册表中写入了相关的数据
        result = ChangeDisplaySettingsEx(NULL,&lpDevMode,NULL,CDS_UPDATEREGISTRY,NULL);
    }
}
