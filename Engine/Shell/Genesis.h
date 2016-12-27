//
//  Genesis.h
//  GenesisEngineLib
//
//  Created by 宋 琦 on 13-9-18.
//  Copyright (c) 2013年 webJet. All rights reserved.
//

#ifndef GenesisEngineLib_Genesis_h
#define GenesisEngineLib_Genesis_h
#include <vector>

namespace EngineShell
{
    enum InputAciton
    {
        IA_DOWN = 0,
        IA_MOVE,
        IA_UP,
        IA_CANCEL,
    };
    
    struct TouchData
    {
        float x,y;
        int id;
    };
    
    typedef std::vector<TouchData> TouchDataVector;

    void InitEngine( const int& w, const int& h, const char* path,const char* appWritableDic, const char* scene,bool bUsePrecompileShader );
    void Update();
    void TouchPoint( const TouchDataVector& touchDatas, const InputAciton& action );
    void OnResumed();
    void OnStopped();
    void UIInsertText(const char* wstr);
    void UIDeleteBackward();

}
#endif
