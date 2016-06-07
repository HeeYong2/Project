#ifndef Tool_Constant_h__
#define Tool_Constant_h__

const unsigned	int WINSIZEX = 1280;
const unsigned	int WINSIZEY = 720;

const D3DXVECTOR3	g_vLook = D3DXVECTOR3(0.f , 0.f , 1.f);

enum RESOURCE { RESOURCE_LOGO , RESOURCE_STAGE , RESOURCE_STATIC ,RESOURCE_END};
enum EDITMODE { EDITUPDOWN , EDITCENTER , EDITHILL , EDITEND };
#endif // Tool_Constant_h__
