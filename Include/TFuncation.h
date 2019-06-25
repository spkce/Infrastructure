
#ifndef __TFUNCATION_H__
#define __TFUNCATION_H__

#include "def.h"

namespace Infra
{

#undef PARAM_NUM
#undef TEMPLATE_PARAM_TYPE
#undef PARAM_LIST
#undef PARAM_INPUT


//TFuncation1
#define PARAM_NUM 1
#define TEMPLATE_PARAM_TYPE typename P1
#define PARAM_LIST P1 p1
#define PARAM_INPUT p1
#include "funtemplate.h"
#undef PARAM_NUM
#undef TEMPLATE_PARAM_TYPE
#undef PARAM_LIST
#undef PARAM_INPUT

//TFuncation2
#define PARAM_NUM 2
#define TEMPLATE_PARAM_TYPE typename P1, typename P2
#define PARAM_LIST P1 p1, P2 p2
#define PARAM_INPUT p1, p2
#include "funtemplate.h"
#undef PARAM_NUM
#undef TEMPLATE_PARAM_TYPE
#undef PARAM_LIST
#undef PARAM_INPUT


}////Infra

#endif //__TFUNCATION_H__
